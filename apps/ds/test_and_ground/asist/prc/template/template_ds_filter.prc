PROC $sc_$cpu_ds_filter
;*******************************************************************************
;  Test Name:  ds_filter
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	This test verifies that the CFS Data Storage (DS) application filters
;	messages according to the requirements. This test also verifies that
;	the DS application handles anomalies appropriately.
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
;   cDS2000	Upon receipt of a message, if the Packet Processing State is
;		ENABLED, DS shall determine if the message shall be stored based
;		on the contents of the Packet Filter table and the Destination
;		File table.
;   cDS2000.1	The Packet Filter Table contents shall include:
;			a. Message ID
;			b. Destination file table index
;			c. Filter type (time/sequence count)
;			d. Algoithm N value
;			e. Algoithm X value
;			f. Algoithm O value
;   cDS2000.2	The Destination File Table contents shall include:
;			a. Pathname
;			b. Basename
;			c. Extension
;			d. Filename type (time/sequence)
;			e. Default Destination enable state
;			f. Max file size (in bytes)
;			g. Max file age (in seconds)
;			h. Initial Sequence Count
;   cDS2001	DS shall store "N of X" messages starting at offset O.
;   cDS2001.1	If either the X or the N value is zero, then the message shall
;		not be stored.
;   cDS2002	DS shall store each message in up to <PLATFORM_DEFINED>
;		different destination files.
;   cDS2002.1	Destination files shall be created when the first message is
;		received that is passed by the filter algorithm for that
;		particular message and selects that particular destination file.
;   cDS2003	The input argument to the filter algorithm shall be determined
;		by the Filter Type indicator.
;   cDS2003.1	If Filter Type indicates Sequence based filtering, then the
;		algorithm input argument is a value equal to the packet
;		sequence count.
;   cDS2003.2	If Filter Type indicated Time based filtering, then the
;		algorithm input argument is a value created from the lower 11
;		bits of packet timestamp seconds plus the high 4 bits of
;		timestamp subseconds.
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
;   cDS3003     Each DS destination file shall contain a primary cFE file header
;               with the following information:
;                       a. Content Type
;                       b. Sub Type
;                       c. Primary header length
;                       d. Spacecraft ID
;                       e. Processor ID
;                       f. Application ID
;                       g. File creation time (seconds)
;                       h. File creation time (sub-seconds)
;   cDS3004     Each DS destination file shall contain a secondary file header
;               with the following information:
;                       a. File close time (seconds)
;                       b. File close time (sub-seconds)
;                       c. File table index
;                       d. Qualified filename
;   cDS5002	Upon receipt of a Close File command, DS shall close the
;		command-specified files.
;   cDS5008	Upon receipt of a Set File Index for a Packet Filter Table entry
;		command, DS shall update the destination file index.
;   cDS5009	Upon receipt of a Set Filter Type for a Packet Filter Table
;		entry command, DS shall update the filter type to the command-
;		specified filter type.
;   cDS5010	Upon receipt of a Set Filter Parameter for a Packet Filter Table
;		entry command, DS shall update the N, X, O Parameters to the
;		command-specified values.
;   cDS5016	Upon receipt of an Add Message ID to Packet Filter Table
;		command, DS shall add the command-specified Message ID to the
;		next unused entry in the Packet Filter Table
;   cDS5016.1	If the command-specified MessageID is invalid, DS shall reject
;		the command and send an event message.
;   cDS5016.2	If the Packet Filter Table is not loaded, DS will reject the
;		command and send an event message.
;   cDS5016.3	If the command-specified MessageID is already specified in the
;		Packet Filter Table, DS will reject the command and send an
;		event message.
;   cDS5016.4	If there are no unused entries in the Packet Filter Table, DS 
;		will reject the command and send an event message.
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
;	11/16/09	Walt Moleski	Original Procedure.
;       12/08/10        Walt Moleski    Modified the procedure to use variables
;                                       for the application name and ram disk.
;       09/14/11        Walt Moleski    Updated for DS 2.2.0.0
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
#define DS_2000		3
#define DS_20001	4
#define DS_20002	5
#define DS_2001		6
#define DS_20011	7
#define DS_2002		8
#define DS_20021	9
#define DS_2003 	10
#define DS_20031	11
#define DS_20032	12
#define DS_3000 	13
#define DS_30001 	14
#define DS_300011	15
#define DS_30002	16
#define DS_300021	17
#define DS_3003		18
#define DS_3004		19
#define DS_5002		20
#define DS_5008		21
#define DS_5009		22
#define DS_5010		23
#define DS_5016		24
#define DS_50161	25
#define DS_50162	26
#define DS_50163	27
#define DS_50164	28
#define DS_8000		29
#define DS_9000		30

global ut_req_array_size = 30
global ut_requirement[0 .. ut_req_array_size]

for i = 0 to ut_req_array_size DO
  ut_requirement[i] = "U"
enddo

;**********************************************************************
; Set the local values
;**********************************************************************
local cfe_requirements[0 .. ut_req_array_size] = ["DS_1002", "DS_1004", "DS_1005", "DS_2000", "DS_2000.1", "DS_2000.2", "DS_2001", "DS_2001.1", "DS_2002", "DS_2002.1", "DS_2003", "DS_2003.1", "DS_2003.2", "DS_3000", "DS_3000.1", "DS_3000.1.1", "DS_3000.2", "DS_3000.2.1", "DS_3003", "DS_3004", "DS_5002", "DS_5008", "DS_5009", "DS_5010", "DS_5016", "DS_5016.1", "DS_5016.2", "DS_5016.3", "DS_5016.4", "DS_8000", "DS_9000" ]

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
local addMID = 0x0906
local fullMID = 0xA01

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
s $sc_$cpu_ds_tbl3

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
s ftp_file("CF:0/apps", "ds_filtfile.tbl", destFileName, hostCPU, "P")
s ftp_file("CF:0/apps", "ds_filtfilter.tbl", filterFileName, hostCPU, "P")

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
write ";  Step 1.6: Dump the Packet Filter Table. "
write ";***********************************************************************"
;; Dump the Packet Filter Table
s get_tbl_to_cvt (ramDir,filterTblName,"A","$cpu_filtertbl16",hostCPU,filterTblPktId)

local entryCount = 0
;; Verify the table contents loaded in Step 1.2
for i = 0 to DS_PACKETS_IN_FILTER_TABLE-1 do
  if ($SC_$CPU_DS_PF_TBL[i].MessageID <> DS_UNUSED) then
    entryCount = entryCount + 1
    write "==> MessageID = ",%hex($SC_$CPU_DS_PF_TBL[i].MessageID,4)
    for j = 0 to DS_FILTERS_PER_PACKET-1 do
      if ($SC_$CPU_DS_PF_TBL[i].FilterParams[j].FilterType <> DS_UNUSED) then
	write "==> File Index = ",$SC_$CPU_DS_PF_TBL[i].FilterParams[j].Index
	write "==> Filter Type = ",$SC_$CPU_DS_PF_TBL[i].FilterParams[j].FilterType
	write "==> N Value = ",$SC_$CPU_DS_PF_TBL[i].FilterParams[j].N_Value
	write "==> X Value = ",$SC_$CPU_DS_PF_TBL[i].FilterParams[j].X_Value
	write "==> O Value = ",$SC_$CPU_DS_PF_TBL[i].FilterParams[j].O_Value
      endif
    enddo
  else
    i = DS_PACKETS_IN_FILTER_TABLE 
  endif
enddo

if (entryCount > 0) then
  write "<*> Passed (2000.1) - The Packet Filter Table contains ",entryCount," valid entries."
  ut_setrequirements DS_20001, "P"
else
  write "<!> Failed (2000.1) - The Packet Filter Table is empty."
  ut_setrequirements DS_20001, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 1.7: Dump the Destination File Table. "
write ";***********************************************************************"
;; Dump the Destination File Table
s get_tbl_to_cvt (ramDir,fileTblName,"A","$cpu_filetbl17",hostCPU,fileTblPktId)

entryCount = 0
;; Verify the table contents loaded in Step 1.2
for i = 0 to DS_DEST_FILE_CNT-1 do
  if ($SC_$CPU_DS_DF_TBL[i].FileNameType <> DS_UNUSED) then
    entryCount = entryCount + 1
    write "==> Pathname = '",$SC_$CPU_DS_DF_TBL[i].Pathname,"'"
    write "==> Basename = '",$SC_$CPU_DS_DF_TBL[i].Basename,"'"
    write "==> Extension = '",$SC_$CPU_DS_DF_TBL[i].Extension,"'"
    write "==> Name Type = ",p@$SC_$CPU_DS_DF_TBL[i].FileNameType
    write "==> State     = ",p@$SC_$CPU_DS_DF_TBL[i].FileState
    write "==> Max Size = ",$SC_$CPU_DS_DF_TBL[i].FileSize
    write "==> Max Age = ",$SC_$CPU_DS_DF_TBL[i].FileAge
    write "==> Seq Count = ",$SC_$CPU_DS_DF_TBL[i].SeqCnt
  else
    i = DS_DEST_FILE_CNT 
  endif
enddo

if (entryCount > 0) then
  write "<*> Passed (2000.2) - The Destination File Table contains ",entryCount," valid entries."
  ut_setrequirements DS_20002, "P"
else
  write "<!> Failed (2000.2) - The Destination File Table is empty."
  ut_setrequirements DS_20002, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.0: Message Filtering Test."
write ";***********************************************************************"
write ";  Step 2.1: Utilizing the TST_DS application, send valid messages to "
write ";  the DS application using an entry that specifies Sequence filtering "
write ";  and the maximum number of Destination File entries. "
write ";***********************************************************************"
write ";  Step 2.1.1: Parse the tables to find an entry to use."
write ";***********************************************************************"
;; Turn off logging for this step
logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

;; Parse the Packet Filter Table to find a MessageID
;; using the maximum Destination Files and Sequence filtering
local filterEntry = 0
entryCount = 0
local foundEntry = FALSE
local seqFileIDs[0 .. DS_FILTERS_PER_PACKET-1]
for i = 0 to DS_PACKETS_IN_FILTER_TABLE-1 do
  if ($SC_$CPU_DS_PF_TBL[i].MessageID <> 0 AND foundEntry = FALSE) then
    ;; Check for max files being used
    entryCount = 0
    for j = 0 to DS_FILTERS_PER_PACKET-1 do
      if ($SC_$CPU_DS_PF_TBL[i].FilterParams[j].FilterType = DS_BY_COUNT) then
	entryCount = entryCount + 1
        seqFileIDs[j] = $SC_$CPU_DS_PF_TBL[i].FilterParams[j].Index
      endif
    enddo
    if (entryCount = DS_FILTERS_PER_PACKET) then
      filterEntry = i
      foundEntry = TRUE
    endif
  endif
enddo

%liv (log_procedure) = logging

write "==> Found filter entry at ", filterEntry

local seqMsgID = $SC_$CPU_DS_PF_TBL[filterEntry].MessageID

wait 5

write ";***********************************************************************"
write ";  Step 2.1.2: Send the TST_DS command to send a message to DS using the"
write ";  MessageID found above. Send a message that will not exceed the file "
write ";  size constraint. The HK should contain info about the files opened. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

local expPassedPkts = $SC_$CPU_DS_PassedPktCnt + 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqMsgID MsgType=1 Pattern=x'5A'

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

;; Verify that the Passed Packet counter incremented properly
if (expPassedPkts = $SC_$CPU_DS_PassedPktCnt) then
  write "<*> Passed (2000) - Passed packet counter incremented properly."
  ut_setrequirements DS_2000, "P"
else
  write "<!> Failed (2000) - Passed packet counter not set properly. Expected '",expPassedPkts,"'. Count = '",$SC_$CPU_DS_PassedPktCnt,"'."
  ut_setrequirements DS_2000, "F"
endif

;; Loop for the Maximum number of Destinations to determine if
;; the correct number of files were created
local hkSlot = 0
local fileCreateCnt = 0
local fileName = ""
for i = 0 to DS_FILTERS_PER_PACKET-1 do
  hkSlot = seqFileIDs[i]
  if ($SC_$CPU_DS_FileState[hkSlot].FileName <> "") then
    fileName = $SC_$CPU_DS_FileState[hkSlot].FileName
    write "<*> Passed - A file was created with name '",fileName,"' for entry #",i
    fileCreateCnt = fileCreateCnt + 1
  endif
enddo

;; Verify that the Max destination files were created and opened
if (fileCreateCnt = DS_FILTERS_PER_PACKET) then
  write "<*> Passed (2002;2002.1;3000;3000.1;3000.1.1) - The maximum number of files for a single message were created."
  ut_setrequirements DS_2002, "P"
  ut_setrequirements DS_20021, "P"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30001, "A"
  ut_setrequirements DS_300011, "A"
else
  write "<!> Failed (2002;2002.1;3000;3000.1;3000.1.1) - The maximum number of files for a single message did not get created."
  ut_setrequirements DS_2002, "F"
  ut_setrequirements DS_20021, "F"
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30001, "F"
  ut_setrequirements DS_300011, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.2: Utilizing the TST_DS application, send valid messages to "
write ";  the DS application using an entry that specifies Time filtering and "
write ";  the maximum number of Destination File entries. "
write ";***********************************************************************"
write ";  Step 2.2.1: Parse the tables to find an entry to use."
write ";***********************************************************************"
;; Turn off logging for this step
logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

;; Parse the Packet Filter Table to find a MessageID
;; using the maximum Destination Files and Time filtering
filterEntry = 0
entryCount = 0
foundEntry = FALSE
local timeFileIDs[0 .. DS_FILTERS_PER_PACKET-1]
for i = 0 to DS_PACKETS_IN_FILTER_TABLE-1 do
  if ($SC_$CPU_DS_PF_TBL[i].MessageID <> 0 AND foundEntry = FALSE) then
    ;; Check for max files being used
    entryCount = 0
    for j = 0 to DS_FILTERS_PER_PACKET-1 do
      if ($SC_$CPU_DS_PF_TBL[i].FilterParams[j].FilterType = DS_BY_TIME) then
	entryCount = entryCount + 1
        timeFileIDs[j] = $SC_$CPU_DS_PF_TBL[i].FilterParams[j].Index
      endif
    enddo
    if (entryCount = DS_FILTERS_PER_PACKET) then
      filterEntry = i
      foundEntry = TRUE
    endif
  endif
enddo

%liv (log_procedure) = logging

write "==> Found filter entry at ", filterEntry

local timeMsgID = $SC_$CPU_DS_PF_TBL[filterEntry].MessageID

wait 5

write ";***********************************************************************"
write ";  Step 2.2.2: Send the TST_DS command to send a message to DS using the"
write ";  MessageID found above. Send a message that will not exceed the file "
write ";  size constraint. The HK should contain info about the files opened. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

expPassedPkts = $SC_$CPU_DS_PassedPktCnt + 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=timeMsgID MsgType=1 Pattern=x'BB'

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

;; Verify that the Passed Packet counter incremented properly
if (expPassedPkts = $SC_$CPU_DS_PassedPktCnt) then
  write "<*> Passed (2000) - Passed packet counter incremented properly."
  ut_setrequirements DS_2000, "P"
else
  write "<!> Failed (2000) - Passed packet counter not set properly. Expected '",expPassedPkts,"'. Count = '",$SC_$CPU_DS_PassedPktCnt,"'."
  ut_setrequirements DS_2000, "F"
endif

;; Loop for the Maximum number of Destinations to determine if
;; the correct number of files were created
local hkSlot = 0
local fileCreateCnt = 0
local fileName = ""
for i = 0 to DS_FILTERS_PER_PACKET-1 do
  hkSlot = timeFileIDs[i]
  if ($SC_$CPU_DS_FileState[hkSlot].FileName <> "") then
    fileName = $SC_$CPU_DS_FileState[hkSlot].FileName
    write "<*> Passed - A file was created with name '",fileName,"' for entry #",i
    fileCreateCnt = fileCreateCnt + 1
  endif
enddo

;; Verify that the Max destination files were created and opened
if (fileCreateCnt = DS_FILTERS_PER_PACKET) then
  write "<*> Passed (2002;2002.1;3000;3000.2;3000.2.1) - The maximum number of files for a single message were created."
  ut_setrequirements DS_2002, "P"
  ut_setrequirements DS_20021, "P"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30002, "A"
  ut_setrequirements DS_300021, "A"
else
  write "<!> Failed (2002;2002.1;3000;3000.2;3000.2.1) - The maximum number of files for a single message did not get created."
  ut_setrequirements DS_2002, "F"
  ut_setrequirements DS_20021, "F"
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30002, "F"
  ut_setrequirements DS_300021, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 2.3: Close the files opened above. "
write ";*********************************************************************"
;; Send the close command for each destination file used in the previous steps
local seqFileName="", timeFileName=""
local slashLoc=0

;; May be able to use th CLoseAllFiles command here. 
;; I would have to rework how to download the files however
 
for i = 0 to DS_FILTERS_PER_PACKET-1 do
  ;; Save the filenames in order to download these files after they are closed
  seqFileName = $SC_$CPU_DS_FileState[seqFileIDs[i]].FileName
  timeFileName = $SC_$CPU_DS_FileState[timeFileIDs[i]].FileName

  ;; Setup the event capture
  ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_CMD_EID, "DEBUG", 1

  cmdCtr = $SC_$CPU_DS_CMDPC + 2
  ;; Send the Close commands
  /$SC_$CPU_DS_CloseFile FileIndex=seqFileIDs[i]
  /$SC_$CPU_DS_CloseFile FileIndex=timeFileIDs[i]

  ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
  if (UT_TW_Status = UT_Success) then
    write "<*> Passed (1004;5002) - DS Close File commands sent properly."
    ut_setrequirements DS_1004, "P"
    ut_setrequirements DS_5002, "P"
  else
    write "<!> Failed (1004;5002) - DS Close File command did not increment CMDPC."
    ut_setrequirements DS_1004, "F"
    ut_setrequirements DS_5002, "F"
  endif

  ;; Check for the event message
  ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 2
  if (UT_TW_Status = UT_Success) then
    write "<*> Passed (1004;5002) - Expected Event Msg ",DS_CLOSE_CMD_EID," rcv'd."
    ut_setrequirements DS_1004, "P"
    ut_setrequirements DS_5002, "P"
  else
    write "<!> Failed (1004;5002) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_CLOSE_CMD_EID,"."
    ut_setrequirements DS_1004, "F"
    ut_setrequirements DS_5002, "F"
  endif

  ;; Download the files in order to perform post-test analysis
  if (seqFileName <> "") then
    ut_setrequirements DS_3003, "A"
    ut_setrequirements DS_3004, "A"
    write "==> Downloading '",seqFileName,"'"
    slashLoc = %locate(seqFileName,"/")
    while (slashLoc <> 0) do
      seqFileName = %substring(seqFileName,slashLoc+1,%length(seqFileName))
      slashLoc = %locate(seqFileName,"/")
    enddo
    write "==> FileName Only = '",seqFileName,"'"
    ;; Need to parse the filename out of the variable above
    ;; before using it in the ftp_file call below
    s ftp_file(ramDir, seqFileName, seqFileName, hostCPU, "G")
  else
    ut_setrequirements DS_3003, "F"
    ut_setrequirements DS_3004, "F"
  endif

  ;; Download the files in order to perform post-test analysis
  if (timeFileName <> "") then
    ut_setrequirements DS_3003, "A"
    ut_setrequirements DS_3004, "A"
    write "==> Downloading '",timeFileName,"'"
    slashLoc = %locate(timeFileName,"/")
    while (slashLoc <> 0) do
      timeFileName = %substring(timeFileName,slashLoc+1,%length(timeFileName))
      slashLoc = %locate(timeFileName,"/")
    enddo
    write "==> FileName Only = '",timeFileName,"'"
    ;; Need to parse the filename out of the variable above
    ;; before using it in the ftp_file call below
    s ftp_file(ramDir, timeFileName, timeFileName, hostCPU, "G")
  else
    ut_setrequirements DS_3003, "F"
    ut_setrequirements DS_3004, "F"
  endif

enddo

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

wait 5

write ";***********************************************************************"
write ";  Step 2.4: Utilizing the TST_DS application, send 'N of X' messages "
write ";  to the DS application using an entry that specifies a single "
write ";  Destination File entry. "
write ";***********************************************************************"
write ";  Step 2.4.1: Parse the tables to find an entry to use."
write ";***********************************************************************"
;; Turn off logging for this step
logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

;; Parse the Packet Filter Table to find a MessageID
;; using the maximum Destination Files and Sequence filtering
filterEntry = 0
foundEntry = FALSE
for i = 0 to DS_PACKETS_IN_FILTER_TABLE-1 do
  if ($SC_$CPU_DS_PF_TBL[i].MessageID <> seqMsgID) AND ;;
     ($SC_$CPU_DS_PF_TBL[i].MessageID <> timeMsgID AND foundEntry = FALSE) then
    filterEntry = i
    foundEntry = TRUE
  endif
enddo

%liv (log_procedure) = logging

write "==> Found filter entry at ", filterEntry

local oneMsgID = $SC_$CPU_DS_PF_TBL[filterEntry].MessageID
local msgsToSend = $SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].X_Value
local msgsStored = $SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].N_Value
local storageOffset = $SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].O_Value
write "==> MsgID = '",%hex(oneMsgID,4),"' - N = ",msgsStored," X = ",msgsToSend," O = ",storageOffset

wait 5

write ";***********************************************************************"
write ";  Step 2.4.2: Send the TST_DS command to send X messages to DS. "
write ";***********************************************************************"
;; Not sure if this should be another TST_DS command or 
;; just send X SendMessage commands from here

;; Setup the message counters to expect
expPassedPkts = $SC_$CPU_DS_PassedPktCnt + msgsStored
local expFilteredPkts = $SC_$CPU_DS_FilteredPktCnt + (msgsToSend - msgsStored)

;; Only 'N' messages should be stored. The file write counter increments by 4 
;; for file creation & then by 1 for each subsequent write
local expFileWrites = $SC_$CPU_DS_FileWriteCnt + 4
expFileWrites = expFileWrites + (msgsStored - 1)

write "==> Passed Packet Cnt = ",$SC_$CPU_DS_PassedPktCnt," expected Cnt = ",expPassedPkts
write "==> File Write Cnt = ",$SC_$CPU_DS_FileWriteCnt," expected Cnt = ",expFileWrites

;; Send 'msgsToSend' messages to DS
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

local dataPattern = 0
for i = 1 to msgsToSend do
  ;; Set the data pattern to send
  dataPattern = i
  ;; Send the command
  /$SC_$CPU_TST_DS_SENDMESSAGE MsgID=oneMsgID MsgType=1 Pattern=dataPattern
  wait 5
enddo

;; Check for the event messages
if ($SC_$CPU_find_event[1].num_found_messages = msgsToSend) then
  write "<*> Passed - Rcv'd the expected number of Send events."
else
  write "<!> Failed - Expected ", msgsToSend, " Send events. Rcv'd ",$SC_$CPU_find_event[1].num_found_messages
endif

;; Should have 'N' messages stored at this point
;; Verify that the Passed Packet counter incremented properly
if (expPassedPkts = $SC_$CPU_DS_PassedPktCnt) AND ;;
   (expFilteredPkts = $SC_$CPU_DS_FilteredPktCnt) AND ;;
   (expFileWrites = $SC_$CPU_DS_FileWriteCnt) then
  write "<*> Passed (2000) - Counters incremented properly."
  ut_setrequirements DS_2000, "P"
else
  write "<!> Failed (2000) - Counters not set properly. Expected Passed pkt = '",expPassedPkts,"' HK = '",$SC_$CPU_DS_PassedPktCnt,"'. Expected File Writes = '",expFileWrites,"' HK  = '",$SC_$CPU_DS_FileWriteCnt,"'. Expected Filtered Pkt = ",expFilteredPkts,"' HK = '",$SC_$CPU_DS_FilteredPktCnt
  ut_setrequirements DS_2000, "F"
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the file was created
local fileTblEntry = $SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].Index
if ($SC_$CPU_DS_FileState[fileTblEntry].FileName <> "") then
  fileName = $SC_$CPU_DS_FileState[fileTblEntry].FileName
  write "<*> Passed (2002.1;3000) - A file was created with name '",fileName,"'"
  ut_setrequirements DS_20021, "P"
  ut_setrequirements DS_3000, "P"
else
  write "<!> Failed (2002.1;3000) - HK does not contain the expected file state information. "
  ut_setrequirements DS_20021, "F"
  ut_setrequirements DS_3000, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.5: Close the Destination File opened in the above step."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Close commands
/$SC_$CPU_DS_CloseFile FileIndex=fileTblEntry

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

;; Download the file
if (fileName <> "") then
  ut_setrequirements DS_2001, "A"
  write "==> Downloading '",fileName,"'"
  slashLoc = %locate(fileName,"/")
  while (slashLoc <> 0) do
    fileName = %substring(fileName,slashLoc+1,%length(fileName))
    slashLoc = %locate(fileName,"/")
  enddo
  write "==> FileName Only = '",fileName,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the ftp_file call below
  s ftp_file(ramDir, fileName, fileName, hostCPU, "G")
else
  ut_setrequirements DS_2001, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 2.6: Send the Set Filter Parameter for a Packet Filter Table "
write ";  entry command specifying a different N_Value. "
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PARMS_CMD_EID, "DEBUG", 1

;; Increment the N value by 1
msgsStored = msgsStored + 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Set Filter Params Command
/$SC_$CPU_DS_SetFilterParams MessageID=oneMsgID ParamIndex=0 N_Value=msgsStored X_Value=msgsToSend O_Value=storageOffset

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5010) - DS Set Filter Parameters command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5010, "P"
else
  write "<!> Failed (1004;5010) - DS Set Filter Parameters command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5010, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5010) - Expected Event Msg ",DS_PARMS_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5010, "P"
else
  write "<!> Failed (1004;5010) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_PARMS_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5010, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.7: Dump the Packet Filter Table. "
write ";***********************************************************************"
;; Dump the Packet Filter Table
s get_tbl_to_cvt (ramDir,filterTblName,"A","$cpu_filtertbl27",hostCPU,filterTblPktId)

;; Verify the table contents reflect the commanded changes
if ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].N_Value = msgsStored) AND ;;
   ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].X_Value = msgsToSend) AND ;;
   ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].O_Value = storageOffset) then
  write "<*> Passed (5010) - Parameter changes are reflected in the table."
  ut_setrequirements DS_5010, "P"
else  
  write "<!> Failed (5010) - Expected parameter changes were not set in the Table entry."
  ut_setrequirements DS_5010, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.8: Utilizing the TST_DS application, send 'N of X' messages "
write ";  to the DS application using the entry modified above. "
write ";***********************************************************************"
;; Not sure if this should be another TST_DS command or 
;; just send X SendMessage commands from here
write "==> MsgID = '",%hex(oneMsgID,4),"' - N = ",msgsStored," X = ",msgsToSend," O = ",storageOffset

;; Setup the message counters to expect
expPassedPkts = $SC_$CPU_DS_PassedPktCnt + msgsStored
expFilteredPkts = $SC_$CPU_DS_FilteredPktCnt + (msgsToSend - msgsStored)

;; Only 'N' messages should be stored. The file write counter increments by 4 
;; for file creation & then by 1 for each subsequent write
local expFileWrites = $SC_$CPU_DS_FileWriteCnt + 4
expFileWrites = expFileWrites + (msgsStored - 1)

write "==> Passed Packet Cnt = ",$SC_$CPU_DS_PassedPktCnt," expected Cnt = ",expPassedPkts
write "==> File Write Cnt = ",$SC_$CPU_DS_FileWriteCnt," expected Cnt = ",expFileWrites

;; Send 'msgsToSend' messages to DS
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

for i = 1 to msgsToSend do
  ;; Set the data pattern to send
  dataPattern = %hex(i,2)
  ;; Send the command
  /$SC_$CPU_TST_DS_SENDMESSAGE MsgID=oneMsgID MsgType=1 Pattern=dataPattern
  wait 5
enddo

;; Check for the event messages
if ($SC_$CPU_find_event[1].num_found_messages = msgsToSend) then
  write "<*> Passed - Rcv'd the expected number of Send events."
else
  write "<!> Failed - Expected ", msgsToSend, " Send events. Rcv'd ",$SC_$CPU_find_event[1].num_found_messages
endif

;; Should have 'N' messages stored at this point
;; Verify that the Passed Packet counter incremented properly
if (expPassedPkts = $SC_$CPU_DS_PassedPktCnt) AND ;;
   (expFilteredPkts = $SC_$CPU_DS_FilteredPktCnt) AND ;;
   (expFileWrites = $SC_$CPU_DS_FileWriteCnt) then
  write "<*> Passed (2000) - Counters incremented properly."
  ut_setrequirements DS_2000, "P"
else
  write "<!> Failed (2000) - Counters not set properly. Expected Passed pkt = '",expPassedPkts,"' HK = '",$SC_$CPU_DS_PassedPktCnt,"'. Expected File Writes = '",expFileWrites,"' HK  = '",$SC_$CPU_DS_FileWriteCnt,"'. Expected Filtered Pkt = ",expFilteredPkts,"' HK = '",$SC_$CPU_DS_FilteredPktCnt
  ut_setrequirements DS_2000, "F"
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify the file was created
fileTblEntry = $SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].Index
if ($SC_$CPU_DS_FileState[fileTblEntry].FileName <> "") then
  fileName = $SC_$CPU_DS_FileState[fileTblEntry].FileName
  write "<*> Passed (2002.1;3000) - A file was created with name '",fileName,"'"
  ut_setrequirements DS_20021, "P"
  ut_setrequirements DS_3000, "P"
else
  write "<!> Failed (2002.1;3000) - HK does not contain the expected file state information. "
  ut_setrequirements DS_20021, "F"
  ut_setrequirements DS_3000, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.9: Close the Destination File opened in the above step."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Close commands
/$SC_$CPU_DS_CloseFile FileIndex=fileTblEntry

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

;; Download the file
if (fileName <> "") then
  ut_setrequirements DS_2001, "A"
  write "==> Downloading '",fileName,"'"
  slashLoc = %locate(fileName,"/")
  while (slashLoc <> 0) do
    fileName = %substring(fileName,slashLoc+1,%length(fileName))
    slashLoc = %locate(fileName,"/")
  enddo
  write "==> FileName Only = '",fileName,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the ftp_file call below
  s ftp_file(ramDir, fileName, fileName, hostCPU, "G")
else
  ut_setrequirements DS_2001, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.10: Set the N_Value and X_Value to zero (0) for two separate "
write ";  Filter Table entries. Use the one from above and find another."
write ";***********************************************************************"
write ";  Step 2.10.1: Parse the tables to find an entry to use."
write ";***********************************************************************"
;; Turn off logging for this step
logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

;; Parse the Packet Filter Table to find a MessageID
local xFilterEntry = 0
foundEntry = FALSE
for i = 0 to DS_PACKETS_IN_FILTER_TABLE-1 do
  if ($SC_$CPU_DS_PF_TBL[i].MessageID <> seqMsgID) AND ;;
     ($SC_$CPU_DS_PF_TBL[i].MessageID <> timeMsgID) AND ;;
     ($SC_$CPU_DS_PF_TBL[i].MessageID <> oneMsgID AND foundEntry = FALSE) then
    xFilterEntry = i
    foundEntry = TRUE
  endif
enddo

%liv (log_procedure) = logging

write "==> Found filter entry at ", xFilterEntry

local xMsgID = $SC_$CPU_DS_PF_TBL[xFilterEntry].MessageID
local nValue = $SC_$CPU_DS_PF_TBL[xFilterEntry].FilterParams[0].N_Value
local oValue = $SC_$CPU_DS_PF_TBL[xFilterEntry].FilterParams[0].O_Value
local xFileIndex = $SC_$CPU_DS_PF_TBL[xFilterEntry].FilterParams[0].Index

wait 5

write ";*********************************************************************"
write ";  Step 2.10.2: Send the Set Filter Parameter entry command specifying "
write ";  zero (0) for the N_Value parameter. "
write ";*********************************************************************"
;; Setup for event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PARMS_CMD_EID, "DEBUG", 1

;; Increment the N value by 1
msgsStored = msgsStored = 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Set Fiter Params Command
/$SC_$CPU_DS_SetFilterParams MessageID=oneMsgID ParamIndex=0 N_Value=0 X_Value=msgsToSend O_Value=storageOffset

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5010) - DS Set Filter Parameters command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5010, "P"
else
  write "<!> Failed (1004;5010) - DS Set Filter Parameters command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5010, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5010) - Expected Event Msg ",DS_PARMS_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5010, "P"
else
  write "<!> Failed (1004;5010) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_PARMS_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5010, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 2.10.3: Send the Set Filter Parameter entry command specifying "
write ";  zero (0) for the X_Value parameter. "
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PARMS_CMD_EID, "DEBUG", 1

;; Increment the N value by 1
msgsStored = msgsStored = 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Set Fiter Params Command
/$SC_$CPU_DS_SetFilterParams MessageID=xMsgID ParamIndex=0 N_Value=0 X_Value=0 O_Value=oValue

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5010) - DS Set Filter Parameters command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5010, "P"
else
  write "<!> Failed (1004;5010) - DS Set Filter Parameters command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5010, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5010) - Expected Event Msg ",DS_PARMS_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5010, "P"
else
  write "<!> Failed (1004;5010) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_PARMS_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5010, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.11: Dump the Packet Filter Table. "
write ";***********************************************************************"
;; Dump the Packet Filter Table
s get_tbl_to_cvt (ramDir,filterTblName,"A","$cpu_filtertbl211",hostCPU,filterTblPktId)

;; Verify the table contents reflect the commanded changes
if ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].N_Value = 0) AND ;;
   ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].X_Value = msgsToSend) AND ;;
   ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].O_Value = storageOffset) AND ;;
   ($SC_$CPU_DS_PF_TBL[xFilterEntry].FilterParams[0].N_Value = 0) AND ;;
   ($SC_$CPU_DS_PF_TBL[xFilterEntry].FilterParams[0].X_Value = 0) AND ;;
   ($SC_$CPU_DS_PF_TBL[xFilterEntry].FilterParams[0].O_Value = oValue) then
  write "<*> Passed (5010) - Parameter changes are reflected in the table."
  ut_setrequirements DS_5010, "P"
else  
  write "<!> Failed (5010) - Expected parameter changes were not set in the Table entry."
  ut_setrequirements DS_5010, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.12: Send the TST_DS command to send messages to DS using the"
write ";  message IDs whose parameters were set in the above steps."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

;; Send the commands
for i = 1 to msgsToSend do
  ;; Send the command
  /$SC_$CPU_TST_DS_SENDMESSAGE MsgID=oneMsgID MsgType=1 Pattern=x'55'
  wait 5
enddo

/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=xMsgID MsgType=1 Pattern=x'AA'
wait 5

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Check for the event messages
if ($SC_$CPU_find_event[1].num_found_messages = msgsToSend+1) then
  write "<*> Passed - Rcv'd the expected number of Send events."
else
  write "<!> Failed - Expected ",msgsToSend+1," Send events. Rcv'd ",$SC_$CPU_find_event[1].num_found_messages
endif

;; Verify that the messages were not stored
;; If the HK does not specify a filename, then the file was not created
;; This verifies DS2001.1
fileName = $SC_$CPU_DS_FileState[fileTblEntry].FileName
if (fileName = "") then
  write "<*> Passed (2001.1) - A file was not created."
  ut_setrequirements DS_20011, "P"
else
  write "<!> Failed (2001.1) - A file was created with name '",fileName,"'"
  ut_setrequirements DS_20011, "F"
endif

fileName = $SC_$CPU_DS_FileState[xFileIndex].FileName
if (fileName = "") then
  write "<*> Passed (2001.1) - A file was not created."
  ut_setrequirements DS_20011, "P"
else
  write "<!> Failed (2001.1) - A file was created with name '",fileName,"'"
  ut_setrequirements DS_20011, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.13: Send the Set File Index for a Packet Filter Table entry "
write ";  command for an entry that specifies Sequence filtering with a  "
write ";  Destination File entry that specifies Time file type naming. "
write ";***********************************************************************"
write ";  Step 2.13.1: Parse the tables to find an entry to use."
write ";***********************************************************************"
;; Turn off logging for this step
logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

;; Parse the Packet Filter Table to find a MessageID
filterEntry = 0
foundEntry = FALSE
for i = 0 to DS_PACKETS_IN_FILTER_TABLE-1 do
  if ($SC_$CPU_DS_PF_TBL[i].MessageID <> seqMsgID) AND ;;
     ($SC_$CPU_DS_PF_TBL[i].MessageID <> timeMsgID) AND ;;
     ($SC_$CPU_DS_PF_TBL[i].MessageID <> oneMsgID) AND ;;
     ($SC_$CPU_DS_PF_TBL[i].FilterParams[0].FilterType = DS_BY_COUNT) AND ;;
     ($SC_$CPU_DS_PF_TBL[i].MessageID <> xMsgID AND foundEntry = FALSE) then
    filterEntry = i
    foundEntry = TRUE
  endif
enddo

; Now parse the Destination File table for an entry that
;; specifies Time file naming
local timeFileEntry = 0
foundEntry = FALSE
for i = 0 to DS_DEST_FILE_CNT-1 do
  if ($SC_$CPU_DS_DF_TBL[i].FileNameType = DS_BY_TIME AND foundEntry = FALSE) then
    timeFileEntry = i
    foundEntry = TRUE
  endif
enddo

%liv (log_procedure) = logging

write "==> Found filter entry at ", filterEntry
write "==> Found file entry at ", timeFileEntry

local timeFileMsgID = $SC_$CPU_DS_PF_TBL[filterEntry].MessageID

wait 5

write ";***********************************************************************"
write ";  Step 2.13.2: Send the Set File Index command using the entries found "
write ";  in the above step. "
write ";***********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FILE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Set Filter File Command
/$SC_$CPU_DS_SetFilterFile MessageID=timeFileMsgID ParamIndex=0 FileIndex=timeFileEntry

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5008) - DS Set Filter File command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5008, "P"
else
  write "<!> Failed (1004;5008) - DS Set Filter File command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5008, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5008) - Expected Event Msg ",DS_FILE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5008, "P"
else
  write "<!> Failed (1004;5008) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_FILE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5008, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.13.3: Dump the Packet Filter Table. "
write ";***********************************************************************"
;; Dump the Packet Filter Table
s get_tbl_to_cvt (ramDir,filterTblName,"A","$cpu_filtertbl2133",hostCPU,filterTblPktId)

;; Verify the table contents reflect the commanded changes
if ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].Index = timeFileEntry) then
  write "<*> Passed (5008) - Parameter changes are reflected in the table."
  ut_setrequirements DS_5008, "P"
else  
  write "<!> Failed (5008) - Expected parameter changes were not set in the Table entry."
  ut_setrequirements DS_5008, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.14: Send the TST_DS command to send a message to DS using the "
write ";  MessageID found above. Send a message that will not exceed the file "
write ";  size constraint. The HK should contain info about the file opened. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

expPassedPkts = $SC_$CPU_DS_PassedPktCnt + 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=timeFileMsgID MsgType=1 Pattern=x'FF'

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

;; Verify that the Passed Packet counter incremented properly
if (expPassedPkts = $SC_$CPU_DS_PassedPktCnt) then
  write "<*> Passed (2000) - Passed packet counter incremented properly."
  ut_setrequirements DS_2000, "P"
else
  write "<!> Failed (2000) - Passed packet counter not set properly. Expected '",expPassedPkts,"'. Count = '",$SC_$CPU_DS_PassedPktCnt,"'."
  ut_setrequirements DS_2000, "F"
endif

;; Verify that the file was created
fileName = ""
if ($SC_$CPU_DS_FileState[timeFileEntry].FileName <> "") then
  fileName = $SC_$CPU_DS_FileState[timeFileEntry].FileName
  write "<*> Passed (2002.1;3000;3000.2;3000.2.1) - A file was created with name '",fileName,"'"
  ut_setrequirements DS_20021, "P"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30002, "A"
  ut_setrequirements DS_300021, "A"
else
  write "<!> Failed (2002.1;3000;3000.2;3000.2.1) - HK does not contain the expected file state information."
  ut_setrequirements DS_20021, "F"
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30002, "F"
  ut_setrequirements DS_300021, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.15: Close the Destination File opened in the above step."
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Close commands
/$SC_$CPU_DS_CloseFile FileIndex=timeFileEntry

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

;; Download the file
if (fileName <> "") then
  ut_setrequirements DS_3003, "A"
  ut_setrequirements DS_3004, "A"
  write "==> Downloading '",fileName,"'"
  slashLoc = %locate(fileName,"/")
  while (slashLoc <> 0) do
    fileName = %substring(fileName,slashLoc+1,%length(fileName))
    slashLoc = %locate(fileName,"/")
  enddo
  write "==> FileName Only = '",fileName,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the ftp_file call below
  s ftp_file(ramDir, fileName, fileName, hostCPU, "G")
else
  ut_setrequirements DS_3003, "F"
  ut_setrequirements DS_3004, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.16: Send the Set File Index for a Packet Filter Table entry "
write ";  command for an entry that specifies Time filtering with a  "
write ";  Destination File entry that specifies Sequence file type naming. "
write ";***********************************************************************"
write ";  Step 2.16.1: Parse the tables to find an entry to use."
write ";***********************************************************************"
;; Turn off logging for this step
logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

;; Parse the Packet Filter Table to find a MessageID
filterEntry = 0
foundEntry = FALSE
for i = 0 to DS_PACKETS_IN_FILTER_TABLE-1 do
  if ($SC_$CPU_DS_PF_TBL[i].MessageID <> seqMsgID) AND ;;
     ($SC_$CPU_DS_PF_TBL[i].MessageID <> timeMsgID) AND ;;
     ($SC_$CPU_DS_PF_TBL[i].MessageID <> oneMsgID) AND ;;
     ($SC_$CPU_DS_PF_TBL[i].MessageID <> xMsgID AND foundEntry = FALSE) AND ;;
     ($SC_$CPU_DS_PF_TBL[i].FilterParams[0].FilterType = DS_BY_TIME) then
    filterEntry = i
    foundEntry = TRUE
  endif
enddo

;; Now parse the Destination File table for an entry that
;; specifies Sequence file naming
local seqFileEntry = 0
foundEntry = FALSE
for i = 0 to DS_DEST_FILE_CNT-1 do
  if ($SC_$CPU_DS_DF_TBL[i].FileNameType = DS_BY_COUNT) AND (foundEntry = FALSE) then
    seqFileEntry = i
    foundEntry = TRUE
  endif
enddo

%liv (log_procedure) = logging

write "==> Found filter entry at ", filterEntry
write "==> Found file entry at ", seqFileEntry

local seqFileMsgID = $SC_$CPU_DS_PF_TBL[filterEntry].MessageID

wait 5

write ";***********************************************************************"
write ";  Step 2.16.2: Send the Set File Index command using the entries found "
write ";  in the above step. "
write ";***********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FILE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Set Filter File Command
/$SC_$CPU_DS_SetFilterFile MessageID=seqFileMsgID ParamIndex=0 FileIndex=seqFileEntry

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5008) - DS Set Filter File command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5008, "P"
else
  write "<!> Failed (1004;5008) - DS Set Filter File command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5008, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5008) - Expected Event Msg ",DS_FILE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5008, "P"
else
  write "<!> Failed (1004;5008) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_FILE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5008, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.16.3: Dump the Packet Filter Table. "
write ";***********************************************************************"
;; Dump the Packet Filter Table
s get_tbl_to_cvt (ramDir,filterTblName,"A","$cpu_filtertbl2163",hostCPU,filterTblPktId)

;; Verify the table contents reflect the commanded changes
if ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].Index = seqFileEntry) then
  write "<*> Passed (5008) - Parameter changes are reflected in the table."
  ut_setrequirements DS_5008, "P"
else  
  write "<!> Failed (5008) - Expected parameter changes were not set in the Table entry."
  ut_setrequirements DS_5008, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.17: Send the TST_DS command to send a message to DS using the "
write ";  MessageID found above. Send a message that will not exceed the file "
write ";  size constraint. The HK should contain info about the file opened. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

expPassedPkts = $SC_$CPU_DS_PassedPktCnt + 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqFileMsgID MsgType=1 Pattern=x'55'

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

;; Verify that the Passed Packet counter incremented properly
if (expPassedPkts = $SC_$CPU_DS_PassedPktCnt) then
  write "<*> Passed (2000) - Passed packet counter incremented properly."
  ut_setrequirements DS_2000, "P"
else
  write "<!> Failed (2000) - Passed packet counter not set properly. Expected '",expPassedPkts,"'. Count = '",$SC_$CPU_DS_PassedPktCnt,"'."
  ut_setrequirements DS_2000, "F"
endif

;; Verify that the file was created
fileName = ""
if ($SC_$CPU_DS_FileState[seqFileEntry].FileName <> "") then
  fileName = $SC_$CPU_DS_FileState[seqFileEntry].FileName
  write "<*> Passed (2002.1;3000;3000.1;3000.1.1) - A file was created with name '",fileName,"'"
  ut_setrequirements DS_20021, "P"
  ut_setrequirements DS_3000, "P"
  ut_setrequirements DS_30001, "A"
  ut_setrequirements DS_300011, "A"
else
  write "<!> Failed (2002.1;3000;3000.1;3000.1.1) - HK does not contain the expected file state information."
  ut_setrequirements DS_20021, "F"
  ut_setrequirements DS_3000, "F"
  ut_setrequirements DS_30001, "F"
  ut_setrequirements DS_300011, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.18: Close the Destination File opened in the above step."
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

;; Download the file
if (fileName <> "") then
  ut_setrequirements DS_3003, "A"
  ut_setrequirements DS_3004, "A"
  write "==> Downloading '",fileName,"'"
  slashLoc = %locate(fileName,"/")
  while (slashLoc <> 0) do
    fileName = %substring(fileName,slashLoc+1,%length(fileName))
    slashLoc = %locate(fileName,"/")
  enddo
  write "==> FileName Only = '",fileName,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the ftp_file call below
  s ftp_file(ramDir, fileName, fileName, hostCPU, "G")
else
  ut_setrequirements DS_3003, "F"
  ut_setrequirements DS_3004, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.0: Commanding Tests "
write ";*********************************************************************"
write ";  Step 3.1: Using the Packet Filter Table entry from Step 2.18, send "
write ";  the Set File Index command using a different Destination File index."
write ";*********************************************************************"
;; Find a different index
local newFileEntry = 0
foundEntry = FALSE
for i = 0 to DS_DEST_FILE_CNT-1 do
  if (i <> seqFileEntry) AND ;;
     ($SC_$CPU_DS_DF_TBL[i].FileNameType = DS_BY_COUNT) AND ;;
     (foundEntry = FALSE) then
    newFileEntry = i
    foundEntry = TRUE
  endif
enddo

write "==> Found file entry at ", newFileEntry

;; Send the Set File Command
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FILE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Set Filter File Command
/$SC_$CPU_DS_SetFilterFile MessageID=seqFileMsgID ParamIndex=0 FileIndex=newFileEntry

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5008) - DS Set Filter File command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5008, "P"
else
  write "<!> Failed (1004;5008) - DS Set Filter File command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5008, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5008) - Expected Event Msg ",DS_FILE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5008, "P"
else
  write "<!> Failed (1004;5008) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_FILE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5008, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.2: Dump the Packet Filter Table. "
write ";***********************************************************************"
;; Dump the Packet Filter Table
s get_tbl_to_cvt (ramDir,filterTblName,"A","$cpu_filtertbl32",hostCPU,filterTblPktId)

;; Verify the table contents reflect the commanded changes
if ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].Index = newFileEntry) then
  write "<*> Passed (5008) - Parameter changes are reflected in the table."
  ut_setrequirements DS_5008, "P"
else  
  write "<!> Failed (5008) - Expected parameter changes were not set in the Table entry."
  ut_setrequirements DS_5008, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.3: Send the TST_DS command to send a message to DS using the "
write ";  MessageID used above. Send a message that will not exceed the file "
write ";  size constraint. The HK should contain info about the file opened. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

expPassedPkts = $SC_$CPU_DS_PassedPktCnt + 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqFileMsgID MsgType=1 Pattern=x'AA'

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

;; Verify that the Passed Packet counter incremented properly
if (expPassedPkts = $SC_$CPU_DS_PassedPktCnt) then
  write "<*> Passed (2000) - Passed packet counter incremented properly."
  ut_setrequirements DS_2000, "P"
else
  write "<!> Failed (2000) - Passed packet counter not set properly. Expected '",expPassedPkts,"'. Count = '",$SC_$CPU_DS_PassedPktCnt,"'."
  ut_setrequirements DS_2000, "F"
endif

;; Verify that the file was created
fileName = ""
if ($SC_$CPU_DS_FileState[newFileEntry].FileName <> "") then
  fileName = $SC_$CPU_DS_FileState[newFileEntry].FileName
  write "<*> Passed (2002.1;3000) - A file was created with name '",fileName,"'"
  ut_setrequirements DS_20021, "P"
  ut_setrequirements DS_3000, "P"
else
  write "<!> Failed (2002.1;3000;) - HK does not contain the expected file state information."
  ut_setrequirements DS_20021, "F"
  ut_setrequirements DS_3000, "F"
endif

;; Close the above file
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Close commands
/$SC_$CPU_DS_CloseFile FileIndex=newFileEntry

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

;; Download the file
if (fileName <> "") then
  ut_setrequirements DS_2001, "A"
  write "==> Downloading '",fileName,"'"
  slashLoc = %locate(fileName,"/")
  while (slashLoc <> 0) do
    fileName = %substring(fileName,slashLoc+1,%length(fileName))
    slashLoc = %locate(fileName,"/")
  enddo
  write "==> FileName Only = '",fileName,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the ftp_file call below
  s ftp_file(ramDir, fileName, fileName, hostCPU, "G")
else
  ut_setrequirements DS_2001, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.4: Send the Set File Index command with an invalid length."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FILE_CMD_ERR_EID, "ERROR", 1

local errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc0000008038F"

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
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_FILE_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.5: Send the Set File Index command with invalid arguments."
write ";*********************************************************************"
write ";  Step 3.5.1: Send the Set File Index command with  message ID that "
write ";  does not exist in the table. "
write ";*********************************************************************"
;; Send the Set File Command
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FILE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the Set Filter File Command
/$SC_$CPU_DS_SetFilterFile MessageID=x'F345' ParamIndex=0 FileIndex=newFileEntry

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Filter File command failed as expected."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Filter File command did not increment CMDEC."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Expected Event Msg ",DS_FILE_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_FILE_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.5.2: Send the Set File Index command with an invalid "
write ";  parameter index. "
write ";*********************************************************************"
;; Send the Set File Command
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FILE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the Set Filter File Command
/$SC_$CPU_DS_SetFilterFile MessageID=seqFileMsgID ParamIndex=DS_FILTERS_PER_PACKET FileIndex=newFileEntry

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Filter File command failed as expected."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Filter File command did not increment CMDEC."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Expected Event Msg ",DS_FILE_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_FILE_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.5.3: Send the Set File Index command with an invalid "
write ";  file index. "
write ";*********************************************************************"
;; Send the Set File Command
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FILE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the Set Filter File Command
/$SC_$CPU_DS_SetFilterFile MessageID=seqFileMsgID ParamIndex=0 FileIndex=DS_DEST_FILE_CNT

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Filter File command failed as expected."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Filter File command did not increment CMDEC."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Expected Event Msg ",DS_FILE_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_FILE_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.6: Send the Set Filter Type command. "
write ";*********************************************************************"
;; Get the Filter Table Type from an existing entry
local existingFilterType = $SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].FilterType
local newFilterType = 0
if (existingFilterType = DS_BY_COUNT) then
  newFilterType = DS_BY_TIME
else
  newFilterType = DS_BY_COUNT
endif

;; Get the message ID for the entry
seqFileMsgID = $SC_$CPU_DS_PF_TBL[filterEntry].MessageID

;; Setup to capture the event
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FTYPE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Set Filter Type Command
/$SC_$CPU_DS_SetFilterType MessageID=seqFileMsgID, ParamIndex=0, FilterType=newFilterType 

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5009) - DS Set Filter Type command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5009, "P"
else
  write "<!> Failed (1004;5009) - DS Set Filter Type command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5009, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5009) - Expected Event Msg ",DS_FTYPE_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5009, "P"
else
  write "<!> Failed (1004;5009) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_FTYPE_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5009, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.7: Dump the Packet Filter Table. "
write ";***********************************************************************"
;; Dump the Packet Filter Table
s get_tbl_to_cvt (ramDir,filterTblName,"A","$cpu_filtertbl37",hostCPU,filterTblPktId)

;; Verify the table contents reflect the commanded changes
if ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].FilterType = newFilterType) then
  write "<*> Passed (5009) - Parameter changes are reflected in the table."
  ut_setrequirements DS_5009, "P"
else  
  write "<!> Failed (5009) - Expected parameter changes were not set in the Table entry."
  ut_setrequirements DS_5009, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.8: Send the TST_DS command to send a message to DS using the "
write ";  MessageID used above. Send a message that will not exceed the file "
write ";  size constraint. The HK should contain info about the file opened. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

expPassedPkts = $SC_$CPU_DS_PassedPktCnt + 1

;; Send the command
/$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqFileMsgID MsgType=1 Pattern=x'55'

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

;; Verify that the Passed Packet counter incremented properly
if (expPassedPkts = $SC_$CPU_DS_PassedPktCnt) then
  write "<*> Passed (2000) - Passed packet counter incremented properly."
  ut_setrequirements DS_2000, "P"
else
  write "<!> Failed (2000) - Passed packet counter not set properly. Expected '",expPassedPkts,"'. Count = '",$SC_$CPU_DS_PassedPktCnt,"'."
  ut_setrequirements DS_2000, "F"
endif

;; Verify that the file was created
fileName = ""
if ($SC_$CPU_DS_FileState[newFileEntry].FileName <> "") then
  fileName = $SC_$CPU_DS_FileState[newFileEntry].FileName
  write "<*> Passed (2002.1;3000) - A file was created with name '",fileName,"'"
  ut_setrequirements DS_20021, "P"
  ut_setrequirements DS_3000, "P"
else
  write "<!> Failed (2002.1;3000) - HK does not contain the expected file state information."
  ut_setrequirements DS_20021, "F"
  ut_setrequirements DS_3000, "F"
endif

;; Close the above file
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Close commands
/$SC_$CPU_DS_CloseFile FileIndex=newFileEntry

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

;; Download the file
if (fileName <> "") then
  ut_setrequirements DS_2001, "A"
  write "==> Downloading '",fileName,"'"
  slashLoc = %locate(fileName,"/")
  while (slashLoc <> 0) do
    fileName = %substring(fileName,slashLoc+1,%length(fileName))
    slashLoc = %locate(fileName,"/")
  enddo
  write "==> FileName Only = '",fileName,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the ftp_file call below
  s ftp_file(ramDir, fileName, fileName, hostCPU, "G")
else
  ut_setrequirements DS_2001, "F"
endif

wait 5


write ";*********************************************************************"
write ";  Step 3.9: Send the Set Filter Type command with an invalid length."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FTYPE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc0000008049E"

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
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_FTYPE_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.10: Send the Set Filter Type command with invalid arguments."
write ";*********************************************************************"
write ";  Step 3.10.1: Send the Set Filter Type command with an invalid message"
write ";*********************************************************************"
;; Send the Set File Command
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FTYPE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the Set Filter Type Command
/$SC_$CPU_DS_SetFilterType MessageID=x'1345' ParamIndex=0 FilterType=DS_BY_COUNT

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Filter Type command failed as expected."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Filter Type command did not increment CMDEC."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Expected Event Msg ",DS_FTYPE_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_FTYPE_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.10.2: Send the Set Filter Type command with an invalid "
write ";  parameter index. "
write ";*********************************************************************"
;; Send the Set File Command
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FTYPE_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the Set Filter Type Command
/$SC_$CPU_DS_SetFilterType MessageID=seqFileMsgID ParamIndex=DS_FILTERS_PER_PACKET FilterType=DS_BY_COUNT

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Filter Type command failed as expected."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Filter Type command did not increment CMDEC."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Expected Event Msg ",DS_FTYPE_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_FTYPE_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.10.3: Send the Set Filter Type command with an invalid filter "
write ";  type. "
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FTYPE_CMD_ERR_EID, "ERROR", 1

;; Send the Set Filter Type Command
errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the Set Filter Type Command
/$SC_$CPU_DS_SetFilterType MessageID=seqFileMsgID ParamIndex=0 FilterType=3

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Filter Type command failed as expected."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Filter Type command did not increment CMDEC."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Expected Event Msg ",DS_FTYPE_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_FTYPE_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.11: Send the Set Filter Parameter command. "
write ";*********************************************************************"
;; Get the Filter Parameters from an existing entry
local existingNVal = $SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].N_Value
local existingXVal = $SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].X_Value
local existingOVal = $SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].O_Value
local newNVal = existingNVal + 3
local newXVal = existingXVal + 5
local newOVal = existingOVal + 2

;; Setup for event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PARMS_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Set Filter Parameters Command
/$SC_$CPU_DS_SetFilterParams MessageID=seqFileMsgID ParamIndex=0 N_Value=newNVal X_Value=newXVal O_Value=newOVal

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5010) - DS Set Filter Parameters command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5010, "P"
else
  write "<!> Failed (1004;5010) - DS Set Filter Parameters command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5010, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5010) - Expected Event Msg ",DS_PARMS_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5010, "P"
else
  write "<!> Failed (1004;5010) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_PARMS_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5010, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.12: Dump the Packet Filter Table. "
write ";***********************************************************************"
;; Dump the Packet Filter Table
s get_tbl_to_cvt (ramDir,filterTblName,"A","$cpu_filtertbl312",hostCPU,filterTblPktId)

;; Verify the table contents reflect the commanded changes
if ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].N_Value = newNVal) AND ;;
   ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].X_Value = newXVal) AND ;;
   ($SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].O_Value = newOVal) then
  write "<*> Passed (5010) - Parameter changes are reflected in the table."
  ut_setrequirements DS_5010, "P"
else  
  write "<!> Failed (5010) - Expected parameter changes were not set in the Table entry."
  ut_setrequirements DS_5010, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.13: Determine if the Filtering Algorithm works properly for "
write ";  Sequence based filtering. "
write ";*********************************************************************"
write ";  Step 3.13.1: Determine if the Filter Type is set to Sequence based. "
write ";*********************************************************************"
;; Get the Filter Table Type from the existing entry
existingFilterType = $SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].FilterType

;; Set the Filter Type to Sequence based filtering if it is not already
if (existingFilterType = DS_BY_TIME) then
  ;; Setup to capture the event
  ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FTYPE_CMD_EID, "DEBUG", 1

  cmdCtr = $SC_$CPU_DS_CMDPC + 1
  ;; Send the Set Filter Type Command
  /$SC_$CPU_DS_SetFilterType MessageID=seqFileMsgID, ParamIndex=0, FilterType=DS_BY_COUNT

  ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
  if (UT_TW_Status = UT_Success) then
    write "<*> Passed (1004;5009) - DS Set Filter Type command sent properly."
    ut_setrequirements DS_1004, "P"
    ut_setrequirements DS_5009, "P"
  else
    write "<!> Failed (1004;5009) - DS Set Filter Type command did not increment CMDPC."
    ut_setrequirements DS_1004, "F"
    ut_setrequirements DS_5009, "F"
  endif

  ;; Check for the event message
  ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
  if (UT_TW_Status = UT_Success) then
    write "<*> Passed (1004;5009) - Expected Event Msg ",DS_FTYPE_CMD_EID," rcv'd."
    ut_setrequirements DS_1004, "P"
    ut_setrequirements DS_5009, "P"
  else
    write "<!> Failed (1004;5009) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_FTYPE_CMD_EID,"."
    ut_setrequirements DS_1004, "F"
    ut_setrequirements DS_5009, "F"
  endif
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.13.2: Send the TST_DS command to send a message to DS using "
write ";  the MessageID used above. Send 'X' messages. The HK should contain "
write ";  information about the file opened. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

expPassedPkts = $SC_$CPU_DS_PassedPktCnt + newNVal

;; Send the command
for i = 1 to newXVal do
  ;; Set the data pattern to send
  dataPattern = i
  /$SC_$CPU_TST_DS_SENDMESSAGE MsgID=seqFileMsgID MsgType=1 Pattern=dataPattern
  wait 5
enddo

;; Check for the event message
if ($SC_$CPU_find_event[1].num_found_messages = newXVal) then
  write "<*> Passed - Expected Event Msgs ",TST_DS_MSG1_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Expected ",newXVal," event messages. Rcv'd ",$SC_$CPU_find_event[1].num_found_messages,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify that the Passed Packet counter incremented properly
if (expPassedPkts = $SC_$CPU_DS_PassedPktCnt) then
  write "<*> Passed (2000) - Passed packet counter incremented properly."
  ut_setrequirements DS_2000, "P"
else
  write "<!> Failed (2000) - Passed packet counter not set properly. Expected '",expPassedPkts,"'. Count = '",$SC_$CPU_DS_PassedPktCnt,"'."
  ut_setrequirements DS_2000, "F"
endif

;; Verify that the file was created
fileName = ""
if ($SC_$CPU_DS_FileState[newFileEntry].FileName <> "") then
  fileName = $SC_$CPU_DS_FileState[newFileEntry].FileName
  write "<*> Passed (2002.1;2003;2003.1;3000) - A file was created with name '",fileName,"'"
  ut_setrequirements DS_20021, "P"
  ut_setrequirements DS_2003, "A"
  ut_setrequirements DS_20031, "A"
  ut_setrequirements DS_3000, "P"
else
  write "<!> Failed (2002.1;2003;2003.1;3000) - HK does not contain the expected file state information."
  ut_setrequirements DS_20021, "F"
  ut_setrequirements DS_2003, "F"
  ut_setrequirements DS_20031, "F"
  ut_setrequirements DS_3000, "F"
endif

write ";***********************************************************************"
write ";  Step 3.13.3: Close and download the file created above. "
write ";***********************************************************************"
;; Close the above file
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Close commands
/$SC_$CPU_DS_CloseFile FileIndex=newFileEntry

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

;; Download the file
if (fileName <> "") then
  ut_setrequirements DS_2001, "A"
  write "==> Downloading '",fileName,"'"
  slashLoc = %locate(fileName,"/")
  while (slashLoc <> 0) do
    fileName = %substring(fileName,slashLoc+1,%length(fileName))
    slashLoc = %locate(fileName,"/")
  enddo
  write "==> FileName Only = '",fileName,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the ftp_file call below
  s ftp_file(ramDir, fileName, fileName, hostCPU, "G")
else
  ut_setrequirements DS_2001, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.14: Send the Set Filter Parameter command with an invalid "
write ";  length."
write ";*********************************************************************"
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PARMS_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1

;; CPU1 is the default
rawcmd = "18BBc000000C058A"

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
  write "<!> Failed (1005) - Event message ",$SC_$CPU_evs_eventid," received. Expected Event message ",DS_PARMS_CMD_ERR_EID, "."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.15: Send the Set Filter Parameter command with invalid "
write ";  arguments."
write ";***********************************************************************"
write ";  Step 3.15.1: Send the Set Filter Parameter command with an invalid "
write ";  message ID. "
write ";***********************************************************************"
;; Send the Set File Command
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PARMS_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the Set Filter Type Command
/$SC_$CPU_DS_SetFilterParams MessageID=x'ABCD' ParamIndex=0 N_Value=newNVal X_Value=newXVal O_Value=newOVal

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Filter Parameters command failed as expected."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Filter Parameters command did not increment CMDEC."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Expected Event Msg ",DS_PARMS_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_PARMS_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.15.2: Send the Set Filter Parameters command with an invalid "
write ";  parameter index. "
write ";*********************************************************************"
;; Send the Set File Command
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PARMS_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the Set Filter Type Command
/$SC_$CPU_DS_SetFilterParams MessageID=x'ABCD' ParamIndex=DS_FILTERS_PER_PACKET  N_Value=newNVal X_Value=newXVal O_Value=newOVal

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Filter File command failed as expected."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Filter File command did not increment CMDEC."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Expected Event Msg ",DS_PARMS_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_PARMS_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.15.3: Send the Set Filter Parameters command with an N value "
write ";  greater than the X value. "
write ";*********************************************************************"
;; Send the Set File Command
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PARMS_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the Set Filter Type Command
/$SC_$CPU_DS_SetFilterParams MessageID=seqFileMsgID ParamIndex=0  N_Value=5 X_Value=3 O_Value=0

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Filter File command failed as expected."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Filter File command did not increment CMDEC."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Expected Event Msg ",DS_PARMS_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_PARMS_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.15.4: Send the Set Filter Parameters command with an O value "
write ";  greater than the X value. "
write ";*********************************************************************"
;; Send the Set File Command
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_PARMS_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the Set Filter Type Command
/$SC_$CPU_DS_SetFilterParams MessageID=seqFileMsgID ParamIndex=0  N_Value=1 X_Value=3 O_Value=4

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - DS Set Filter File command failed as expected."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - DS Set Filter File command did not increment CMDEC."
  ut_setrequirements DS_1005, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005) - Expected Event Msg ",DS_PARMS_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
else
  write "<!> Failed (1005) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_PARMS_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 3.16: Determine if the Filtering Algorithm works properly for "
write ";  Time based filtering. "
write ";*********************************************************************"
write ";  Step 3.16.1: Determine if the Filter Type is set to Time based. "
write ";*********************************************************************"
;; Get the Filter Table Type from the existing entry
existingFilterType = $SC_$CPU_DS_PF_TBL[filterEntry].FilterParams[0].FilterType

;; Set the Filter Type to Time based filtering
if (existingFilterType = DS_BY_COUNT) then
  ;; Setup to capture the event
  ut_setupevents "$SC", "$CPU", {DSAppName}, DS_FTYPE_CMD_EID, "DEBUG", 1

  cmdCtr = $SC_$CPU_DS_CMDPC + 1
  ;; Send the Set Filter Type Command
  /$SC_$CPU_DS_SetFilterType MessageID=seqFileMsgID, ParamIndex=0, FilterType=DS_BY_TIME

  ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
  if (UT_TW_Status = UT_Success) then
    write "<*> Passed (1004;5009) - DS Set Filter Type command sent properly."
    ut_setrequirements DS_1004, "P"
    ut_setrequirements DS_5009, "P"
  else
    write "<!> Failed (1004;5009) - DS Set Filter Type command did not increment CMDPC."
    ut_setrequirements DS_1004, "F"
    ut_setrequirements DS_5009, "F"
  endif

  ;; Check for the event message
  ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
  if (UT_TW_Status = UT_Success) then
    write "<*> Passed (1004;5009) - Expected Event Msg ",DS_FTYPE_CMD_EID," rcv'd."
    ut_setrequirements DS_1004, "P"
    ut_setrequirements DS_5009, "P"
  else
    write "<!> Failed (1004;5009) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_FTYPE_CMD_EID,"."
    ut_setrequirements DS_1004, "F"
    ut_setrequirements DS_5009, "F"
  endif
endif

wait 5

write ";***********************************************************************"
write ";  Step 3.16.2: Send the TST_DS command to send a message to DS using "
write ";  the MessageID used above. Send 'X' messages. The HK should contain "
write ";  information about the file opened. "
write ";***********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_MSG1_SENT_INF_EID, "INFO", 1

expPassedPkts = $SC_$CPU_DS_PassedPktCnt + newNVal
local subSecs = 0
local subSecsInc = x'10000000'

;; Send the command
for i = 1 to newXVal do
  ;; Set the sub-seconds to send
  subSecs = subSecs + subSecsInc

  ;; Set the data pattern to send
  dataPattern = i
  /$SC_$CPU_TST_DS_SENDTIMEMESSAGE MsgID=seqFileMsgID MsgType=1 Pattern=dataPattern Seconds=0 SubSecs=subSecs
  wait 5
enddo

;; Check for the event message
if ($SC_$CPU_find_event[1].num_found_messages = newXVal) then
  write "<*> Passed - Expected Event Msgs ",TST_DS_MSG1_SENT_INF_EID," rcv'd."
else
  write "<!> Failed - Expected ",newXVal," event messages. Rcv'd ",$SC_$CPU_find_event[1].num_found_messages,"."
endif

;; Sent the Get File Info command to check the File Status
cmdCtr = $SC_$CPU_DS_CMDPC + 1

/$SC_$CPU_DS_GetFileInfo

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}

;; Verify that the Passed Packet counter incremented properly
if (expPassedPkts = $SC_$CPU_DS_PassedPktCnt) then
  write "<*> Passed (2000) - Passed packet counter incremented properly."
  ut_setrequirements DS_2000, "P"
else
  write "<!> Failed (2000) - Passed packet counter not set properly. Expected '",expPassedPkts,"'. Count = '",$SC_$CPU_DS_PassedPktCnt,"'."
  ut_setrequirements DS_2000, "F"
endif

;; Verify that the file was created
fileName = ""
if ($SC_$CPU_DS_FileState[newFileEntry].FileName <> "") then
  fileName = $SC_$CPU_DS_FileState[newFileEntry].FileName
  write "<*> Passed (2002.1;2003;2003.2;3000) - A file was created with name '",fileName,"'"
  ut_setrequirements DS_20021, "P"
  ut_setrequirements DS_2003, "A"
  ut_setrequirements DS_20032, "A"
  ut_setrequirements DS_3000, "P"
else
  write "<!> Failed (2002.1;2003;2003.2;3000) - HK does not contain the expected file state information."
  ut_setrequirements DS_20021, "F"
  ut_setrequirements DS_2003, "F"
  ut_setrequirements DS_20032, "F"
  ut_setrequirements DS_3000, "F"
endif

write ";***********************************************************************"
write ";  Step 3.16.3: Close and download the file created above. "
write ";***********************************************************************"
;; Close the above file
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_CLOSE_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the Close commands
/$SC_$CPU_DS_CloseFile FileIndex=newFileEntry

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

;; Download the file
if (fileName <> "") then
  ut_setrequirements DS_2001, "A"
  write "==> Downloading '",fileName,"'"
  slashLoc = %locate(fileName,"/")
  while (slashLoc <> 0) do
    fileName = %substring(fileName,slashLoc+1,%length(fileName))
    slashLoc = %locate(fileName,"/")
  enddo
  write "==> FileName Only = '",fileName,"'"
  ;; Need to parse the filename out of the variable above
  ;; before using it in the ftp_file call below
  s ftp_file(ramDir, fileName, fileName, hostCPU, "G")
else
  ut_setrequirements DS_2001, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.0: Packet Filter Table Add command test "
write ";*********************************************************************"
write ";  Step 4.1: Send the Add Messge ID command "
write ";*********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_ADD_MID_CMD_EID, "DEBUG", 1

cmdCtr = $SC_$CPU_DS_CMDPC + 1
;; Send the command
/$SC_$CPU_DS_AddMID MessageID=addMID

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004;5016) - DS Add MID command sent properly."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5016, "P"
else
  write "<!> Failed (1004;5016) - DS Add MID command did not increment CMDPC."
  ut_setrequirements DS_1004, "F"
  ut_setrequirements DS_5016, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1004) - Expected Event Msg ",DS_ADD_MID_CMD_EID," rcv'd."
  ut_setrequirements DS_1004, "P"
  ut_setrequirements DS_5002, "P"
else
  write "<!> Failed (1004) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_ADD_MID_CMD_EID,"."
  ut_setrequirements DS_1004, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.2: Dump the table and verify that the MID used above exists "
write ";*********************************************************************"
;; Dump the Packet Filter Table
s get_tbl_to_cvt (ramDir,filterTblName,"A","$cpu_filtertbl42",hostCPU,filterTblPktId)

local foundMID = FALSE
;; Verify the table contains the added MessgeID
for i = 0 to DS_PACKETS_IN_FILTER_TABLE-1 do
  if ($SC_$CPU_DS_PF_TBL[i].MessageID = addMID) then
    write "<*> Passed (5016) - Found Message ID ",%hex(addMID,4)," at entry ",i," of the table."
    foundMID = TRUE
    break
  endif
enddo

if (foundMID = FALSE) then
  write "<!> Failed (5016) - Message ID ",%hex(addMID,4)," was not added to the table."
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.3: Try to add an invalid Message ID to the Filter table "
write ";*********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_ADD_MID_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the command
/$SC_$CPU_DS_AddMID MessageID=0

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;5016.1) - DS Add MID command failed as expected."
  ut_setrequirements DS_1005, "P"
  ut_setrequirements DS_50161, "P"
else
  write "<!> Failed (1005;5016.1) - DS Add MID command did not increment CMDEC."
  ut_setrequirements DS_1005, "F"
  ut_setrequirements DS_50161, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;5016.1) - Expected Event Msg ",DS_ADD_MID_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
  ut_setrequirements DS_50161, "P"
else
  write "<!> Failed (1005;5016.1) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_ADD_MID_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
  ut_setrequirements DS_50161, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.4: Try to add a Message ID that is already contained in the "
write ";  Filter table "
write ";*********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_ADD_MID_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the command
/$SC_$CPU_DS_AddMID MessageID=seqMsgID 

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;5016.3) - DS Add MID command failed as expected."
  ut_setrequirements DS_1005, "P"
  ut_setrequirements DS_50163, "P"
else
  write "<!> Failed (1005;5016.3) - DS Add MID command did not increment CMDPC."
  ut_setrequirements DS_1005, "F"
  ut_setrequirements DS_50163, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;5016.3) - Expected Event Msg ",DS_ADD_MID_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
  ut_setrequirements DS_50163, "P"
else
  write "<!> Failed (1005;5016.3) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_ADD_MID_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
  ut_setrequirements DS_50163, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.5: Try to add a message ID when the table is not loaded "
write ";*********************************************************************"
write ";  Step 4.5.1: Remove the default Packet Filter Table "
write ";*********************************************************************"
;; Remove the default packet filter table
s ftp_file("CF:0/apps", "na", filterFileName, hostCPU, "R")

wait 5

write ";*********************************************************************"
write ";  Step 4.5.2: Stop and restart the DS and TST_DS apps "
write ";*********************************************************************"
/$SC_$CPU_ES_DELETEAPP Application="TST_DS"
wait 2
/$SC_$CPU_ES_DELETEAPP Application=DSAppName
wait 10

;; Restart the apps
s $sc_$cpu_ds_start_apps("4.5.2")
wait 5

write ";*********************************************************************"
write ";  Step 4.5.3: Try to add a Message ID to an unloaded Filter table "
write ";*********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_ADD_MID_CMD_ERR_EID, "ERROR", 1

errcnt = $SC_$CPU_DS_CMDEC + 1
;; Send the command
/$SC_$CPU_DS_AddMID MessageID=seqMsgID 

ut_tlmwait $SC_$CPU_DS_CMDEC, {errcnt}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;5016.2) - DS Add MID command failed as expected."
  ut_setrequirements DS_1005, "P"
  ut_setrequirements DS_50162, "P"
else
  write "<!> Failed (1005;5016.2) - DS Add MID command did not increment CMDPC."
  ut_setrequirements DS_1005, "F"
  ut_setrequirements DS_50162, "F"
endif

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;5016.2) - Expected Event Msg ",DS_ADD_MID_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
  ut_setrequirements DS_50162, "P"
else
  write "<!> Failed (1005;5016.2) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_ADD_MID_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
  ut_setrequirements DS_50162, "F"
endif

wait 5

write ";*********************************************************************"
write ";  Step 4.6: Try to add a message ID when the table is full. "
write ";*********************************************************************"
write ";  Step 4.6.1: Create and upload a full Packet Filter Table "
write ";*********************************************************************"
;; Create a full Packet Filter Table
s $sc_$cpu_ds_tbl5

;; Upload the new filter table
s ftp_file("CF:0/apps", "ds_fullfilter.tbl", filterFileName, hostCPU, "P")

wait 5

write ";*********************************************************************"
write ";  Step 4.6.2: Stop and restart the DS and TST_DS apps "
write ";*********************************************************************"
/$SC_$CPU_ES_DELETEAPP Application="TST_DS"
wait 2
/$SC_$CPU_ES_DELETEAPP Application=DSAppName
wait 10

;; Restart the apps
s $sc_$cpu_ds_start_apps("4.6.2")
wait 5

write ";*********************************************************************"
write ";  Step 4.6.3: Try to add a Message ID to a full Packet Filter table "
write ";*********************************************************************"
;; Setup the event capture
ut_setupevents "$SC", "$CPU", {DSAppName}, DS_ADD_MID_CMD_ERR_EID, "ERROR", 1

;; Send the command
/$SC_$CPU_DS_AddMID MessageID=fullMID 

;; Check for the event message
ut_tlmwait $SC_$CPU_find_event[1].num_found_messages, 1
if (UT_TW_Status = UT_Success) then
  write "<*> Passed (1005;5016.4) - Expected Event Msg ",DS_ADD_MID_CMD_ERR_EID," rcv'd."
  ut_setrequirements DS_1005, "P"
  ut_setrequirements DS_50164, "P"
else
  write "<!> Failed (1005;5016.4) - Event message ", $SC_$CPU_evs_eventid," rcv'd. Expected Event Msg ",DS_ADD_MID_CMD_ERR_EID,"."
  ut_setrequirements DS_1005, "F"
  ut_setrequirements DS_50164, "F"
endif

wait 5

step5_0:
write ";*********************************************************************"
write ";  Step 5.0: Clean-up - Send the Processor Reset command.             "
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
write ";  End procedure $SC_$CPU_ds_filter"
write ";*********************************************************************"
ENDPROC
