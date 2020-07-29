PROC $sc_$cpu_ds_movefile
;*******************************************************************************
;  Test Name:  ds_movefile
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	This test verifies that the CFS Data Storage (DS) application writes
;	messages to files according to the requirements and moves these files to
;	the table-specified directory after they are closed.
;
;	This test should only be executed if the DS_MOVE_FILES confuguration
;	parameter is set to TRUE. Also, for this test to work properly, this
;	configuration parameter must be set to 1 in order to generate the
;	necessary File Table rdl since STOL expects this to be 1.
;
;  Requirements Tested
;   cDS1004	If DS accepts any command as valid, DS shall execute the
;		command, increment the DS Valid Command Counter and issue an
;		event message.
;   cDS3000	DS shall construct filenames based on the following definitions
;		provided in the Destination File table:
;		path + base + [time/sequence characters] + extension
;   cDS3000.1	If Filename Type indicates naming based on Sequence, the
;		constructed filename will include a character representation of
;		the Sequence Count value from the Destination File table.
;   cDS3000.1.1	If Filename Type indicates naming based on Sequence, the value
;		of Sequence Count shall be incremented each time a file is
;		created.
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
;   cDS5002	Upon receipt of a Close File command, DS shall close the
;		command-specified file.
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
;	Date		   Name		Description
;	01/03/11	Walt Moleski	Original Procedure.
;	01/23/15	Walt Moleski	Updated the comments to detail what
;					must be done prior to executing this
;					test procedure.
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
#include "fm_events.h"

%liv (log_procedure) = logging

#define DS_1004		0
#define DS_3000		1
#define DS_30001	2
#define DS_300011	3
#define DS_3001		4
#define DS_30011	5
#define DS_30012	6
#define DS_3002 	7
#define DS_30021 	8
#define DS_5002		9
#define DS_8000		10
#define DS_9000		11

global ut_req_array_size = 11
global ut_requirement[0 .. ut_req_array_size]

for i = 0 to ut_req_array_size DO
  ut_requirement[i] = "U"
enddo

;**********************************************************************
; Set the local values
;**********************************************************************
local cfe_requirements[0 .. ut_req_array_size] = ["DS_1004", "DS_3000", "DS_3000.1", "DS_3000.1.1", "DS_3001", "DS_3001.1", "DS_3001.2", "DS_3002", "DS_3002.1", "DS_5002", "DS_8000", "DS_9000" ]

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
write ";  Step 1.1: Command a Power-on Reset on $CPU."
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
s $sc_$cpu_ds_tbl4

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
s ftp_file("CF:0/apps", "ds_movefile.tbl", destFileName, hostCPU, "P")
s ftp_file("CF:0/apps", "ds_movefilter.tbl", filterFileName, hostCPU, "P")

wait 5

write ";***********************************************************************"
write ";  Step 1.3: Display the Housekeeping pages "
write ";***********************************************************************"
page $SC_$CPU_DS_HK
page $SC_$CPU_TST_DS_HK
page $SC_$CPU_DS_FILTER_TBL
page $SC_$CPU_DS_FILE_TBL
page $SC_$CPU_FM_DIR_LIST

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

write ";*********************************************************************"
write ";  Step 1.5: Start the File Manager (FM) Application "
write ";********************************************************************"
ut_setupevents "$SC", "$CPU", "CFE_ES", CFE_ES_START_INF_EID, "INFO", 1
ut_setupevents "$SC", "$CPU", "FM", FM_STARTUP_EID, "INFO", 2

s load_start_app ("FM",hostCPU,"FM_AppMain")

; Wait for app startup events
ut_tlmwait  $SC_$CPU_find_event[2].num_found_messages, 1
IF (UT_TW_Status = UT_Success) THEN
  write "<*> Passed - FM Application Started"
else
  write "<!> Failed - FM Application start Event Message not received."
endif

local fmPkt = x'088C'

/$SC_$CPU_TO_ADDPACKET STREAM=fmPkt PKT_SIZE=X'0' PRIORITY=X'0' RELIABILITY=X'0' BUFLIMIT=x'4'

wait 5

write ";***********************************************************************"
write ";  Step 1.6: Enable DEBUG Event Messages "
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
write ";  Step 2.1.3: Create the directory where files will be moved when they "
write ";  are closed by the DS application. "
write ";***********************************************************************"
local moveDirName = $SC_$CPU_DS_DF_TBL[seqFileEntry].MoveDirName
local pathName = $SC_$CPU_DS_DF_TBL[seqFileEntry].PathName

write "==> Creating File Move Directory =  ", moveDirName

/$SC_$CPU_FM_DirCreate DirName=moveDirName
wait 5

write ";***********************************************************************"
write ";  Step 2.1.4: Send the TST_DS command to send a message to DS using the"
write ";  MessageID found above. Since the message being set will not exceed "
write ";  the file size constraint, the FileState HK should contain info about"
write ";  the file that was created. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

;; 4 writes per file
local expFileWrites = $SC_$CPU_DS_FileWriteCnt + 16
local expPassedPkts = $SC_$CPU_DS_PassedPktCnt + 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=1 Pattern=x'55'

;; Check for the event message
ut_tlmwait  $SC_$CPU_find_event[1].num_found_messages, 1
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

  slashLoc = %locate(fileName1,"/")
  ;; loop until all slashes are found for the Filter Table Name
  while (slashLoc <> 0) do
    fileName1 = %substring(fileName1,slashLoc+1,%length(fileName1))
    slashLoc = %locate(fileName1,"/")
  enddo

  write "==> FileName1 filename only = '",fileName1,"'"

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

local expHdrUpdates = $SC_$CPU_DS_FileUpdCnt + 4

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=3 Pattern=x'AA'

;; Check for the event message
ut_tlmwait  $SC_$CPU_find_event[1].num_found_messages, 1
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
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName2 = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (3000;3000.1;3000.1.1) - A file was created with name '",fileName2,"'"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30001, "A"
  ut_setrequirements DS_300011, "A"

  slashLoc = %locate(fileName2,"/")
  ;; loop until all slashes are found for the File Name
  while (slashLoc <> 0) do
    fileName2 = %substring(fileName2,slashLoc+1,%length(fileName2))
    slashLoc = %locate(fileName2,"/")
  enddo

  write "==> FileName2 filename only = '",fileName2,"'"

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
    write "<!> Failed - The header update counter is not what was expected."
    write "=> Header Update Cnt = ",$SC_$CPU_DS_FileUpdCnt,"; Expected ",expHdrUpdates
  endif

  ;; Verify that this file was moved to the "MoveDirName" specified in table
  ;; Issue the FM_DirListTlm command for the original directory
  /$SC_$CPU_FM_DirListTlm DirName=pathName Offset=0 GetStatFlag=0
  wait 10

  ;; Parse the Directory listing packet for fileName1
  for i = 1 to $SC_$CPU_FM_PktFiles do
    if (fileName1 = $SC_$CPU_FM_DirList[i].name) then
      write "<!> Failed - File '",fileName1,"' is still contained in directory '",pathName,"' when it should have been moved."
      break;
    endif
  enddo

  ;; Issue the FM_DirListTlm command for "MoveDirName"
  /$SC_$CPU_FM_DirListTlm DirName=moveDirName Offset=0 GetStatFlag=0
  wait 10

  ;; Need to parse the directory to find fileName1
  for i = 1 to $SC_$CPU_FM_PktFiles do
    if (fileName1 = $SC_$CPU_FM_DirList[i].name) then
      write "<*> Passed - File '",fileName1,"' is contained in directory '",moveDirName,"' as expected."
    endif
  enddo
else
  write "<!> Failed (3000;3000.1;3000.1.1;3001) - The File Info packet does not contain the expected state information."
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

ut_tlmwait  $SC_$CPU_DS_CMDPC, {cmdCtr}
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
ut_tlmwait  $SC_$CPU_find_event[1].num_found_messages, 1
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
ut_tlmwait  $SC_$CPU_find_event[1].num_found_messages, 1
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

  slashLoc = %locate(fileName3,"/")
  ;; loop until all slashes are found for the File Name
  while (slashLoc <> 0) do
    fileName3 = %substring(fileName3,slashLoc+1,%length(fileName3))
    slashLoc = %locate(fileName3,"/")
  enddo

  write "==> FileName3 filename only = '",fileName3,"'"

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

  ;; Verify that this file was moved to the "MoveDirName" specified in table
  ;; Issue the FM_DirListTlm command for the original directory
  /$SC_$CPU_FM_DirListTlm DirName=pathName Offset=0 GetStatFlag=0
  wait 10

  ;; Parse the Directory listing packet for fileName1
  for i = 1 to $SC_$CPU_FM_PktFiles do
    if (fileName2 = $SC_$CPU_FM_DirList[i].name) then
      write "<!> Failed - File '",fileName2,"' is still contained in directory '",pathName,"' when it should have been moved."
      break;
    endif
  enddo

  ;; Issue the FM_DirListTlm command for "MoveDirName"
  /$SC_$CPU_FM_DirListTlm DirName=moveDirName Offset=0 GetStatFlag=0
  wait 10

  ;; Need to parse the directory to find fileName1
  for i = 1 to $SC_$CPU_FM_PktFiles do
    if (fileName2 = $SC_$CPU_FM_DirList[i].name) then
      write "<*> Passed - File '",fileName2,"' is contained in directory '",moveDirName,"' as expected."
    endif
  enddo
else
  write "<!> Failed (3000;3000.1;3000.1.1;3001) - The File Info packet does not contain the expected state information."
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30001, "F"
  ut_setrequirements DS_300011, "F"
  ut_setrequirements DS_3001, "F"
endif

write ";***********************************************************************"
write ";  Step 2.4: Wait for the file created above to exceed its maximum age."
write ";***********************************************************************"
;; Loop here for each HK packet and see the Age updating
;; This verifies DS3002
local fileAge = $SC_$CPU_DS_DF_TBL[seqFileEntry].FileAge
local reportedAge = $SC_$CPU_DS_FileState[seqFileEntry].FileAge
local waitTime = fileAge - reportedAge
local originalAge = reportedAge

fileName1 = $SC_$CPU_DS_FileState[seqFileEntry].FileName

while (waitTime > 0) do
  ;; Send the Get File Info command to check the File Status
  cmdCtr = $SC_$CPU_DS_CMDPC + 1

  /$SC_$CPU_DS_GetFileInfo

  ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

  reportedAge = $SC_$CPU_DS_FileState[seqFileEntry].FileAge
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
  if ($SC_$CPU_DS_FileState[seqFileEntry].FileName = "") AND ;;
     ($SC_$CPU_DS_FileState[seqFileEntry].FileAge = 0) then
    waitTime = 0
    write "TLM reports the file is no longer open"
  endif
enddo

wait 5

;; Verify that this file was moved to the "MoveDirName" specified in table
;; Issue the FM_DirListTlm command for the original directory
/$SC_$CPU_FM_DirListTlm DirName=pathName Offset=0 GetStatFlag=0
wait 10

;; Parse the Directory listing packet for fileName1
for i = 1 to $SC_$CPU_FM_PktFiles do
  if (fileName3 = $SC_$CPU_FM_DirList[i].name) then
    write "<!> Failed - File '",fileName3,"' is still contained in directory '",pathName,"' when it should have been moved."
    break;
  endif
enddo

;; Issue the FM_DirListTlm command for "MoveDirName"
/$SC_$CPU_FM_DirListTlm DirName=moveDirName Offset=0 GetStatFlag=0
wait 10

;; Need to parse the directory to find fileName1
for i = 1 to $SC_$CPU_FM_PktFiles do
  if (fileName3 = $SC_$CPU_FM_DirList[i].name) then
    write "<*> Passed - File '",fileName3,"' is contained in directory '",moveDirName,"' as expected."
  endif
enddo

wait 5

step3_0:
write ";***********************************************************************"
write ";  Step 3.0: Clean-up - Send the Processor Reset command in order to    "
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
write ";  End procedure $SC_$CPU_ds_movefile"
write ";*********************************************************************"
ENDPROC
