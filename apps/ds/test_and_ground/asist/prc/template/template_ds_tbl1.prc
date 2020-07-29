PROC $sc_$cpu_ds_tbl1
;*******************************************************************************
;  Test Name:  ds_tbl1
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	This procedure creates the initial CFS Data Storage (DS) Destination
;	File and Packet Filter Table load image files. These files are currently
;	used by the General Commanding test procedure.
;	
;  Prerequisite Conditions
;	None.
;
;  Assumptions and Constraints
;	None.
;
;  Change History
;	Date		   Name		Description
;	11/03/09	Walt Moleski	Inital implemetation.
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
;
;  Expected Test Results and Analysis
;
;**********************************************************************

local logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

#include "ut_statusdefs.h"
#include "ut_cfe_info.h"
#include "cfe_platform_cfg.h"
#include "ds_platform_cfg.h"
#include "ds_events.h"
#include "ds_appdefs.h"

write ";***********************************************************************"
write ";  Step 1.0: Define DS Destination File Table 1."
write ";***********************************************************************"
;; Setup the appid based upon the CPU being used
local fileAppid = 0x0F76
local filterAppid = 0x0F77
local filter1MID = 0x0887   ; MM HK
local filter2MID = 0x08A4   ; CS HK
local dsCmdMID = 0x18BB     ; DS_CMD_MID
local dsSendHKMID = 0x18BC  ; DS_SEND_HK_MID
local DSAppName = "DS"  
local fileTblName = DSAppName & "." & DS_DESTINATION_TBL_NAME
local filterTblName = DSAppName & "." & DS_FILTER_TBL_NAME
local hostCPU = "$CPU"

;; Setup the Destination File Table
;; Entry 1
$SC_$CPU_DS_DF_TBL_Description  = "General Commands Test"
$SC_$CPU_DS_DF_TBL[0].Pathname  = "/ram/"
$SC_$CPU_DS_DF_TBL[0].Basename  = "ds_hk"
$SC_$CPU_DS_DF_TBL[0].Extension = ".dat"
$SC_$CPU_DS_DF_TBL[0].FileNameType = DS_BY_COUNT
$SC_$CPU_DS_DF_TBL[0].FileState = DS_ENABLED
$SC_$CPU_DS_DF_TBL[0].FileSize = 1024
$SC_$CPU_DS_DF_TBL[0].FileAge = 600
$SC_$CPU_DS_DF_TBL[0].SeqCnt = 100

;; Entry 2
$SC_$CPU_DS_DF_TBL[1].Pathname  = "/ram/"
$SC_$CPU_DS_DF_TBL[1].Basename  = "dummy"
$SC_$CPU_DS_DF_TBL[1].Extension = ".txt"
$SC_$CPU_DS_DF_TBL[1].FileNameType = DS_BY_TIME
$SC_$CPU_DS_DF_TBL[1].FileState = DS_DISABLED
$SC_$CPU_DS_DF_TBL[1].FileSize = 1024
$SC_$CPU_DS_DF_TBL[1].FileAge = 600
$SC_$CPU_DS_DF_TBL[1].SeqCnt = 200

local maxEntry = DS_DEST_FILE_CNT - 1

;; Clear the remaining entries in the table
;;for i = 2 to maxEntry do
;;  $SC_$CPU_DS_DF_TBL[i].Pathname  = ""
;;  $SC_$CPU_DS_DF_TBL[i].Basename  = ""
;;  $SC_$CPU_DS_DF_TBL[i].Extension = ""
;;  $SC_$CPU_DS_DF_TBL[i].FileNameType = 0
;;  $SC_$CPU_DS_DF_TBL[i].FileState = 0
;;  $SC_$CPU_DS_DF_TBL[i].FileSize = 0
;;  $SC_$CPU_DS_DF_TBL[i].FileAge = 0
;;  $SC_$CPU_DS_DF_TBL[i].SeqCnt = 0
;;enddo

;;local endmnemonic = "$SC_$CPU_DS_DF_TBL[" & maxEntry & "].SeqCnt"
local endmnemonic = "$SC_$CPU_DS_DF_TBL[1].SeqCnt"

s create_tbl_file_from_cvt(hostCPU,fileAppid,"General Command Test File Table","ds_gcfile.tbl",fileTblName,"$SC_$CPU_DS_DF_TBL_Description",endmnemonic)

;; Setup the Packet Filter Table
$SC_$CPU_DS_PF_TBL_Description  = "General Commands Test"
;; Entry 1
$SC_$CPU_DS_PF_TBL[0].MessageID = filter1MID
$SC_$CPU_DS_PF_TBL[0].FilterParams[0].Index = 0
$SC_$CPU_DS_PF_TBL[0].FilterParams[0].FilterType = DS_BY_COUNT
$SC_$CPU_DS_PF_TBL[0].FilterParams[0].N_Value = 1
$SC_$CPU_DS_PF_TBL[0].FilterParams[0].X_Value = 1
$SC_$CPU_DS_PF_TBL[0].FilterParams[0].O_Value = 0
for j = 1 to DS_FILTERS_PER_PACKET-1 do
  $SC_$CPU_DS_PF_TBL[0].FilterParams[j].Index = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[0].FilterParams[j].FilterType = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[0].FilterParams[j].N_Value = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[0].FilterParams[j].X_Value = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[0].FilterParams[j].O_Value = DS_UNUSED
enddo

;; Entry 2
$SC_$CPU_DS_PF_TBL[1].MessageID = filter2MID
$SC_$CPU_DS_PF_TBL[1].FilterParams[0].Index = 1
$SC_$CPU_DS_PF_TBL[1].FilterParams[0].FilterType = DS_BY_COUNT
$SC_$CPU_DS_PF_TBL[1].FilterParams[0].N_Value = 1
$SC_$CPU_DS_PF_TBL[1].FilterParams[0].X_Value = 2
$SC_$CPU_DS_PF_TBL[1].FilterParams[0].O_Value = 1
for j = 1 to DS_FILTERS_PER_PACKET-1 do
  $SC_$CPU_DS_PF_TBL[1].FilterParams[j].Index = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[1].FilterParams[j].FilterType = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[1].FilterParams[j].N_Value = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[1].FilterParams[j].X_Value = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[1].FilterParams[j].O_Value = DS_UNUSED
enddo

maxEntry = DS_PACKETS_IN_FILTER_TABLE - 1
local maxFilterParams = DS_FILTERS_PER_PACKET - 1

;; Loop for the remaining entries defined in the table
for entry = 2 to maxEntry do
  $SC_$CPU_DS_PF_TBL[entry].MessageID = DS_UNUSED
  for j = 0 to DS_FILTERS_PER_PACKET-1 do
    $SC_$CPU_DS_PF_TBL[entry].FilterParams[j].Index = DS_UNUSED
    $SC_$CPU_DS_PF_TBL[entry].FilterParams[j].FilterType = DS_UNUSED
    $SC_$CPU_DS_PF_TBL[entry].FilterParams[j].N_Value = DS_UNUSED
    $SC_$CPU_DS_PF_TBL[entry].FilterParams[j].X_Value = DS_UNUSED
    $SC_$CPU_DS_PF_TBL[entry].FilterParams[j].O_Value = DS_UNUSED
  enddo
enddo

endmnemonic = "$SC_$CPU_DS_PF_TBL[" & maxEntry & "].FilterParams[" & maxFilterParams & "].O_Value"

s create_tbl_file_from_cvt(hostCPU,filterAppid,"General Command Test Filter Table","ds_gcfilter.tbl",filterTblName,"$SC_$CPU_DS_PF_TBL_Description",endmnemonic)

%liv (log_procedure) = logging

write ";*********************************************************************"
write ";  End procedure $SC_$CPU_ds_tbl1"
write ";*********************************************************************"
ENDPROC
