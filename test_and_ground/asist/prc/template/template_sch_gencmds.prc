PROC $sc_$cpu_sch_gencmds
;*******************************************************************************
;  Test Name:  sch_gencmds
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	This test verifies that the CFS Scheduler general commands function
;	properly. The SCH_NOOP and SCH_Reset commands will be tested as well
;	as invalid commands to see if the SCH application handles these properly.
;
;  Requirements Tested
;    SCH1000	Upon receipt of a No-Op command, SCH shall increment the SCH
;		Valid Command Counter and generate an event message.
;    SCH1001	Upon receipt of a Reset command, SCH shall reset the following
;		housekeeping variables to a value of zero:
;		  a) Valid Ground Command Counter
;		  b) Ground Command Rejected Counter
;		  e) Valid commands sent by the SCH APP
;		  f) Commands sent by SCH APP that were reported as erroneous
;		     by the Software Bus
;		  g) Number of Slots (minor frames) processed
;		  h) Number of Times Slots skipped
;		  i) Number of Times Multiple Slots processed
;		  j) Number of times that SCH woke up in the same slot as
;		     last time
;		  k) Number of bad entries found in the Scheduler table
;		    (indication of corrupted table)
;    SCH1002	For all SCH commands, if the length contained in the message
;		header is not equal to the expected length, SCH shall reject the
;		command and issue an event message.
;    SCH1004	If SCH accepts any command as valid, SCH shall execute the
;		command, increment the SCH Valid Command Counter and issue an
;		event message.
;    SCH1005	If SCH rejects any command, SCH shall abort the command
;		execution, increment the SCH Command Rejected Counter and issue
;		an error event message.
;    SCH8000	SCH shall generate a housekeeping message containing the
;		following:
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
;		  l) Synchronization Status (flywheeling)
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
;	None.
;
;  Change History
;	Date		   Name			Description
;	01/09/09	Ezinne Uzo-Okoro  Original Procedure.
;	03/15/10	Walt Moleski	  Cleaned up comments that did not
;					  contain line breaks and updated to
;					  handle 0-based arrays
;	07/28/11	Walt Moleski	  Added variables for App Name and
;					  ram directory
;	08/02/12	Walt Moleski	  Added code to look for the 
;					  Scheduler app executing.
;       06/13/17        W. Moleski        Updated to use CPU1 for commanding and;                                         added a hostCPU variable for the
;                                         utility procs to connect to the proper;                                         host.
;
;  Arguments
;	None.
;
;  Procedures Called
;	Name			Description
;    ut_tlmwait		Wait for a specified telemetry point to update to a
;			specified value. 
;    ut_sendcmd		Send commands to the spacecraft. Verifies command
;                       processed and command error counters.
;    ut_sendrawcmd	Send raw commands to the spacecraft. Verifies command
;                       processed and command error counters.
;    ut_pfindicate	Print the pass fail status of a particular requirement
;                       number.
;    ut_setupevents     Directive to look for a particular event and
;                       increment a value in the CVT to indicate receipt
;    ut_setrequirements	A directive to set the status of the cFE requirements
;			array.
;
;  Expected Test Results and Analysis
;
;**********************************************************************

local logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

#include "ut_statusdefs.h"
#include "ut_cfe_info.h"
#include "cfe_platform_cfg.h"
#include "cfe_evs_events.h"
#include "cfe_es_events.h"
#include "to_lab_events.h"
#include "sch_platform_cfg.h"
#include "sch_events.h"
#include "tst_sch_events.h"

%liv (log_procedure) = logging

#define SCH_1000	0
#define SCH_1001	1
#define SCH_1002	2
#define SCH_1004	3
#define SCH_1005	4
#define SCH_8000	5
#define SCH_9000	6
#define SCH_9001	7

global ut_req_array_size = 7
global ut_requirement[0 .. ut_req_array_size]

for i = 0 to ut_req_array_size DO
  ut_requirement[i] = "U"
enddo

;**********************************************************************
; Set the local values
;**********************************************************************
local cfe_requirements[0 .. ut_req_array_size] = ["SCH_1000", "SCH_1001", "SCH_1002", "SCH_1004", "SCH_1005", "SCH_8000", "SCH_9000", "SCH_9001"]

;**********************************************************************
; Define local variables
;**********************************************************************
LOCAL rawcmd
LOCAL stream1
local cmdCtr, errcnt
local SCHAppName = "SCH"
local SCHLabAppName = "SCH_LAB_APP"
local ramDir = "RAM:0"
local hostCPU = "$CPU"

write ";*********************************************************************"
write ";  Step 1.0:  Initialize the CPU for this test. "
write ";*********************************************************************"
write ";  Step 1.1:  Command a Power-On Reset. "
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
  if ($SC_$CPU_ES_ALE[file_index].ES_AL_AppName = SCHLabAppName) then
    found_app = TRUE
  endif
enddo

if (found_app = TRUE) then
  /$SC_$CPU_ES_DELETEAPP Application=SCHLabAppName
  wait 10
endif

write ";*********************************************************************"
write ";  Step 1.3: Display Housekeeping pages "
write ";**********************************************************************"
page $SC_$CPU_SCH_HK
page $SC_$CPU_TST_SCH_HK

write ";*********************************************************************"
write ";  Step 1.4: Determine if the Scheduler (SCH) Application is running. "
write ";  If no, start it. If yes, send the Restart command. Also, add any "
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

  ut_tlmwait  $SC_$CPU_find_event[2].num_found_messages, 1
  IF (UT_TW_Status = UT_Success) THEN
    write "<*> Passed (9001) - SCH Application Started"
    ut_setrequirements SCH_9001, "P"
  else
    write "<!> Failed (9001) - SCH Application start Event Message not received."
    ut_setrequirements SCH_9001, "F"
  endif
else
  ;; Create and Load the default files just in case they don't exist
  s $SC_$CPU_sch_sdtloadfile
  s $SC_$CPU_sch_mdtloadfile

  s load_start_app (SCHAppName,hostCPU,"SCH_AppMain")

  ; Wait for app startup events
  ut_tlmwait  $SC_$CPU_find_event[2].num_found_messages, 1
  IF (UT_TW_Status = UT_Success) THEN
    if ($SC_$CPU_num_found_messages = 1) then
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

;;; Need to set the stream based upon the cpu being used
;;; CPU1 is the default
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
  if ($SC_$CPU_num_found_messages = 1) then
    write "<*> Passed - TST_SCH Application Started"
  else
    write "<!> Failed - CFE_ES start Event Message for TST_SCH not received."
    write "Event Message count = ",$SC_$CPU_num_found_messages
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
write ";  Step 1.6: Enable DEBUG Event Messages "
write ";*********************************************************************"
cmdCtr = $SC_$CPU_EVS_CMDPC + 1

;; Enable DEBUG events for the appropriate applications ONLY
/$SC_$CPU_EVS_EnaAppEVTType Application=SCHAppName DEBUG

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
;;write "<*> Verify by inspection of SCH_HK page."
;;ut_setrequirements SCH_9000, "I"

;; Check the HK tlm items to see if they are initialized properly
if ($SC_$CPU_SCH_CMDPC = 1) AND ($SC_$CPU_SCH_CMDEC = 0) AND ;;
   ($SC_$CPU_SCH_SKIPSLOTCTR = 0) AND ($SC_$CPU_SCH_ACTFAILURECTR = 0) AND ;;
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
write ";  Step 2.0: SCH General Commanding tests."
write ";*********************************************************************"
write ";  Step 2.1: Send a valid No-Op command."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_NOOP_CMD_EID, "INFO", 1

ut_sendcmd "$SC_$CPU_SCH_NOOP"
if (ut_sc_status = UT_SC_Success) then
  write "<*> Passed (1000;1004) - NOOP command sent properly."
  ut_setrequirements SCH_1000, "P"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1000;1004) - NOOP command not sent properly (", ut_sc_status, ")."
  ut_setrequirements SCH_1000, "F"
  ut_setrequirements SCH_1004, "F"
endif

if ($SC_$CPU_num_found_messages = 1) THEN
  write "<*> Passed (1000;1004) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1000, "P"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1000;1004) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",SCH_NOOP_CMD_EID, "."
  ut_setrequirements SCH_1000, "F"
  ut_setrequirements SCH_1004, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 2.2: Send a No-Op command with an invalid command length."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_CMD_LEN_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_SCH_CMDEC + 1

;;; CPU1 is the default
rawcmd = "1895C000000200B2"

ut_sendrawcmd "$SC_$CPU_SCH", (rawcmd)

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errcnt}
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
write ";  Step 2.3: Send a valid Reset Ctrs command."
write ";*********************************************************************"
;; Check that the counters are not 0
if ($SC_$CPU_SCH_CMDPC = 0) then
  ;; Send a NOOP command
  /$SC_$CPU_SCH_NOOP
  wait 5
endif

if ($SC_$CPU_SCH_CMDEC = 0) then
  /raw {rawcmd}
  wait 5
endif

;; Need to check for other items to see if they are non-zero
;; the easiest is to have TST_SCH set these to non-zero
;; Setup for the expected event
ut_setupevents "$SC","$CPU","TST_SCH",TST_SCH_SETCOUNTERS_INF_EID,"INFO",1
/$SC_$CPU_TST_SCH_SetCounters
wait 5

local oldGoodActs = $SC_$CPU_SCH_ActSuccessCtr
local oldMultiSlot = $SC_$CPU_SCH_MULTSLOTCTR

;; Verify that all the counters are now non-zero
write "Multi Slots Processed = ", $SC_$CPU_SCH_MULTSLOTCTR
write "Same Slot = ", $SC_$CPU_SCH_SAMESLOTCTR
write "Successful Activities =  ", $SC_$CPU_SCH_ACTSUCCESSCTR
write "Bad Table Entries = ", $SC_$CPU_SCH_BADTBLDATACTR

;; Setup for the expected event
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_RESET_CMD_EID, "DEBUG", 1

/$SC_$CPU_SCH_RESETCTRS
wait 5

ut_tlmwait $SC_$CPU_SCH_CMDPC, 0
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1001;1004) - Valid Command Counter was reset."
  ut_setrequirements SCH_1001, "P"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1001;1004) - Valid Command Counter was NOT reset (",ut_tw_status,")."
  ut_setrequirements SCH_1001, "F"
  ut_setrequirements SCH_1004, "F"
endif

ut_tlmwait $SC_$CPU_SCH_CMDEC, 0
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1001) - Command Rejected Counter was reset."
  ut_setrequirements SCH_1001, "P"
else
  write "<!> Failed (1001) - Command Rejected Counter was NOT reset (",ut_tw_status,")."
  ut_setrequirements SCH_1001, "F"
endif

;; Check that this is < old value
if ($SC_$CPU_SCH_ActSuccessCtr < oldGoodActs) then
  write "<*> Passed (1001) - Successful Activities Counter was reset."
  ut_setrequirements SCH_1001, "P"
else
  write "<!> Failed (1001) - Successful Activities Counter does not appear to have reset. Previous value = ",oldGoodActs,"; Current value = ",$SC_$CPU_SCH_ActSuccessCtr
  ut_setrequirements SCH_1001, "F"
endif

if ($SC_$CPU_num_found_messages = 1) THEN
  write "<*> Passed (1001) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1001, "P"
else
  write "<!> Failed (1001) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",SCH_RESET_CMD_EID, "."
  ut_setrequirements SCH_1001, "F"
endif

ut_tlmwait $SC_$CPU_SCH_ActFailureCtr, 0
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1001;1004) - Failed Activities Counter was reset."
  ut_setrequirements SCH_1001, "P"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1001;1004) - Failed Activities Counter was NOT reset. Value = ",$SC_$CPU_SCH_ACTFAILURECTR
  ut_setrequirements SCH_1001, "F"
  ut_setrequirements SCH_1004, "F"
endif

;; Check that this is < old value
if ($SC_$CPU_SCH_MultSlotCtr < oldMultiSlot) then
  write "<*> Passed (1001;1004) - Multiple Slot Counter was reset."
  ut_setrequirements SCH_1001, "P"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1001;1004) - Multiple Slot Counter was NOT reset. Value = ",$SC_$CPU_SCH_MULTSLOTCTR
  ut_setrequirements SCH_1001, "F"
  ut_setrequirements SCH_1004, "F"
endif

ut_tlmwait $SC_$CPU_SCH_SameSlotCtr, 0
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1001;1004) - Same Slot Counter was reset."
  ut_setrequirements SCH_1001, "P"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1001;1004) - Same Slot Counter was NOT reset. Value = ",$SC_$CPU_SCH_SAMESLOTCTR
  ut_setrequirements SCH_1001, "F"
  ut_setrequirements SCH_1004, "F"
endif

ut_tlmwait $SC_$CPU_SCH_BadTblDataCtr, 0
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1001;1004) - Bad Table Data Counter was reset."
  ut_setrequirements SCH_1001, "P"
  ut_setrequirements SCH_1004, "P"
else
  write "<!> Failed (1001;1004) - Bad Table Data Counter was NOT reset. Value = ",$SC_$CPU_SCH_BadTblDataCtr
  ut_setrequirements SCH_1001, "F"
  ut_setrequirements SCH_1004, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 2.4: Send a ResetCtrs command with an invalid command length."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_CMD_LEN_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_SCH_CMDEC + 1

;;; CPU1 is the default
rawcmd = "1895C000000201B3"

ut_sendrawcmd "$SC_$CPU_SCH", (rawcmd)

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - Command Rejected Counter incremented."
  ut_setrequirements SCH_1002, "P"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1002;1005) - Command Rejected Counter did not increment as expected."
  ut_setrequirements SCH_1002, "F"
  ut_setrequirements SCH_1005, "F"
endif

if ($SC_$CPU_num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",SCH_CMD_LEN_ERR_EID, "."
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 2.5: Send an invalid SCH command."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_CC_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_SCH_CMDEC + 1

;;; CPU1 is the default
rawcmd = "1895c00000010f"

ut_sendrawcmd "$SC_$CPU_SCH", (rawcmd)

ut_tlmwait $SC_$CPU_SCH_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Command Rejected Counter incremented."
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Command Rejected Counter did not increment as expected."
  ut_setrequirements SCH_1005, "F"
endif

if ($SC_$CPU_num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements SCH_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",SCH_CMD_LEN_ERR_EID, "."
  ut_setrequirements SCH_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.0:  Perform a Power-on Reset to clean-up from this test."
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
                                                                                
drop ut_requirement ; clear global variables
drop ut_req_array_size ; clear global variables

write ";*********************************************************************"
write ";  End procedure $SC_$CPU_sch_gencmds                                 "
write ";*********************************************************************"
ENDPROC
