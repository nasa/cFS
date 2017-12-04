PROC $sc_$cpu_sch_sdtloadfile

local logging = %liv(log_procedure)
%liv(log_procedure) = FALSE
;*******************************************************************************
;  Name:  sch_sdtloadfile
;
;  Test Description
;	This procedure populates the CFS Scheduler device table and generates
;	a table load image for that table. This table image can be loaded via
;	the CFE_TBL_LOAD command.
;
;  Change History
;
;	Date	   Name		Description
;	12/16/08   E. Uzo-Okoro	Original Procedure.
;	03/15/10   W. Moleski	Updated to use 0-based array for the SDT
;	07/28/11   W. Moleski	Updated for 2.1.1.0
;	08/03/11   W. Moleski	Updated Entry 8 (TST_SCH_DEND_HK) to be disabled
;				by default, frequency=5 and GroupData=4
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
LOCAL i
local SCHAppName = "SCH"
local SCHDefTblName = SCHAppName & ".SCHED_DEF"
local defTblDir = "CF:0/apps"
local hostCPU = "$CPU"

;;; Set the pkt and app IDs for the tables based upon the cpu being used
;;; Right now, the pktIDs are not used
;;; CPU1 is the default
tblAppId = "0FB5"
tblPktId = 4021

write ";*********************************************************************"
write ";  Create & upload the Schedule Definition Table file."
write ";********************************************************************"
;; States are 0=Unused; 1=Enabled; 2=Disabled; 

; Entry 0
entry = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 1 
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 8

; Entry 1
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 2 
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 8

; Entry 2
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 3 
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 8

; Entry 3
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 4
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 8

; Entry 4
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 5 
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 8

; Entry 5
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 6 
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 8

; Entry 6
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 7
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 8

; Entry 7 - Msg 8 = SCH_SEND_HK request
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 8
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 8

; Entry 8 - Msg 9 = TST_SCH_SEND_HK request
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 9
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 4

; Entry 9
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_UNUSED 
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_UNUSED 
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = SCH_UNUSED
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = SCH_UNUSED
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = SCH_UNUSED
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = SCH_UNUSED

; Entry 10
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 4
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 10
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 2

; Entry 11
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 4
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 11
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 2

; Entry 12
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 4
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 12
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 2

; Entry 13
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 4
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 13
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 2

; Entry 14
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 4
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 14
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 2

; Entry 15
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 4
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 15
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 2

; Entry 16
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 4
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 16
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 2

; Entry 17
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 4
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 17
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 2

; Entry 18
entry = entry + 1
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 4
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 2
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 18
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 2

local maxEntries = SCH_TABLE_ENTRIES - 1

;;zero out the rest of the table
for entry=19 to maxEntries do
$SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_UNUSED
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_NONE
$SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 0
$SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 0
enddo

local endMnemonic = "$SC_$CPU_SCH_DefaultScheduleTable[" & maxEntries & "]"

;; Create the Table Load file
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Schedule Definition Table 1","sch_def_schtbl.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

%liv(log_procedure) = logging

;; Parse the filename configuration parameter for the default table filename
local tableFileName = SCH_SCHEDULE_FILENAME
local slashLoc = %locate(tableFileName,"/")

;; loop until all slashes are found
while (slashLoc <> 0) do
  tableFileName = %substring(tableFileName,slashLoc+1,%length(tableFileName))
  slashLoc = %locate(tableFileName,"/")
enddo

write "==> Default Schedule Definition Table filename = '",tableFileName,"'"

s ftp_file(defTblDir, "sch_def_schtbl.tbl", tableFileName, hostCPU, "P")

ENDPROC
