PROC $sc_$cpu_ds_gencmds
;*******************************************************************************
;  Test Name:  ds_gencmds
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	This test verifies that the CFS Data Storage (DS) general commands
;	function properly. The NOOP and Reset Counters commands will be tested.
;	Invalid versions of these commands will also be tested to ensure that
;	the DS application handled these properly.
;
;  Requirements Tested
;    cDS1000	Upon receipt of a No-Op command, DS shall increment the DS
;		Valid Command Counter and generate an event message.
;    cDS1001	Upon receipt of a Reset Counters command, DS shall reset the
;		following housekeeping variables to a value of zero:
;			a. Valid Ground Command Counter
;			b. Command Rejected Counter
;			c. Packets discarded (DS was disabled) Counter
;			d. Packets discarded (pkt has no filter) Counter
;			e. Packets discarded (failed filter test) Counter
;			f. Packets that passed filter test Counter
;			g. Good destination file I/O Counter
;			h. Bad destination file I/O Counter
;			i. Good updates to secondary header Counter
;			j. Bad updates to secondary header Counter
;			k. Destination file table loads Counter
;			l. Failed attempts to get table data pointer Counter
;			m. Packet filter table loads Counter
;			n. Failed attempts to get table data pointer Counter
;    cDS1002	For all DS commands, if the length contained in the message
;		header is not equal to the expected length, DS shall reject the
;		command and issue an event message.
;    cDS1004	If DS accepts any command as valid, DS shall execute the
;		command, increment the DS Valid Command Counter and issue an
;		event message.
;    cDS1005	If DS rejects any command, DS shall abort the command execution,
;		increment the DS Command Rejected Counter and issue an event
;		message.
;    cDS8000	DS shall generate a housekeeping message containing the
;		following:
;			a. Valid Ground Command Counter
;			b. Command Rejected Counter
;			c. Packets discarded (DS was disabled) Counter
;			d. Packets discarded (pkt has no filter) Counter
;			e. Packets discarded (failed filter test) Counter
;			f. Packets that passed filter test Counter
;			g. Good destination file I/O Counter
;			h. Bad destination file I/O Counter
;			i. Good updates to secondary header Counter
;			j. Bad updates to secondary header Counter
;			k. Destination file table loads Counter
;			l. Failed attempts to get table data pointer Counter
;			m. Packet filter table loads Counter
;			n. Failed attempts to get table data pointer Counter
;			o. Application State
;			p. Destinatation file(s) state:
;				1. File age
;				2. File size
;				3. File rate
;				4. Sequence count
;				5. Enable State
;				6. Open State
;				7. Filename
;    cDS9000	Upon initialization of the SC Application, SC shall initialize
;		the following data to Zero:
;			a. Valid Ground Command Counter
;			b. Command Rejected Counter
;			c. Packets discarded (DS was disabled) Counter
;			d. Packets discarded (pkt has no filter) Counter
;			e. Packets discarded (failed filter test) Counter
;			f. Packets that passed filter test Counter
;			g. Good destination file I/O Counter
;			h. Bad destination file I/O Counter
;			i. Good updates to secondary header Counter
;			j. Bad updates to secondary header Counter
;			k. Destination file table loads Counter
;			l. Failed attempts to get table data pointer Counter
;			m. Packet filter table loads Counter
;			n. Failed attempts to get table data pointer Counter
;
;  Prerequisite Conditions
;	The cFE is up and running and ready to accept commands.
;	The DS commands and telemetry items exist in the GSE database.
;	The display pages exist for the DS Housekeeping.
;	A DS Test application (TST_DS) exists in order to fully test the DS
;		Application.
;
;  Assumptions and Constraints
;	None.
;
;  Change History
;	Date		   Name		Description
;	09/29/09	Walt Moleski	Original Procedure.
;       12/08/10        Walt Moleski    Modified the procedure to use variables
;                                       for the application name and ram disk.
;       01/31/17        Walt Moleski    Updated for DS 2.5.0.0 using CPU1 for
;                                       commanding and added a hostCPU variable
;                                       for the utility procs to connect to the
;                                       proper host IP address.
;
;  Arguments
;	None.
;
;  Procedures Called
;	Name			Description
;       ut_tlmwait	Wait for a specified telemetry point to update to a
;			specified value. 
;       ut_pfindicate	Print the pass fail status of a particular requirement
;			number.
;       ut_setupevents	Performs setup to verify that a particular event
;			message was received by ASIST.
;	ut_setrequirements	A directive to set the status of the cFE
;			requirements array.
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
#include "cfe_tbl_events.h"
#include "to_lab_events.h"
#include "ds_platform_cfg.h"
#include "ds_events.h"
#include "ds_appdefs.h"
#include "tst_ds_events.h"

%liv (log_procedure) = logging

#define DS_1000		0
#define DS_1001		1
#define DS_1002		2
#define DS_1004		3
#define DS_1005		4
#define DS_8000		5
#define DS_9000		6

global ut_req_array_size = 6
global ut_requirement[0 .. ut_req_array_size]

for i = 0 to ut_req_array_size DO
  ut_requirement[i] = "U"
enddo

;**********************************************************************
; Set the local values
;**********************************************************************
local cfe_requirements[0 .. ut_req_array_size] = ["DS_1000", "DS_1001", "DS_1002", "DS_1004", "DS_1005", "DS_8000", "DS_9000" ]

;**********************************************************************
; Define local variables
;**********************************************************************
LOCAL rawcmd, stream
local DSAppName = "DS"
local hostCPU = "$CPU"

write ";***********************************************************************"
write ";  Step 1.0: Data Storage Test Setup."
write ";***********************************************************************"
write ";  Step 1.1: Command a Power-on Reset on $CPU."
write ";***********************************************************************"
/$SC_$CPU_ES_POWERONRESET
wait 10

close_data_center
wait 60

cfe_startup {hostCPU}
wait 5


write ";***********************************************************************"
write ";  Step 1.2: Upload the default DS table load images to $CPU."
write ";***********************************************************************"
;; Create the initial tables for this procedure
s $sc_$cpu_ds_tbl1

;; Parse the filename configuration parameters for the default table filenames
local destFileName = DS_DEF_DEST_FILENAME
local filterFileName = DS_DEF_FILTER_FILENAME
local slashLoc = %locate(destFileName,"/")

;; loop until all slashes are found for the Destination File Table Name
while (slashLoc <> 0) do
  destFileName = %substring(destFileName,slashLoc+1,%length(destFileName))
  slashLoc = %locate(destFileName,"/")
enddo

slashLoc = %locate(filterFileName,"/")
;; loop until all slashes are found for the Filter Table Name
while (slashLoc <> 0) do
  filterFileName = %substring(filterFileName,slashLoc+1,%length(filterFileName))
  slashLoc = %locate(filterFileName,"/")
enddo

write "==> Default Filter Table filename = '",filterFileName,"'"

;; Upload the files to $CPU
s ftp_file("CF:0/apps", "ds_gcfile.tbl", destFileName, hostCPU, "P")
s ftp_file("CF:0/apps", "ds_gcfilter.tbl", filterFileName, hostCPU, "P")

wait 5

write ";***********************************************************************"
write ";  Step 1.3: Display the Housekeeping pages "
write ";***********************************************************************"
page $SC_$CPU_DS_HK
page $SC_$CPU_DS_FILE_TBL
page $SC_$CPU_DS_FILTER_TBL
page $SC_$CPU_TST_DS_HK

write ";***********************************************************************"
write ";  Step 1.4:  Start the Data Storage (DS) and Test Applications. Verify "
write ";  that the DS Housekeeping telemetry packet is being generated and the "
write ";  appropriate items are initialized to zero (0). "
write ";***********************************************************************"
s $sc_$cpu_ds_start_apps("1.4")
wait 5

;; Verify the Housekeeping Packet is being generated
local hkPktId

;; Set the DS HK packet ID based upon the cpu being used
hkPktId = "p0B8"

;; Verify the HK Packet is getting generated by waiting for the
;; sequencecount to increment twice
local seqTlmItem = hkPktId & "scnt"
local currSCnt = {seqTlmItem}
local expectedSCnt = currSCnt + 2

ut_tlmwait {seqTlmItem}, {expectedSCnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (8000) - Housekeeping packet is being generated."
  ut_setrequirements DS_8000, "P"
else
  write "<!> Failed (8000) - Housekeeping packet sequence count did not increment. Housekeeping packet is not being recieved."
  ut_setrequirements DS_8000, "F"
endif

;; Check the HK tlm items to see if they are initialized properly
;; Removed the IgnoredPktCnt from the test since it increments constantly
;; b/c of the DS_SEND_HK_MID that constantly gets rcv'd
if ($SC_$CPU_DS_CMDPC = 0) AND ($SC_$CPU_DS_CMDEC = 0) AND ;;
   ($SC_$CPU_DS_DisabledPktCnt = 0) AND ;;
   ($SC_$CPU_DS_FilteredPktCnt = 0) AND ($SC_$CPU_DS_PassedPktCnt = 0) AND ;;
   ($SC_$CPU_DS_FileWriteCnt = 0) AND ($SC_$CPU_DS_FileWriteErrCnt = 0) AND ;;
   ($SC_$CPU_DS_FileUpdCnt = 0) AND ($SC_$CPU_DS_FileUpdErrCnt = 0) AND ;;
   ($SC_$CPU_DS_DestLoadCnt = 1) AND ($SC_$CPU_DS_DestPtrErrCnt = 0) AND ;;
   ($SC_$CPU_DS_FilterLoadCnt = 1) AND ($SC_$CPU_DS_FilterPtrErrCnt = 0) then
  write "<*> Passed (9000) - Housekeeping telemetry initialized properly."
  ut_setrequirements DS_9000, "P"
else
  write "<!> Failed (9000) - Housekeeping telemetry NOT initialized properly at startup."
  write "CMDPC                = ", $SC_$CPU_DS_CMDPC
  write "CMDEC                = ", $SC_$CPU_DS_CMDEC
  write "Disabled Pkts        = ", $SC_$CPU_DS_DisabledPktCnt
  write "Failed Filter Pkts   = ", $SC_$CPU_DS_FilteredPktCnt
  write "Passed Filter Pkts   = ", $SC_$CPU_DS_PassedPktCnt
  write "Good i/o writes      = ", $SC_$CPU_DS_FileWriteCnt
  write "Bad i/o writes       = ", $SC_$CPU_DS_FileWriteErrCnt
  write "Good hdr writes      = ", $SC_$CPU_DS_FileUpdCnt
  write "Bad hdr writes       = ", $SC_$CPU_DS_FileUpdErrCnt
  write "Dest Tbl Loads       = ", $SC_$CPU_DS_DestLoadCnt
  write "Dest Tbl ptr fails   = ", $SC_$CPU_DS_DestPtrErrCnt
  write "Filter Tbl Loads     = ", $SC_$CPU_DS_FilterLoadCnt
  write "Filter Tbl ptr fails = ", $SC_$CPU_DS_FilterPtrErrCnt
  ut_setrequirements DS_9000, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 1.5: Enable DEBUG Event Messages "
write ";***********************************************************************"
local cmdCtr = $SC_$CPU_EVS_CMDPC + 1

;; Enable DEBUG events for the DS application ONLY
/$SC_$CPU_EVS_EnaAppEVTType Application=DSAppName DEBUG

ut_tlmwait $SC_$CPU_EVS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Enable Debug events command sent properly."
else
  write "<!> Failed - Enable Debug events command."
endif

write ";***********************************************************************"
write ";  Step 2.0: Commanding Test."
write ";***********************************************************************"
write ";  Step 2.1: Send the NO-OP command."
write ";***********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_NOOP_CMD_EID, "INFO", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the NO-OP Command
/$SC_$CPU_DS_NOOP

ut_tlmwait  $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1000;1004) - DS NO-OP command sent properly."
  ut_setrequirements DS_1000, "P"
  ut_setrequirements DS_1004, "P"
else
  write "<!> Failed (1000;1004) - DS NO-OP command did not increment CMDPC."
  ut_setrequirements DS_1000, "F"
  ut_setrequirements DS_1004, "F"
endif

;; Check for the event message
ut_tlmwait  $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1000;1004) - Expected Event Msg ",DS_NOOP_CMD_EID," rcv'd."
  ut_setrequirements DS_1000, "P"
  ut_setrequirements DS_1004, "P"
else
  write "<!> Failed (1000;1004) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_NOOP_CMD_EID,"."
  ut_setrequirements DS_1000, "F"
  ut_setrequirements DS_1004, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.2: Send the NO-OP command with an invalid length."
write ";***********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_NOOP_CMD_ERR_EID, "ERROR", 1

local errcnt = $SC_$CPU_DS_CMDEC + 1
  
;; CPU1 is the default
rawcmd = "18BBc000000200B0"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_NOOP_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.3: Utilizing the TST_DS application, send the command that  "
write ";  will set all the counters that get reset to zero (0) by the Reset  "
write ";  command to a non-zero value."
write ";***********************************************************************"
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_SET_COUNTERS_INF_EID, "INFO", 1

/$SC_$CPU_TST_DS_SetCounters

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_SET_COUNTERS_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_SET_COUNTERS_INF_EID,"."
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.4: Verify that all the counters are non-zero and send the   "
write ";  Reset command if so.                                               "
write ";***********************************************************************"
local oldIgnoredPktCnt = $SC_$CPU_DS_IgnoredPktCnt
write "Ignored Pkts before reset = ", $SC_$CPU_DS_IgnoredPktCnt

;; Check the HK telemetry
if ($SC_$CPU_DS_CMDPC > 0) AND ($SC_$CPU_DS_CMDEC > 0) AND ;;
   ($SC_$CPU_DS_DisabledPktCnt > 0) AND ($SC_$CPU_DS_IgnoredPktCnt > 0) AND ;;
   ($SC_$CPU_DS_FilteredPktCnt > 0) AND ($SC_$CPU_DS_PassedPktCnt > 0) AND ;;
   ($SC_$CPU_DS_FileWriteCnt > 0) AND ($SC_$CPU_DS_FileWriteErrCnt > 0) AND ;;
   ($SC_$CPU_DS_FileUpdCnt > 0) AND ($SC_$CPU_DS_FileUpdErrCnt > 0) AND ;;
   ($SC_$CPU_DS_DestLoadCnt > 0) AND ($SC_$CPU_DS_DestPtrErrCnt > 0) AND ;;
   ($SC_$CPU_DS_FilterLoadCnt > 0) AND ($SC_$CPU_DS_FilterPtrErrCnt > 0) then
  write "<*> Counters are all non-zero. Sending reset command."

  ;; Send the reset command
  ut_setupevents "$SC", "$CPU", {DSAppName}, DS_RESET_CMD_EID, "DEBUG", 1

  cmdCtr = $SC_$CPU_SC_CMDPC + 1

  ;; Send the Reset Command
  /$SC_$CPU_DS_ResetCtrs
  wait 5

  ;; Check for the event message
  ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
  if (UT_TW_Status = UT_Success) then
    write "<*> Passed (1001;1004) - Expected Event Msg ",DS_RESET_CMD_EID," rcv'd."
    ut_setrequirements DS_1001, "P"
    ut_setrequirements DS_1004, "P"
  else
    write "<!> Failed (1001;1004) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_RESET_CMD_EID,"."
    ut_setrequirements DS_1001, "F"
    ut_setrequirements DS_1004, "F"
  endif

  ;; Check to see if the counters were reset
  if ($SC_$CPU_DS_CMDPC = 0) AND ($SC_$CPU_DS_CMDEC = 0) AND ;;
     ($SC_$CPU_DS_DisabledPktCnt = 0) AND ;;
     ($SC_$CPU_DS_IgnoredPktCnt < oldIgnoredPktCnt) AND ;;
     ($SC_$CPU_DS_FilteredPktCnt = 0) AND ($SC_$CPU_DS_PassedPktCnt = 0) AND ;;
     ($SC_$CPU_DS_FileWriteCnt = 0) AND ($SC_$CPU_DS_FileWriteErrCnt = 0) AND ;;
     ($SC_$CPU_DS_FileUpdCnt = 0) AND ($SC_$CPU_DS_FileUpdErrCnt = 0) AND ;;
     ($SC_$CPU_DS_DestLoadCnt = 0) AND ($SC_$CPU_DS_DestPtrErrCnt = 0) AND ;;
     ($SC_$CPU_DS_FilterLoadCnt = 0) AND ($SC_$CPU_DS_FilterPtrErrCnt = 0) then
    write "<*> Passed (1001) - Counters all reset to zero."
    ut_setrequirements DS_1001, "P"
  else
    write "<!> Failed (1001) - Counters did not reset to zero."
    ut_setrequirements DS_1001, "F"
  endif
else
  write "<!> Reset command not sent because at least 1 counter is set to 0."
endif

;; Write out the counters for verification
write "CMDPC                = ", $SC_$CPU_DS_CMDPC
write "CMDEC                = ", $SC_$CPU_DS_CMDEC
write "Disabled Pkts        = ", $SC_$CPU_DS_DisabledPktCnt
write "Ignored Pkts         = ", $SC_$CPU_DS_IgnoredPktCnt
write "Failed Filter Pkts   = ", $SC_$CPU_DS_FilteredPktCnt
write "Passed Filter Pkts   = ", $SC_$CPU_DS_PassedPktCnt
write "Good i/o writes      = ", $SC_$CPU_DS_FileWriteCnt
write "Bad i/o writes       = ", $SC_$CPU_DS_FileWriteErrCnt
write "Good hdr writes      = ", $SC_$CPU_DS_FileUpdCnt
write "Bad hdr writes       = ", $SC_$CPU_DS_FileUpdErrCnt
write "Dest Tbl Loads       = ", $SC_$CPU_DS_DestLoadCnt
write "Dest Tbl ptr fails   = ", $SC_$CPU_DS_DestPtrErrCnt
write "Filter Tbl Loads     = ", $SC_$CPU_DS_FilterLoadCnt
write "Filter Tbl ptr fails = ", $SC_$CPU_DS_FilterPtrErrCnt

wait 5

write ";***********************************************************************"
write ";  Step 2.5: Send the Reset command with an invalid length.             "
write ";***********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_RESET_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; CPU1 is the default 
rawcmd = "18BBc000000201B0"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_RESET_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.6: Send an invalid command.    "
write ";***********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CMD_CODE_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; CPU1 is the default 
rawcmd = "18BBc0000001AA"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Command Rejected Counter incremented."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_CMD_CODE_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.7: Send an invalid HK Request.    "
write ";***********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_HK_REQUEST_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; CPU1 is the default 
rawcmd = "18BCC0000100"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_CMD_CODE_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.0: Clean-up - Send the Processor Reset command.             "
write ";*********************************************************************"
/$SC_$CPU_ES_PROCESSORRESET
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

drop ut_requirement ; needed to clear global variables
drop ut_req_array_size ; needed to clear global variables

write ";*********************************************************************"
write ";  End procedure $SC_$CPU_sc_gencmds"
write ";*********************************************************************"
ENDPROC
