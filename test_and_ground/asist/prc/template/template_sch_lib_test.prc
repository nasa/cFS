PROC $sc_$cpu_sch_lib_test
;*******************************************************************************
;  Test Name:  sch_lib_test
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	This test verifies that the CFS Scheduler (SCH) application supports
;	the schedule library functions properly. These functions allow an
;	external application to control the enabling/disabling of the SCH
;	appplication processing activities.
;
;  Requirements Tested
;    SCH8000    SCH shall generate a housekeeping message containing the
;               following:
;                 a) Valid Ground Command Counter
;                 b) Ground Command Rejected Counter
;                 e) Valid commands sent by the SCH APP
;                 f) Commands sent by SCH APP that were reported as erroneous
;		     by the Software Bus
;                 g) Number of Slots processed
;                 h) Number of Times Slots skipped
;                 i) Number of Times Multiple Slots processed
;                 j) Number of times that SCH woke up in the same slot as
;                    last time
;                 k) Number of bad entries found in the Scheduler table
;                    (indication of corrupted table)
;                 l) Synchronization Status (flywheeling)
;    SCH9000    Upon any Initialization of the SCH Application (cFE Power On,
;               cFE Processor Reset or SCH Application Reset), SCH shall
;               initialize the following data to Zero:
;                 a) Valid Ground Command Counter
;                 b) Ground Command Rejected Counter
;                 e) Valid commands sent by the SCH APP
;                 f) Commands sent by SCH APP that were reported as erroneous by
;		     the Software Bus
;                 g) Number of Slots processed
;                 h) Number of Times Slots skipped
;                 i) Number of Times Multiple Slots processed
;                 j) Number of times that SCH woke up in the same slot as
;                    last time
;                 k) Number of bad entries found in the Scheduler table
;                    (indication of corrupted table)
;                 l) Synchronization Status (not flywheeling)
;    SCH9001    Upon any Initialization, the SCH Application shall inhibit
;               processing of the Schedule Definition Table until the cFE
;               indicates that all of the applications have started.
;
;  Prerequisite Conditions
;	The CFS is up and running and ready to accept commands.
;	The SCH commands and telemetry items exist in the GSE database.
;	A display page exists for the SCH Housekeeping telemetry packet.
;
;  Assumptions and Constraints
;	A Schedule Definition Table (SDT) and Message Definition Table (MDT)
;	have been loaded.
;
;  Change History
;	Date		Name		Description
;	08/10/11	Walt Moleski	Initial implementation
;       08/02/12        Walt Moleski	Added code to look for the Scheduler
;					app executing.
;       06/13/17        W. Moleski	Updated to use CPU1 for commanding and
;					added a hostCPU variable for the utility
;					procs to connect to the proper host.
;
;  Arguments
;	None.
;
;  Procedures Called
;	Name			              Description
;      ut_tlmwait               Wait for a specified telemetry point to update
;                               to a specified value. 
;      ut_sendcmd               Send commands to the spacecraft. Verifies
;				command processed and command error counters.
;      ut_pfindicate            Print the pass fail status of a particular
;				requirement number.
;      ut_setupevents           Directive to look for a particular event and 
;                               increment a value in the CVT to indicate receipt
;      ut_setrequirements	Directive to status requirements. 
;      get_tbl_to_cvt           Procedure to generate and download table file to
;				cvt
;      create_tbl_file_from_cvt	Procedure to generate a table file from the
;				contents of the cvt.
;      ftp_file			Procedure to upload file/table from Ground to
;				onboard CPU 
;      load_table               Procedure to load a table from Ground to CPU.
;      ut_tlmupdate             Directive to wait for telemetry packet update.
;
;  Expected Test Results and Analysis
;
;*******************************************************************************

local logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

#include "ut_statusdefs.h"
#include "ut_cfe_info.h"
#include "cfe_platform_cfg.h"
#include "cfe_es_events.h"
#include "cfe_tbl_events.h"
#include "sch_platform_cfg.h"
#include "sch_events.h"
#include "sch_msgdefs.h"
#include "tst_sch_events.h"

%liv (log_procedure) = logging

#define SCH_8000    0
#define SCH_9000    1
#define SCH_9001    2

global ut_req_array_size = 2
global ut_requirement[0 .. ut_req_array_size]

for i = 0 to ut_req_array_size DO
  ut_requirement[i] = "U"
enddo

;**********************************************************************
; Set the local values
;**********************************************************************
local cfe_requirements[0 .. ut_req_array_size] = ["SCH_8000","SCH_9000","SCH_9001"]

;**********************************************************************
; Define local variables
;**********************************************************************
LOCAL rawcmd
LOCAL stream1
LOCAL sch_tblAppId
LOCAL sch_tblPktId
LOCAL msg_tblAppId
LOCAL msg_tblPktId

local tblfailverifyctr
local tblpassverifyctr
local validmfctr
local slotprocctr
local tblprocctr
local max_minor_act
local skipslotctr
local actsuccessctr
local SCHAppName = "SCH"
local SCHLabAppName = "SCH_LAB_APP"
local ramDir = "RAM:0"
local SCHDefTblName = SCHAppName & ".SCHED_DEF"
local MSGDefTblName = SCHAppName & ".MSG_DEFS"
local cmdCtr
local hostCPU = "$CPU"

;;; Set the pkt and app IDs for the tables based upon the cpu being used
;;; Right now, the pktIDs are not used
;;; CPU1 is the default
sch_tblAppId = "0FB5"
sch_tblPktId = 4021
msg_tblAppId = "0FB4"
msg_tblPktId = 4020

write ";*********************************************************************"
write ";  Step 1.0:  Initialize the CPU for this test. "
write ";*********************************************************************"
write ";  Step 1.1:  Command a Power-On Reset on $CPU. "
write ";********************************************************************"
/$SC_$CPU_ES_POWERONRESET
wait 10

close_data_center
wait 60

cfe_startup {hostCPU}
wait 5

write ";*********************************************************************"
write ";  Step 1.2: Determine if the SCH_LAB application is running. If so,  "
write ";  we must delete it in order to start the SCH application. "
write ";**********************************************************************"
s get_file_to_cvt (ramDir,"cfe_es_app_info.log","$sc_$cpu_es_app_info.log",hostCPU)

local found_app = FALSE

;Loop thru the table looking for the SCH_LAB_APP
for file_index = 1 to CFE_ES_MAX_APPLICATIONS do
  if ($SC_$CPU_ES_ALE[file_index].ES_AL_AppName = SCHLABAppName) then
    found_app = TRUE
  endif
enddo

if (found_app = TRUE) then
  /$SC_$CPU_ES_DELETEAPP Application=SCHLABAppName
  wait 10
endif

write ";*********************************************************************"
write ";  Step 1.3: Display Housekeeping pages "
write ";**********************************************************************"
page $SC_$CPU_SCH_HK
page $SC_$CPU_TST_SCH_HK

write ";*********************************************************************"
write ";  Step 1.4: Determine if the Scheduler (SCH) Application is running. "
write ";  If no, start it. If yes, send the restart command. Also add any "
write ";  required subscriptions.  "
write ";********************************************************************"
found_app = FALSE

;Loop thru the table looking for the Scheduler Application
for file_index = 1 to CFE_ES_MAX_APPLICATIONS do
  if ($SC_$CPU_ES_ALE[file_index].ES_AL_AppName = SCHAppName) then
    found_app = TRUE
  endif
enddo

ut_setupevents "$SC", "$CPU", "CFE_ES", CFE_ES_START_INF_EID, "INFO", 1
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_INITSTATS_INF_EID, "INFO", 2

if (found_app = TRUE) then
  /$SC_$CPU_ES_RESTARTAPP Application=SCHAppName

  ut_tlmwait $SC_$CPU_find_event[2].num_found_messages, 1
  IF (UT_TW_Status = UT_Success) THEN
    write "<*> Passed (9001) - SCH Application Started"
    ut_setrequirements SCH_9001, "P" 
  else
    write "<!> Failed (9001) - SCH Application start Event Message not received."
    ut_setrequirements SCH_9001, "F"
  endif
else
  ;; Create and Load the default tables just in case they don't exist
  s $SC_$CPU_sch_sdtloadfile
  s $SC_$CPU_sch_mdtloadfile

  s load_start_app (SCHAppName,hostCPU,"SCH_AppMain")

  ; Wait for app startup events
  ut_tlmwait $SC_$CPU_find_event[2].num_found_messages, 1
  IF (UT_TW_Status = UT_Success) THEN
    write "<*> Passed (9001) - SCH Application Started"
    ut_setrequirements SCH_9001, "P" 
    if ($SC_$CPU_find_event[1].num_found_messages = 1) then
      write "<*> Passed - CFE_ES Event Message rcv'd"
    else
      write "<!> Failed - CFE_ES start Event Message for SCH not received."
    endif
  else
    write "<!> Failed (9001) - SCH Application start Event Message not received."
    ut_setrequirements SCH_9001, "F"
  endif
endif

;; Need to set the stream based upon the cpu being used
;; CPU1 is the default
stream1 = x'0897'

write "Sending command to add subscription for SCH HK packet."
/$SC_$CPU_TO_ADDPACKET Stream=stream1 Pkt_Size=x'0' Priority=x'0' Reliability=x'1' Buflimit=x'4'
wait 10

write ";**********************************************************************"
write ";  Step 1.5: Start the SCH Test Application (TST_SCH) and "
write ";  add any required subscriptions.  "
write ";**********************************************************************"
ut_setupevents "$SC", "$CPU", "CFE_ES", CFE_ES_START_INF_EID, "INFO", 1
ut_setupevents "$SC", "$CPU", "TST_SCH", TST_SCH_INITSTATS_INF_EID, "INFO", 2

s load_start_app ("TST_SCH",hostCPU,"TST_SCH_AppMain")
                                                                                
; Wait for app startup events
ut_tlmwait  $SC_$CPU_find_event[2].num_found_messages, 1
IF (UT_TW_Status = UT_Success) THEN
  if ($SC_$CPU_find_event[1].num_found_messages = 1) then
    write "<*> Passed - TST_SCH Application Started"
  else
    write "<!> Failed - CFE_ES start Event Message for TST_SCH not received."
    write "Event Message count = ",$SC_$CPU_num_found_messages
  endif
else
  write "<!> Failed - TST_SCH Application start Event Message not received."
endif

;;; Need to set the stream based upon the cpu being used (using the TST_SCH HK Packet IDs)
;;; CPU1 is the default
stream1 = x'0936'

write "Sending command to add subscription for TST_SCH HK packet."
/$SC_$CPU_TO_ADDPACKET Stream=stream1 Pkt_Size=x'0' Priority=x'0' Reliability=x'1' Buflimit=x'4'
wait 10

;; Enable Entry #8 - Slot 1 - entry 3
;; TST_SCH_SEND_HK request
/$SC_$CPU_SCH_EnableEntry SlotNumber=1 EntryNumber=3
wait 5

write ";*********************************************************************"
write ";  Step 1.6: Enable DEBUG Event Messages "
write ";*********************************************************************"
local cmdCtr = $SC_$CPU_EVS_CMDPC + 2
  
;; Enable DEBUG events for the appropriate applications ONLY
/$SC_$CPU_EVS_EnaAppEVTType Application=SCHAppName DEBUG
wait 1
/$SC_$CPU_EVS_EnaAppEVTType Application="CFE_TBL" DEBUG
  
ut_tlmwait $SC_$CPU_EVS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Enable Debug events command sent properly."
else
  write "<!> Failed - Enable Debug events command."
endif

write ";*********************************************************************"
write ";  Step 1.7: Verify that the SCH Housekeeping telemetry items are "
write ";  initialized to zero (0). "
write ";*********************************************************************"
;; Add the HK message receipt test
local hkPktId

;; Set the HK packet ID based upon the cpu being used
;; CPU1 is the default
hkPktId = "p097"

;; Verify the HK Packet is getting generated by waiting for the
;; sequencecount to increment twice
local seqTlmItem = hkPktId & "scnt"
local currSCnt = {seqTlmItem}
local expectedSCnt = currSCnt + 2

ut_tlmwait {seqTlmItem}, {expectedSCnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (8000) - Housekeeping packet is being generated."
  ut_setrequirements SCH_8000, "P"
else
  write "<!> Failed (8000) - Housekeeping packet sequence count did not increment. Housekeeping packet is not being recieved."
  ut_setrequirements SCH_8000, "F"
endif

;; Check the HK tlm items to see if they are 0 or NULL by inspection
;; since the SCH application sends its HK packet too fast (300ms)
;;write "<*> Verify by inspection of SCH_HK Page"
;;ut_setrequirements SCH_9000, "I"

;; Check the HK tlm items to see if they are initialized properly
if ($SC_$CPU_SCH_CMDPC = 1) AND ($SC_$CPU_SCH_CMDEC = 0) AND ;;
   ($SC_$CPU_SCH_SKIPSLOTCTR = 0) AND ;;
   ($SC_$CPU_SCH_ACTFAILURECTR = 0) AND ;;
   ($SC_$CPU_SCH_BADTBLDATACTR = 0) AND ($SC_$CPU_SCH_SYNCTOMET = 1) THEN
  write "<*> Passed (9000) - Housekeeping telemetry initialized properly."
  ut_setrequirements SCH_9000, "P"
else
  write "<!> Failed (9000) - Housekeeping telemetry NOT initialized properly at startup."
  ut_setrequirements SCH_9000, "F"
  write "CMDPC = ", $SC_$CPU_SCH_CMDPC
  write "CMDEC = ", $SC_$CPU_SCH_CMDEC
  write "Slots Skipped = ", $SC_$CPU_SCH_SKIPSLOTCTR
  write "Bad Table Entries = ", $SC_$CPU_SCH_BADTBLDATACTR
  write "Failed Activities =  ", $SC_$CPU_SCH_ACTFAILURECTR
  write "Sync to MET =  ", $SC_$CPU_SCH_SYNCTOMET
endif

write "========= Other HK Telemetry =========="
write "Slots Processed = ", $SC_$CPU_SCH_SLOTPROCCTR
write "Multi Slots Processed = ", $SC_$CPU_SCH_MULTSLOTCTR
write "Same Slot = ", $SC_$CPU_SCH_SAMESLOTCTR
write "Successful Activities =  ", $SC_$CPU_SCH_ACTSUCCESSCTR
write "TBL Loads Passed =  ", $SC_$CPU_SCH_TBLPASSVERIFYCTR
write "TBL Loads Failed =  ", $SC_$CPU_SCH_TBLFAILVERIFYCTR
write "TBL Processed =  ", $SC_$CPU_SCH_TBLPROCCTR
write "Valid Major Frame tones =  ", $SC_$CPU_SCH_VALIDMFCTR
write "Missed Major Frame tones =  ", $SC_$CPU_SCH_MISSMFCTR
write "Unexp Major Frame tones =  ", $SC_$CPU_SCH_UNEXPCTDMFCTR
write "Minor Frames since tone =  ", $SC_$CPU_SCH_MINORSINCETONE
write "Next Slot to be processed =  ", $SC_$CPU_SCH_NEXTSLOT
write "Last Synch Slot =  ", $SC_$CPU_SCH_LASTSYNCMETSLOT
write "Unexpected Major Frame =  ", $SC_$CPU_SCH_UNEXPCTDMAJORFRAME
write "Major Frame Source =  ", $SC_$CPU_SCH_MAJORFRAMESOURCE

wait 5

;; Determine if Library is present or not
if (SCH_LIB_PRESENCE = 0) then
  write "*** Schedule Library is not present. Skipping Step 2.0"
  goto step3_0
endif
 
write ";*********************************************************************"
write ";  Step 2.0: Validate Schedule Library Functions "
write ";*********************************************************************"
write ";  Step 2.1: Get the processing state of the Scheduler Application "
write ";*********************************************************************"
ut_setupevents "$SC","$CPU","TST_SCH",TST_SCH_GET_SCH_STATE1_INF_EID,"INFO", 1

cmdCtr = $SC_$CPU_TST_SCH_CMDPC + 1

;; Send the command
/$SC_$CPU_TST_SCH_GetSchState

ut_tlmwait $SC_$CPU_TST_SCH_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Get Scheduler Processing State command sent successfully."
else
  write "<!> Failed - Get Scheduler Processing State command did not execute successfully."
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) then
  write "<*> Passed - Event Msg ",$SC_$CPU_find_event[1].eventid," Found!"
else
  write "<!> Failed - Expected Event Message not received."
endif

;; Enable NOOP command group
/$SC_$CPU_SCH_EnableGroup GroupData=2

wait 5

write ";*********************************************************************"
write ";  Step 2.2: Disable processing for the Scheduler Application and "
write ";  verify that activities are not being processed. "
write ";*********************************************************************"
ut_setupevents "$SC","$CPU","TST_SCH",TST_SCH_DISABLE_SCH_INF_EID,"INFO", 1

;; Send the command
/$SC_$CPU_TST_SCH_DisableSchProc

;; Wait for the event message since the above command turns off the HK request
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Event Msg ",$SC_$CPU_find_event[1].eventid," Found!"
else
  write "<!> Failed - Expected Event Message not received."
endif

wait 5

write ";*********************************************************************"
write ";  Step 2.3: Get the processing state of the Scheduler Application "
write ";*********************************************************************"
ut_setupevents "$SC","$CPU","TST_SCH",TST_SCH_GET_SCH_STATE2_INF_EID,"INFO", 1

;; Send the command
/$SC_$CPU_TST_SCH_GetSchState

;; Wait for the event message since the HK request will not be rcv'd 
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Event Msg ",$SC_$CPU_find_event[1].eventid," Found!"
else
  write "<!> Failed - Expected Event Message not received."
endif

wait 5

write ";*********************************************************************"
write ";  Step 2.4: Enable processing for the Scheduler Application and "
write ";  verify that activity processing resumes. "
write ";*********************************************************************"
ut_setupevents "$SC","$CPU","TST_SCH",TST_SCH_ENABLE_SCH_INF_EID,"INFO", 1

;; Send the command
/$SC_$CPU_TST_SCH_EnableSchProc

;; Wait for the event message since the enable command will start the NOOP
;; commands executing which would also increment the TST_SCH_CMDPC 
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Event Msg ",$SC_$CPU_find_event[1].eventid," Found!"
else
  write "<!> Failed - Expected Event Message not received."
endif

;; Wait to see if the NOOP event message start appearing again
wait 10

;; Disable the NOOP group
/$SC_$CPU_SCH_DisableGroup GroupData=2
wait 5

;; Skip the Library not present steps
goto step4_0

step3_0:
write ";*********************************************************************"
write ";  Step 3.0: Send Schedule Library Commands when library is not present"
write ";*********************************************************************"
write ";  Step 3.1: Send the Get Processing State command. "
write ";*********************************************************************"
ut_setupevents "$SC","$CPU","TST_SCH",TST_SCH_NO_SCHLIB_INF_EID,"INFO", 1

;; Send the command
/$SC_$CPU_TST_SCH_GetSchState

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Event Msg ",$SC_$CPU_find_event[1].eventid," Found!"
else
  write "<!> Failed - Expected Event Message not received."
endif

write ";*********************************************************************"
write ";  Step 3.2: Send the Disable Processing State command. "
write ";*********************************************************************"
ut_setupevents "$SC","$CPU","TST_SCH",TST_SCH_NO_SCHLIB_INF_EID,"INFO", 1

;; Send the command
/$SC_$CPU_TST_SCH_DisableSchProc

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Event Msg ",$SC_$CPU_find_event[1].eventid," Found!"
else
  write "<!> Failed - Expected Event Message not received."
endif

write ";*********************************************************************"
write ";  Step 3.3: Send the Disable Processing State command. "
write ";*********************************************************************"
ut_setupevents "$SC","$CPU","TST_SCH",TST_SCH_NO_SCHLIB_INF_EID,"INFO", 1

;; Send the command
/$SC_$CPU_TST_SCH_EnableSchProc

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Event Msg ",$SC_$CPU_find_event[1].eventid," Found!"
else
  write "<!> Failed - Expected Event Message not received."
endif

step4_0:
write ";*********************************************************************"
write ";  Step 4.0:  Perform a Power-on Reset to clean-up from this test."
write ";*********************************************************************"
/$SC_$CPU_ES_POWERONRESET
wait 10

close_data_center
wait 60

cfe_startup {hostCPU}
wait 5

write "**** Requirements Status Reporting"
                                                                                
write "--------------------------"
write "   Requirement(s) Report"
write "--------------------------"

FOR i = 0 to ut_req_array_size DO
  ut_pfindicate {cfe_requirements[i]} {ut_requirement[i]}
ENDDO

; clear global variables
drop ut_requirement 
drop ut_req_array_size 

write ";*********************************************************************"
write ";  End procedure $SC_$CPU_sch_lib_test "
write ";*********************************************************************"
ENDPROC
