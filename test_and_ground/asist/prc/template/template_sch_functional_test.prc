PROC $sc_$cpu_sch_functional_test
;*******************************************************************************
;  Test Name:  sch_functional_test
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	This test verifies that the CFS SCH responds correctly and timely to
;	definitions and changes in the SCH schedule and message definition
;	tables in order to control the enabling/disabling of individual
;	activities.
;
;  Requirements Tested
;    SCH2000	Upon receipt of a Major Frame Signal, SCH shall start processing
;		the Scheduler Definition Table (SDT) which contains the defined
;		activities
;    SCH2002	SCH shall schedule a maximum of <PLATFORM_DEFINED, 5> activities
;		in each minor frame.
;    SCH2003	SCH shall identify the spacecraft Major Frame Signal as noisy 
;		when two Major Frame Signals occur consecutively that are 
;		separated from the previous Major Frame Signal by less than
;		<PLATFORM_DEFINED, 990> milliseconds and/or greater than 
;		<PLATFORM_DEFINED, 1010> milliseconds.
;    SCH2003.1	If the Major Frame Signal is running fast, SCH shall process the
;		activities contained in the slots not processed prior to 
;		receiving the Major Frame Signal, up to the <PLATFORM_DEFINED>
;		maximum number of activities allowed to be processed in a minor
;		frame
;    SCH2003.2	Any remaining activities shall be scheduled in the subsequent
;		minor frames until there are no unprocessed activities from the
;		previous Major Frame
;    SCH2003.3	If the number of slots not processed prior to receiving the
;		Major Frame Signal is greater than <PLATFORM_DEFINED> number of
;		allowable catch-up slots, SCH shall:
;		   a) begin processing the first slot of the major frame
;		   b) skip the unprocessed slots
;		   c) incrementing the Number of Times Slots skipped counter
;    SCH2003.4	If more than <PLATFORM_DEFINED> consecutive Major Frame Signals
;		are noisy,  SCH shall
;		   a) disable synchronizing to the Major Frame Signal and
;		   b) use the Minor Frame Signal for synchronization
;		   c) Set the SCH synchronization status to flywheel
;    SCH2004	SCH shall schedule each activity’s frequency as specified by
;		that activity's table-defined major frame frequency (eg. every
;		5 seconds)
;    SCH2005	SCH shall schedule each activity's frequency as specified by
;		that activity's table-defined major frame frequency offset
;		(eg. every 5 seconds starting at second 2)
;    SCH2006	Upon receipt of a SDT Load, SCH shall validate the entries of
;		the table and if any entry is invalid, the table is declared
;		invalid and rejected.
;    SCH2006.1	SCH shall identify any Unused SDT entry that contains data
;		other than Unused as invalid
;    SCH2006.2	SCH shall identify any Enabled or Disabled SDT entry that
;		contains a Frequency of zero as invalid
;    SCH2006.3	SCH shall identify any Enabled or Disabled SDT entry that
;		contains a Frequency Offset greater than the entry's specified
;		Frequency as invalid.
;    SCH2006.4	SCH shall identify any Enabled or Disabled SDT entry that
;		contains a Message Index of zero or greater than the maximum
;		number of Messages in the Message Definition Table as invalid.
;    SCH2006.5	SCH shall identify any Enabled or Disabled SDT entry that
;		contains an undefined Activity Type as invalid.
;    SCH2007	Upon receipt of a Message Definition Table Load, SCH shall
;		validate the entries of the table and if any table entry is
;		invalid, reject the table
;    SCH2007.1	SCH shall identify any Unused MDT entry that contains data other
;		than Unused as invalid
;    SCH2007.2	SCH shall identify any MDT entry that has a message definition
;		with a Message ID that is greater than <MISSION_DEFINED> maximum
;		Message ID as invalid
;    SCH2007.3	SCH shall identify any MDT entry that has a message definition
;		with a Message ID that is less than <MISSION_DEFINED> minimum
;		Message ID as invalid
;    SCH2007.4	SCH shall identify any MDT entry that has a message length
;		greater than <PLATFORM_DEFINED> maximum message length as
;		invalid
;    SCH2007.5	SCH shall identify any MDT entry that has a message length that
;		is an odd number of bytes as invalid
;    SCH8000	SCH shall generate a housekeeping message containing the 
;		following:
;		   a) Valid Ground Command Counter
;		   b) Ground Command Rejected Counter
;		   e) Valid commands sent by the SCH App
;		   f) Commands sent by the SCH App that were reported as
;		      erroneous by the Software Bus
;		   g) Number of Slots processed
;		   h) Number of Times Slots skipped
;		   i) Number of Times Multiple Slots processed
;		   j) Number of times that SCH woke up in the same slot as last
;		      time
;		   k) Number of bad entries found in the Scheduler table
;		      (indication of corrupted table)
;		   l) Synchronization Status (not flywheeling)
;    SCH9000	Upon any Initialization of the SCH Application (cFE Power On,
;		cFE Processor Reset or SCH Application Reset), SCH shall
;		initialize the following data to Zero: 
;		   a) Valid Ground Command Counter
;		   b) Ground Command Rejected Counter
;		   e) Valid commands sent by the SCH App
;		   f) Commands sent by the SCH App that were reported as
;		      erroneous by the Software Bus
;		   g) Number of Slots processed
;		   h) Number of Times Slots skipped
;		   i) Number of Times Multiple Slots processed
;		   j) Number of times that SCH woke up in the same slot as
;		      last time
;		   k) Number of bad entries found in the Scheduler table
;		      (indication of corrupted table)
;		   l) Synchronization Status (not flywheeling)
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
;	Date		Name		  Description
;	01/30/09	Ezinne Uzo-Okoro  Original Procedure.
;	03/15/10	Walt Moleski	  Updated comments so that they were not
;					  overrunning lines and updated the
;					  arrays to be 0-based
;	06/06/11	Walt Moleski	  Added variables for ram disk and app
;					  and table names
;       08/02/12	Walt Moleski	  Added code to look for the Scheduler
;					  app executing.
;       06/13/17	W. Moleski	  Updated to use CPU1 for commanding and;					  added a hostCPU variable for the
;					  utility procs to connect to the proper;					  host.
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
;	Verify that correct event messages are received on the ground. 
;	Verify that changes to scheduler and command tables are accepted and are
;	executed.
;
;**********************************************************************

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

#define SCH_2000     0
#define SCH_2002     1
#define SCH_2003     2
#define SCH_2003_1   3
#define SCH_2003_2   4
#define SCH_2003_3   5
#define SCH_2003_4   6
#define SCH_2004     7
#define SCH_2005     8
#define SCH_2006     9 
#define SCH_2006_1  10
#define SCH_2006_2  11
#define SCH_2006_3  12
#define SCH_2006_4  13
#define SCH_2006_5  14
#define SCH_2007    15
#define SCH_2007_1  16
#define SCH_2007_2  17
#define SCH_2007_3  18
#define SCH_2007_4  19
#define SCH_2007_5  20
#define SCH_8000    21
#define SCH_9000    22
#define SCH_9001    23

global ut_req_array_size = 23
global ut_requirement[0 .. ut_req_array_size]

for i = 0 to ut_req_array_size DO
  ut_requirement[i] = "U"
enddo

;**********************************************************************
; Set the local values
;**********************************************************************
local cfe_requirements[0 .. ut_req_array_size] = ["SCH_2000","SCH_2002","SCH_2003","SCH_2003.1","SCH_2003.2","SCH_2003.3","SCH_2003.4","SCH_2004","SCH_2005","SCH_2006","SCH_2006.1","SCH_2006.2","SCH_2006.3","SCH_2006.4","SCH_2006.5","SCH_2007","SCH_2007.1","SCH_2007.2","SCH_2007.3","SCH_2007.4","SCH_2007.5","SCH_8000","SCH_9000","SCH_9001"]

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
  ut_tlmwait  $SC_$CPU_find_event[2].num_found_messages, 1
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

write ";*********************************************************************"
write ";  Step 2.0: SCH Activity tests"
write ";*********************************************************************"
write ";  Step 2.1: Determine if the SCH application starts processing the  "
write ";  table upon receipt of the Major Frame Signal. "
write ";*********************************************************************"
;; Setup to capture several events contained in the SDT
ut_setupevents "$SC","$CPU",{SCHAppName},SCH_NOOP_CMD_EID,"INFO", 1
ut_setupevents "$SC","$CPU","TST_SCH",TST_SCH_NOOP_INF_EID,"INFO", 2

;; Need to enable NOOP commands
/$SC_$CPU_SCH_EnableGroup GroupData=2
wait 3

;; Need to get Major Frame signal and verify that SCH starts processing table
validmfctr = $SC_$CPU_SCH_VALIDMFCTR

ut_tlmupdate $SC_$CPU_SCH_VALIDMFCTR
if ($SC_$CPU_SCH_VALIDMFCTR > validmfctr) then
  write "<*> Passed - Major Frame Signal is being received."
ELSE
  write "<!> Failed - Did not rcv the Major Frame Signal."
ENDIF

;; If the events are rcv'd, pass the requirement
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  if ($SC_$CPU_find_event[2].num_found_messages <> 0) THEN
    write "<*> Passed (2000) - SDT is being processed."
    ut_setrequirements SCH_2000, "P"
  else
    ut_tlmwait $SC_$CPU_find_event[2].num_found_messages, 1
    if (UT_TW_Status = UT_Success) then
      write "<*> Passed (2000) - SDT is being processed."
      ut_setrequirements SCH_2000, "P"
    else
      write "<!> Failed (2000) - Expected SDT event messages were not rcv'd."
      ut_setrequirements SCH_2000, "F"
    endif
  endif
else
  write "<!> Failed (2000) - Expected SDT event messages were not rcv'd."
  ut_setrequirements SCH_2000, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 2.2: Dump the Schedule Definition Table "
write ";*********************************************************************"
;; Disable the NOOP group
/$SC_$CPU_SCH_DisableGroup GroupData=2
wait 5

;; get current schedule definition table 
s get_tbl_to_cvt (ramDir,SCHDefTblName,"A","sch_def_schtbl.tbl",hostCPU,sch_tblAppId)
wait 1

write
write "Schedule Definition Table Used Entries"
FOR i = 0 to SCH_TABLE_ENTRIES-1 DO
  if ($SC_$CPU_SCH_DefaultScheduleTable[i].EnableState <> SCH_UNUSED) then
    write "Entry ", i, " = "
    write "   Type  = ", p@$SC_$CPU_SCH_DefaultScheduleTable[i].SE_Type
    write "   Freq  = ", $SC_$CPU_SCH_DefaultScheduleTable[i].SE_Frequency
    write "   Rem   = ", $SC_$CPU_SCH_DefaultScheduleTable[i].Remainder
    write "   Msg   = ", $SC_$CPU_SCH_DefaultScheduleTable[i].MessageIndex
    write "   Group = ", $SC_$CPU_SCH_DefaultScheduleTable[i].GroupData
  endif
ENDDO

;; These requirements can be verified by selecting a valid slot in the table
;; and adding the frequency and offset and determine if the command was execute
;; within that time. If not, fail the requirement
write "=> Using entry 11 to determine the frequency and offset requirements"
ut_setupevents "$SC","$CPU",{SCHAppName},SCH_NOOP_CMD_EID,"INFO", 1

local freqTime = $SC_$CPU_SCH_DefaultScheduleTable[11].SE_Frequency
local offsetTime = $SC_$CPU_SCH_DefaultScheduleTable[11].Remainder
local maxWaitTime = freqTime + offsetTime

;; Enable Entry #11 - Slot 2 - entry 1
/$SC_$CPU_SCH_EnableEntry SlotNumber=2 EntryNumber=1
wait maxWaitTime

;; Check for at least 1 message. May get 2 if the msg comes out right away
if ($SC_$CPU_find_event[1].num_found_messages > 0) then
  write "<*> Passed (2004;2005) - Frequency and Offset were processed correctly"
  ut_setrequirements SCH_2004, "P"
  ut_setrequirements SCH_2005, "P"
else
  write "<!> Failed (2004;2005) - Did not receive the expected number of events for frequency and Offset specified"
  ut_setrequirements SCH_2004, "F"
  ut_setrequirements SCH_2005, "F"
endif

/$SC_$CPU_SCH_DisableEntry SlotNumber=2 EntryNumber=1
wait 5

write ";*********************************************************************"
write ";  Step 2.3: Validate maximum Minor Frame Activities"
write ";*********************************************************************"
write ";  Step 2.3.1: Load the SDT with all minor frames of the MAX "
write ";  activites scheduled. "
write ";*********************************************************************"
s $SC_$CPU_sch_sdt2

;; Load the table
ut_setupevents "$SC","$CPU","CFE_TBL",CFE_TBL_FILE_LOADED_INF_EID,"INFO", 1

cmdCtr = $SC_$CPU_TBL_CMDPC + 1

s load_table ("sch_def_sdt2.tbl",hostCPU)

ut_tlmwait $SC_$CPU_TBL_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Load command for SDT2 sent successfully."
else
  write "<!> Failed - Load command for SDT2 did not execute successfully."
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) then
  write "<*> Passed - Event Msg ",$SC_$CPU_find_event[1].eventid," Found!"
else
  write "<!> Failed - Event Message not received for Load command."
endif

write ";*********************************************************************"
write ";  Step 2.3.2: Validate the SDT loaded above. "
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
  write "<!> Failed - SDT Table validation failed. Expected Event Msg ",CFE_TBL_VALIDATION_INF_EID, " not rcv'd."
endif

write ";*********************************************************************"
write ";  Step 2.3.3: Activate the SDT loaded above. "
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

write ";*********************************************************************"
write ";  Step 2.3.4: This step verifies that the SCH application schedules a"
write ";  max of ",SCH_ENTRIES_PER_SLOT," activities in each minor frame."
write ";*********************************************************************"
;; Count the number of activities to be scheduled
local enabled_act = 0
local group2_act = 0
local group4_act = 0
FOR i = 0 to SCH_TABLE_ENTRIES-1 DO
  if ($SC_$CPU_SCH_DefaultScheduleTable[i].EnableState = SCH_ENABLED) then
    enabled_act = enabled_act + 1
  endif
  if ($SC_$CPU_SCH_DefaultScheduleTable[i].GroupData = 2) then
    group2_act = group2_act + 1
  endif
  if ($SC_$CPU_SCH_DefaultScheduleTable[i].GroupData = 4) then
    group4_act = group4_act + 1
  endif
ENDDO

;; The max activities should be scheduled
max_minor_act = (enabled_act + group2_act) * 2
max_minor_act = max_minor_act + 18

/$SC_$CPU_SCH_RESETCTRS
wait 10

actsuccessctr = $SC_$CPU_SCH_ACTSUCCESSCTR
write "=> Before wait(10): Successful Activity counter = ",actsuccessctr
write "=> Max Activities = ", max_minor_act

;; Need to enable NOOP commands
/$SC_$CPU_SCH_EnableGroup GroupData=2

; wait for messages to accumulate
wait 10

local act_diff = $SC_$CPU_SCH_ACTSUCCESSCTR - actsuccessctr
write "=> After wait(10): Successful Activity counter = ",$SC_$CPU_SCH_ACTSUCCESSCTR
write "=> Successful Activities in 10 seconds = ",act_diff

;; Disable the NOOP group
/$SC_$CPU_SCH_DisableGroup GroupData=2
wait 1

write "=> After Disable: Successful Activity counter = ",$SC_$CPU_SCH_ACTSUCCESSCTR

IF (act_diff <= max_minor_act) THEN
  write "<*> Passed (2002) - Minor Frame activities do not exceed the maximum."
  ut_setrequirements SCH_2002, "P"
ELSE
  write "<!> Failed (2002) - Minor Frame activities exceed the maximum!!"
  ut_setrequirements SCH_2002, "F"
ENDIF

wait 5

write ";*********************************************************************"
write ";  Step 2.4: Validate Skipping Slots"
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_SKIPPED_SLOTS_EID, "ERROR", 1
ut_setupevents "$SC", "$CPU", {SCHAppName}, SCH_MULTI_SLOTS_EID, "INFO", 2 

skipslotctr = $SC_$CPU_SCH_SKIPSLOTCTR

write "*** Jam MET time."
write

local expectedAct = (enabled_act + group4_act) * 2
expectedAct = expectedAct + 16
actsuccessctr = $SC_$CPU_SCH_ACTSUCCESSCTR
write "==> Successful activities before Slot skip = ",actsuccessctr

;; Need to enable NOOP commands
/$SC_$CPU_SCH_EnableGroup GroupData=4

/$SC_$CPU_TIME_SETCLOCKSTCF SECONDS=0 MICROSECS=0
wait 2
/$SC_$CPU_TIME_SETSTATE FLYWHEEL
wait 3
/$SC_$CPU_TIME_SETCLOCKMET SECONDS=0 MICROSECS=50000
wait 2
/$SC_$CPU_TIME_SETSTATE VALID
wait 3  

;; Need to disable NOOP commands
/$SC_$CPU_SCH_DisableGroup GroupData=4
wait 1

act_diff = $SC_$CPU_SCH_ACTSUCCESSCTR - actsuccessctr
write "==> Expected normal activities = ",expectedAct
write "==> Successful activities = ",act_diff

;; Check if the Skip Slot counter incremented
IF ($SC_$CPU_SCH_SKIPSLOTCTR <> skipslotctr) THEN
  write "<*> Passed (2003.3) - SCH skips slots as expected"
  ut_setrequirements SCH_2003_3, "P"
ELSE
  write "<!> Failed (2003.3) - SCH does not skip slots as expected"
  ut_setrequirements SCH_2003_3, "F"
ENDIF

;; Check for event messages
IF ($SC_$CPU_find_event[1].num_found_messages > 0) THEN
  write "<*> Passed (2003.3) - Event ID ", $SC_$CPU_find_event[1].eventid, " rcv'd"
  ut_setrequirements SCH_2003_3, "P"
ELSE
  write "<!> Failed (2003.3) - Expected Event ",SCH_SKIPPED_SLOTS_EID," NOT rcv'd!!!"
  ut_setrequirements SCH_2003_3, "F"
ENDIF

IF ($SC_$CPU_find_event[2].num_found_messages > 0) THEN
  write "<*> Passed (2003.1;2003.2) - Event ID ", $SC_$CPU_find_event[2].eventid, " rcv'd"
  ut_setrequirements SCH_2003_1, "P"  
  ut_setrequirements SCH_2003_2, "P"  
ELSE
  write "<!> Failed (2003.1;2003.2) - Expected Event ",SCH_MULTI_SLOTS_EID," NOT rcv'd!!!"
  ut_setrequirements SCH_2003_1, "F"  
  ut_setrequirements SCH_2003_2, "F"  
ENDIF

wait 5

write ";*********************************************************************"
write ";  Step 2.5: Disabling Major Frame Synchronization"
write ";*********************************************************************"
ut_setupevents "$SC","$CPU",{SCHAppName},SCH_NOISY_MAJOR_FRAME_ERR_EID,"ERROR",1

write "*** Jam 1Hz time."
write

FOR i = 0 to 3 DO
  write "***** i = ", i, " *****"
  write
  /$SC_$CPU_TIME_SETCLOCKSTCF SECONDS=0 MICROSECS=0
  wait 1
  /$SC_$CPU_TIME_SETCLOCKMET SECONDS=0 MICROSECS=i*20000
  wait 1
  /$SC_$CPU_TIME_SETSTATE VALID
  wait 1

  ;; Check if the Error Event was generated
  if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
    break
  endif
ENDDO

ut_tlmwait $SC_$CPU_SCH_IGNOREMF, 1
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2003;2003.4) - Major Frame is noisy, Minor Frame used for sync as expected"
  ut_setrequirements SCH_2003, "P"
  ut_setrequirements SCH_2003_4, "P"
ELSE
  write "<!> Failed (2003;2003.4) - Major Frame is not noisy and Minor Frame not used"
  ut_setrequirements SCH_2003, "F"
  ut_setrequirements SCH_2003_4, "F"
ENDIF

IF ($SC_$CPU_find_event[1].num_found_messages > 0) THEN
  write "<*> Passed (2003.4) - Event ID ", $SC_$CPU_find_event[1].eventid, " rcv'd"
  ut_setrequirements SCH_2003_4, "P"
ELSE
  write "<!> Failed (2003.4) - Expected Event ",SCH_NOISY_MAJOR_FRAME_ERR_EID," NOT rcv'd!!!"
  ut_setrequirements SCH_2003_4, "F"
ENDIF

;; Restore Major Frame Sync before testing validation errors
/$SC_$CPU_SCH_EnableSync
wait 5

write ";*********************************************************************"
write ";  Step 2.6: Validate Schedule Definition Tables."
write ";*********************************************************************"
;; Creating 11 invalid schedule definition tables
s $sc_$cpu_sch_badsdttbls

;; Requirement 2006.1 Test
s load_table ("sdt_20061.tbl",hostCPU)
wait 5

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=SCHDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2006;2006.1) - SDT load was rejected as expected"
  ut_setrequirements SCH_2006, "P"
  ut_setrequirements SCH_2006_1, "P"
ELSE
  write "<!> Failed (2006;2006.1) - SDT was not rejected as expected"
  ut_setrequirements SCH_2006, "F"
  ut_setrequirements SCH_2006_1, "F"
ENDIF

;; Requirement 2006.2 Tests
s load_table ("sdt_20062E.tbl",hostCPU)
wait 5

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=SCHDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2006;2006.2) - SDT load was rejected as expected"
  ut_setrequirements SCH_2006, "P"
  ut_setrequirements SCH_2006_2, "P"
ELSE
  write "<!> Failed (2006;2006.2) - SDT was not rejected as expected"
  ut_setrequirements SCH_2006, "F"
  ut_setrequirements SCH_2006_2, "F"
ENDIF

s load_table ("sdt_20062D.tbl",hostCPU)
wait 5

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=SCHDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2006;2006.2) - SDT load was rejected as expected"
  ut_setrequirements SCH_2006, "P"
  ut_setrequirements SCH_2006_2, "P"
ELSE
  write "<!> Failed (2006;2006.2) - SDT was not rejected as expected"
  ut_setrequirements SCH_2006, "F"
  ut_setrequirements SCH_2006_2, "F"
ENDIF

;; Requirement 2006.3 Tests
s load_table ("sdt_20063D.tbl",hostCPU)
wait 5

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=SCHDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2006;2006.3) - SDT load was rejected as expected"
  ut_setrequirements SCH_2006, "P"
  ut_setrequirements SCH_2006_3, "P"
ELSE
  write "<!> Failed (2006;2006.3) - SDT was not rejected as expected"
  ut_setrequirements SCH_2006, "F"
  ut_setrequirements SCH_2006_3, "F"
ENDIF

s load_table ("sdt_20063E.tbl",hostCPU)
wait 5

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=SCHDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2006;2006.3) - SDT load was rejected as expected"
  ut_setrequirements SCH_2006, "P"
  ut_setrequirements SCH_2006_3, "P"
ELSE
  write "<!> Failed (2006;2006.3) - SDT was not rejected as expected"
  ut_setrequirements SCH_2006, "F"
  ut_setrequirements SCH_2006_3, "F"
ENDIF

;; Requirement 2006.4 Tests
s load_table ("sdt_20064D.tbl",hostCPU)
wait 5

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=SCHDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2006;2006.4) - SDT load was rejected as expected"
  ut_setrequirements SCH_2006, "P"
  ut_setrequirements SCH_2006_4, "P"
ELSE
  write "<!> Failed (2006;2006.4) - SDT was not rejected as expected"
  ut_setrequirements SCH_2006, "F"
  ut_setrequirements SCH_2006_4, "F"
ENDIF

s load_table ("sdt_20064E.tbl",hostCPU)
wait 5

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=SCHDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2006;2006.4) - SDT load was rejected as expected"
  ut_setrequirements SCH_2006, "P"
  ut_setrequirements SCH_2006_4, "P"
ELSE
  write "<!> Failed (2006;2006.4) - SDT was not rejected as expected"
  ut_setrequirements SCH_2006, "F"
  ut_setrequirements SCH_2006_4, "F"
ENDIF

;; Requirement 2006.5 Tests
s load_table ("sdt_20065D.tbl",hostCPU)
wait 5

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=SCHDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2006;2006.5) - SDT load was rejected as expected"
  ut_setrequirements SCH_2006, "P"
  ut_setrequirements SCH_2006_5, "P"
ELSE
  write "<!> Failed (2006;2006.5) - SDT was not rejected as expected"
  ut_setrequirements SCH_2006, "F"
  ut_setrequirements SCH_2006_5, "F"
ENDIF

s load_table ("sdt_20065E.tbl",hostCPU)
wait 5

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=SCHDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2006;2006.5) - SDT load was rejected as expected"
  ut_setrequirements SCH_2006, "P"
  ut_setrequirements SCH_2006_5, "P"
ELSE
  write "<!> Failed (2006;2006.5) - SDT was not rejected as expected"
  ut_setrequirements SCH_2006, "F"
  ut_setrequirements SCH_2006_5, "F"
ENDIF

;; MessageIndex = 0 Tests
s load_table ("sdt_2006msg0E.tbl",hostCPU)
wait 5

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=SCHDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2006) - SDT load was rejected as expected"
  ut_setrequirements SCH_2006, "P"
ELSE
  write "<!> Failed (2006) - SDT was not rejected as expected"
  ut_setrequirements SCH_2006, "F"
ENDIF

s load_table ("sdt_2006msg0D.tbl",hostCPU)
wait 5

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=SCHDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2006) - SDT load was rejected as expected"
  ut_setrequirements SCH_2006, "P"
ELSE
  write "<!> Failed (2006) - SDT was not rejected as expected"
  ut_setrequirements SCH_2006, "F"
ENDIF

wait 5

write ";*********************************************************************"
write ";  Step 2.7: Validate Message Definition Tables."
write ";*********************************************************************"
; loading invalid message definition tables
;; 7 table files are create to see if SCH detects each error
s $SC_$CPU_sch_badmdttbls

;; This file should have 0 good and 7 bad entries
s load_table ("mdt_20071.tbl",hostCPU)
wait 5

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=MSGDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}, 30
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2007;2007.2) - MDT failed validation as expected"
  ut_setrequirements SCH_2007, "P"
  ut_setrequirements SCH_2007_2, "P"
ELSE
  write "<!> Failed (2007;2007.2) - MDT was not rejected as expected"
  ut_setrequirements SCH_2007, "F"
ENDIF

;; Should have 2 good & 5 bad. Should detect MSGID 0001 as invalid
s load_table ("mdt_20072.tbl",hostCPU)
wait 5

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=MSGDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}, 30
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2007;2007.3) - MDT was not validated as expected"
  ut_setrequirements SCH_2007, "P"
  ut_setrequirements SCH_2007_3, "P"
ELSE
  write "<!> Failed (2007;2007.3) - MDT was not rejected as expected"
  ut_setrequirements SCH_2007, "F"
  ut_setrequirements SCH_2007_3, "F"
ENDIF

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

;; Should have 3 good & 4 bad. Should detect MSGID 0000 as invalid
s load_table ("mdt_20073.tbl",hostCPU)
wait 5

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=MSGDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}, 30
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2007;2007.3) - MDT was not validated as expected"
  ut_setrequirements SCH_2007, "P"
  ut_setrequirements SCH_2007_3, "P"
ELSE
  write "<!> Failed (2007;2007.3) - MDT was not rejected as expected"
  ut_setrequirements SCH_2007, "F"
  ut_setrequirements SCH_2007_3, "F"
ENDIF

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

; Should have 4 good & 3 bad. Should detect MID 2001 as invalid
s load_table ("mdt_20074.tbl",hostCPU)
wait 5

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=MSGDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}, 30
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2007;2007.2) - MDT was not validated as expected"
  ut_setrequirements SCH_2007, "P"
  ut_setrequirements SCH_2007_2, "P"
ELSE
  write "<!> Failed (2007;2007.2) - MDT was not rejected as expected"
  ut_setrequirements SCH_2007, "F"
  ut_setrequirements SCH_2007_2, "F"
ENDIF

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

; Should have 5 good & 2 bad. Should detect MID 1848 with len = 135 as invalid
s load_table ("mdt_20075.tbl",hostCPU)
wait 5

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=MSGDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}, 30
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2007;2007.4) - MDT was not validated as expected"
  ut_setrequirements SCH_2007, "P"
  ut_setrequirements SCH_2007_4, "P"
ELSE
  write "<!> Failed (2007;2007.4) - MDT was not rejected as expected"
  ut_setrequirements SCH_2007, "F"
  ut_setrequirements SCH_2007_4, "F"
ENDIF

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

; Should have 5 good & 2 bad. Should detect MID 1848 with len = 9 as invalid
s load_table ("mdt_20076.tbl",hostCPU)
wait 5

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=MSGDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}, 30
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2007;2007.5) - MDT was not validated as expected"
  ut_setrequirements SCH_2007, "P"
  ut_setrequirements SCH_2007_5, "P"
ELSE
  write "<!> Failed (2007;2007.5) - MDT was not rejected as expected"
  ut_setrequirements SCH_2007, "F"
  ut_setrequirements SCH_2007_5, "F"
ENDIF

tblfailverifyctr = $SC_$CPU_SCH_TBLFAILVERIFYCTR + 1

; Should have 6 good & 1 bad. Should detect index 6 as invalid
s load_table ("mdt_20077.tbl",hostCPU)
wait 5

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=MSGDefTblName
ut_tlmwait $SC_$CPU_SCH_TBLFAILVERIFYCTR, {tblfailverifyctr}, 30
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed (2007;2007.1) - MDT was not validated as expected"
  ut_setrequirements SCH_2007, "P"
  ut_setrequirements SCH_2007_1, "P"
ELSE
  write "<!> Failed (2007;2007.1) - MDT was not rejected as expected"
  ut_setrequirements SCH_2007, "F"
  ut_setrequirements SCH_2007_1, "F"
ENDIF

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

; clear global variables
drop ut_requirement 
drop ut_req_array_size 

write ";*********************************************************************"
write ";  End procedure $SC_$CPU_sch_functional_test "
write ";*********************************************************************"
ENDPROC
