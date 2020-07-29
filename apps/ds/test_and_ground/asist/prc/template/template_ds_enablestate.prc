PROC $sc_$cpu_ds_enablestate
;*******************************************************************************
;  Test Name:  ds_resetnocds
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	This test verifies that the CFS Data Storage (DS) application sets its
;	its state according to the DS_DEF_ENABLE_STATE and DS_CDS_ENABLE_STATE
;	configuration settings when any reset occurs (Application Reset,
;	Processor Reset, or Power-On Reset). 
;
;  Requirements Tested
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
;    cDS9000	Upon initialization of the DS Application, DS shall initialize
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
;    cDS9003    Upon a cFE Processor Reset or DS Application Reset, DS shall
;               restore the following if the associated <PLATFORM_DEFINED>
;               Preserve Flag is set to TRUE:
;                       a. Packet Filter Table
;                       b. Destination File Table
;                       c. File Sequence number for all Destination File Sets
;                       d. Packet Processing State (ENABLED or DISABLED)
;    cDS9004	Upon a cFE Processor Reset or DS Application Reset, DS shall
;		restore the following if the associated <PLATFORM_DEFINED> 
;		Preserve State set to FALSE:
;			a. File Sequence Counters
;                       b. <PLATFORM_DEFINED> Packet Processing State
;    cDS9008    Upon cFE Power-On, DS shall set the Packet Processing State to
;               the <PLATFORM_DEFINED> state (ENABLED or DISABLED).
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
;       02/06/17        Walt Moleski    Added this test to verify the settings
;                                       of the STATE configuration parameters.
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

#define DS_8000		0
#define DS_9000		1
#define DS_9003		2
#define DS_9004		3
#define DS_9008		4

global ut_req_array_size = 4
global ut_requirement[0 .. ut_req_array_size]

for i = 0 to ut_req_array_size DO
  ut_requirement[i] = "U"
enddo

;**********************************************************************
; Set the local values
;**********************************************************************
local cfe_requirements[0 .. ut_req_array_size] = ["DS_8000", "DS_9000", "DS_9003", "DS_9004", "DS_9008" ]

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

write "==> Default Destination File Table filename = '",destFileName,"'"

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

write ";***********************************************************************"
write ";  Step 1.4:  Start the Data Storage (DS) and Test Applications.     "
write ";***********************************************************************"
s $sc_$cpu_ds_start_apps("1.4")
wait 5

;; Verify the Housekeeping Packet is being generated
local hkPktId = "p0B8"

;; Wait for the sequencecount to increment twice
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

;; Verify DS_DEF_ENABLE_STATE
if (DS_DEF_ENABLE_STATE = 1) then
  if (p@$SC_$CPU_DS_AppEnaState = "Enabled") then
    write "<*> Passed (9008) - DS Application State is enabled as expected."
    ut_setrequirements DS_9008, "P"
  else
    write "<!> Failed (9008) - DS Application State is NOT enabled as expected.  State = '",p@$SC_$CPU_DS_AppEnaState,"'"
    ut_setrequirements DS_9008, "F"
  endif
else
  if (p@$SC_$CPU_DS_AppEnaState = "Disabled") then
    write "<*> Passed (9008) - DS Application State is disabled as expected."
    ut_setrequirements DS_9008, "P"
  else
    write "<!> Failed (9008) - DS Application State is NOT disabled as expected.  State = '",p@$SC_$CPU_DS_AppEnaState,"'"
    ut_setrequirements DS_9008, "F"
  endif
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

wait 5

write ";***********************************************************************"
write ";  Step 2.0: State Enable Tests."
write ";***********************************************************************"
write ";  Step 2.1: Change the DS application state to the opposite state."
write ";***********************************************************************"
local expectedState
cmdCtr = $SC_$CPU_DS_CMDPC + 1
if (p@$SC_$CPU_DS_AppEnaState = "Enabled") then
  expectedState = "Disabled"
  ;; Send the command
  /$SC_$CPU_DS_Disable
else
  expectedState = "Enabled"
  ;; Send the command
  /$SC_$CPU_DS_Enable
endif

ut_tlmwait $SC_$CPU_DS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - DS Application State command sent properly."
else
  write "<!> Failed - DS Application State command did not increment CMDPC."
endif

wait 5

write ";*********************************************************************"
write ";  Step 2.2: Perform a Processor Reset. "
write ";*********************************************************************"
/$SC_$CPU_ES_PROCESSORRESET
wait 10

close_data_center
wait 60

cfe_startup {hostCPU}
wait 5

write ";***********************************************************************"
write ";  Step 2.3: Start the Data Storage (DS) and Test Applications.     "
write ";***********************************************************************"
s $sc_$cpu_ds_start_apps("2.3")
wait 5

;; Check the DS state based upon the DS_CDS_ENABLE_STATE parameter 
;; If this parameter is set to 1, the DS Application State is preserved.
;; Otherwise, the Default state should be set
if (DS_CDS_ENABLE_STATE = 1) then
  if (p@$SC_$CPU_DS_AppEnaState = expectedState) then
    write "<*> Passed (9004) - DS Application State is set as expected."
    ut_setrequirements DS_9004, "P"
  else
    write "<!> Failed (9004) - DS Application State is NOT as expected. State = '",p@$SC_$CPU_DS_AppEnaState,"'. Expected '",expectedState,"'"
    ut_setrequirements DS_9004, "F"
  endif
else
  if (DS_DEF_ENABLE_STATE = 1) then
    if (p@$SC_$CPU_DS_AppEnaState = "Enabled") then
      write "<*> Passed (9003;9008) - DS Application State is enabled as expected."
      ut_setrequirements DS_9003, "P"
      ut_setrequirements DS_9008, "P"
    else
      write "<!> Failed (9003;9008) - DS Application State is NOT enabled as expected. State = '",p@$SC_$CPU_DS_AppEnaState,"'"
      ut_setrequirements DS_9003, "F"
      ut_setrequirements DS_9008, "F"
    endif
  else
    if (p@$SC_$CPU_DS_AppEnaState = "Disabled") then
     write "<*> Passed (9003;9008) - DS Application State is disabled as expected."
      ut_setrequirements DS_9003, "P"
      ut_setrequirements DS_9008, "P"
    else
      write "<!> Failed (9003;9008) - DS Application State is NOT disabled as expected. State = '",p@$SC_$CPU_DS_AppEnaState,"'"
      ut_setrequirements DS_9003, "F"
      ut_setrequirements DS_9008, "F"
    endif
  endif
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.4: Enable DEBUG Event Messages "
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

write ";*********************************************************************"
write ";  Step 2.5: Restart the DS and TST_DS Applications. "
write ";*********************************************************************"
write ";  Step 2.5.1: Stop the DS and TST_DS Applications. "
write ";*********************************************************************"
local cmdCtr = $SC_$CPU_ES_CMDPC + 2

/$SC_$CPU_ES_DELETEAPP Application="TST_DS"
wait 4
/$SC_$CPU_ES_DELETEAPP Application=DSAppName
wait 4

ut_tlmwait $SC_$CPU_ES_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - DS and TST_DS stop app commands sent properly."
else
  write "<!> Failed - Stop App commands did not increment CMDPC."
endif

wait 5

write ";*********************************************************************"
write ";  Step 2.5.2: Start the DS and TST_DS Applications. "
write ";*********************************************************************"
s $sc_$cpu_ds_start_apps("2.5.2")
wait 5

write ";***********************************************************************"
write ";  Step 2.6: Enable DEBUG Event Messages "
write ";***********************************************************************"
local cmdCtr = $SC_$CPU_EVS_CMDPC + 2

;; Enable DEBUG events for the DS application ONLY
/$SC_$CPU_EVS_EnaAppEVTType Application=DSAppName DEBUG
wait 2
/$SC_$CPU_EVS_EnaAppEVTType Application="CFE_TBL" DEBUG
  
ut_tlmwait $SC_$CPU_EVS_CMDPC, {cmdCtr}
if (UT_TW_Status = UT_Success) then
  write "<*> Passed - Enable Debug events command sent properly."
else
  write "<!> Failed - Enable Debug events command."
endif

wait 5

write ";***********************************************************************"
write ";  Step 2.7: Check the Enable State after reset"
write ";***********************************************************************"
;; Check the DS state based upon the DS_CDS_ENABLE_STATE parameter 
;; If this parameter is set to 1, the DS Application State is preserved.
;; Otherwise, the Default state should be set
if (DS_CDS_ENABLE_STATE = 1) then
  if (p@$SC_$CPU_DS_AppEnaState = expectedState) then
    write "<*> Passed (9004) - DS Application State is set as expected."
    ut_setrequirements DS_9004, "P"
  else
    write "<!> Failed (9004) - DS Application State is NOT as expected. State = '",p@$SC_$CPU_DS_AppEnaState,"'. Expected '",expectedState,"'"
    ut_setrequirements DS_9004, "F"
  endif
else
  if (DS_DEF_ENABLE_STATE = 1) then
    if (p@$SC_$CPU_DS_AppEnaState = "Enabled") then
      write "<*> Passed (9003;9008) - DS Application State is enabled as expected."
      ut_setrequirements DS_9003, "P"
      ut_setrequirements DS_9008, "P"
    else
      write "<!> Failed (9003;9008) - DS Application State is NOT enabled as expected. State = '",p@$SC_$CPU_DS_AppEnaState,"'"
      ut_setrequirements DS_9003, "F"
      ut_setrequirements DS_9008, "F"
    endif
  else
    if (p@$SC_$CPU_DS_AppEnaState = "Disabled") then
     write "<*> Passed (9003;9008) - DS Application State is disabled as expected."
      ut_setrequirements DS_9003, "P"
      ut_setrequirements DS_9008, "P"
    else
      write "<!> Failed (9003;9008) - DS Application State is NOT disabled as expected. State = '",p@$SC_$CPU_DS_AppEnaState,"'"
      ut_setrequirements DS_9003, "F"
      ut_setrequirements DS_9008, "F"
    endif
  endif
endif

write ";*********************************************************************"
write ";  Step 6.0: Clean-up - Send the Reset command.             "
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

drop ut_requirement ; needed to clear global variables
drop ut_req_array_size ; needed to clear global variables

write ";*********************************************************************"
write ";  End procedure $SC_$CPU_ds_resetnocds"
write ";*********************************************************************"
ENDPROC
