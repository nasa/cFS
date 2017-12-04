PROC $sc_$cpu_sch_mdtloadfile
;*******************************************************************************
;  Name:  sch_mdtloadfile
;
;  Test Description
;	This procedure populates the CFS Scheduler message definition table and
;	generates a table load image for that table. This table image can be
;	loaded via the CFE_TBL_LOAD command.
;
;  Change History
;	Date	   Name		Description
;	01/06/09   E. Uzo-Okoro	Original Procedure.
;	03/15/10   W. Moleski	Updated to handle 0-based arrays for the MDT
;	06/17/11   W. Moleski	Added variables for App and table names as well
;				as the upload dir and end mnemonic names
;	08/07/12   W. Moleski	Updated Entry #15 to use the EVS_NOOP cmd code
;       06/13/17   W. Moleski	Updated to use CPU1 for commanding and added a
;				hostCPU variable for the utility procs to
;				connect to the proper host.
;
;  Arguments
;	None.
;
;  Procedures Called
;	Name			Description
;
;**********************************************************************

local logging = %liv(log_procedure)
%liv(log_procedure) = FALSE

#include "cfe_platform_cfg.h"
#include "ut_statusdefs.h"
#include "sch_platform_cfg.h"
#include "sch_events.h"
#include "sch_msgdefs.h"

;**********************************************************************
; Define local variables
;**********************************************************************
LOCAL tblAppId, tblPktId
LOCAL entry
LOCAL entry2
LOCAL i
local entryMID[0..17]
local unusedMID
local SCHAppName = "SCH"
local MSGDefTblName = SCHAppName & ".MSG_DEFS"
local defTblDir = "CF:0/apps"
local hostCPU = "$CPU"

;;; Set the pkt and app IDs for the tables based upon the cpu being used
;;; Right now, the pktIDs are not used
;;; CPU1 is the default
tblAppId = "0FB4"
tblPktId = 4020
unusedMID = x'1897'
entryMID[0] = x'1808'
entryMID[1] = x'1809'
entryMID[2] = x'180B'
entryMID[3] = x'180C'
entryMID[4] = x'180D'
entryMID[5] = x'1881'
entryMID[6] = x'1885'
entryMID[7] = x'1896'
entryMID[8] = x'1934'
entryMID[9] = x'1880'
entryMID[10] = x'1895'
entryMID[11] = x'1933'
entryMID[12] = x'1884'
entryMID[13] = x'1806'
entryMID[14] = x'1801'
entryMID[15] = x'1803'
entryMID[16] = x'1804'
entryMID[17] = x'1805'

write ";********************************************************************"
write ";  Create & upload the Message Definition Table file."
write ";********************************************************************"

; Entry 0 - Unused
$SC_$CPU_SCH_DefaultMessageTable[0].MessageBuffer[1] = unusedMID 
for entry2=2 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[0].MessageBuffer[entry2] = 0
enddo

; Entry 1 - ES_SEND_HK
entry = 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[0]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 2 - EVS_SEND_HK
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[1]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 3 - SB_SEND_HK
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[2]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 4 - TBL_SEND_HK
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[3]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 5 - TIME_SEND_HK
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[4]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 6 - TO_SEND_HK
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[5]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 7 - CI_SEND_HK
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[6]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 8 - SCH_SEND_HK
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[7]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 9 - TST_SCH_SEND_HK
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[8]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 10 - TO_NOOP
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[9]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 11 - SCH_NOOP
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[10]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 12 - TST_SCH_NOOP
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[11]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

;; Entry 13 - CI_NOOP
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[12]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

;; Entry 14 - ES_NOOP
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[13]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

;; Entry 15 - EVS_NOOP
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[14]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

;; Entry 16 - SB_NOOP
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[15]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

;; Entry 17 - TBL_NOOP
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[16]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

;; Entry 18 - TIME_NOOP
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = entryMID[17]
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

;;zero out the rest of the table
local maxMsgs = SCH_MAX_MESSAGES - 1

for entry=18 to maxMsgs do
   for entry2=1 to SCH_MAX_MSG_WORDS do
	$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
   enddo
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[1] = unusedMID 
enddo

local endMnemonic = "$SC_$CPU_SCH_DefaultMessageTable[" & maxMsgs & "]"

;; Create the Table Load file
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Message Definition Table 1","sch_def_msgtbl.tbl",MSGDefTblName,"$SC_$CPU_SCH_DefaultMessageTable[0]",endMnemonic)

%liv(log_procedure) = logging

;; Parse the filename configuration parameter for the default table filename
local tableFileName = SCH_MESSAGE_FILENAME
local slashLoc = %locate(tableFileName,"/")

;; loop until all slashes are found
while (slashLoc <> 0) do
  tableFileName = %substring(tableFileName,slashLoc+1,%length(tableFileName))
  slashLoc = %locate(tableFileName,"/")
enddo

write "==> Default Schedule Message Table filename = '",tableFileName,"'"

s ftp_file(defTblDir, "sch_def_msgtbl.tbl", tableFileName, hostCPU, "P")

ENDPROC
