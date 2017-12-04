PROC $sc_$cpu_sch_timing_analysis
;*******************************************************************************
;  Test Name:  sch_timing_analysis
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	This test verifies that the CFS SCH responds correctly to changes in
;	schedule and message definition tables. It will also verify the timing
;	of the SCH schedule and message definition table processing.
;
;  Requirements Tested
;    SCH1002    For all SCH commands, if the length contained in the message
;               header is not equal to the expected length, SCH shall reject the
;               command and issue an event message.
;    SCH1004    If SCH accepts any command as valid, SCH shall execute the
;               command, increment the SCH Valid Command Counter and issue an
;               event message.
;    SCH1005    If SCH rejects any command, SCH shall abort the command
;               execution, increment the SCH Command Rejected Counter and issue
;               an error event message.
;    SCH2001	The Schedule Definition Table shall schedule activities with a
;		minimum minor frame resolution of <PLATFORM_DEFINED, 10>
;		milliseconds and a major frame resolution of
;		<PLATFORM_DEFINED, 1> second. 
;    SCH3000	For each Enabled activity defined in the SDT, SCH shall send the
;		specified message defined in the Message Definition Table (MDT)
;		via a Software Bus message. 
;    SCH3001	SCH shall associate each activity with a single Group and zero
;		to 24 Multi-Groups.
;    SCH3001.1	SCH shall support a maximum of 255 Groups
;    SCH3002	SCH shall support a maximum of <PLATFORM_DEFINED, 255> messages
;		in the Message Definition Table
;    SCH3003	SCH shall allow the user to combine one or more groups of
;		activities in the Schedule Definition Table into one or more
;		Multi-Groups.
;    SCH3003.1	SCH shall support a maximum of 24 Multi-Groups.
;    SCH4000	Upon receipt of an Enable Activity Command, SCH shall enable the
;		command-specified activity defined in the Schedule Definition
;		Table
;    SCH4000.1	If an Unused Activity definition is specified by the command,
;		the command shall be rejected
;    SCH4000.2	If the command specified activity identifier is out of range,
;		the command shall be rejected 
;    SCH4001	Upon receipt of a Disable Activity Command, SCH shall disable
;		further processing of the command-specified activity defined in
;		the Schedule Definition Table
;    SCH4001.1	If an Unused Activity definition is specified by the command,
;		the command shall be rejected
;    SCH4001.2	If the command specified activity identifier is out of range,
;		the command shall be rejected 
;    SCH4002	Upon receipt of an Enable Group Command, SCH shall enable all
;		the activities associated with the command-specified Group
;		and/or Multi-Group
;    SCH4002.1	If no Group or Multi-Group is specified then the command shall
;		be rejected
;    SCH4003	Upon receipt of a Disable Group Command, SCH shall disable
;		further processing of all activities associated with the
;		command-specified Group and/or Multi-Group.
;    SCH4003.1	If no Group or Multi-Group is specified then the command shall
;		be rejected
;    SCH4004	Upon receipt of a SCH Diagnostic Command, SCH shall produce a
;		Diagnostic Message containing, at a minimum, the Activity State
;		for all Activities.
;    SCH4005	Upon receipt of a SCH Enable Major Frame Synchronization
;		Command, SCH shall synchronize its Scheduler Definition Table
;		(SDT) containing defined activities to the next Major Frame
;		Signal.
;    SCH9000	Upon any Initialization of the SCH Application (cFE Power On,
;		cFE Processor Reset or SCH Application Reset), SCH shall
;		initialize the following data to Zero: 
;		  a) Valid Ground Command Counter
;		  b) Ground Command Rejected Counter
;		  e) Valid commands sent by the SCH APP
;		  f) Commands sent by SCH APP that were reported as erroneous
;		     by the Software Bus
;		  g) Number of Slots processed
;		  h) Number of Times Slots skipped
;		  i) Number of Times Multiple Slots processed
;		  j) Number of times that SCH woke up in the same slot as
;		     last time
;		  k) Number of bad entries found in the Scheduler table
;		     (indication of corrupted table)
;		  l) Synchronization Status (not flywheeling)
;    SCH9001	Upon any Initialization, the SCH Application shall inhibit
;		processing of the Schedule Definition Table until the cFE
;		indicates that all of the applications have started. 
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
;	Date	  Name			Description
;	01/30/09  Ezinne Uzo-Okoro	Original Procedure.
;	03/15/10  Walt Moleski		Updated comments to contain the
;					proper line feeds and to support
;					0-based array for MDT and SDT
;	07/28/11  Walt Moleski		Added variables for App Names,
;					table names and ram directory
;	08/08/11  Walt Moleski		Re-designed this procedure
;       08/02/12  Walt Moleski		Added code to look for the
;					Scheduler app executing.
;       06/13/17  W. Moleski		Updated to use CPU1 for commanding and
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
;      ut_setrequirements	Directive to status LRO requirements. 
;      ut_setevents             Directive to status LRO event messages.
;      get_tbl_to_cvt           Procedure to generate and download table file to
;				cvt.
;      create_tbl_file_from_cvt Procedure to generate a table file from the
;				contents of the cvt.
;      ftp_file			Procedure to upload file/table from Ground to
;				CPU 
;      load_table               Procedure to load a table from Ground to CPU.
;      ut_tlmupdate             Directive to wait for telemetry packet update.
;
;  Expected Test Results and Analysis
;
;  Verify that correct event messages are received on the ground. 
;  Verify that changes to scheduler and command tables are accepted and are
;  executed.
;
;**********************************************************************
local logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

#include "ut_statusdefs.h"
#include "ut_cfe_info.h"
#include "cfe_platform_cfg.h"
#include "cfe_es_events.h"
#include "cfe_evs_events.h"
#include "cfe_sb_events.h"
#include "cfe_tbl_events.h"
#include "to_lab_events.h"
#include "ci_lab_events.h"
#include "sch_platform_cfg.h"
#include "sch_events.h"
#include "sch_msgdefs.h"
#include "tst_sch_events.h"

%liv (log_procedure) = logging

#define SCH_1002     0
#define SCH_1004     1
#define SCH_1005     2
#define SCH_2001     3
#define SCH_3000     4
#define SCH_3001     5 
#define SCH_3001_1   6
#define SCH_3002     7
#define SCH_3003     8 
#define SCH_3003_1   9
#define SCH_4000    10 
#define SCH_4000_1  11
#define SCH_4000_2  12
#define SCH_4001    13 
#define SCH_4001_1  14
#define SCH_4001_2  15
#define SCH_4002    16 
#define SCH_4002_1  17
#define SCH_4003    18 
#define SCH_4003_1  19
#define SCH_4004    20
#define SCH_4005    21
#define SCH_8000    22
#define SCH_9000    23
#define SCH_9001    24

#define SCH_NUM_TEST          x'51'
#define SCH_GROUP_TEST        x'008000'
#define SCH_GRP_ALL           x'ff'

global SCH_EnableState
global SCH_SE_Type
global SCH_SE_Frequency
global SCH_Remainder
global SCH_MessageIndex
global SCH_GroupData

global ut_req_array_size = 24
global ut_requirement[0 .. ut_req_array_size]

for i = 0 to ut_req_array_size DO
  ut_requirement[i] = "U"
enddo

;**********************************************************************
; Set the local values
;**********************************************************************
local cfe_requirements[0 .. ut_req_array_size] = ["SCH_1002","SCH_1004","SCH_1005","SCH_2001","SCH_3000","SCH_3001","SCH_3001.1","SCH_3002","SCH_3003","SCH_3003.1","SCH_4000","SCH_4000.1","SCH_4000.2","SCH_4001","SCH_4001.1","SCH_4001.2","SCH_4002","SCH_4002.1","SCH_4003","SCH_4003.1","SCH_4004","SCH_4005","SCH_8000","SCH_9000","SCH_9001"]

;**********************************************************************
; Define local variables
;**********************************************************************
LOCAL rawcmd
LOCAL stream1
LOCAL sch_tblAppId
LOCAL sch_tblPktId
LOCAL msg_tblAppId
LOCAL msg_tblPktId

local SCH_slot
local SCH_entry
local SCH_CSLOT
local slot
local entry
local SCH_TFREQ
local SCH_TREM
local SCH_TSLOT
local SCH_TESTGROUP

local previous_SCH_EnableState[0..7]
local previous_SCH_SE_Type[0..7]
local previous_SCH_SE_Frequency[0..7]
local previous_SCH_Remainder[0..7]
local previous_SCH_MessageIndex[0..7]
local previous_SCH_GroupData[0..7]

local current_SCH_EnableState[0..7]
local current_SCH_SE_Type[0..7]
local current_SCH_SE_Frequency[0..7]
local current_SCH_Remainder[0..7]
local current_SCH_MessageIndex[0..7]
local current_SCH_GroupData[0..7]

local capture_index

local SCHAppName = "SCH"
local SCHLabAppName = "SCH_LAB_APP"
local ramDir = "RAM:0"
local SCHDefTblName = SCHAppName & ".SCHED_DEF"
local MSGDefTblName = SCHAppName & ".MSG_DEFS"
local hostCPU = "$CPU"

;;; Set the pkt and app IDs for the tables based upon the cpu being used
;;; Right now, the pktIDs are not used
;;; CPU1 is the default
sch_tblAppId = "0FB5"
sch_tblPktId = 4021
msg_tblAppId = "0FB4"
msg_tblPktId = 4020
local unusedMID = x'1897'

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
start get_file_to_cvt (ramDir, "cfe_es_app_info.log", "$sc_$cpu_es_app_info.log", hostCPU)

local found_app = FALSE

;Loop thru the table looking for the SCH_LAB_APP
for file_index = 1 to CFE_ES_MAX_APPLICATIONS do
  if ($SC_$CPU_ES_ALE[file_index].ES_AL_AppName = SCHLabAppName) then
    found_app = TRUE
  endif
enddo

if (found_app = TRUE) then
  /$SC_$CPU_ES_DELETEAPP Application=SCHLabAppName
  wait 10
endif

write ";**********************************************************************"
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
  s $SC_$CPU_sch_sdtloadfile
  s $SC_$CPU_sch_mdtloadfile

  s load_start_app (SCHAppName,hostCPU,"SCH_AppMain")

  ; Wait for app startup events
  ut_tlmwait $SC_$CPU_find_event[2].num_found_messages, 1
  IF (UT_TW_Status = UT_Success) THEN
    if ($SC_$CPU_find_event[1].num_found_messages = 1) then
      write "<*> Passed - SCH Application Started"
      ut_setrequirements SCH_9001, "P" 
    else
      write "<!> Failed - CFE_ES start Event Message for SCH not received."
    endif
  else
    write "<!> Failed - SCH Application start Event Message not received."
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
wait 3

; Wait for app startup events
ut_tlmwait $SC_$CPU_find_event[2].num_found_messages, 1
IF (UT_TW_Status = UT_Success) THEN
  if ($SC_$CPU_find_event[1].num_found_messages = 1) then
    write "<*> Passed - TST_SCH Application Started"
  else
    write "<!> Failed - CFE_ES start Event Message for TST_SCH not received."
    write "Event Message count = ",$SC_$CPU_find_event[1].num_found_messages
  endif
else
  write "<!> Failed - TST_SCH Application start Event Message not received."
endif

;;; Need to set the stream based upon the cpu being used
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
write ";  Step 1.6: Verify that the SCH Housekeeping telemetry items are "
write ";  initialized to zero (0). "
write ";*********************************************************************"
;; Set the HK packet ID based upon the cpu being used
;; CPU1 is the default
local hkPktId = "p097"

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

write ";*********************************************************************"
write ";  Step 1.7: Enable DEBUG Event Messages "
write ";*********************************************************************"
local cmdCtr = $SC_$CPU_EVS_CMDPC + 2

;; Enable DEBUG events for the appropriate applications ONLY
/$SC_$CPU_EVS_EnaAppEVTType Application=SCHAppName DEBUG
wait 2
/$SC_$CPU_EVS_EnaAppEVTType Application="CFE_TBL" DEBUG

ut_tlmwait $SC_$CPU_EVS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Enable Debug events command sent properly."
else
  write "<!> Failed - Enable Debug events command."
endif

write ";*********************************************************************"
write ";  Step 1.8: Enable Major Frame Synchronization"
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_ENA_SYNC_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_SCH_CMDPC + 1

/$SC_$CPU_SCH_ENABLESYNC

ut_tlmwait $SC_$CPU_SCH_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4005) - Major Frame Synchronization Command was executed as expected"
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4005, "P"
ELSE
  write "<!> Failed (1004;4005) - Major Frame Synchronization Command did not increment the CMDPC"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4005, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 1.9: Verify the Message Definition Table Size "
write ";*********************************************************************"
write
write "Message Definition Table Used Entries"
FOR i = 0 to SCH_MAX_MESSAGES-1 DO
  if ($SC_$CPU_SCH_DefaultMessageTable[i].MessageBuffer[1] <> unusedMID) then
    write "Entry ", i, " MID = ", %hex($SC_$CPU_SCH_DefaultMessageTable[i].MessageBuffer[1],4)
  endif
ENDDO

;; if we get here without error, Pass the requirement
ut_setrequirements SCH_3002, "P"

write ";*********************************************************************"
write ";  Step 2.0: SCH Timing Analysis tests."
write ";*********************************************************************"
write ";  Step 2.1: Enable the NOOP Group and verify the timing of activities."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_ENA_GRP_CMD_EID, "DEBUG", 1
ut_setupevents "$SC", "$CPU", "TO_LAB_APP", TO_NOOP_INF_EID, "INFO", 2

;; Need to enable NOOP commands
/$SC_$CPU_SCH_EnableGroup GroupData=2

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4002) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4002, "P"
else
  write "<!> Failed (1004;4002) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4002, "F"
endif

;; Set the requirement to 'A' in order to do some post-test analysis
ut_setrequirements SCH_2001, "A"

;; Wait for some Noops to execute
wait 10

if ($SC_$CPU_find_event[2].num_found_messages > 0) then
  write "<*> Passed (3000) - TO_NOOP Event messages are being generated"
  ut_setrequirements SCH_3000, "P"
else
  write "<!> Failed (3000) - Did not receive any TO_NOOP Event messages"
  ut_setrequirements SCH_3000, "F"
endif

ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_DIS_GRP_CMD_EID, "DEBUG", 1

;; Disable the NOOP group
/$SC_$CPU_SCH_DisableGroup GroupData=2

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4003) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4003, "P"
else
  write "<!> Failed (1004;4003) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4003, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.0: Group Tests "
write ";*********************************************************************"
write ";  Step 3.1: Load the Schedule Definition Table for this test. "
write ";*********************************************************************"
s $SC_$CPU_sch_sdt3

;; Load the table
ut_setupevents "$SC","$CPU","CFE_TBL",CFE_TBL_FILE_LOADED_INF_EID,"INFO", 1

cmdCtr = $SC_$CPU_TBL_CMDPC + 1

s load_table ("sch_def_sdt3.tbl",hostCPU)

ut_tlmwait $SC_$CPU_TBL_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Load command for SDT3 sent successfully."
else
  write "<!> Failed - Load command for SDT3 did not execute successfully."
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) then
  write "<*> Passed - Event Msg ",$SC_$CPU_find_event[1].eventid," Found!"
else
  write "<!> Failed - Event Message not received for Load command."
endif

write ";*********************************************************************"
write ";  Step 3.2: Validate the SDT loaded above. "
write ";*********************************************************************"
ut_setupevents "$SC","$CPU","CFE_TBL",CFE_TBL_VAL_REQ_MADE_INF_EID, "DEBUG", 1
ut_setupevents "$SC","$CPU","CFE_TBL",CFE_TBL_VALIDATION_INF_EID, "INFO", 2

cmdCtr = $SC_$CPU_TBL_CMDPC + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=SCHDefTblName

ut_tlmwait $SC_$CPU_TBL_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - SDT validate command sent."
else
  write "<!> Failed - Table ",SCHDefTblName," validation failed."
endif

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Event Msg ",$SC_$CPU_find_event[1].eventid," Found!"
else
  write "<!> Failed - Event Message not received for Validate command."
endif

;; Wait for the validation message
;; If the message is rcv'd, then validation passed
ut_tlmwait $SC_$CPU_find_event[2].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - SDT Validation was successful. Event Msg ",$SC_$CPU_find_event[2].eventid," rcv'd."
else
  write "<!> Failed - RTS Table validation failed. Expected Event Msg ",CFE_TBL_VALIDATION_INF_EID, " not rcv'd."
endif

write ";*********************************************************************"
write ";  Step 3.3: Activate the SDT loaded above. "
write ";*********************************************************************"
ut_setupevents "$SC","$CPU","CFE_TBL",CFE_TBL_LOAD_PEND_REQ_INF_EID,"DEBUG",1
ut_setupevents "$SC","$CPU","CFE_TBL",CFE_TBL_UPDATE_SUCCESS_INF_EID,"INFO",2

cmdCtr = $SC_$CPU_TBL_CMDPC + 1

/$SC_$CPU_TBL_ACTIVATE ATableName=SCHDefTblName

ut_tlmwait $SC_$CPU_TBL_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Activate SDT Table ",SCHDefTblName," command sent properly."
else
  write "<!> Failed - Activate SDT Table ",SCHDefTblName," command."
endif

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Event message ",$SC_$CPU_find_event[1].eventid, " received"
else
  write "<!> Failed - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",CFE_TBL_LOAD_PEND_REQ_INF_EID, "."
endif

ut_tlmwait $SC_$CPU_find_event[2].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - SDT Table Updated successfully. Event Msg ",$SC_$CPU_find_event[2].eventid," rcv'd."
else
  write "<!> Failed  - SDT Table update failed. Event Msg not received for activate command."
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.4: Send the Enable Group command without arguments"
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_ENA_GRP_CMD_ERR_EID, "ERROR", 1

local errCtr = $SC_$CPU_SCH_CMDEC + 1

; enable group without argument
/$SC_$CPU_SCH_EnableGroup GroupData=""

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;4002.1) - Enable Group command was rejected as expected"
  ut_setrequirements SCH_1005, "P"
  ut_setrequirements SCH_4002_1, "P"
ELSE
  write "<!> Failed (1005;4002.1) - Enable Group command was not rejected as expected"
  ut_setrequirements SCH_1005, "F"
  ut_setrequirements SCH_4002_1, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.5: Enable Group 1. This should only enable the TO_NOOP cmd "
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_ENA_GRP_CMD_EID, "DEBUG", 1
ut_setupevents "$SC", "$CPU", "TO_LAB_APP", TO_NOOP_INF_EID, "INFO", 2

cmdCtr = $SC_$CPU_SCH_CMDPC + 1

; enable group with argument
/$SC_$CPU_SCH_EnableGroup GroupData=x'01000000'

ut_tlmwait $SC_$CPU_SCH_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4002) - Enable Group command sent properly."
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4002, "P"
ELSE
  write "<!> Failed (1004;4002) - Enable Group command did not increment CMDPC"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4002, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
ELSE
  write "<!> Failed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[2].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (3001) - TO_NOOP Event message received. Verifies grouping of activities."
  ut_setrequirements SCH_3001, "P"
ELSE
  write "<!> Failed (3001) - Expected TO_NOOP Event message not received"
  ut_setrequirements SCH_3001, "F"
ENDIF

wait 5 

write ";*********************************************************************"
write ";  Step 3.6: Enable Group 255. This should enable the SB_NOOP cmd "
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_ENA_GRP_CMD_EID, "DEBUG", 1
ut_setupevents "$SC", "$CPU", "CFE_SB", CFE_SB_CMD0_RCVD_EID, "INFO", 2

cmdCtr = $SC_$CPU_SCH_CMDPC + 1

; enable group with argument
/$SC_$CPU_SCH_EnableGroup GroupData=x'FF000000'

ut_tlmwait $SC_$CPU_SCH_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4002) - Enable Group command sent properly."
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4002, "P"
ELSE
  write "<!> Failed (1004;4002) - Enable Group command did not increment CMDPC"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4002, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
ELSE
  write "<!> Failed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[2].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (3001;3001.1) - SB_NOOP Event message received. Verifies grouping of activities."
  ut_setrequirements SCH_3001, "P"
  ut_setrequirements SCH_3001_1, "P"
ELSE
  write "<!> Failed (3001;3001.1) - Expected SB_NOOP Event message not received"
  ut_setrequirements SCH_3001, "F"
  ut_setrequirements SCH_3001_1, "F"
ENDIF

wait 5 

write ";*********************************************************************"
write ";  Step 3.7: Enable Multi-Group 1. This should enable the TST_SCH_NOOP "
write ";  and CI_NOOP commands. "
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_ENA_GRP_CMD_EID, "DEBUG", 1
ut_setupevents "$SC", "$CPU", "TST_SCH", TST_SCH_NOOP_INF_EID, "INFO", 2
ut_setupevents "$SC", "$CPU", "CI_LAB_APP", CI_COMMANDNOP_INF_EID, "INFO", 3

cmdCtr = $SC_$CPU_SCH_CMDPC + 1

; enable group with argument
/$SC_$CPU_SCH_EnableGroup GroupData=1

ut_tlmwait $SC_$CPU_SCH_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4002) - Enable Group command sent properly."
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4002, "P"
ELSE
  write "<!> Failed (1004;4002) - Enable Group command did not increment CMDPC"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4002, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
ELSE
  write "<!> Failed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
ENDIF

;; Wait for both NOOP messages
wait 10

if ($SC_$CPU_find_event[2].num_found_messages > 0) then
  write "<*> Passed (3003) - TST_SCH_NOOP Event message received. Verifies grouping of activities."
  ut_setrequirements SCH_3003, "P"
ELSE
  write "<!> Failed (3003) - Expected TST_SCH_NOOP Event message not received"
  ut_setrequirements SCH_3003, "F"
ENDIF

if ($SC_$CPU_find_event[3].num_found_messages > 0) then
  write "<*> Passed (3003) - CI_NOOP Event message received. Verifies grouping of activities."
  ut_setrequirements SCH_3003, "P"
ELSE
  write "<!> Failed (3003) - Expected CI_NOOP Event message not received"
  ut_setrequirements SCH_3003, "F"
ENDIF

wait 5 

write ";*********************************************************************"
write ";  Step 3.8: Enable Multi-Group 24. This should enable the ES_NOOP "
write ";  and EVS_NOOP commands. "
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_ENA_GRP_CMD_EID, "DEBUG", 1
ut_setupevents "$SC", "$CPU", "CFE_ES", CFE_ES_NOOP_INF_EID, "INFO", 2
ut_setupevents "$SC", "$CPU", "CFE_EVS", CFE_EVS_NOOP_EID, "INFO", 3

cmdCtr = $SC_$CPU_SCH_CMDPC + 1

; enable group with argument
/$SC_$CPU_SCH_EnableGroup GroupData=x'800000'

ut_tlmwait $SC_$CPU_SCH_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4002) - Enable Group command sent properly."
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4002, "P"
ELSE
  write "<!> Failed (1004;4002) - Enable Group command did not increment CMDPC"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4002, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
ELSE
  write "<!> Failed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
ENDIF

;; Wait for both NOOP messages
wait 10

if ($SC_$CPU_find_event[2].num_found_messages > 0) then
  write "<*> Passed (3003;3003.1) - ES_NOOP Event message received. Verifies grouping of activities."
  ut_setrequirements SCH_3003, "P"
  ut_setrequirements SCH_3003_1, "P"
else
  write "<!> Failed (3003;3003.1) - Expected ES_NOOP Event message not received"
  ut_setrequirements SCH_3003, "F"
  ut_setrequirements SCH_3003_1, "F"
endif

if ($SC_$CPU_find_event[3].num_found_messages > 0) then
  write "<*> Passed (3003;3003.1) - EVS_NOOP Event message received. Verifies grouping of activities."
  ut_setrequirements SCH_3003, "P"
  ut_setrequirements SCH_3003_1, "P"
else
  write "<!> Failed (3003;3003.1) - Expected EVS_NOOP Event message not received"
  ut_setrequirements SCH_3003, "F"
  ut_setrequirements SCH_3003_1, "F"
endif

wait 5 

write ";*********************************************************************"
write ";  Step 3.9: Send the Enable Group Command for a non-existing group."
write ";*********************************************************************"
ut_setupevents "$SC","$CPU",{SCHAppName},SCH_ENA_GRP_NOT_FOUND_ERR_EID,"ERROR",1

errCtr = $SC_$CPU_SCH_CMDEC + 1

/$SC_$CPU_SCH_EnableGroup GroupData=x'F0000000'

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;4002) - Enable Group command failed as expected."
  ut_setrequirements SCH_1005, "P"
  ut_setrequirements SCH_4002, "P"
ELSE
  write "<!> Failed (1005;4002) - Enable Group command did not increment CMDEC"
  ut_setrequirements SCH_1005, "F"
  ut_setrequirements SCH_4002, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
ELSE
  write "<!> Failed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1005, "F"
ENDIF

wait 5

write ";*********************************************************************"
write ";  Step 3.10: Send the EnableGroup command with an invalid length."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_CMD_LEN_ERR_EID, "ERROR", 1

errCtr = $SC_$CPU_SCH_CMDEC + 1

;;; CPU1 is the default
rawcmd = "1895C000000604B3"

ut_sendrawcmd "$SC_$CPU_SCH", (rawcmd)

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - Command Rejected Counter incremented."
  ut_setrequirements SCH_1002, "P"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1002;1005) - Command Rejected Counter did not increment as expected."
  ut_setrequirements SCH_1002, "F"
  ut_setrequirements SCH_1005, "F"
endif

ut_tlmwait $SC_$CPU_num_found_messages, 1
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Expected Event message ",SCH_CMD_LEN_ERR_EID, " not received."
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.11: Disabling Invalid Group"
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_DIS_GRP_CMD_ERR_EID, "ERROR", 1

errCtr = $SC_$CPU_SCH_CMDEC + 1

; disable group without argument
/$SC_$CPU_SCH_DisableGroup GroupData=""
ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;4003.1) - Disable Group command was rejected as expected"
  ut_setrequirements SCH_1005, "P"
  ut_setrequirements SCH_4003_1, "P"
ELSE
  write "<!> Failed (4003.1) - Disable Group command was not rejected as expected"
  ut_setrequirements SCH_1005, "F"
  ut_setrequirements SCH_4003_1, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.12: Disabling Group 1 (TO_NOOP) "
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_DIS_GRP_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_SCH_CMDPC + 1

; disable group with argument
/$SC_$CPU_SCH_DisableGroup GroupData=x'01000000'

ut_tlmwait $SC_$CPU_SCH_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4003) - Disable Group command was executed as expected"
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4003, "P"
ELSE
  write "<!> Failed (1004;4003) - Disable Group command was not executed as expected"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4003, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
endif

;; Setup to capture the TO_NOOP event
ut_setupevents "$SC", "$CPU", "TO_LAB_APP", TO_NOOP_INF_EID, "INFO", 1

wait 10

if ($SC_$CPU_find_event[1].num_found_messages = 0) then
  write "<*> Passed (4003) - The TO_NOOP event msg was not received as expected"
  ut_setrequirements SCH_4003, "P"
else
  write "<!> Failed (4003) - The TO_NOOP message was received after it was disabled."
  ut_setrequirements SCH_4003, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.13: Disabling Multi-Group 1 (TST_SCH_NOOP and CI_NOOP) "
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_DIS_GRP_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_SCH_CMDPC + 1

; disable group with argument
/$SC_$CPU_SCH_DisableGroup GroupData=1

ut_tlmwait $SC_$CPU_SCH_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4003) - Disable Group command was executed as expected"
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4003, "P"
ELSE
  write "<!> Failed (1004;4003) - Disable Group command was not executed as expected"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4003, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
endif

;; Setup to capture NOOP events after Disabling them
ut_setupevents "$SC", "$CPU", "TST_SCH", TST_SCH_NOOP_INF_EID, "INFO", 1
ut_setupevents "$SC", "$CPU", "CI_LAB_APP", CI_COMMANDNOP_INF_EID, "INFO", 2

wait 10

if ($SC_$CPU_find_event[1].num_found_messages = 0) then
  write "<*> Passed (4003) - The TST_SCH_NOOP event msg was not received as expected"
  ut_setrequirements SCH_4003, "P"
else
  write "<!> Failed (4003) - The TST_SCH_NOOP message was received after it was disabled."
  ut_setrequirements SCH_4003, "F"
endif

if ($SC_$CPU_find_event[2].num_found_messages = 0) then
  write "<*> Passed (4003) - The CI_NOOP event msg was not received as expected"
  ut_setrequirements SCH_4003, "P"
else
  write "<!> Failed (4003) - The CI_NOOP message was received after it was disabled."
  ut_setrequirements SCH_4003, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.14: Send the Disable Group Command for a non-existing group."
write ";*********************************************************************"
ut_setupevents "$SC","$CPU",{SCHAppName},SCH_DIS_GRP_NOT_FOUND_ERR_EID,"ERROR",1

errCtr = $SC_$CPU_SCH_CMDEC + 1

/$SC_$CPU_SCH_DisableGroup GroupData=x'F0000000'

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;4002) - Disable Group command failed as expected."
  ut_setrequirements SCH_1005, "P"
  ut_setrequirements SCH_4002, "P"
ELSE
  write "<!> Failed (1005;4002) - Disable Group command did not increment CMDEC"
  ut_setrequirements SCH_1005, "F"
  ut_setrequirements SCH_4002, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
ELSE
  write "<!> Failed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1005, "F"
ENDIF

wait 5

write ";*********************************************************************"
write ";  Step 3.15: Send the DisableGroup command with an invalid length."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_CMD_LEN_ERR_EID, "ERROR", 1

errCtr = $SC_$CPU_SCH_CMDEC + 1

;;; CPU1 is the default
rawcmd = "1895C000000605B2"

ut_sendrawcmd "$SC_$CPU_SCH", (rawcmd)

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - Command Rejected Counter incremented."
  ut_setrequirements SCH_1002, "P"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1002;1005) - Command Rejected Counter did not increment as expected."
  ut_setrequirements SCH_1002, "F"
  ut_setrequirements SCH_1005, "F"
endif

ut_tlmwait $SC_$CPU_num_found_messages, 1
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Expected Event message ",SCH_CMD_LEN_ERR_EID, " not received."
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.0: Enable/Disable Entry Command Tests "
write ";*********************************************************************"
write ";  Step 4.1: Send an EnableEntry command with invalid arguments "
write ";*********************************************************************"
ut_setupevents "$SC","$CPU",{SCHAppName},SCH_ENABLE_CMD_ARG_ERR_EID, "ERROR", 1

errCtr = $SC_$CPU_SCH_CMDEC + 1

; attempt to enable unused activity
/$SC_$CPU_SCH_ENABLEENTRY SlotNumber=25 EntryNumber=25

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr} 
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;4000.2) - Enable activity command was rejected as expected"
  ut_setrequirements SCH_1005, "P"
  ut_setrequirements SCH_4000_2, "P"
ELSE
  write "<!> Failed (1005;4000.2) - Enable activity command was not rejected as expected"
  ut_setrequirements SCH_1005, "F"
  ut_setrequirements SCH_4000_2, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1005, "F"
endif

;; Reset the message counter
ut_setupevents "$SC","$CPU",{SCHAppName},SCH_ENABLE_CMD_ARG_ERR_EID, "ERROR", 1

errCtr = $SC_$CPU_SCH_CMDEC + 1

; enable activity with out-of-range slotnumber
/$SC_$CPU_SCH_ENABLEENTRY SlotNumber=x'cafe' EntryNumber=0

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr} 
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;4000.2) - Enable activity command was rejected as expected"
  ut_setrequirements SCH_1005, "P"
  ut_setrequirements SCH_4000_2, "P"
ELSE
  write "<!> Failed (1005;4000.2) - Enable activity command was not rejected as expected"
  ut_setrequirements SCH_1005, "F"
  ut_setrequirements SCH_4000_2, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.2: Send the EnableEntry command for an unused entry."
write ";*********************************************************************"
ut_setupevents "$SC","$CPU",{SCHAppName},SCH_ENABLE_CMD_ENTRY_ERR_EID, "ERROR", 1

errCtr = $SC_$CPU_SCH_CMDEC + 1

; attempt to enable unused activity
/$SC_$CPU_SCH_ENABLEENTRY SlotNumber=25 EntryNumber=1

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr} 
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;4000.1) - Enable activity command was rejected as expected"
  ut_setrequirements SCH_1005, "P"
  ut_setrequirements SCH_4000_1, "P"
ELSE
  write "<!> Failed (1005;4000.1) - Enable activity command was not rejected as expected"
  ut_setrequirements SCH_1005, "F"
  ut_setrequirements SCH_4000_1, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.3: Send the EnableEntry command with an invalid length."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_CMD_LEN_ERR_EID, "ERROR", 1

errCtr = $SC_$CPU_SCH_CMDEC + 1

;;; CPU1 is the default
rawcmd = "1895C000000602B7"

ut_sendrawcmd "$SC_$CPU_SCH", (rawcmd)

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - Command Rejected Counter incremented."
  ut_setrequirements SCH_1002, "P"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1002;1005) - Command Rejected Counter did not increment as expected."
  ut_setrequirements SCH_1002, "F"
  ut_setrequirements SCH_1005, "F"
endif

ut_tlmwait $SC_$CPU_num_found_messages, 1
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Expected Event message ",SCH_CMD_LEN_ERR_EID, " not received."
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.4: Send a valid EnableEntry command for the TO_NOOP."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_ENABLE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_SCH_CMDPC + 1

/$SC_$CPU_SCH_ENABLEENTRY SlotNumber=2 EntryNumber=0

ut_tlmwait $SC_$CPU_SCH_CMDPC, {cmdCtr} 
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4000) - Enable activity command was sent successfully"
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4000, "P"
ELSE
  write "<!> Failed (1004;4000) - Enable activity command did not increment the CMDPC"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4000, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.5: Send the DisableEntry command with invalid arguments "
write ";*********************************************************************"
ut_setupevents "$SC","$CPU",{SCHAppName},SCH_DISABLE_CMD_ARG_ERR_EID,"ERROR", 1

errCtr = $SC_$CPU_SCH_CMDEC + 1

; disable activity with an invalid entrynumber
/$SC_$CPU_SCH_DisableEntry SlotNumber=25 EntryNumber=25 

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr} 
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;4001.2) - Disable activity command was rejected as expected"
  ut_setrequirements SCH_1005, "P"
  ut_setrequirements SCH_4001_2, "P"
ELSE
  write "<!> Failed (1005;4001.2) - Disable activity command was not rejected as expected"
  ut_setrequirements SCH_1005, "F"
  ut_setrequirements SCH_4001_2, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1005, "F"
endif

;; Reset the message capture counter
ut_setupevents "$SC","$CPU",{SCHAppName},SCH_DISABLE_CMD_ARG_ERR_EID,"ERROR", 1

errCtr = $SC_$CPU_SCH_CMDEC + 1

; disable activity with out-of-range slotnumber
/$SC_$CPU_SCH_DisableEntry SlotNumber=x'cafe' EntryNumber=1

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr} 
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;4001.2) - Disable activity command was rejected as expected"
  ut_setrequirements SCH_1005, "P"
  ut_setrequirements SCH_4001_2, "P"
ELSE
  write "<!> Failed (1005;4001.2) - Disable activity command was not rejected as expected"
  ut_setrequirements SCH_1005, "F"
  ut_setrequirements SCH_4001_2, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.6: Send the DisbleEntry command for an unused entry."
write ";*********************************************************************"
ut_setupevents "$SC","$CPU",{SCHAppName},SCH_DISABLE_CMD_ENTRY_ERR_EID,"ERROR",1

errCtr = $SC_$CPU_SCH_CMDEC + 1

; attempt to enable unused activity
/$SC_$CPU_SCH_DisableEntry SlotNumber=25 EntryNumber=1

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr} 
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;4001.1) - Enable activity command was rejected as expected"
  ut_setrequirements SCH_1005, "P"
  ut_setrequirements SCH_4001_1, "P"
ELSE
  write "<!> Failed (1005;4001.1) - Enable activity command was not rejected as expected"
  ut_setrequirements SCH_1005, "F"
  ut_setrequirements SCH_4001_1, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.7: Send the DisableEntry command with an invalid length."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_CMD_LEN_ERR_EID, "ERROR", 1

errCtr = $SC_$CPU_SCH_CMDEC + 1

;;; CPU1 is the default
rawcmd = "1895C000000603B6"

ut_sendrawcmd "$SC_$CPU_SCH", (rawcmd)

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - Command Rejected Counter incremented."
  ut_setrequirements SCH_1002, "P"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1002;1005) - Command Rejected Counter did not increment as expected."
  ut_setrequirements SCH_1002, "F"
  ut_setrequirements SCH_1005, "F"
endif

ut_tlmwait $SC_$CPU_num_found_messages, 1
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Expected Event message ",SCH_CMD_LEN_ERR_EID, " not received."
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.8: Send a valid DisableEntry command for the TO_NOOP."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_DISABLE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_SCH_CMDPC + 1

/$SC_$CPU_SCH_DisableEntry SlotNumber=2 EntryNumber=0

ut_tlmwait $SC_$CPU_SCH_CMDPC, {cmdCtr} 
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4001) - Enable activity command was sent successfully"
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4001, "P"
ELSE
  write "<!> Failed (1004;4001) - Enable activity command did not increment the CMDPC"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4001, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 5.0: Verify Diagnostic Command"
write ";*********************************************************************"
write ";  Step 5.1: Send a valid Diagnostic Command"
write ";*********************************************************************"
;; Display the Diagnostics page
page $SC_$CPU_SCH_DIAG

;; Need to set the stream based upon the cpu being used for the SCH DIAG Packet
;; CPU1 is the default
local diagPktId = "p098"
stream1 = x'0898'

write "Sending command to add subscription for SCH Diag packet."
/$SC_$CPU_TO_ADDPACKET Stream=stream1 Pkt_Size=x'0' Priority=x'0' Reliability=x'1' Buflimit=x'4'
wait 10

;; Verify the Diagnostic Packet is getting generated by checking for the
;; sequence count This should be 1 since this is the first diag packet
local diagTlmItem = diagPktId & "scnt"

ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_SEND_DIAG_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_SCH_CMDPC + 1

/$SC_$CPU_SCH_SENDDIAG 
ut_tlmwait $SC_$CPU_SCH_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;4004) - Diagnostic Command was executed as expected"
  ut_setrequirements SCH_1004, "P"
  ut_setrequirements SCH_4004, "P"
ELSE
  write "<!> Failed (1004;4004) - Diagnostic Command was not executed as expected"
  ut_setrequirements SCH_1004, "F"
  ut_setrequirements SCH_4004, "F"
ENDIF

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1004) - Event message ",$SC_$CPU_find_event[1].eventid, " not received"
  ut_setrequirements SCH_1004, "F"
endif

;; Verify the Diagnostic Packet is getting generated by checking for the
;; sequence count. This should be 1 since this is the first diag packet since
;; the reset was performed at the beginning of this proc.
ut_tlmwait {diagTlmItem}, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (4004) - Diagnostic Packet is being generated."
  ut_setrequirements SCH_4004, "P"
else
  write "<!> Failed (4004) - Diagnostic packet sequence count did not increment. Packet is not being recieved. Expected ",{expectedSCnt},"; Got ",{diagTlmItem}
  ut_setrequirements SCH_4004, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 5.2: Send the Diagnostic Command with an invalid length."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_CMD_LEN_ERR_EID, "ERROR", 1

errCtr = $SC_$CPU_SCH_CMDEC + 1

;;; CPU1 is the default
rawcmd = "1895C000000207B6"

ut_sendrawcmd "$SC_$CPU_SCH", (rawcmd)

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - Command Rejected Counter incremented."
  ut_setrequirements SCH_1002, "P"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1002;1005) - Command Rejected Counter did not increment as expected."
  ut_setrequirements SCH_1002, "F"
  ut_setrequirements SCH_1005, "F"
endif

ut_tlmwait $SC_$CPU_num_found_messages, 1
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Expected Event message ",SCH_CMD_LEN_ERR_EID, " not received."
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 5.3: Send the Enable Major Frame Synchronization Command with "
write ";  an invalid length. "
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_CMD_LEN_ERR_EID, "ERROR", 1

errCtr = $SC_$CPU_SCH_CMDEC + 1

;;; CPU1 is the default
rawcmd = "1895C000000206B7"

ut_sendrawcmd "$SC_$CPU_SCH", (rawcmd)

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - Command Rejected Counter incremented."
  ut_setrequirements SCH_1002, "P"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1002;1005) - Command Rejected Counter did not increment as expected."
  ut_setrequirements SCH_1002, "F"
  ut_setrequirements SCH_1005, "F"
endif

ut_tlmwait $SC_$CPU_num_found_messages, 1
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Expected Event message ",SCH_CMD_LEN_ERR_EID, " not received."
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 6.0:  Perform a Power-on Reset to clean-up from this test."
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
drop SCH_EnableState
drop SCH_SE_Type
drop SCH_SE_Frequency
drop SCH_Remainder
drop SCH_MessageIndex
drop SCH_GroupData
write ";*********************************************************************"
write ";  End procedure $SC_$CPU_sch_timing_analysis                         "
write ";*********************************************************************"
ENDPROC
