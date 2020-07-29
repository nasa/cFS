PROC $sc_$cpu_ds_tbl5
;*******************************************************************************
;  Test Name:  ds_tbl5
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	This procedure creates a full CFS Data Storage (DS) Packet Filter Table
;	load image file.
;	
;  Prerequisite Conditions
;	None.
;
;  Assumptions and Constraints
;	None.
;
;  Change History
;	Date		   Name		Description
;	09/14/11	Walt Moleski	Inital implemetation.
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
write ";  Step 1.0: Define DS Packet Filter Tables."
write ";***********************************************************************"
;; Setup the appid based upon the CPU being used
local filterAppid = 0x0F77
local firstMID = 0x0900
local DSAppName = "DS"   
local filterTblName = DSAppName & "." & DS_FILTER_TBL_NAME
local hostCPU = "$CPU"

;; Setup the Packet Filter Table
$SC_$CPU_DS_PF_TBL_Description  = "Full Table Add MID Test"

local fileIndex = 0
;; Loop for each table entry
for i = 0 to DS_PACKETS_IN_FILTER_TABLE-1 do
  $SC_$CPU_DS_PF_TBL[i].MessageID = FirstMID

  ;; Loop for each Filter
  for j = 1 to DS_FILTERS_PER_PACKET-1 do
    $SC_$CPU_DS_PF_TBL[i].FilterParams[j].Index = fileIndex
    $SC_$CPU_DS_PF_TBL[i].FilterParams[j].FilterType = DS_BY_COUNT
    $SC_$CPU_DS_PF_TBL[i].FilterParams[j].N_Value = 1
    $SC_$CPU_DS_PF_TBL[i].FilterParams[j].X_Value = 1
    $SC_$CPU_DS_PF_TBL[i].FilterParams[j].O_Value = 0
  enddo

  FirstMID = FirstMID + 1
  fileIndex = fileIndex + 1
  if (fileIndex = 16) then
    fileIndex = 0
  endif
enddo

local maxEntry = DS_PACKETS_IN_FILTER_TABLE - 1
local maxFilterParams = DS_FILTERS_PER_PACKET - 1
local endmnemonic = "$SC_$CPU_DS_PF_TBL[" & maxEntry & "].FilterParams[" & maxFilterParams & "].O_Value"

s create_tbl_file_from_cvt(hostCPU,filterAppid,"Full Filter Table Test","ds_fullfilter.tbl",filterTblName,"$SC_$CPU_DS_PF_TBL_Description",endmnemonic)

%liv (log_procedure) = logging

write ";*********************************************************************"
write ";  End procedure $SC_$CPU_ds_tbl5"
write ";*********************************************************************"
ENDPROC
