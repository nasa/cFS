PROC scx_cpu3_sch_dcr18259test
;**********************************************************************

local logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

#include "ut_statusdefs.h"
#include "ut_cfe_info.h"
#include "cfe_platform_cfg.h"
#include "cfe_evs_events.h"
#include "cfe_es_events.h"
#include "cfe_tbl_events.h"
#include "to_lab_events.h"
#include "sch_platform_cfg.h"
#include "cs_platform_cfg.h"
#include "cs_events.h"
#include "cs_tbldefs.h"
#include "cs_msgdefs.h"
#include "tst_cs_events.h"
#include "sch_events.h"
#include "tst_sch_events.h"

%liv (log_procedure) = logging

;**********************************************************************
; Define local variables
;**********************************************************************
LOCAL rawcmd
LOCAL stream
LOCAL cs_defTblId, cs_defPktId, cs_resTblId, cs_resPktId
LOCAL sch_tblAppId, sch_tblPktId, msg_tblAppId, msg_tblPktId
local i,tblIndex,tblName,foundTbl
local SCHAppName = "SCH"
local SCHLabAppName = "SCH_LAB_APP"
local CSAppName = "CS"
local ramDir = "RAM:0"
local cs_tblDefTblName = CSAppName & "." & CS_DEF_TABLES_TABLE_NAME
local cs_tblResTblName = CSAppName & "." & CS_RESULTS_TABLES_TABLE_NAME
local SCHDefTblName = SCHAppName & ".SCHED_DEF"

;;; Set the pkt and app IDs for the tables based upon the cpu being used
;; CPU1 is the default
cs_defTblId = "0FAE"
cs_resTblId = "0FB2"
cs_defPktId = 4014
cs_resPktId = 4018
sch_tblAppId = "0FB5"
sch_tblPktId = 4021
msg_tblAppId = "0FB4"
msg_tblPktId = 4020

if ("CPU3" = "CPU2") then
  cs_defTblId = "0FCC"
  cs_resTblId = "0FD0"
  cs_defPktId = 4044
  cs_resPktId = 4048
  sch_tblAppId = "0FD3"
  sch_tblPktId = 4051
  msg_tblAppId = "0FD2"
  msg_tblPktId = 4050
elseif ("CPU3" = "CPU3") then
  cs_defTblId = "0FEC"
  cs_resTblId = "0FF0"
  cs_defPktId = 4076
  cs_resPktId = 4080
  sch_tblAppId = "0FF3"
  sch_tblPktId = 4083
  msg_tblAppId = "0FF2"
  msg_tblPktId = 4082
endif

write ";*********************************************************************"
write ";  Step 1.0: Test Setup."
write ";*********************************************************************"
write ";  Step 1.1: Command a Power-on Reset on CPU3."
write ";*********************************************************************"
/SCX_CPU3_ES_POWERONRESET
wait 10

close_data_center
wait 75

cfe_startup CPU3
wait 5

write ";*********************************************************************"
write ";  Step 1.2: Determine if the SCH_LAB application is running. If so,  "
write ";  we must delete it in order to start the SCH application. "
write ";**********************************************************************"
start get_file_to_cvt (ramDir,"cfe_es_app_info.log","scx_cpu3_es_app_info.log","CPU3")

local found_app = FALSE

;Loop thru the table looking for the SCH_LAB_APP
for file_index = 1 to CFE_ES_MAX_APPLICATIONS do
  if (SCX_CPU3_ES_ALE[file_index].ES_AL_AppName = SCHLabAppName) then
    found_app = TRUE
  endif
enddo

if (found_app = TRUE) then
  /SCX_CPU3_ES_DELETEAPP Application=SCHLabAppName
  wait 10
endif

write ";**********************************************************************"
write ";  Step 1.3: Display the Housekeeping pages "
write ";**********************************************************************"
page SCX_CPU3_CS_HK
page SCX_CPU3_TST_CS_HK
page SCX_CPU3_CS_TBL_DEF_TABLE
page SCX_CPU3_CS_TBL_RESULTS_TBL
page SCX_CPU3_SCH_HK
page SCX_CPU3_TST_SCH_HK


write ";*********************************************************************"
write ";  Step 1.4: Determine if the Scheduler (SCH) Application is running. "
write ";  If no, start it. If yes, send the restart command. Also add any "
write ";  required subscriptions.  "
write ";********************************************************************"
found_app = FALSE

;Loop thru the table looking for the Scheduler Application
for file_index = 1 to CFE_ES_MAX_APPLICATIONS do
  if (SCX_CPU3_ES_ALE[file_index].ES_AL_AppName = SCHAppName) then
    found_app = TRUE
  endif
enddo

ut_setupevents "SCX", "CPU3", "CFE_ES", CFE_ES_START_INF_EID, "INFO", 1
ut_setupevents "SCX", "CPU3", {SCHAppName}, SCH_INITSTATS_INF_EID, "INFO", 2

if (found_app = TRUE) then
  /SCX_CPU3_ES_RESTARTAPP Application=SCHAppName

  ut_tlmwait SCX_CPU3_find_event[2].num_found_messages, 1
  IF (UT_TW_Status = UT_Success) THEN
    write "<*> Passed - SCH Application Started"
  else
    write "Failed - SCH Application start Event Message not received."
  endif
else
  s SCX_CPU3_sch_sdtloadfile
  s SCX_CPU3_sch_mdtloadfile

  s load_start_app (SCHAppName,"CPU3","SCH_AppMain")

  ; Wait for app startup events
  ut_tlmwait SCX_CPU3_find_event[2].num_found_messages, 1
  IF (UT_TW_Status = UT_Success) THEN
    if (SCX_CPU3_find_event[1].num_found_messages = 1) then
      write "<*> Passed - SCH Application Started"
    else
      write "<!> Failed - CFE_ES start Event Message for SCH not received."
    endif
  else
    write "<!> Failed - SCH Application start Event Message not received."
  endif
endif

;; Need to set the stream based upon the cpu being used
;; CPU1 is the default
stream = x'0897'

if ("CPU3" = "CPU2") then
  stream = x'0997'
elseif ("CPU3" = "CPU3") then
  stream = x'0A97'
endif

write "Sending command to add subscription for SCH HK packet."
/SCX_CPU3_TO_ADDPACKET Stream=stream Pkt_Size=x'0' Priority=x'0' Reliability=x'1' Buflimit=x'4'
wait 10

write ";**********************************************************************"
write ";  Step 1.5: Start the SCH Test Application (TST_SCH) and "
write ";  add any required subscriptions.  "
write ";**********************************************************************"
ut_setupevents "SCX", "CPU3", "CFE_ES", CFE_ES_START_INF_EID, "INFO", 1
ut_setupevents "SCX", "CPU3", "TST_SCH", TST_SCH_INITSTATS_INF_EID, "INFO", 2

s load_start_app ("TST_SCH","CPU3","TST_SCH_AppMain")
wait 3

; Wait for app startup events
ut_tlmwait SCX_CPU3_find_event[2].num_found_messages, 1
IF (UT_TW_Status = UT_Success) THEN
  if (SCX_CPU3_find_event[1].num_found_messages = 1) then
    write "<*> Passed - TST_SCH Application Started"
  else
    write "<!> Failed - CFE_ES start Event Message for TST_SCH not received."
    write "Event Message count = ",SCX_CPU3_find_event[1].num_found_messages
  endif
else
  write "<!> Failed - TST_SCH Application start Event Message not received."
endif

;;; Need to set the stream based upon the cpu being used
;;; CPU1 is the default
stream = x'0936'

if ("CPU3" = "CPU2") then
  stream = x'0A36'
elseif ("CPU3" = "CPU3") then
  stream = x'0B36'
endif

write "Sending command to add subscription for TST_SCH HK packet."
/SCX_CPU3_TO_ADDPACKET Stream=stream Pkt_Size=x'0' Priority=x'0' Reliability=x'1' Buflimit=x'4'
wait 10

;; Enable Entry #8 - Slot 1 - entry 3
;; TST_SCH_SEND_HK request
/SCX_CPU3_SCH_EnableEntry SlotNumber=1 EntryNumber=3
wait 5

write ";*********************************************************************"
write ";  Step 1.6: Start the TST_CS_MemTbl application in order to setup   "
write ";  the OS_Memory_Table for the Checksum (CS) application. "
write ";********************************************************************"
ut_setupevents "SCX", "CPU3", "CFE_ES", CFE_ES_START_INF_EID, "INFO", 1
ut_setupevents "SCX", "CPU3", "TST_CS_MEMTBL", 1, "INFO", 2

s load_start_app ("TST_CS_MEMTBL","CPU3","TST_CS_MemTblMain")

;; Wait for app startup events
ut_tlmwait SCX_CPU3_find_event[2].num_found_messages, 1
IF (UT_TW_Status = UT_Success) THEN
  if (SCX_CPU3_find_event[1].num_found_messages = 1) then
    write "<*> Passed - TST_CS_MEMTBL Application Started"
  else
    write "<!> Failed - CFE_ES start Event Message for TST_CS_MEMTBL not received."
    write "Event Message count = ",SCX_CPU3_find_event[1].num_found_messages
  endif
else
  write "<!> Failed - TST_CS_MEMTBL Application start Event Message not received."
endif

;; These are the TST_CS HK Packet IDs since this app sends this packet
;; CPU1 is the default
stream = x'0930'

if ("CPU3" = "CPU2") then
  stream = x'0A30'
elseif ("CPU3" = "CPU3") then
  stream = x'0B30'
endif

/SCX_CPU3_TO_ADDPACKET STREAM=stream PKT_SIZE=X'0' PRIORITY=X'0' RELIABILITY=X'0' BUFLIMIT=x'4'
wait 5

write ";*********************************************************************"
write ";  Step 1.7: Start the Checksum (CS) and TST_CS applications.         "
write ";********************************************************************"
s scx_cpu3_cs_start_apps("1.7")
wait 5

write ";*********************************************************************"
write ";  Step 1.8: Turn off checksumming for everything except Tables."
write ";********************************************************************"
;; Send the Disable OS Checksumming Command
/SCX_CPU3_CS_DisableOS
wait 2

;; Send the Disable CFE Core Checksumming Command
/SCX_CPU3_CS_DisableCFECore
wait 2

;; Disable Eeprom Checksumming if it is enabled
/SCX_CPU3_CS_DisableEeprom
wait 2

;; Disable User-defined Memory Checksumming if it is enabled
/SCX_CPU3_CS_DisableMemory
wait 2

;; Disable Application Checksumming if it is enabled
/SCX_CPU3_CS_DisableApps
wait 2

;; Send the Enable Tables Command
/SCX_CPU3_CS_EnableTables
wait 2

write ";*********************************************************************"
write ";  Step 1.9: Create & load the Tables Definition Table file to be used "
write ";  during this test."
write ";********************************************************************"
;; Need to populate the table with the SCHED_DEF table
;; States are 0=CS_STATE_EMPTY; 1=CS_STATE_ENABLED; 2=CS_STATE_DISABLED;
;;            3=CS_STATE_UNDEFINED
SCX_CPU3_CS_TBL_DEF_TABLE[0].State = CS_STATE_ENABLED
SCX_CPU3_CS_TBL_DEF_TABLE[0].Name = cs_tblDefTblName
SCX_CPU3_CS_TBL_DEF_TABLE[1].State = CS_STATE_DISABLED
SCX_CPU3_CS_TBL_DEF_TABLE[1].Name = cs_tblResTblName
SCX_CPU3_CS_TBL_DEF_TABLE[2].State = CS_STATE_ENABLED
SCX_CPU3_CS_TBL_DEF_TABLE[2].Name = SCHDefTblName 

;; Clear out the remaining entries in the table
for i = 3 to CS_MAX_NUM_TABLES_TABLE_ENTRIES-1 do
  SCX_CPU3_CS_TBL_DEF_TABLE[i].State = CS_STATE_EMPTY
  SCX_CPU3_CS_TBL_DEF_TABLE[i].Name = ""
enddo

local lastEntry = CS_MAX_NUM_TABLES_TABLE_ENTRIES - 1
local endmnemonic = "SCX_CPU3_CS_TBL_DEF_TABLE[" & lastEntry & "].Name"

;; Create the Table Load file
s create_tbl_file_from_cvt ("CPU3",cs_defTblId,"DCR 18259 Table Load","cs_sch_def_tbl_ld",cs_tblDefTblName,"SCX_CPU3_CS_TBL_DEF_TABLE[0].State",endmnemonic)

wait 5

;; Load the Table file created above
s load_table("cs_sch_def_tbl_ld","CPU3")
wait 5

ut_setupevents "SCX","CPU3","CFE_TBL",CFE_TBL_VALIDATION_INF_EID,"INFO", 1
ut_setupevents "SCX","CPU3","CFE_TBL",CFE_TBL_UPDATE_SUCCESS_INF_EID,"INFO", 2

local cmdCtr = SCX_CPU3_TBL_CMDPC + 1

/SCX_CPU3_TBL_VALIDATE INACTIVE VTABLENAME=cs_tblDefTblName

ut_tlmwait SCX_CPU3_TBL_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Tables Definition Table validate command sent."
else
  write "<!> Failed - Tables Definition Table validation failed."
endif

ut_tlmwait SCX_CPU3_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Event message ",SCX_CPU3_find_event[1].eventid, " received"
else
  write "<!> Failed - Event message ",SCX_CPU3_evs_eventid," received. Expected Event message ",CFE_TBL_VALIDATION_INF_EID, "."
endif

cmdCtr = SCX_CPU3_TBL_CMDPC + 1

/SCX_CPU3_TBL_ACTIVATE ATableName=cs_tblDefTblName

ut_tlmwait SCX_CPU3_TBL_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Activate Tables Definition Table command sent properly."
else
  write "<!> Failed - Activate Tables Definition Table command."
endif

ut_tlmwait SCX_CPU3_find_event[2].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Tables Definition Table Updated successfully."
  Write "<*> Passed - Event Msg ",SCX_CPU3_find_event[2].eventid," Found!"
else
  write "<!> Failed - Tables Definition Table update failed."
  Write "<!> Failed - Event Message not received for activate command."
endif

wait 5

write ";*********************************************************************"
write ";  Step 1.10: Enable DEBUG Event Messages "
write ";*********************************************************************"
local cmdCtr = SCX_CPU3_EVS_CMDPC + 3

;; Enable DEBUG events for the SCH, CS and CFE_TBL applications ONLY
/SCX_CPU3_EVS_EnaAppEVTType Application=CSAppName DEBUG
wait 2
/SCX_CPU3_EVS_EnaAppEVTType Application=SCHAppName DEBUG
wait 2
/SCX_CPU3_EVS_EnaAppEVTType Application="CFE_TBL" DEBUG

ut_tlmwait SCX_CPU3_EVS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Enable Debug events command sent properly."
else
  write "<!> Failed - Enable Debug events command."
endif

write ";*********************************************************************"
write ";  Step 1.11: Dump the Table Results Table."
write ";*********************************************************************"
cmdCtr = SCX_CPU3_TBL_CMDPC + 1

s get_tbl_to_cvt (ramDir,cs_tblResTblName,"A","cpu3_tblrestbl1_9","CPU3",cs_resTblId)
wait 5

ut_tlmwait SCX_CPU3_TBL_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Dump of Table Results Table successful."
else
  write "<!> Failed - Dump of Table Results Table did not increment TBL_CMDPC."
endif

write ";*********************************************************************"
write ";  Step 2.0: Table Update Tests."
write ";*********************************************************************"
write ";  Step 2.1: Enable the NOOP Group and verify the timing of activities."
write ";*********************************************************************"
ut_setupevents "SCX", "CPU3", {SCHAppName}, SCH_ENA_GRP_CMD_EID, "DEBUG", 1
ut_setupevents "SCX", "CPU3", "TO_LAB_APP", TO_NOOP_INF_EID, "INFO", 2

;; Need to enable NOOP commands
/SCX_CPU3_SCH_EnableGroup GroupData=2

ut_tlmwait SCX_CPU3_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4002) - Event message ",SCX_CPU3_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4002, "P"
else
  write "<!> Failed (1004;4002) - Event message ",SCX_CPU3_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4002, "F"
endif

;; Wait for some Noops to execute
wait 20

if (SCX_CPU3_find_event[2].num_found_messages > 0) then
  write "<*> Passed (3000) - TO_NOOP Event messages are being generated"
  ut_setrequirements SCH_3000, "P"
else
  write "<!> Failed (3000) - Did not receive any TO_NOOP Event messages"
  ut_setrequirements SCH_3000, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 2.2: Disable the NOOP Group."
write ";*********************************************************************"
ut_setupevents "SCX", "CPU3", {SCHAppName}, SCH_DIS_GRP_CMD_EID, "DEBUG", 1

;; Disable the NOOP group
/SCX_CPU3_SCH_DisableGroup GroupData=2

ut_tlmwait SCX_CPU3_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4003) - Event message ",SCX_CPU3_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4003, "P"
else
  write "<!> Failed (1004;4003) - Event message ",SCX_CPU3_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4003, "F"
endif

wait 20

write ";*********************************************************************"
write ";  Step 2.3: Disable the TST_SCH_HK Request."
write ";*********************************************************************"
;; Enable Entry #8 - Slot 1 - entry 3
;; TST_SCH_SEND_HK request
/SCX_CPU3_SCH_DisableEntry SlotNumber=1 EntryNumber=3
wait 30

write ";*********************************************************************"
write ";  Step 2.3: Enable the TST_SCH_HK Request."
write ";*********************************************************************"
;; Enable Entry #8 - Slot 1 - entry 3
;; TST_SCH_SEND_HK request
/SCX_CPU3_SCH_EnableEntry SlotNumber=1 EntryNumber=3
wait 15

write ";*********************************************************************"
write ";  Step 3.0: Clean-up. "
write ";*********************************************************************"
/SCX_CPU3_ES_POWERONRESET
wait 10

close_data_center
wait 75

cfe_startup CPU3
wait 5

write ";*********************************************************************"
write ";  End procedure "
write ";*********************************************************************"
ENDPROC
