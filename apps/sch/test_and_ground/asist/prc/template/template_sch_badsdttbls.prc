PROC $sc_$cpu_sch_badsdttbls
;*******************************************************************************
;  Name:  sch_badsdttbls
;
;  Test Description
;	This procedure populates the CFS Scheduler Definition table and creates
;	11 table load images. Each table image induces the 11 Validation errors 
;	that the SCH application is required to detect. Each image can be loaded
;	via the CFE_TBL_LOAD command.
;
;  Change History
;	Date	   Name		Description
;	06/29/11   W. Moleski	Original implementation
;       06/13/17   W. Moleski   Updated to use CPU1 for commanding and added a
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
LOCAL i
local SCHAppName = "SCH"
local SCHDefTblName = SCHAppName & ".SCHED_DEF"
local defTblDir = "RAM:0"
local hostCPU = "$CPU"

;;; Set the pkt and app IDs for the tables based upon the cpu being used
;;; Right now, the pktIDs are not used
;;; CPU1 is the default
tblAppId = "0FB5"
tblPktId = 4021

write ";*********************************************************************"
write ";  Create an invalid SDT image for each error SCH is required to detect."
write ";********************************************************************"
;; States are 0=Unused; 2=Disabled; 1=Enabled;

; Entry 0 [Bad] 2006.1
entry = 0 
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_UNUSED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 5

; Entry 1 [Bad] 2006.2
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 6

; Entry 2
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 4
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 3
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 1

; Entry 3 [Bad] 2006.2
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 3
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 4
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 5

; Entry 4 [Bad] 2006.3
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 6
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 14
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 5
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 1

; Entry 5 [Bad] 2006.3
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 7
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 10
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 6

; Entry 6 [Bad] 2006.4
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 700
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 5

; Entry 7 [Bad] 2006.5
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = x'FE'
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 7
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 5

; Entry 8 [Bad] 2006.5
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 8
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 5

; Entry 9 [Bad] 2006.4
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 700
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 5

; Entry 10 [Bad] MessageIndex = 0
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 5

; Entry 11 [Bad] MessageIndex = 0
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 5

local maxEntries = SCH_TABLE_ENTRIES - 1

;zero out the rest of the table
for entry=12 to maxEntries do
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 0
enddo

local endMnemonic = "$SC_$CPU_SCH_DefaultScheduleTable[" & maxEntries & "]"

;; Create the Table Load file for 2006.1
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Schedule Definition Table Req_2006", "sdt_20061.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

;; Fix the bad table entry
$SC_$CPU_SCH_DefaultScheduleTable[0].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[0].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[0].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[0].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[0].MessageIndex = 1
$SC_$CPU_SCH_DefaultScheduleTable[0].GroupData = 5

;; Create the Table Load file for 2006.2 - Enabled
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Schedule Definition Table Req_2006", "sdt_20062E.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

;; Fix the bad table entry
$SC_$CPU_SCH_DefaultScheduleTable[1].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[1].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[1].SE_Frequency = 4
$SC_$CPU_SCH_DefaultScheduleTable[1].Remainder = 2
$SC_$CPU_SCH_DefaultScheduleTable[1].MessageIndex = 2
$SC_$CPU_SCH_DefaultScheduleTable[1].GroupData = 6

;; Create the Table Load file for 2006.2 - Disabled
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Schedule Definition Table Req_2006", "sdt_20062D.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

;; Fix the bad table entry
$SC_$CPU_SCH_DefaultScheduleTable[3].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[3].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[3].SE_Frequency = 4
$SC_$CPU_SCH_DefaultScheduleTable[3].Remainder = 3
$SC_$CPU_SCH_DefaultScheduleTable[3].MessageIndex = 4
$SC_$CPU_SCH_DefaultScheduleTable[3].GroupData = 5

;; Create the Table Load file for 2006.3 - Disabled
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Schedule Definition Table Req_2006", "sdt_20063D.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

;; Fix the bad table entry
$SC_$CPU_SCH_DefaultScheduleTable[4].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[4].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[4].SE_Frequency = 6
$SC_$CPU_SCH_DefaultScheduleTable[4].Remainder = 4
$SC_$CPU_SCH_DefaultScheduleTable[4].MessageIndex = 5
$SC_$CPU_SCH_DefaultScheduleTable[4].GroupData = 1

;; Create the Table Load file for 2006.3 - Enabled
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Schedule Definition Table Req_2006", "sdt_20063E.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

;; Fix the bad table entry
$SC_$CPU_SCH_DefaultScheduleTable[5].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[5].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[5].SE_Frequency = 7
$SC_$CPU_SCH_DefaultScheduleTable[5].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[5].MessageIndex = 1
$SC_$CPU_SCH_DefaultScheduleTable[5].GroupData = 6

;; Create the Table Load file for 2006.4 - Disabled
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Schedule Definition Table Req_2006", "sdt_20064D.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

;; Fix the bad table entry
$SC_$CPU_SCH_DefaultScheduleTable[6].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[6].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[6].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[6].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[6].MessageIndex = 7
$SC_$CPU_SCH_DefaultScheduleTable[6].GroupData = 5

;; Create the Table Load file for 2006.5 - Disabled
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Schedule Definition Table Req_2006", "sdt_20065D.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

;; Fix the bad table entry
$SC_$CPU_SCH_DefaultScheduleTable[7].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[7].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[7].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[7].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[7].MessageIndex = 7
$SC_$CPU_SCH_DefaultScheduleTable[7].GroupData = 5

;; Create the Table Load file for 2006.5 - Enabled
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Schedule Definition Table Req_2006", "sdt_20065E.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

;; Fix the bad table entry
$SC_$CPU_SCH_DefaultScheduleTable[8].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[8].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[8].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[8].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[8].MessageIndex = 8
$SC_$CPU_SCH_DefaultScheduleTable[8].GroupData = 5

;; Create the Table Load file for 2006.4 - Enabled
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Schedule Definition Table Req_2006", "sdt_20064E.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

;; Fix the bad table entry for 2006.4 - Enabled
$SC_$CPU_SCH_DefaultScheduleTable[9].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[9].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[9].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[9].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[9].MessageIndex = 8
$SC_$CPU_SCH_DefaultScheduleTable[9].GroupData = 5

;; Create the Table Load file for MessageIndex = 0 - Enabled
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Schedule Definition Table", "sdt_2006msg0E.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

;; Fix the bad table entry for MessageIndex = 0 - Enabled
$SC_$CPU_SCH_DefaultScheduleTable[10].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[10].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[10].SE_Frequency = 2
$SC_$CPU_SCH_DefaultScheduleTable[10].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[10].MessageIndex = 9
$SC_$CPU_SCH_DefaultScheduleTable[10].GroupData = 5

;; Create the Table Load file for MessageIndex = 0 - Enabled
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Schedule Definition Table", "sdt_2006msg0D.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

%liv(log_procedure) = logging

ENDPROC
