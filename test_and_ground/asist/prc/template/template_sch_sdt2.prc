PROC $sc_$cpu_sch_sdt2

local logging = %liv(log_procedure)
%liv(log_procedure) = FALSE
;*******************************************************************************
;  Name:  sch_sdt2
;
;  Test Description
;	This procedure populates the CFS Scheduler device table and generates
;	a table load image for that table. This table image can be loaded via
;	the CFE_TBL_LOAD command.
;
;  Change History
;	Date	   Name		Description
;	06/27/11   W. Moleski	Original implementation
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
;; States are 0=SCH_Unused; 1=SCH_Enabled; 2=SCH_Disabled;

local maxEntries = SCH_TABLE_ENTRIES - 1
local one5thEntries = SCH_TABLE_ENTRIES / 5
local two5thEntries = one5thEntries * 2
local three5thEntries = one5thEntries * 3
local four5thEntries = one5thEntries * 4

;; Set the first 9 entries to send HK Requests
; Entry 0
$SC_$CPU_SCH_DefaultScheduleTable[0].EnableState = SCH_UNUSED
$SC_$CPU_SCH_DefaultScheduleTable[0].SE_Type = SCH_UNUSED
$SC_$CPU_SCH_DefaultScheduleTable[0].SE_Frequency = SCH_UNUSED
$SC_$CPU_SCH_DefaultScheduleTable[0].Remainder = SCH_UNUSED
$SC_$CPU_SCH_DefaultScheduleTable[0].MessageIndex = SCH_UNUSED
$SC_$CPU_SCH_DefaultScheduleTable[0].GroupData = SCH_UNUSED

; Entry 1
$SC_$CPU_SCH_DefaultScheduleTable[1].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[1].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[1].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[1].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[1].MessageIndex = 1
$SC_$CPU_SCH_DefaultScheduleTable[1].GroupData = 8

; Entry 2
$SC_$CPU_SCH_DefaultScheduleTable[2].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[2].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[2].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[2].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[2].MessageIndex = 2
$SC_$CPU_SCH_DefaultScheduleTable[2].GroupData = 8

; Entry 3
$SC_$CPU_SCH_DefaultScheduleTable[3].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[3].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[3].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[3].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[3].MessageIndex = 3
$SC_$CPU_SCH_DefaultScheduleTable[3].GroupData = 8

; Entry 4
$SC_$CPU_SCH_DefaultScheduleTable[4].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[4].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[4].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[4].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[4].MessageIndex = 4
$SC_$CPU_SCH_DefaultScheduleTable[4].GroupData = 8

; Entry 5
$SC_$CPU_SCH_DefaultScheduleTable[5].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[5].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[5].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[5].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[5].MessageIndex = 5
$SC_$CPU_SCH_DefaultScheduleTable[5].GroupData = 8

; Entry 6
$SC_$CPU_SCH_DefaultScheduleTable[6].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[6].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[6].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[6].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[6].MessageIndex = 6
$SC_$CPU_SCH_DefaultScheduleTable[6].GroupData = 8

; Entry 7
$SC_$CPU_SCH_DefaultScheduleTable[7].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[7].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[7].SE_Frequency = 5
$SC_$CPU_SCH_DefaultScheduleTable[7].Remainder = 1
$SC_$CPU_SCH_DefaultScheduleTable[7].MessageIndex = 7
$SC_$CPU_SCH_DefaultScheduleTable[7].GroupData = 8

; Entry 8
$SC_$CPU_SCH_DefaultScheduleTable[8].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[8].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[8].SE_Frequency = 1
$SC_$CPU_SCH_DefaultScheduleTable[8].Remainder = 0
$SC_$CPU_SCH_DefaultScheduleTable[8].MessageIndex = 8
$SC_$CPU_SCH_DefaultScheduleTable[8].GroupData = 8

; Entry 9
$SC_$CPU_SCH_DefaultScheduleTable[9].EnableState = SCH_ENABLED
$SC_$CPU_SCH_DefaultScheduleTable[9].SE_Type = SCH_ACTIVITY_SEND_MSG
$SC_$CPU_SCH_DefaultScheduleTable[9].SE_Frequency = 1
$SC_$CPU_SCH_DefaultScheduleTable[9].Remainder = 0
$SC_$CPU_SCH_DefaultScheduleTable[9].MessageIndex = 9
$SC_$CPU_SCH_DefaultScheduleTable[9].GroupData = 8

;; Set the first 1/5 entries to send MDT Msg 10 - TO_NOOP
for entry=10 to one5thEntries do
  $SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
  $SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
  $SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
  $SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 0
  $SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 10
  $SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 2
enddo

;; Set the second 1/5 entries to send MDT Msg 12 - TST_SCH_NOOP
for entry=one5thEntries+1 to two5thEntries do
  $SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
  $SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
  $SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
  $SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 1
  $SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 12
  $SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 2
enddo

;; Set the third 1/5 entries to send MDT Msg 13 - CI_NOOP
for entry=two5thEntries+1 to three5thEntries do
  $SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
  $SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
  $SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
  $SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 2
  $SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 13
  $SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 4
enddo

;; Set the fourth 1/5 entries to send MDT Msg 14 - ES_NOOP
for entry=three5thEntries+1 to four5thEntries do
  $SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
  $SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
  $SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
  $SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 3
  $SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 14
  $SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 4
enddo

;; Set the last 1/5 entries to send MDT Msg 15 - EVS_NOOP
for entry=four5thEntries+1 to maxEntries do
  $SC_$CPU_SCH_DefaultScheduleTable[entry].EnableState = SCH_DISABLED
  $SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Type = SCH_ACTIVITY_SEND_MSG
  $SC_$CPU_SCH_DefaultScheduleTable[entry].SE_Frequency = 5
  $SC_$CPU_SCH_DefaultScheduleTable[entry].Remainder = 4
  $SC_$CPU_SCH_DefaultScheduleTable[entry].MessageIndex = 15
  $SC_$CPU_SCH_DefaultScheduleTable[entry].GroupData = 2
enddo

local endMnemonic = "$SC_$CPU_SCH_DefaultScheduleTable[" & maxEntries & "]"

;; Create the Table Load file
s create_tbl_file_from_cvt (hostCPU,tblAppId,"SCH Full Schedule Definition Table","sch_def_sdt2.tbl",SCHDefTblName,"$SC_$CPU_SCH_DefaultScheduleTable[0]",endMnemonic)

%liv(log_procedure) = logging

ENDPROC
