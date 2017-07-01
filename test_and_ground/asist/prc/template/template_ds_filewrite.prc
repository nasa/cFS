PROC $sc_$cpu_ds_filewrite
;*******************************************************************************
;  Test Name:  ds_filewrite
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	This test verifies that the CFS Data Storage (DS) application writes
;	messages to files according to the requirements. Also, this test
;	verifies the DS application commands and that DS handles anomalies
;	appropriately.
;
;  Requirements Tested
;   cDS1002	For all DS commands, if the length contained in the message
;		header is not equal to the expected length, DS shall reject the
;		command and issue an event message.
;   cDS1004	If DS accepts any command as valid, DS shall execute the
;		command, increment the DS Valid Command Counter and issue an
;		event message.
;   cDS1005	If DS rejects any command, DS shall abort the command execution,
;		increment the DS Command Rejected Counter and issue an event
;		message.
;   cDS3000	DS shall construct filenames based on the following definitions
;		provided in the Destination File table:
;		path + base + [time/sequence characters] + extension
;   cDS3000.1	If Filename Type indicates naming based on Sequence, the
;		constructed filename will include a character representation of
;		the Sequence Count value from the Destination File table.
;   cDS3000.1.1	If Filename Type indicates naming based on Sequence, the value
;		of Sequence Count shall be incremented each time a file is
;		created.
;   cDS3000.2	If Filename Type indicates naming based on Time, the constructed
;		filename will include a character representation of the packet
;		timestamp.
;   cDS3000.2.1	If Filename Type indicates naming based on Time, the file date
;		and time shall be represented in the following format:
;		"YYYYDDDHHMMSS".
;   cDS3001	Prior to writing a packet to an existing destination file, DS
;		shall verify that the resulting file size (after the write)
;		shall not exceed the file size limit as defined in the
;		Destination File Table.
;   cDS3001.1	If the resulting file size would exceed the file size limit then
;		DS shall close the existing file and open another file.
;   cDS3001.2	A minimum of one packet shall be written to a file.
;   cDS3002	Periodically, DS shall test the age of all open destination
;		files. If any file age exceeds the maximum age defined for that
;		file in the Destination File Table, then DS shall close the
;		file.
;   cDS3002.1	DS shall update file age and test age limits every time DS 
;		processes its housekeeping telemetry request command.
;   cDS3003	Each DS destination file shall contain a primary cFE file header
;		with the following information:
;			a. Content Type
;			b. Sub Type
;			c. Primary header length
;			d. Spacecraft ID
;			e. Processor ID
;			f. Application ID
;			g. File creation time (seconds)
;			h. File creation time (sub-seconds)
;   cDS3004	Each DS destination file shall contain a secondary file header
;		with the following information:
;			a. File close time (seconds)
;			b. File close time (sub-seconds)
;			c. File table index
;			d. Qualified filename
;   cDS3005	For every file write error, DS shall:
;			a. Send an event message
;			b. Increment the total number of file write errors
;			c. Close the destination file
;			d. Disable the destination
;   cDS5000	Upon receipt of a Disable command, DS shall stop filtering and
;		storing messages. NOTE: This command will set the Packet
;		Processing State to DISABLED.
;   cDS5001	Upon receipt of a Enable command, DS shall begin filtering and
;		storing messages. NOTE: This command will set the Packet
;		Processing State to ENABLED.
;   cDS5002	Upon receipt of a Close File command, DS shall close the
;		command-specified files.
;   cDS5003	Upon receipt of a Set File Basename command, DS shall set the
;		file basename for the command-specified destination file.
;   cDS5004	Upon receipt of a Set Next File Sequence Counter command, DS
;		shall set the command-specified file counter for the
;		command-specified Destination File.
;   cDS5005	Upon receipt of an Enable Destination File command, DS shall
;		enable file collection for the command-specified Destination
;		file.
;   cDS5006	Upon receipt of a Disable Destination File command, DS shall
;		disable file collection for the command-specified Destination
;		file.
;   cDS5011	Upon receipt of a Set Destination Type for a destination file
;		type command, DS shall update the Destination type fo the
;		command-specified destination type.
;   cDS5012	Upon receipt of a Set Path for a destination filename command,
;		DS shall update the path to the command-specified path.
;   cDS5013	Upon receipt of a Set Extension for a destination filename
;		command, DS shall update the file extension to the command-
;		spacified extension.
;   cDS5014	Upon receipt of a Set Maximum Size for a destination file 
;		command, DS shall update the maximum file size to the command-
;		specified maximum.
;   cDS5015	Upon receipt of a Set Maximum Age for a destination file
;		command, DS shall update the maximum destination file age to the
;		command-specified maximum age.
;   cDS8000	DS shall generate a housekeeping message containing the
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
;   cDS9000	Upon initialization of the SC Application, SC shall initialize
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
;
;	Date		   Name		Description
;	10/01/09	Walt Moleski	Original Procedure.
;	12/08/10	Walt Moleski	Modified the procedure to use variables
;					for the application name and ram disk.
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

;; Turn off logging for the includes
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
#include "ds_appdefs.h"
#include "ds_events.h"
#include "tst_ds_events.h"

%liv (log_procedure) = logging

#define DS_1002		0
#define DS_1004		1
#define DS_1005		2
#define DS_3000		3
#define DS_30001	4
#define DS_300011	5
#define DS_30002	6
#define DS_300021	7
#define DS_3001		8
#define DS_30011	9
#define DS_30012	10
#define DS_3002 	11
#define DS_30021 	12
#define DS_3003 	13
#define DS_3004 	14
#define DS_3005 	15
#define DS_5000		16
#define DS_5001		17
#define DS_5002		18
#define DS_5003		19
#define DS_5004		20
#define DS_5005		21
#define DS_5006		22
#define DS_5011		23
#define DS_5012		24
#define DS_5013		25
#define DS_5014		26
#define DS_5015		27
#define DS_8000		28
#define DS_9000		29

global ut_req_array_size = 29
global ut_requirement[0 .. ut_req_array_size]

for i = 0 to ut_req_array_size DO
  ut_requirement[i] = "U"
enddo

;**********************************************************************
; Set the local values
;**********************************************************************
local cfe_requirements[0 .. ut_req_array_size] = ["DS_1002", "DS_1004", "DS_1005", "DS_3000", "DS_3000.1", "DS_3000.1.1", "DS_3000.2", "DS_3000.2.1", "DS_3001", "DS_3001.1", "DS_3001.2", "DS_3002", "DS_3002.1", "DS_3003", "DS_3004", "DS_3005", "DS_5000", "DS_5001", "DS_5002", "DS_5003", "DS_5004", "DS_5005", "DS_5006", "DS_5011", "DS_5012", "DS_5013", "DS_5014", "DS_5015", "DS_8000", "DS_9000" ]

;**********************************************************************
; Define local variables
;**********************************************************************
LOCAL rawcmd
LOCAL fileTblPktId, fileTblAppId
LOCAL filterTblPktId, filterTblAppId
local DSAppName = "DS"
local fileTblName = DSAppName & "." & DS_DESTINATION_TBL_NAME
local filterTblName = DSAppName & "." & DS_FILTER_TBL_NAME
local ramDir = "RAM:0"
local hostCPU = "$CPU"

;; Set the pkt and app Ids for the appropriate CPU
;; CPU1 is the default
fileTblPktId = "0F76"
fileTblAppId = 3958
filterTblPktId = "0F77"
filterTblAppId = 3959

write ";***********************************************************************"
write ";  Step 1.0: Data Storage Test Setup."
write ";***********************************************************************"
write ";  Step 1.1: Command a Power-on Reset on ",hostCPU
write ";***********************************************************************"
/$SC_$CPU_ES_POWERONRESET
wait 10
                                                                                
close_data_center
wait 60

cfe_startup {hostCPU}
wait 5

write ";***********************************************************************"
write ";  Step 1.2: Upload the initial DS table load images to $CPU."
write ";***********************************************************************"
;; Create the initial tables for this procedure
s $sc_$cpu_ds_tbl2

;; Parse the filename configuration parameters for the default table filenames
local destFileName = DS_DEF_DEST_FILENAME
local filterFileName = DS_DEF_FILTER_FILENAME
local slashLoc = %locate(destFileName,"/")

;; loop until all slashes are found for the Destination File Table Name
while (slashLoc <> 0) do
  destFileName = %substring(destFileName,slashLoc+1,%length(destFileName))
  slashLoc = %locate(destFileName,"/")
enddo

write "==> Default Destination File Table filename = '",destFileName,"'"

slashLoc = %locate(filterFileName,"/")
;; loop until all slashes are found for the Filter Table Name
while (slashLoc <> 0) do
  filterFileName = %substring(filterFileName,slashLoc+1,%length(filterFileName))
  slashLoc = %locate(filterFileName,"/")
enddo

write "==> Default Filter Table filename = '",filterFileName,"'"

;; Upload the files to $CPU
s ftp_file("CF:0/apps", "ds_fwfile.tbl", destFileName, hostCPU, "P")
s ftp_file("CF:0/apps", "ds_fwfilter.tbl", filterFileName, hostCPU, "P")

wait 5

write ";***********************************************************************"
write ";  Step 1.3: Display the Housekeeping pages "
write ";***********************************************************************"
page $SC_$CPU_DS_HK
page $SC_$CPU_TST_DS_HK
page $SC_$CPU_DS_FILTER_TBL
page $SC_$CPU_DS_FILE_TBL

write ";***********************************************************************"
write ";  Step 1.4:  Start the Data Storage (DS) and Test Applications.     "
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
local cmdCtr = $SC_$CPU_EVS_CMDPC + 2

;; Enable DEBUG events for the DS and CFE_TBL applications ONLY
/$SC_$CPU_EVS_EnaAppEVTType Application=DSAppName DEBUG
wait 2
/$SC_$CPU_EVS_EnaAppEVTType Application="CFE_TBL" DEBUG

ut_tlmwait $SC_$CPU_EVS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Enable Debug events command sent properly."
else
  write "<!> Failed - Enable Debug events command."
endif

write ";***********************************************************************"
write ";  Step 2.0: File Creation Tests."
write ";***********************************************************************"
write ";  Step 2.1: Utilizing the TST_DS application, send a message to the DS"
write ";  application that will pass the filtering algorithm and uses a "
write ";  Destination File Table entry that specifies Sequence file type naming"
write ";***********************************************************************"
write ";  Step 2.1.1: Dump the Destination File and Packet Filter Tables"
write ";***********************************************************************"
;; Dump the Packet Filter Table
s get_tbl_to_cvt (ramDir,filterTblName,"A","$cpu_filtertbl211",hostCPU,filterTblPktId)
;; Dump the Destination File Table
s get_tbl_to_cvt (ramDir,fileTblName,"A","$cpu_filetbl211",hostCPU,fileTblPktId)

write ";***********************************************************************"
write ";  Step 2.1.2: Parse the tables to find a MessageID to use for the "
write ";  Sequence file type naming entry."
write ";***********************************************************************"
;; Turn off logging for this step
logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

;; Parse the Destination File Table to find a Sequence File Name Type
;; The first entry found will be used
local seqFileEntry = 0
local foundEntry = FALSE
for i = 0 to DS_DEST_FILE_CNT-1 do
  if ($SC_$CPU_DS_DF_TBL[i].FileNameType = DS_BY_COUNT) AND (foundEntry = FALSE) then
    seqFileEntry = i
    foundEntry = TRUE
  endif
enddo

;; Since the table index is 0-based, subtract 1
;;if (foundEntry = TRUE) then
;;  seqFileEntry = seqFileEntry - 1
;;endif

;; Parse the Packet Filter Table to find a MessageID 
;; using the Destination File Entry found above
local filterEntry = 0
foundEntry = FALSE
for i = 0 to DS_PACKETS_IN_FILTER_TABLE-1 do
  for j = 0 to DS_FILTERS_PER_PACKET-1 do
    if ($SC_$CPU_DS_PF_TBL[i].FilterParams[j].Index = seqFileEntry) AND ;;
       ($SC_$CPU_DS_PF_TBL[i].MessageID <> 0) AND (foundEntry = FALSE) then
      filterEntry = i
      foundEntry = TRUE
    endif
  enddo
enddo

%liv (log_procedure) = logging

write "==> Found file entry at ", seqFileEntry
write "==> Found filter entry at ", filterEntry

local seqMsgID = $SC_$CPU_DS_PF_TBL[filterEntry].MessageID

write ";***********************************************************************"
write ";  Step 2.1.3: Send the TST_DS command to send a message to DS using the"
write ";  MessageID found above. Since the message being set will not exceed "
write ";  the file size constraint, the FileState HK should contain info about"
write ";  the file that was created. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

local expFileWrites = $SC_$CPU_DS_FileWriteCnt + 4
local expPassedPkts = $SC_$CPU_DS_PassedPktCnt + 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=1 Pattern=x'55'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the destination file was created with the appropriate name and 
;; is marked "open"
local fileName1 = ""
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName1 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000;3000.1;3000.1.1) - A file was created with name '",fileName1,"'"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30001, "A"
  ut_setrequirements DS_300011, "A"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif

  ;; Check the counters
  if (expFileWrites = $SC_$CPU_DS_FileWriteCnt) AND ;;
     (expPassedPkts = $SC_$CPU_DS_PassedPktCnt) then
    write "<*> Passed - The file write and passed packet counters are correct."
  else
    write "<!> Failed - The file write and passed packet counters are not as expected."
    write "=> File Write Cnt = ",$SC_$CPU_DS_FileWriteCnt,"; Expected ",expFileWrites
    write "=> Passed Pkt Cnt = ",$SC_$CPU_DS_PassedPktCnt,"; Expected ",expPassedPkts
  endif
else
  write "<!> Failed (3000;3000.1;3000.1.1;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30001, "F"
  ut_setrequirements DS_300011, "F"
  ut_setrequirements DS_3001, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.2: Send the TST_DS command again using a message type that "
write ";  will exceed the file size of the destination file entry."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG3_SENT_INF_EID, "INFO", 1

local expHdrUpdates = $SC_$CPU_DS_FileUpdCnt + 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'AA'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify a new destination file was created with the appropriate name, 
;; is marked "open" and the first file is now closed.
local fileName2 = ""
local slashLoc
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName2 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000;3000.1;3000.1.1) - A file was created with name '",fileName2,"'"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30001, "A"
  ut_setrequirements DS_300011, "A"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif

  if (fileName1 <> fileName2) then
    write "<*> Passed (3001.1) - A new file was opened"
    ut_setrequirements DS_30011, "P"
  else
    write "<!> Failed (3001.1) - The message did not create a new file"
    ut_setrequirements DS_30011, "F"
  endif

  ;; Verify the Header Update counter
  if (expHdrUpdates = $SC_$CPU_DS_FileUpdCnt) then
    write "<*> Passed - The header update counter incremented as expected."
  else
    write "<!> Failed (3001.1) - The header update counter is not what was expected."
    write "=> Header Update Cnt = ",$SC_$CPU_DS_FileUpdCnt,"; Expected ",expHdrUpdates
  endif

  ;; Download the first file in order to perform post-test analysis
  if (fileName1 <> "") then
    ut_setrequirements DS_3003, "A"
    ut_setrequirements DS_3004, "A"
    write "==> Downloading '",fileName1,"'"
    ;;write "==> Location of / = ",%locate(fileName1,"/")
    slashLoc = %locate(fileName1,"/")
    while (slashLoc <> 0) do
      fileName1 = %substring(fileName1,slashLoc+1,%length(fileName1))
      slashLoc = %locate(fileName1,"/")
    enddo
    write "==> FileName Only = '",fileName1,"'"
    ;; Need to parse the filename out of the variable above
    ;; before using it in the call below
    s ftp_file(ramDir, fileName1, fileName1, hostCPU, "G")
  else
    write "<!> Failed (3003;3004) - Filename not specified. Could not download."
    ut_setrequirements DS_3003, "F"
    ut_setrequirements DS_3004, "F"
  endif
else
  write "<!> Failed (3000;3000.1;3000.1.1;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30001, "F"
  ut_setrequirements DS_300011, "F"
  ut_setrequirements DS_3001, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.3: Test that DS will store at least 1 message in a destination"
write ";  file even though the message will exceed the file size."
write ";***********************************************************************"
write ";  Step 2.3.1: Send the Close command for the file created in Step 2.2."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Close command
/$SC_$CPU_DS_CloseFile FileIndex=seqFileEntry

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5002) - DS Close File command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5002, "P"
else
  write "<!> Failed (1004;5002) - DS Close File command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5002, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5002) - Expected Event Msg ",DS_CLOSE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5002, "P"
else
  write "<!> Failed (1004;5002) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_CLOSE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5002, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.3.2: Send the TST_DS_SendMessage command using a message type "
write ";  that exceeds the size of the destination file entry."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG5_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=5 Pattern=x'A5'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG5_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG5_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify a new destination file was created with the appropriate name, 
;; is marked "open" and the last file is now closed.
local fileName3 = ""
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName3 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000;3000.1;3000.1.1) - A file was created with name '",fileName3,"'"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30001, "A"
  ut_setrequirements DS_300011, "A"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif

  if (fileName2 <> fileName3) then
    write "<*> Passed (3001.1) - A new file was opened"
    ut_setrequirements DS_30011, "P"
    ut_setrequirements DS_30012, "A"
  else
    write "<!> Failed (3001.1) - The message did not create a new file"
    ut_setrequirements DS_30011, "F"
    ut_setrequirements DS_30012, "F"
  endif

  ;; Download the first file in order to perform post-test analysis
  if (fileName2 <> "") then
    ut_setrequirements DS_3003, "A"
    ut_setrequirements DS_3004, "A"
    write "==> Downloading '",fileName2,"'"
    slashLoc = %locate(fileName2,"/")
    while (slashLoc <> 0) do
      fileName2 = %substring(fileName2,slashLoc+1,%length(fileName2))
      slashLoc = %locate(fileName2,"/")
    enddo
    write "==> FileName Only = '",fileName2,"'"
    ;; Need to parse the filename out of the variable above
    ;; before using it in the call below
    s ftp_file(ramDir, fileName2, fileName2, hostCPU, "G")
  else
    write "<!> Failed (3003;3004) - Filename not specified. Could not download."
    ut_setrequirements DS_3003, "F"
    ut_setrequirements DS_3004, "F"
  endif
else
  write "<!> Failed (3000;3000.1;3000.1.1;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30001, "F"
  ut_setrequirements DS_300011, "F"
  ut_setrequirements DS_3001, "F"
endif

write ";***********************************************************************"
write ";  Step 2.4: Utilizing the TST_DS application, send valid messages to "
write ";  the DS application that will pass filtering and specifies Time file "
write ";  type naming."
write ";***********************************************************************"
write ";  Step 2.4.1: Parse the tables to find a MessageID to use for the Time "
write ";  file type naming entry."
write ";***********************************************************************"
;; Turn off logging for this step
logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

;; Parse the Destination File Table to find a Sequence File Name Type
;; The first entry found will be used
local timeFileEntry = 0
foundEntry = FALSE
for i = 0 to DS_DEST_FILE_CNT-1 do
  if ($SC_$CPU_DS_DF_TBL[i].FileNameType = DS_BY_TIME) AND (foundEntry = FALSE) then
    timeFileEntry = i
    foundEntry = TRUE
  endif
enddo

;; Parse the Packet Filter Table to find a MessageID 
;; using the Destination File Entry found above
filterEntry = 0
foundEntry = FALSE
for i = 0 to DS_PACKETS_IN_FILTER_TABLE-1 do
  for j = 0 to DS_FILTERS_PER_PACKET-1 do
    if ($SC_$CPU_DS_PF_TBL[i].FilterParams[j].Index = timeFileEntry) AND ;;
       ($SC_$CPU_DS_PF_TBL[i].MessageID <> 0) AND (foundEntry = FALSE) then
      filterEntry = i
      foundEntry = TRUE
    endif
  enddo
enddo

%liv (log_procedure) = logging

write "==> Found file entry at ", timeFileEntry
write "==> Found filter entry at ", filterEntry

local timeMsgID = $SC_$CPU_DS_PF_TBL[filterEntry].MessageID
local fileAge = $SC_$CPU_DS_DF_TBL[timeFileEntry].FileAge

write ";***********************************************************************"
write ";  Step 2.4.2: Send the TST_DS command to send a message to DS using the"
write ";  MessageID found above. Since the message being set will not exceed "
write ";  the file size constraint, the FileState HK should contain info about"
write ";  the file that was created. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=timeMsgID MsgType=1 Pattern=x'55'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the destination file was created with the appropriate name and 
;; is marked "open"
fileName1 = ""
if ($SC_$CPU_DS_FileState[timeFileEntry].FileName <> "") then
  fileName1 = $SC_$CPU_DS_FileState[timeFileEntry].FileName
  write "<*> Passed (3000;3000.2;3000.2.1) - A file was created with name '",fileName1,"'"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30002, "A"
  ut_setrequirements DS_300021, "A"

  if ($SC_$CPU_DS_FileState[timeFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif
else
  write "<!> Failed (3000;3000.2;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30002, "F"
  ut_setrequirements DS_300021, "F"
  ut_setrequirements DS_3001, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.5: Wait for the file created above to exceed its maximum age."
write ";***********************************************************************"
;; Loop here for each HK packet and see the Age updating
;; This verifies DS3002
local reportedAge = $SC_$CPU_DS_FileState[timeFileEntry].FileAge
local waitTime = fileAge - reportedAge
local originalAge = reportedAge

fileName1 = $SC_$CPU_DS_FileState[timeFileEntry].FileName

while (waitTime > 0) do
  ;; Sent the Get File Info command to check the File Status
  cmdCtr = $SC_$CPU_DS_CMDPC + 1

  /$SC_$CPU_DS_GetFileInfo
  wait 5

  reportedAge = $SC_$CPU_DS_FileState[timeFileEntry].FileAge
  write "new file age = ",reportedAge

  ;; If the age is updating, then DS_3002 and DS_3002.1 are verified
  if (originalAge <> reportedAge) then
    ut_setrequirements DS_3002, "P"
    ut_setrequirements DS_30021, "P"
  endif

  waitTime = fileAge - reportedAge
  write "timeout calculation = ",waitTime

  ;; Check the File Status to see if the file was closed
  ;; NOTE: If the file was closed, the name will be empty and age will be 0
  if ($SC_$CPU_DS_FileState[timeFileEntry].FileName = "") AND ;;
     ($SC_$CPU_DS_FileState[timeFileEntry].FileAge = 0) then
    waitTime = 0
    write "HK reports the file is no longer open"
  endif
enddo

;; Download the file in order to perform post-test analysis
if (fileName1 <> "") then
  ut_setrequirements DS_3003, "A"
  ut_setrequirements DS_3004, "A"
  write "==> Downloading '",fileName1,"'"
  slashLoc = %locate(fileName1,"/")
  while (slashLoc <> 0) do
    fileName1 = %substring(fileName1,slashLoc+1,%length(fileName1))
    slashLoc = %locate(fileName1,"/")
  enddo
  write "==> FileName Only = '",fileName1,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the call below
  s ftp_file(ramDir, fileName1, fileName1, hostCPU, "G")
else
  write "<!> Failed (3003;3004) - Filename not specified. Could not download."
  ut_setrequirements DS_3003, "F"
  ut_setrequirements DS_3004, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.6: Using the TST_DS application, send a message for the "
write ";  sequence file type naming entry that is the exact size specified as "
write ";  maximum file size."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG4_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=4 Pattern=x'55'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG4_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG4_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify a new destination file was created with the appropriate name, 
;; is marked "open".
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName2 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000;3000.1;3000.1.1) - A file was created with name '",fileName2,"'"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30001, "A"
  ut_setrequirements DS_300011, "A"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif
else
  write "<!> Failed (3000;3000.1;3000.1.1;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30001, "F"
  ut_setrequirements DS_300011, "F"
  ut_setrequirements DS_3001, "F"
endif

;; Download the file in order to perform post-test analysis
if (fileName3 <> "") then
  ut_setrequirements DS_3003, "A"
  ut_setrequirements DS_3004, "A"
  write "==> Downloading '",fileName3,"'"
  slashLoc = %locate(fileName3,"/")
  while (slashLoc <> 0) do
    fileName3 = %substring(fileName3,slashLoc+1,%length(fileName3))
    slashLoc = %locate(fileName3,"/")
  enddo
  write "==> FileName Only = '",fileName3,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the call below
  s ftp_file(ramDir, fileName3, fileName3, hostCPU, "G")
else
  write "<!> Failed (3003;3004) - Filename not specified. Could not download."
  ut_setrequirements DS_3003, "F"
  ut_setrequirements DS_3004, "F"
endif

write ";***********************************************************************"
write ";  Step 2.7: Send the TST_DS command again using a message type that "
write ";  will exceed the file size of the destination file entry."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG3_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'AA'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify a new destination file was created with the appropriate name, 
;; is marked "open" and the first file is now closed.
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName3 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000;3000.1;3000.1.1) - A file was created with name '",fileName3,"'"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30001, "A"
  ut_setrequirements DS_300011, "A"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif

  if (fileName3 <> fileName2) then
    write "<*> Passed (3001.1) - A new file was opened"
    ut_setrequirements DS_30011, "P"
  else
    write "<!> Failed (3001.1) - The message did not create a new file"
    ut_setrequirements DS_30011, "F"
  endif

  ;; Download the first file in order to perform post-test analysis
  if (fileName2 <> "") then
    ut_setrequirements DS_3003, "A"
    ut_setrequirements DS_3004, "A"
    write "==> Downloading '",fileName2,"'"
    slashLoc = %locate(fileName2,"/")
    while (slashLoc <> 0) do
      fileName2 = %substring(fileName2,slashLoc+1,%length(fileName2))
      slashLoc = %locate(fileName2,"/")
    enddo
    write "==> FileName Only = '",fileName2,"'"
    ;; Need to parse the filename out of the variable above
    ;; before using it in the call below
    s ftp_file(ramDir, fileName2, fileName2, hostCPU, "G")
  else
    write "<!> Failed (3003;3004) - Filename not specified. Could not download."
    ut_setrequirements DS_3003, "F"
    ut_setrequirements DS_3004, "F"
  endif
else
  write "<!> Failed (3000;3000.1;3000.1.1;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30001, "F"
  ut_setrequirements DS_300011, "F"
  ut_setrequirements DS_3001, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.8: Utilizing the TST_DS application, send a messages to the DS"
write ";  application that will cause several file write errors."
write ";***********************************************************************"
write ";  Step 2.8.1: Utilizing the TST_DS application, send messages to the DS"
write ";  application that will create a time file."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG2_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=timeMsgID MsgType=2 Pattern=x'AA'
wait 10

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG2_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify a new destination file was created with the appropriate name
;; and is marked "open" 
if ($SC_$CPU_DS_FileState[timeFileEntry].FileName <> "") then
  fileName1 = $SC_$CPU_DS_FileState[timeFileEntry].FileName
  write "<*> Passed (3000;3000.2;3000.2.1) - A file was created with name '",fileName1,"'"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30002, "A"
  ut_setrequirements DS_300021, "A"

  if ($SC_$CPU_DS_FileState[timeFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif
else
  write "<!> Failed (3000;3000.2;3000.2.1;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30002, "F"
  ut_setrequirements DS_300021, "F"
  ut_setrequirements DS_3001, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.8.2: Send the DS Set Basename command to set the basenames for"
write ";  the Destination File Table entries used above such that the created "
write ";  file names will exceed the OS_MAX_API_NAME limit and generate a file"
write ";  write error."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_BASE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command for the Sequence File Type naming entry
/$SC_$CPU_DS_SetBasename FileIndex=seqFileEntry BaseName="sequenceFile"
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5003) - DS Set Basename command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5003, "P"
else
  write "<!> Failed (1004;5003) - DS Set Basename command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5003, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5003) - Expected Event Msg ",DS_BASE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5003, "P"
else
  write "<!> Failed (1004;5003) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_BASE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5003, "F"
endif

;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_BASE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command for the Time File Type naming entry
/$SC_$CPU_DS_SetBasename FileIndex=timeFileEntry BaseName="timeFile"
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5003) - DS Set Basename command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5003, "P"
else
  write "<!> Failed (1004;5003) - DS Set Basename command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5003, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5003) - Expected Event Msg ",DS_BASE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5003, "P"
else
  write "<!> Failed (1004;5003) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_BASE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5003, "F"
endif

write ";***********************************************************************"
write ";  Step 2.8.3: Using the TST_DS application, send messages to the DS "
write ";  application that will cause the file write errors to occur."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CREATE_FILE_ERR_EID, "ERROR", 2
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG3_SENT_INF_EID, "INFO", 3

local expectedErrCnt = $SC_$CPU_DS_FileWriteErrCnt + 2

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=1 Pattern=x'55'
wait 5

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=timeMsgID MsgType=3 Pattern=x'55'
wait 5

;; Check for the event messages
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID," was not generated."
endif

ut_tlmwait $SC_$CPU_find_event[3].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID," was not generated."
endif

if ($SC_$CPU_find_event[2].num_found_messages = 2) then
  write "<*> Passed (3005) - Expected Event Msg ",DS_CREATE_FILE_ERR_EID," rcv'd twice."
  ut_setrequirements DS_3005, "P"
else
  write "<!> Failed (3005) - Expected Event Msg ",DS_CREATE_FILE_ERR_EID," to be generated twice."
  ut_setrequirements DS_3005, "F"
endif

;; Check that File Write Error count in HK incremented twice as well
if (expectedErrCnt = $SC_$CPU_DS_FileWriteErrCnt) then
  write "<*> Passed (3005) - File Write Error count incremented properly."
  ut_setrequirements DS_3005, "P"
else
  write "<!> Failed (3005) - File Write Error does not reflect the expected count."
  ut_setrequirements DS_3005, "F"
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Check that the Open State in HK indicates closed
if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_CLOSED) then
  write "<*> Passed (3005) - The sequence file entry is marked 'CLOSED'"
  ut_setrequirements DS_3005, "P"
else
  write "<!> Failed (3005) - The sequence file entry is still marked 'OPEN'"
  ut_setrequirements DS_3005, "F"
endif

if ($SC_$CPU_DS_FileState[timeFileEntry].OpenState = DS_CLOSED) then
  write "<*> Passed (3005) - The time file entry is marked 'CLOSED'"
  ut_setrequirements DS_3005, "P"
else
  write "<!> Failed (3005) - The time file entry is still marked 'OPEN'"
  ut_setrequirements DS_3005, "F"
endif

;; Check that the Enable State in HK indicates disabled
if ($SC_$CPU_DS_FileState[seqFileEntry].EnableState = DS_DISABLED) then
  write "<*> Passed (3005) - The sequence file entry has been disabled."
  ut_setrequirements DS_3005, "P"
else
  write "<!> Failed (3005) - The sequence file entry is still enabled."
  ut_setrequirements DS_3005, "F"
endif

if ($SC_$CPU_DS_FileState[timeFileEntry].EnableState = DS_DISABLED) then
  write "<*> Passed (3005) - The time file entry has been disabled."
  ut_setrequirements DS_3005, "P"
else
  write "<!> Failed (3005) - The time file entry is still enabled."
  ut_setrequirements DS_3005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.8.4: Download the files that were open prior to Step 2.8.3."
write ";***********************************************************************"
;; Download the sequence file
ut_setrequirements DS_3003, "A"
ut_setrequirements DS_3004, "A"
if (fileName3 <> "") then
  write "==> Downloading '",fileName3,"'"
  slashLoc = %locate(fileName3,"/")
  while (slashLoc <> 0) do
    fileName3 = %substring(fileName3,slashLoc+1,%length(fileName3))
    slashLoc = %locate(fileName3,"/")
  enddo
  write "==> FileName Only = '",fileName3,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the call below
  s ftp_file(ramDir, fileName3, fileName3, hostCPU, "G")
endif

;; Download the time file
if (fileName1 <> "") then
  write "==> Downloading '",fileName1,"'"
  slashLoc = %locate(fileName1,"/")
  while (slashLoc <> 0) do
    fileName1 = %substring(fileName1,slashLoc+1,%length(fileName1))
    slashLoc = %locate(fileName1,"/")
  enddo
  write "==> FileName Only = '",fileName1,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the call below
  s ftp_file(ramDir, fileName1, fileName1, hostCPU, "G")
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.9: Reload the Destination File Table in order to restore the"
write ";  table entries to a usable state."
write ";***********************************************************************"
;; Let's restore the Destination File Table by reloading the initial table
cmdCtr = $SC_$CPU_TBL_CMDPC + 1

s load_table("ds_fwfile.tbl",hostCPU)

ut_tlmwait $SC_$CPU_TBL_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Load command sent successfully."
else
  write "<!> Failed - Load command did not execute successfully."
endif

wait 5

;; Validate the table loaded above
ut_setupevents "$SC","$CPU","CFE_TBL",CFE_TBL_VAL_REQ_MADE_INF_EID,"DEBUG", 1
ut_setupevents "$SC","$CPU","CFE_TBL",CFE_TBL_VALIDATION_INF_EID,"INFO", 2

cmdCtr = $SC_$CPU_TBL_CMDPC + 1

/$SC_$CPU_TBL_VALIDATE INACTIVE VTABLENAME=fileTblName

ut_tlmwait $SC_$CPU_TBL_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - DS Destination File Table validate command sent."
  if ($SC_$CPU_find_event[1].num_found_messages = 1) then
    write "<*> Passed - Event Msg ",$SC_$CPU_find_event[1].eventid," Found!"
  else
    write "<!> Failed - Event Message not received for Validate command."
  endif
else
  write "<!> Failed - DS Destination File Table validation failed."
endif

wait 20

;; Sent the Activate command
ut_setupevents "$SC","$CPU","CFE_TBL",CFE_TBL_LOAD_PEND_REQ_INF_EID,"DEBUG",1
ut_setupevents "$SC","$CPU","CFE_TBL",CFE_TBL_UPDATE_SUCCESS_INF_EID,"INFO",2
  
cmdCtr = $SC_$CPU_TBL_CMDPC + 1

/$SC_$CPU_TBL_ACTIVATE ATableName=fileTblName

ut_tlmwait $SC_$CPU_TBL_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Activate DS Destination File Table command sent properly."
else
  write "<!> Failed - Activate DS Destination File Table command."
endif
 
wait 5

if ($SC_$CPU_find_event[1].num_found_messages = 1) then
  write "<*> Passed - Event message ",$SC_$CPU_find_event[1].eventid, " received"
else
  write "<!> Failed - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",CFE_TBL_LOAD_PEND_REQ_INF_EID, "."
endif

wait 5

if ($SC_$CPU_find_event[2].num_found_messages = 1) then
  write "<*> Passed - DS Destination File Table Updated successfully."
  Write "<*> Passed - Event Msg ",$SC_$CPU_find_event[2].eventid," Found!"
else
  write "<!> Failed - DS Destination File Table update failed."
  Write "<!> Failed - Event Message not received for activate command."
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.0: Commanding Tests"
write ";*********************************************************************"
write ";  Step 3.1: Send the Close File command on an open file. "
write ";*********************************************************************"
write ";  Step 3.1.1: Send the TST_DS command that will open a file."
write ";*********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=1 Pattern=x'55'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the destination file was created with the appropriate name and 
;; is marked "open"
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName1 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000;3000.1;3000.1.1) - A file was created with name '",fileName1,"'"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30001, "A"
  ut_setrequirements DS_300011, "A"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif
else
  write "<!> Failed (3000;3000.1;3000.1.1;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30001, "F"
  ut_setrequirements DS_300011, "F"
  ut_setrequirements DS_3001, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.1.2: Send the Close File command for the file opened above."
write ";*********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_CMD_EID, "DEBUG", 1

local cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Close command
/$SC_$CPU_DS_CloseFile FileIndex=seqFileEntry

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5002) - DS Close File command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5002, "P"
else
  write "<!> Failed (1004;5002) - DS Close File command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5002, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5002) - Expected Event Msg ",DS_CLOSE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5002, "P"
else
  write "<!> Failed (1004;5002) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_CLOSE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5002, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.2: Send the Close File command with an invalid length."
write ";*********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_CMD_ERR_EID, "ERROR", 1

local errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc00000060E95"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Close File Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Close File Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_CLOSE_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.3: Send the Close File command with an invalid file index. "
write ";*********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the Close command
/$SC_$CPU_DS_CloseFile FileIndex=DS_DEST_FILE_CNT

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Close File command failed as expected."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Close File command passed when failure was expected."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Expected Event Msg ",DS_CLOSE_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_CLOSE_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.4: Send the Set File Basename command."
write ";*********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_BASE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command for the Sequence File Type naming entry
/$SC_$CPU_DS_SetBasename FileIndex=seqFileEntry BaseName="newSeq"
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5003) - DS Set Basename command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5003, "P"
else
  write "<!> Failed (1004;5003) - DS Set Basename command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5003, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5003) - Expected Event Msg ",DS_BASE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5003, "P"
else
  write "<!> Failed (1004;5003) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_BASE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5003, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.5: Dump the Destination File Table to verify the change above."
write ";*********************************************************************"
;; Dump the Destination File Table
s get_tbl_to_cvt (ramDir,fileTblName,"A","$cpu_filetbl35",hostCPU,fileTblPktId)

;; Verify the table contains the change made
if ($SC_$CPU_DS_DF_TBL[seqFileEntry].Basename = "newSeq") then
  write "<*> Passed (5003) - Table entry reflects the new Basename."
  ut_setrequirements DS_5003, "P"
else
  write "<!> Failed (5003) - Table entry does not reflect the Basename set in Step 3.4."
  ut_setrequirements DS_5003, "F"
endif

write ";*********************************************************************"
write ";  Step 3.6: Using the TST_DS application, send a message that will"
write ";  create a file using the new basename. "
write ";*********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=1 Pattern=x'55'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the destination file was created with the appropriate name and 
;; is marked "open"
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName1 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000;3000.1;3000.1.1) - A file was created with name '",fileName1,"'"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30001, "A"
  ut_setrequirements DS_300011, "A"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif
else
  write "<!> Failed (3000;3000.1;3000.1.1;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30001, "F"
  ut_setrequirements DS_300011, "F"
  ut_setrequirements DS_3001, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.7: Send the Set File Basename command with an empty string."
write ";*********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_BASE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command for the Sequence File Type naming entry
/$SC_$CPU_DS_SetBasename FileIndex=seqFileEntry BaseName=""
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5003) - DS Set Basename command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5003, "P"
else
  write "<!> Failed (1004;5003) - DS Set Basename command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5003, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5003) - Expected Event Msg ",DS_BASE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5003, "P"
else
  write "<!> Failed (1004;5003) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_BASE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5003, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.8: Dump the Destination File Table to verify the change above."
write ";*********************************************************************"
;; Dump the Destination File Table
s get_tbl_to_cvt (ramDir,fileTblName,"A","$cpu_filetbl38",hostCPU,fileTblPktId)

;; Verify the table contains the change made
if ($SC_$CPU_DS_DF_TBL[seqFileEntry].Basename = "") then
  write "<*> Passed (5003) - Table entry reflects the new Basename."
  ut_setrequirements DS_5003, "P"
else
  write "<!> Failed (5003) - Table entry does not reflect the Basename set in Step 3.7."
  ut_setrequirements DS_5003, "F"
endif

write ";*********************************************************************"
write ";  Step 3.9: Using the TST_DS application, send a message that will"
write ";  create a file using the new basename. "
write ";*********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG3_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'AA'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the destination file was created with the appropriate name and 
;; is marked "open"
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName2 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000;3000.1;3000.1.1) - A file was created with name '",fileName2,"'"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30001, "A"
  ut_setrequirements DS_300011, "A"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif
else
  write "<!> Failed (3000;3000.1;3000.1.1;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30001, "F"
  ut_setrequirements DS_300011, "F"
  ut_setrequirements DS_3001, "F"
endif

;; Download fileName1
if (fileName1 <> "") then
  write "==> Downloading '",fileName1,"'"
  slashLoc = %locate(fileName1,"/")
  while (slashLoc <> 0) do
    fileName1 = %substring(fileName1,slashLoc+1,%length(fileName1))
    slashLoc = %locate(fileName1,"/")
  enddo
  write "==> FileName Only = '",fileName1,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the call below
  s ftp_file(ramDir, fileName1, fileName1, hostCPU, "G")
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.10: Send the Set File Basename command with an invalid length."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_BASE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc000004609EB"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Set Basename Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Set Basename Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_BASE_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.11: Send the Set File Basename command with invalid arguments."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_BASE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the command for the Sequence File Type naming entry
/$SC_$CPU_DS_SetBasename FileIndex=DS_DEST_FILE_CNT BaseName="badSeq"
wait 5

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Basename command failed as expected with an invalid file index."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Basename command did not increment CMDEC as expected."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Expected Event Msg ",DS_BASE_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_BASE_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.12: Send the Set Next File Sequence Counter command. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_SEQ_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command
/$SC_$CPU_DS_SetFileSeqCtr FileIndex=seqFileEntry SeqCount=99
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5004) - DS Set File Sequence Counter command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5004, "P"
else
  write "<!> Failed (1004;5004) - DS Set File Sequence Counter command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5004, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5004) - Expected Event Msg ",DS_SEQ_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5004, "P"
else
  write "<!> Failed (1004;5004) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_SEQ_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5004, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.13: Dump the Destination File Table to verify the change above"
write ";***********************************************************************"
;; Dump the Destination File Table
s get_tbl_to_cvt (ramDir,fileTblName,"A","$cpu_filetbl313",hostCPU,fileTblPktId)

;; Verify the table contains the change made
if ($SC_$CPU_DS_DF_TBL[seqFileEntry].SeqCnt = 99) then
  write "<*> Passed (5004) - Table entry reflects the new Sequence Count."
  ut_setrequirements DS_5004, "P"
else
  write "<!> Failed (5004) - Table entry does not reflect the Sequence Count set in Step 3.12."
  ut_setrequirements DS_5004, "F"
endif

write ";***********************************************************************"
write ";  Step 3.14: Using the TST_DS application, send a message that will"
write ";  create a file using the new sequence counter."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=1 Pattern=x'55'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the destination file was created with the appropriate name and 
;; is marked "open"
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName1 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000;3000.1;3000.1.1) - A file was created with name '",fileName1,"'"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30001, "A"
  ut_setrequirements DS_300011, "A"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif
else
  write "<!> Failed (3000;3000.1;3000.1.1;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30001, "F"
  ut_setrequirements DS_300011, "F"
  ut_setrequirements DS_3001, "F"
endif

;; Download fileName2
if (fileName2 <> "") then
  write "==> Downloading '",fileName2,"'"
  slashLoc = %locate(fileName2,"/")
  while (slashLoc <> 0) do
    fileName2 = %substring(fileName2,slashLoc+1,%length(fileName2))
    slashLoc = %locate(fileName2,"/")
  enddo
  write "==> FileName Only = '",fileName2,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the call below
  s ftp_file(ramDir, fileName2, fileName2, hostCPU, "G")
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.15: Send the Set File Sequence Counter command with an invalid"
write ";  length."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_SEQ_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc000000A0DE2"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Set File Sequence Count Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Set File Sequence Count Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_SEQ_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.16: Send the Set File Sequence Counter command with invalid "
write ";  arguments."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_SEQ_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; Send the command
/$SC_$CPU_DS_SetFileSeqCtr FileIndex=DS_DEST_FILE_CNT SeqCount=155
wait 5

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set File Sequence Counter command failed as expected with an invalid file index."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set File Sequence Counter command did not increment CMDEC as expected."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_SEQ_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.17: Send the Disable Destination File command. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_STATE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command
/$SC_$CPU_DS_DisableFile FileIndex=seqFileEntry
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5006) - DS Disable Destination File command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5006, "P"
else
  write "<!> Failed (1004;5006) - DS Disable Destination File command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5006, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5006) - Expected Event Msg ",DS_STATE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5006, "P"
else
  write "<!> Failed (1004;5006) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_STATE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5006, "F"
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Check that the Enable State in HK indicates disabled
if ($SC_$CPU_DS_FileState[seqFileEntry].EnableState = DS_DISABLED) then
  write "<*> Passed (5006) - The sequence file entry has been disabled."
  ut_setrequirements DS_5006, "P"
else
  write "<!> Failed (5006) - The sequence file entry is still enabled."
  ut_setrequirements DS_5006, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.18: Dump the Destination File Table to verify the change above"
write ";***********************************************************************"
;; Dump the Destination File Table
s get_tbl_to_cvt (ramDir,fileTblName,"A","$cpu_filetbl318",hostCPU,fileTblPktId)

;; Verify the table contains the change made
if ($SC_$CPU_DS_DF_TBL[seqFileEntry].FileState = DS_DISABLED) then
  write "<*> Passed (5006) - Table entry indicates that it is 'DISABLED'."
  ut_setrequirements DS_5006, "P"
else
  write "<!> Failed (5006) - Table entry stil indicates 'ENABLED'."
  ut_setrequirements DS_5006, "F"
endif

write ";***********************************************************************"
write ";  Step 3.19: Using the TST_DS application, send a message to the entry "
write ";  disabled above. The message should not get stored. "
write ";***********************************************************************"
;; Setup to verify the Filtered Packet counter
local pktCtr = $SC_$CPU_DS_FilteredPktCnt + 1

;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=1 Pattern=x'55'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the appropriate HK item counters incremented
if ($SC_$CPU_DS_FilteredPktCnt = pktCtr) then
  write "<*> Passed (5006) - The filtered Packet Counter incremented indicating that the message was not stored."
  ut_setrequirements DS_5006, "P"
else
  write "<!> Failed (5006) - The filtered Pkt counter did not increment as expected. Expected ",pktCtr, " rcv'd ",$SC_$CPU_DS_FilteredPktCnt
  ut_setrequirements DS_5006, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.20: Send the Disable Destination File command with an invalid"
write ";  length."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_STATE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc0000006079C"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Disable Destination File Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Disable Destination File Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_STATE_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.21: Send the Disable Destination File command with invalid "
write ";  arguments."
write ";***********************************************************************"
write ";  Step 3.21.1: Send the command with an invalid file index. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_STATE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; Send the command
/$SC_$CPU_DS_DisableFile FileIndex=DS_DEST_FILE_CNT
wait 5

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Disable Destination File command failed as expected with an invalid file index."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Disable Destination File command did not increment CMDEC as expected."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_STATE_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.21.2: Send the command with an invalid state. "
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_STATE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc0000005079C00000003"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Disable Destination File Command Rejected Counter incremented."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Disable Destination File Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_STATE_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.21.3: Send the command for a destination that is disabled. "
write ";  This command should be accepted. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_STATE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command
/$SC_$CPU_DS_DisableFile FileIndex=seqFileEntry
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5006) - DS Disable Destination File command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5006, "P"
else
  write "<!> Failed (1004;5006) - DS Disable Destination File command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5006, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5006) - Expected Event Msg ",DS_STATE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5006, "P"
else
  write "<!> Failed (1004;5006) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_STATE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5006, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.22: Send the Enable Destination File command. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_STATE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command
/$SC_$CPU_DS_EnableFile FileIndex=seqFileEntry
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5005) - DS Enable Destination File command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5005, "P"
else
  write "<!> Failed (1004;5005) - DS Enable Destination File command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5005) - Expected Event Msg ",DS_STATE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5005, "P"
else
  write "<!> Failed (1004;5005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_STATE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5005, "F"
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Check that the Enable State in HK indicates disabled
if ($SC_$CPU_DS_FileState[seqFileEntry].EnableState = DS_ENABLED) then
  write "<*> Passed (5005) - The sequence file entry has been enabled."
  ut_setrequirements DS_5005, "P"
else
  write "<!> Failed (5005) - The sequence file entry is still disabled."
  ut_setrequirements DS_5005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.23: Dump the Destination File Table to verify the change above"
write ";***********************************************************************"
;; Dump the Destination File Table
s get_tbl_to_cvt (ramDir,fileTblName,"A","$cpu_filetbl323",hostCPU,fileTblPktId)

;; Verify the table contains the change made
if ($SC_$CPU_DS_DF_TBL[seqFileEntry].FileState = DS_ENABLED) then
  write "<*> Passed (5005) - Table entry indicates that it is 'ENABLED'."
  ut_setrequirements DS_5005, "P"
else
  write "<!> Failed (5005) - Table entry still indicates 'DISABLED'."
  ut_setrequirements DS_5005, "F"
endif

write ";***********************************************************************"
write ";  Step 3.24: Using the TST_DS application, send a message to the entry "
write ";  enabled above. The message should get stored. "
write ";***********************************************************************"
;; Store the counters that will be checked after the message is stored.
local fileSize = $SC_$CPU_DS_FileState[seqFileEntry].FileSize
pktCtr = $SC_$CPU_DS_PassedPktCnt + 1
local writeCnt = $SC_$CPU_DS_FileWriteCnt + 1

;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=1 Pattern=x'AA'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the appropriate HK item counters changed since this message
;; will be written to the current open file
if ($SC_$CPU_DS_FileState[seqFileEntry].FileSize > fileSize) AND ;;
   ($SC_$CPU_DS_PassedPktCnt = pktCtr) AND ;;
   ($SC_$CPU_DS_FileWriteCnt = writeCnt) then
  write "<*> Passed (5005) - The message was rcv'd and stored properly."
  ut_setrequirements DS_5005, "P"
else
  write "<!> Failed (5005) - DS HK does not indicate the correct information for the message just sent."
  ut_setrequirements DS_5005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.25: Send the Enable Destination File command with an invalid"
write ";  length."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_STATE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc0000006079C"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Enable Destination File Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Enable Destination File Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_STATE_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.26: Send the Enable Destination File command with invalid "
write ";  arguments."
write ";***********************************************************************"
write ";  Step 3.26.1: Send the command with an invalid file index. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_STATE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; Send the command
/$SC_$CPU_DS_EnableFile FileIndex=DS_DEST_FILE_CNT
wait 5

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Enable Destination File command failed as expected with an invalid file index."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Enable Destination File command did not increment CMDEC as expected."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_STATE_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.26.2: Send the command with an invalid state. "
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_STATE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc0000005079C00000003"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Disable Destination File Command Rejected Counter incremented."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Disable Destination File Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_STATE_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.26.3: Send the command for a destination that is enabled. "
write ";  This command should be accepted. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_STATE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command
/$SC_$CPU_DS_EnableFile FileIndex=seqFileEntry
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5005) - DS Enable Destination File command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5005, "P"
else
  write "<!> Failed (1004;5005) - DS Enable Destination File command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5005) - Expected Event Msg ",DS_STATE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5005, "P"
else
  write "<!> Failed (1004;5005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_STATE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.27: Send the Set Destination File Type command. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_NTYPE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command
/$SC_$CPU_DS_SetFileType FileIndex=seqFileEntry FileType=DS_BY_TIME
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5011) - DS Set Destination File Type command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5011, "P"
else
  write "<!> Failed (1004;5011) - DS Set Destination File Type command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5011, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5011) - Expected Event Msg ",DS_NTYPE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5011, "P"
else
  write "<!> Failed (1004;5011) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_NTYPE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5011, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.28: Dump the Destination File Table to verify the change above"
write ";***********************************************************************"
;; Dump the Destination File Table
s get_tbl_to_cvt (ramDir,fileTblName,"A","$cpu_filetbl328",hostCPU,fileTblPktId)

;; Verify the table contains the change made
if (p@$SC_$CPU_DS_DF_TBL[seqFileEntry].FileNameType = "Time") then
  write "<*> Passed (5011) - Table entry indicates 'Time' naming."
  ut_setrequirements DS_5011, "P"
else
  write "<!> Failed (5011) - Table entry still indicates 'Count' naming."
  ut_setrequirements DS_5011, "F"
endif

write ";***********************************************************************"
write ";  Step 3.29: Using the TST_DS application, send a message that will"
write ";  create a file using the new naming."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG3_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'BB'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the destination file was created with the appropriate name and 
;; is marked "open"
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName2 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000) - A file was created with name '",fileName2,"'"
  ut_setrequirements DS_3000, "P"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif
else
  write "<!> Failed (3000;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_3001, "F"
endif

;; Download fileName1
if (fileName1 <> "") then
  write "==> Downloading '",fileName1,"'"
  slashLoc = %locate(fileName1,"/")
  while (slashLoc <> 0) do
    fileName1 = %substring(fileName1,slashLoc+1,%length(fileName1))
    slashLoc = %locate(fileName1,"/")
  enddo
  write "==> FileName Only = '",fileName1,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the call below
  s ftp_file(ramDir, fileName1, fileName1, hostCPU, "G")
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.30: Send the Set Destination File Type command with an invalid"
write ";  length."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_NTYPE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc0000006069C"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Set Destination File Type Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Set Destination File Type Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_NTYPE_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.31: Send the Set Destination File Type command with invalid "
write ";  arguments."
write ";***********************************************************************"
write ";  Step 3.31.1: Send the command with an invalid file index. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_NTYPE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; Send the command
/$SC_$CPU_DS_SetFileType FileIndex=DS_DEST_FILE_CNT FileType=DS_BY_TIME
wait 5

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Destination File Type command failed as expected with an invalid file index."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Destination File Type command did not increment CMDEC as expected."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_NTYPE_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.31.2: Send the command with an invalid type. "
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_NTYPE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; Send the command
/$SC_$CPU_DS_SetFileType FileIndex=seqFileEntry FileType=3
wait 5

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Destination File Type command failed as expected with an invalid file index."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Destination File Type command did not increment CMDEC as expected."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_NTYPE_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.32: Send the Set Path command. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PATH_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command
/$SC_$CPU_DS_SetPath FileIndex=seqFileEntry PathName="/cf/"
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5012) - DS Set Path command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5012, "P"
else
  write "<!> Failed (1004;5012) - DS Set Path command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5012, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5012) - Expected Event Msg ",DS_PATH_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5012, "P"
else
  write "<!> Failed (1004;5012) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_PATH_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5012, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.33: Dump the Destination File Table to verify the change above"
write ";***********************************************************************"
;; Dump the Destination File Table
s get_tbl_to_cvt (ramDir,fileTblName,"A","$cpu_filetbl333",hostCPU,fileTblPktId)

;; Verify the table contains the change made
if (p@$SC_$CPU_DS_DF_TBL[seqFileEntry].Pathname = "/cf/") then
  write "<*> Passed (5012) - Table entry indicates the correct path."
  ut_setrequirements DS_5012, "P"
else
  write "<!> Failed (5012) - New path not reflected in the table entry."
  ut_setrequirements DS_5012, "F"
endif

write ";***********************************************************************"
write ";  Step 3.34: Using the TST_DS application, send a message that will"
write ";  create a file using the new naming."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG3_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'A5'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the destination file was created with the appropriate name and 
;; is marked "open"
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName1 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000) - A file was created with name '",fileName1,"'"
  ut_setrequirements DS_3000, "P"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif
else
  write "<!> Failed (3000;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_3001, "F"
endif

;; Download fileName2
if (fileName2 <> "") then
  write "==> Downloading '",fileName2,"'"
  slashLoc = %locate(fileName2,"/")
  while (slashLoc <> 0) do
    fileName2 = %substring(fileName2,slashLoc+1,%length(fileName2))
    slashLoc = %locate(fileName2,"/")
  enddo
  write "==> FileName Only = '",fileName2,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the call below
  s ftp_file(ramDir, fileName2, fileName2, hostCPU, "G")
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.35: Send the Set Path command with an invalid length."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PATH_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc0000046089C"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Set Path Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Set Path Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_PATH_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.36: Send the Set Path command with invalid arguments."
write ";***********************************************************************"
write ";  Step 3.36.1: Send the command with an invalid file index. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PATH_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; Send the command
/$SC_$CPU_DS_SetPath FileIndex=DS_DEST_FILE_CNT PathName="/ram/"
wait 5

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Path command failed as expected with an invalid file index."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Path command did not increment CMDEC as expected."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_PATH_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.36.2: Send the command with an empty path. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PATH_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; Send the command
/$SC_$CPU_DS_SetPath FileIndex=seqFileEntry PathName=""
wait 5

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Path command failed as expected with an invalid file index."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Path command did not increment CMDEC as expected."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_PATH_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.36.3: Send the command with a non-existent path. This will "
write ";  work until a message is rcv'd that attempts to create a file."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PATH_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1

;; Send the command
/$SC_$CPU_DS_SetPath FileIndex=seqFileEntry PathName="/ram/waltsDir"
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Path command with a non-existent path sent properly."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Path command did not increment CMDPC as expected."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_PATH_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.36.4: Using the TST_DS application, send a message that will"
write ";  cause a file creation error since the path does not exist. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CREATE_FILE_ERR_EID, "ERROR", 2

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=1 Pattern=x'BB'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG1_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Check for the event message
if ($SC_$CPU_find_event[2].num_found_messages = 1) then
  write "<*> Passed - Expected Event Msg ",DS_CREATE_FILE_ERR_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_CREATE_FILE_ERR_EID,"."
endif

;; Download fileName1
if (fileName1 <> "") then
  write "==> Downloading '",fileName1,"'"
  slashLoc = %locate(fileName1,"/")
  while (slashLoc <> 0) do
    fileName1 = %substring(fileName1,slashLoc+1,%length(fileName1))
    slashLoc = %locate(fileName1,"/")
  enddo
  write "==> FileName Only = '",fileName1,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the call below
  s ftp_file("CF:0", fileName1, fileName1, hostCPU, "G")
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.37: Send the Set Extension command. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_EXT_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; I am using the time file entry because the File Create error in the previous
;; step disabled the sequence file entry
;; Send the command
/$SC_$CPU_DS_SetExtension FileIndex=timeFileEntry Extension=".dat"
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5013) - DS Set Extension command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5013, "P"
else
  write "<!> Failed (1004;5013) - DS Set Extension command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5013, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5013) - Expected Event Msg ",DS_EXT_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5013, "P"
else
  write "<!> Failed (1004;5013) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_EXT_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5013, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.38: Dump the Destination File Table to verify the change above"
write ";***********************************************************************"
;; Dump the Destination File Table
s get_tbl_to_cvt (ramDir,fileTblName,"A","$cpu_filetbl338",hostCPU,fileTblPktId)

;; Verify the table contains the change made
if (p@$SC_$CPU_DS_DF_TBL[timeFileEntry].Extension = ".dat") then
  write "<*> Passed (5013) - Table entry indicates the correct extension."
  ut_setrequirements DS_5013, "P"
else
  write "<!> Failed (5013) - New extension not reflected in the table entry."
  ut_setrequirements DS_5013, "F"
endif

write ";***********************************************************************"
write ";  Step 3.39: Using the TST_DS application, send a message that will"
write ";  create a file using the new naming."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG3_SENT_INF_EID, "INFO", 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=timeMsgID MsgType=3 Pattern=x'AA'

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",TST_DS_MSG3_SENT_INF_EID,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the destination file was created with the appropriate name and 
;; is marked "open"
if ($SC_$CPU_DS_FileState[timeFileEntry].FileName <> "") then
  fileName1 = $SC_$CPU_DS_FileState[timeFileEntry].FileName
  write "<*> Passed (3000) - A file was created with name '",fileName1,"'"
  ut_setrequirements DS_3000, "P"

  if ($SC_$CPU_DS_FileState[timeFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif
else
  write "<!> Failed (3000;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_3001, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.40: Send the Set Extension command with an invalid length."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_EXT_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc000000E0A9C"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Set Extension Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Set Extension Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_EXT_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.41: Send the Set Extension command with invalid arguments."
write ";***********************************************************************"
write ";  Step 3.41.1: Send the command with an invalid file index. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_EXT_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; Send the command
/$SC_$CPU_DS_SetExtension FileIndex=DS_DEST_FILE_CNT Extension=".icu"
wait 5

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Extension command failed as expected with an invalid file index."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Extension command did not increment CMDEC as expected."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_EXT_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.41.2: Send the command with an empty extension. This command "
write ";  will be accepted."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_EXT_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1

;; Send the command
/$SC_$CPU_DS_SetExtension FileIndex=timeFileEntry Extension=""
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Extension command passed with a null extension."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Extension command did not increment CMDPC as expected."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_EXT_CMD_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.42: Send the Set Maximum Size command. "
write ";***********************************************************************"
;; Send the command to set the path back to normal
/$SC_$CPU_DS_SetPath FileIndex=seqFileEntry PathName="/ram/"
wait 5

;; Send the command set the file type back to sequence file naming
/$SC_$CPU_DS_SetFileType FileIndex=seqFileEntry FileType=DS_BY_COUNT
wait 5

;; Send the command to enable the sequence File entry
/$SC_$CPU_DS_EnableFile FileIndex=seqFileEntry
wait 5

;; Send the command to close the current file for this entry
/$SC_$CPU_DS_CloseFile FileIndex=seqFileEntry
wait 5

;; Setup for the Set Maximum Size command
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_SIZE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command
/$SC_$CPU_DS_SetMaxFileSize FileIndex=seqFileEntry MaxSize=5500
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5014) - DS Set Maximum Size command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5014, "P"
else
  write "<!> Failed (1004;5014) - DS Set Maximum Size command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5014, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5014) - Expected Event Msg ",DS_SIZE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5014, "P"
else
  write "<!> Failed (1004;5014) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_SIZE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5014, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.43: Dump the Destination File Table to verify the change above"
write ";***********************************************************************"
;; Dump the Destination File Table
s get_tbl_to_cvt (ramDir,fileTblName,"A","$cpu_filetbl343",hostCPU,fileTblPktId)

;; Verify the table contains the change made
if (p@$SC_$CPU_DS_DF_TBL[seqFileEntry].FileSize = 5500) then
  write "<*> Passed (5014) - Table entry indicates the correct maximum size."
  ut_setrequirements DS_5014, "P"
else
  write "<!> Failed (5014) - New maximum size not reflected in the table entry."
  ut_setrequirements DS_5014, "F"
endif

write ";***********************************************************************"
write ";  Step 3.44: Using the TST_DS application, send 6 messages that will"
write ";  create two files. The first 5 messages should be stored in the first"
write ";  file. The 6th message should be stored in the currently opened file."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG3_SENT_INF_EID, "INFO", 1

;; Send the commands
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'AA'
wait 5

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the destination file was created with the appropriate name and 
;; is marked "open"
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName2 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000) - A file was created with name '",fileName2,"'"
  ut_setrequirements DS_3000, "P"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif
else
  write "<!> Failed (3000;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_3001, "F"
endif

expFileWrites = $SC_$CPU_DS_FileWriteCnt + 1
expPassedPkts = $SC_$CPU_DS_PassedPktCnt + 1

/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'55'
wait 5

;; Check the counters 
if (expFileWrites = $SC_$CPU_DS_FileWriteCnt) AND ;;
   (expPassedPkts = $SC_$CPU_DS_PassedPktCnt) then
  write "<*> Passed - The file write and passed packet counters are correct."
else
  write "<!> Failed - The file write and passed packet counters are not as expected."
    write "=> File Write Cnt = ",$SC_$CPU_DS_FileWriteCnt,"; Expected ",expFileWrites
    write "=> Passed Pkt Cnt = ",$SC_$CPU_DS_PassedPktCnt,"; Expected ",expPassedPkts
endif

/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'FF'
wait 5

/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'A5'
wait 5

/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'BB'
wait 5

/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'5A'
wait 5

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the destination file was created with the appropriate name and 
;; is marked "open"
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName3 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000) - A file was created with name '",fileName3,"'"
  ut_setrequirements DS_3000, "P"

  if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif

  if (fileName2 <> fileName3) then
    write "<*> Passed (3001.1) - A new file was opened"
    ut_setrequirements DS_30011, "P"
  else
    write "<!> Failed (3001.1) - The message did not create a new file"
    ut_setrequirements DS_30011, "F"
  endif
else
  write "<!> Failed (3000;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_3001, "F"
  ut_setrequirements DS_30011, "F"
endif

;; Download fileName2
if (fileName2 <> "") then
  write "==> Downloading '",fileName2,"'"
  slashLoc = %locate(fileName2,"/")
  while (slashLoc <> 0) do
    fileName2 = %substring(fileName2,slashLoc+1,%length(fileName2))
    slashLoc = %locate(fileName2,"/")
  enddo
  write "==> FileName Only = '",fileName2,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the call below
  s ftp_file(ramDir, fileName2, fileName2, hostCPU, "G")
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.45: Send the Set Maximum Size command with an invalid length."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_SIZE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc000000A0B9C"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Set Maximum Size Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Set Maximum Size Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_SIZE_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.46: Send the Set Maximum Age command. "
write ";***********************************************************************"
;; Setup for the Set Maximum Age command
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_AGE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command
/$SC_$CPU_DS_SetMaxFileAge FileIndex=timeFileEntry MaxAge=90
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5015) - DS Set Maximum Age command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5015, "P"
else
  write "<!> Failed (1004;5015) - DS Set Maximum Age command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5015, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5015) - Expected Event Msg ",DS_AGE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5015, "P"
else
  write "<!> Failed (1004;5015) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_AGE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5015, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.47: Dump the Destination File Table to verify the change above"
write ";***********************************************************************"
;; Dump the Destination File Table
s get_tbl_to_cvt (ramDir,fileTblName,"A","$cpu_filetbl347",hostCPU,fileTblPktId)

;; Verify the table contains the change made
if (p@$SC_$CPU_DS_DF_TBL[timeFileEntry].FileAge = 90) then
  write "<*> Passed (5015) - Table entry indicates the correct maximum age."
  ut_setrequirements DS_5015, "P"
else
  write "<!> Failed (5015) - New maximum age not reflected in the table entry."
  ut_setrequirements DS_5015, "F"
endif

write ";***********************************************************************"
write ";  Step 3.48: Using the TST_DS application, send a message that will"
write ";  create a new file using the destination modified above. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG3_SENT_INF_EID, "INFO", 1

;; Send the commands
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=timeMsgID MsgType=3 Pattern=x'AA'
wait 5

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the destination file was created with the appropriate name and 
;; is marked "open"
if ($SC_$CPU_DS_FileState[timeFileEntry].FileName <> "") then
  fileName2 = $SC_$CPU_DS_FileState[timeFileEntry].FileName
  write "<*> Passed (3000) - A file was created with name '",fileName2,"'"
  ut_setrequirements DS_3000, "P"

  if ($SC_$CPU_DS_FileState[timeFileEntry].OpenState = DS_OPEN) then
    write "<*> Passed (3001) - The file is marked 'OPEN'"
    ut_setrequirements DS_3001, "P"
  endif
else
  write "<!> Failed (3000;3001) - The HK does not contain the expected file state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_3001, "F"
endif

;; Download fileName1
if (fileName1 <> "") then
  write "==> Downloading '",fileName1,"'"
  slashLoc = %locate(fileName1,"/")
  while (slashLoc <> 0) do
    fileName1 = %substring(fileName1,slashLoc+1,%length(fileName1))
    slashLoc = %locate(fileName1,"/")
  enddo
  write "==> FileName Only = '",fileName1,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the call below
  s ftp_file(ramDir, fileName1, fileName1, hostCPU, "G")
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.49: Wait for the file created above to exceed its maximum age."
write ";***********************************************************************"
;; Loop here for each HK packet and see the Age updating
;; This verifies DS3002
fileAge = $SC_$CPU_DS_DF_TBL[timeFileEntry].FileAge
reportedAge = $SC_$CPU_DS_FileState[timeFileEntry].FileAge
waitTime = fileAge - reportedAge
originalAge = reportedAge
fileName1 = $SC_$CPU_DS_FileState[timeFileEntry].FileName

while (waitTime > 0) do
  ;; Sent the Get File Info command to check the File Status
  cmdCtr = $SC_$CPU_DS_CMDPC + 1

  /$SC_$CPU_DS_GetFileInfo

  ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

  reportedAge = $SC_$CPU_DS_FileState[timeFileEntry].FileAge
  write "new file age = ",reportedAge

  ;; If the age is updating, then DS_3002 and DS_3002.1 are verified
  if (originalAge <> reportedAge) then
    ut_setrequirements DS_3002, "P"
    ut_setrequirements DS_30021, "P"
  endif

  waitTime = fileAge - reportedAge
  write "timeout calculation = ",waitTime

  ;; Check the File Status to see if the file was closed
  ;; NOTE: If the file was closed, the name will be empty and age will be 0
  if ($SC_$CPU_DS_FileState[timeFileEntry].FileName = "") AND ;;
     ($SC_$CPU_DS_FileState[timeFileEntry].FileAge = 0) then
    waitTime = 0
    write "HK reports the file is no longer open"
  endif
enddo

;; Download the file in order to perform post-test analysis
if (fileName2 <> "") then
  write "==> Downloading '",fileName2,"'"
  slashLoc = %locate(fileName2,"/")
  while (slashLoc <> 0) do
    fileName2 = %substring(fileName2,slashLoc+1,%length(fileName2))
    slashLoc = %locate(fileName2,"/")
  enddo
  write "==> FileName Only = '",fileName2,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the call below
  s ftp_file(ramDir, fileName2, fileName2, hostCPU, "G")
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.50: Send the Set Maximum Age command with an invalid length."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_AGE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc000000A0C9C"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Set Maximum Age Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Set Maximum Age Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_AGE_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.51: Send the Disable command for the DS Application. "
write ";***********************************************************************"
;; Setup for event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_ENADIS_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command
/$SC_$CPU_DS_Disable
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5000) - DS Disable command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5000, "P"
else
  write "<!> Failed (1004;5000) - DS Disable command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5000, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5000) - Expected Event Msg ",DS_ENADIS_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5000, "P"
else
  write "<!> Failed (1004;5000) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_ENADIS_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5000, "F"
endif

;; Verify that the state in HK indicates 'DISABLED'
if (p@$SC_$CPU_DS_AppEnaState = "Disabled") then
  write "<*> Passed (5000) - Housekeeping indicates DS is disabled."
  ut_setrequirements DS_5000, "P"
else
  write "<!> Failed (5000) - DS State indicates Enabled when Disabled is expected."
  ut_setrequirements DS_5000, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.52: Using the TST_DS application, send several messages to the"
write ";  DS application. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG3_SENT_INF_EID, "INFO", 1
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 2
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG2_SENT_INF_EID, "INFO", 3

pktCtr = $SC_$CPU_DS_DisabledPktCnt + 3
fileSize = $SC_$CPU_DS_FileState[seqFileEntry].FileSize

;; Send the command 3 times
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'AA'
wait 5
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=1 Pattern=x'55'
wait 5
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=2 Pattern=x'BB'
wait 5

;; Verify the disabled packet counter incremented and 
;; the file status did not change
if ($SC_$CPU_DS_DisabledPktCnt = pktCtr) AND ;;
   ($SC_$CPU_DS_FileState[seqFileEntry].FileSize = fileSize) then
  write "<*> Passed (8000) - The file stats remained the same and the Disabled Pkt Counter incremented as expected."
  ut_setrequirements DS_8000, "P"
else
  write "<!> Failed (8000) - The HK does not contain the expected information."
  write "Expected file size of ",fileSize,"; Got ",SCX_CPU3_DS_FileState[seqFileEntry].FileSize
  write "Expected Disabled Ctr ",pktCtr,"; Got ",SCX_CPU3_DS_DisabledPktCnt
  ut_setrequirements DS_8000, "F"
endif

;; Verify that the events were generated
if ($SC_$CPU_find_event[1].num_found_messages = 1) AND ;;
   ($SC_$CPU_find_event[2].num_found_messages = 1) AND ;;
   ($SC_$CPU_find_event[3].num_found_messages = 1) then
  write "<*> Passed - Expected Event Msgs were generated."
else
  write "<!> Failed - Expected Event messages were not rcv'd from TST_DS."
endif

write ";***********************************************************************"
write ";  Step 3.53: Send the Disable command with an invalid length."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_ENADIS_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc0000004029C"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Disable Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Disable Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_ENADIS_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.54: Send the Enable command for the DS Application. "
write ";***********************************************************************"
;; Setup for event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_ENADIS_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command
/$SC_$CPU_DS_Enable
wait 5

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5001) - DS Enable command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5001, "P"
else
  write "<!> Failed (1004;5001) - DS Enable command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5001, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5001) - Expected Event Msg ",DS_ENADIS_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5001, "P"
else
  write "<!> Failed (1004;5001) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_ENADIS_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5001, "F"
endif

;; Verify that the state in HK indicates 'ENABLED'
if (p@$SC_$CPU_DS_AppEnaState = "Enabled") then
  write "<*> Passed (5001) - Housekeeping indicates DS is enabled."
  ut_setrequirements DS_5001, "P"
else
  write "<!> Failed (5001) - DS State indicates Disabled when Enabled is expected."
  ut_setrequirements DS_5001, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.55: Using the TST_DS application, send several messages to the"
write ";  DS application. These message should be written to the open file."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG3_SENT_INF_EID, "INFO", 1
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 2
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG2_SENT_INF_EID, "INFO", 3

pktCtr = $SC_$CPU_DS_PassedPktCnt + 3
fileSize = $SC_$CPU_DS_FileState[seqFileEntry].FileSize

;; Send the command 3 times
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'AA'
wait 5
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=1 Pattern=x'55'
wait 5
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=2 Pattern=x'BB'
wait 5

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the disabled packet counter incremented and 
;; the file status did not change
if ($SC_$CPU_DS_PassedPktCnt = pktCtr) AND ;;
   ($SC_$CPU_DS_FileState[seqFileEntry].FileSize > fileSize) then
  write "<*> Passed (8000) - The file stats were updated and the Passed Pkt Counter incremented as expected."
  ut_setrequirements DS_8000, "P"
else
  write "<!> Failed (8000) - The HK does not contain the expected information."
  ut_setrequirements DS_8000, "F"
endif

;; Verify that the events were generated
if ($SC_$CPU_find_event[1].num_found_messages = 1) AND ;;
   ($SC_$CPU_find_event[2].num_found_messages = 1) AND ;;
   ($SC_$CPU_find_event[3].num_found_messages = 1) then
  write "<*> Passed - Expected Event Msgs were generated."
else
  write "<!> Failed - Expected Event messages were not rcv'd from TST_DS."
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.56: Send the Enable command with an invalid length."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_ENADIS_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc0000002029C"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Disable Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Disable Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_ENADIS_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.57: Send the Enable/Disable command with an invalid state."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_ENADIS_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc0000003029E0002"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Enable/Disable Command Rejected Counter incremented."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Enable/Disable Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_ENADIS_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.58: Send the Close File command for the destination file that "
write ";  is still open and download it for post-test analysis. "
write ";***********************************************************************"
;; Send the command to close the current file
/$SC_$CPU_DS_CloseFile FileIndex=seqFileEntry
wait 5

;; Download the file in order to perform post-test analysis
write "==> Downloading '",fileName3,"'"
slashLoc = %locate(fileName3,"/")
while (slashLoc <> 0) do
  fileName3 = %substring(fileName3,slashLoc+1,%length(fileName3))
  slashLoc = %locate(fileName3,"/")
enddo
write "==> FileName Only = '",fileName3,"'"

;; Need to parse the filename out of the variable above
;; before using it in the call below
s ftp_file(ramDir, fileName3, fileName3, hostCPU, "G")

wait 5

write ";***********************************************************************"
write ";  Step 3.59: Using the TST_DS application, send two messages to the"
write ";  DS application to open multiple files."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG3_SENT_INF_EID, "INFO", 1

;; Send the command for each message ID
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'AA'
wait 5
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=timeMsgID MsgType=3 Pattern=x'55'
wait 5

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify that the events were generated
if ($SC_$CPU_find_event[1].num_found_messages = 2) then
  write "<*> Passed - Expected Event Msgs were generated."
else
  write "<!> Failed - Expected Event messages were not rcv'd from TST_DS."
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.60: Send the Close All Files command "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_ALL_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1

;; Send the command to close all file
/$SC_$CPU_DS_CloseAllFiles

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5002) - DS Close All Files command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5002, "P"
else
  write "<!> Failed (1004;5002) - DS Close All Files command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5002, "F"
endif

;; Verify that the events were generated
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004) - Expected Event Msgs were generated."
  ut_setrequirements DS_1004, "P"
else
  write "<!> Failed (1004) - Expected Event messages were not rcv'd from TST_DS."
  ut_setrequirements DS_1004, "F"
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the files have been closed
if ($SC_$CPU_DS_FileState[seqFileEntry].OpenState = DS_CLOSED) then
  write "<*> Passed - Sequence file has been closed"
else
  write "<!> Failed - Sequence file State is not Closed"
endif

if ($SC_$CPU_DS_FileState[timeFileEntry].OpenState = DS_CLOSED) then
  write "<*> Passed - Time file has been closed"
else
  write "<!> Failed - Time file State is not Closed"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.61: Send the Close All Files command with an invalid length."
write ";***********************************************************************"
;; setup event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_ALL_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc0000002119C"

ut_sendrawcmd "$SC_$CPU_DS", (rawcmd)

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1002;1005) - DS Disable Command Rejected Counter incremented."
  ut_setrequirements DS_1002, "P"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1002;1005) - DS Disable Command Rejected Counter did not increment as expected."
  ut_setrequirements DS_1002, "F"
  ut_setrequirements DS_1005, "F"
endif

if ($SC_$CPU_find_event[1].num_found_messages = 1) THEN
  write "<*> Passed (1005) - Event message ",$SC_$CPU_find_event[1].eventid, " received"
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_CLOSE_ALL_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 4.0: Clean-up - Send the Processor Reset command in order to    "
write ";  be able to retrieve files in RAM after this procedure completes. "
write ";***********************************************************************"
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
write ";  End procedure $SC_$CPU_ds_filewrite"
write ";*********************************************************************"
ENDPROC
