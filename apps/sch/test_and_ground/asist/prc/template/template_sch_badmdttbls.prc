PROC $sc_$cpu_sch_badmdttbls

;*******************************************************************************
;  Name:  sch_badmdttbls
;
;  Test Description
;	This procedure populates the CFS Scheduler message definition table and
;	generates a table load image for that table. This table image can be
;	loaded via the CFE_TBL_LOAD command.
;
;  Change History
;	Date	   Name		Description
;	06/29/11   W. Moleski	Original Procedure.
;	06/13/17   W. Moleski	Updated to use CPU1 for commanding and added a
;                               hostCPU variable for the utility procs to
;                               connect to the proper host.
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
#include "sch_msgdefs.h"

;**********************************************************************
; Define local variables
;**********************************************************************
LOCAL tblAppId, tblPktId
LOCAL entry
LOCAL entry2
LOCAL i
local SCHAppName = "SCH"
local MSGDefTblName = SCHAppName & ".MSG_DEFS"
local ramDir = "RAM:0"
local hostCPU = "$CPU"

;;; Set the pkt and app IDs for the tables based upon the cpu being used
;;; Right now, the pktIDs are not used
;;; CPU1 is the default
tblAppId = "0FB4"
tblPktId = 4020
local unusedMID = x'1897'

write ";*********************************************************************"
write ";  Create & upload the Message Definition Table file."
write ";  This is an invalid MDT with 8 bad entries."
write ";********************************************************************"

; Entry 0 [Bad] 2007.1
entry = 0
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = x'FF00'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 1 [Bad] 2007.2
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = x'FFFF'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 2 [Bad]
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = x'0001'
for entry2=2 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 3 [Bad]
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = x'0000'
for entry2=2 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 4 [Bad]
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = x'2001'
for entry2=2 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 5 [Bad] - 2007.4
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = x'1848'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0080'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
enddo

; Entry 6 [Bad] - 2007.1
entry = entry + 1
entry2 = 1
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = unusedMID
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+1] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+2] = x'0031'
$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2+3] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 5
enddo

local maxMsgs = SCH_MAX_MESSAGES - 1
local endMnemonic = "$SC_$CPU_SCH_DefaultMessageTable[" & maxMsgs & "]"

;zero out the rest of the table
for entry=7 to maxMsgs do
   for entry2=1 to SCH_MAX_MSG_WORDS do
	$SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[entry2] = 0
   enddo
   $SC_$CPU_SCH_DefaultMessageTable[entry].MessageBuffer[1] = unusedMID
enddo

;; Create the Table Load file
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Message Definition Table Req_2007","mdt_20071.tbl",MSGDefTblName,"$SC_$CPU_SCH_DefaultMessageTable[0]",endMnemonic)

;; Fix the bad table entry
$SC_$CPU_SCH_DefaultMessageTable[0].MessageBuffer[1] = x'1848'
$SC_$CPU_SCH_DefaultMessageTable[0].MessageBuffer[2] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[0].MessageBuffer[3] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[0].MessageBuffer[4] = x'0000'
$SC_$CPU_SCH_DefaultMessageTable[1].MessageBuffer[1] = x'1848'
$SC_$CPU_SCH_DefaultMessageTable[1].MessageBuffer[2] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[1].MessageBuffer[3] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[1].MessageBuffer[4] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[0].MessageBuffer[entry2] = 0
   $SC_$CPU_SCH_DefaultMessageTable[1].MessageBuffer[entry2] = 0
enddo

;; Create the Table Load file
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Message Definition Table Req_2007","mdt_20072.tbl",MSGDefTblName,"$SC_$CPU_SCH_DefaultMessageTable[0]",endMnemonic)

;; Fix the bad table entry
$SC_$CPU_SCH_DefaultMessageTable[2].MessageBuffer[1] = x'1848'
$SC_$CPU_SCH_DefaultMessageTable[2].MessageBuffer[2] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[2].MessageBuffer[3] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[2].MessageBuffer[4] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[2].MessageBuffer[entry2] = 0
enddo

;; Create the Table Load file
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Message Definition Table Req_2007","mdt_20073.tbl",MSGDefTblName,"$SC_$CPU_SCH_DefaultMessageTable[0]",endMnemonic)

;; Fix the bad table entry
$SC_$CPU_SCH_DefaultMessageTable[3].MessageBuffer[1] = x'1848'
$SC_$CPU_SCH_DefaultMessageTable[3].MessageBuffer[2] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[3].MessageBuffer[3] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[3].MessageBuffer[4] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[3].MessageBuffer[entry2] = 0
enddo

;; Create the Table Load file
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Message Definition Table Req_2007","mdt_20074.tbl",MSGDefTblName,"$SC_$CPU_SCH_DefaultMessageTable[0]",endMnemonic)

;; Fix the bad table entries
$SC_$CPU_SCH_DefaultMessageTable[4].MessageBuffer[1] = x'1848'
$SC_$CPU_SCH_DefaultMessageTable[4].MessageBuffer[2] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[4].MessageBuffer[3] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[4].MessageBuffer[4] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[4].MessageBuffer[entry2] = 0
enddo

;; Create the Table Load file
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Message Definition Table Req_2007","mdt_20075.tbl",MSGDefTblName,"$SC_$CPU_SCH_DefaultMessageTable[0]",endMnemonic)

;; Fix the bad table entries
$SC_$CPU_SCH_DefaultMessageTable[5].MessageBuffer[1] = x'1848'
$SC_$CPU_SCH_DefaultMessageTable[5].MessageBuffer[2] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[5].MessageBuffer[3] = x'0002'
$SC_$CPU_SCH_DefaultMessageTable[5].MessageBuffer[4] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[5].MessageBuffer[entry2] = 0
enddo

;; Create the Table Load file
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Message Definition Table Req_2007","mdt_20076.tbl",MSGDefTblName,"$SC_$CPU_SCH_DefaultMessageTable[0]",endMnemonic)

;; Fix the bad table entries
$SC_$CPU_SCH_DefaultMessageTable[5].MessageBuffer[1] = x'1848'
$SC_$CPU_SCH_DefaultMessageTable[5].MessageBuffer[2] = x'C000'
$SC_$CPU_SCH_DefaultMessageTable[5].MessageBuffer[3] = x'0001'
$SC_$CPU_SCH_DefaultMessageTable[5].MessageBuffer[4] = x'0000'
for entry2=5 to SCH_MAX_MSG_WORDS do
   $SC_$CPU_SCH_DefaultMessageTable[5].MessageBuffer[entry2] = 0
enddo

;; Create the Table Load file
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Message Definition Table Req_2007","mdt_20077.tbl",MSGDefTblName,"$SC_$CPU_SCH_DefaultMessageTable[0]",endMnemonic)

%liv(log_procedure) = logging
ENDPROC
