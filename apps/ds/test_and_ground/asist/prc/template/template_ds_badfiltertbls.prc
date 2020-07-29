PROC $sc_$cpu_ds_badfiltertbls
;*******************************************************************************
;  Test Name:  ds_badfiltertbls
;  Test Level: Build Verification
;  Test Type:  Functional
;
;  Test Description
;	This procedure creates five CFS Data Storage (DS) Packet Filter Table 
;	load images each containing one of the possible five content errors such
;	that the table validation will fail and generate the proper error event
;	message. This table image is used in the ResetNoCDS test procedure.
;	
;  Prerequisite Conditions
;	None.
;
;  Assumptions and Constraints
;	None.
;
;  Change History
;	Date		   Name		Description
;	11/23/09	Walt Moleski	Inital implemetation.
;       12/08/10        Walt Moleski    Modified to use the table name specified
;                                       in the ds_platform_cfg.h file and added
;                                       a variable for the application name.
;       01/31/17        Walt Moleski	Updated for DS 2.5.0.0 using CPU1 for
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
#include "ds_appdefs.h"

write ";***********************************************************************"
write ";  Step 1.0: Define DS Destination File And Packet Filter Tables."
write ";***********************************************************************"
;; Setup the appid based upon the CPU being used
local filterAppid = 0x0F77
local DSAppName = "DS"
local filterTblName = DSAppName & "." & DS_FILTER_TBL_NAME
local hostCPU = "$CPU"

;; Setup the Packet Filter Table
;; Descriptor text is limited by DS_DESCRIPTOR_BUFSIZE
;; which currently is set to 32 -> Description below = 39
$SC_$CPU_DS_PF_TBL_Description  = "Reset No Critical Data Store (CDS) Test"

;; Entry 1
$SC_$CPU_DS_PF_TBL[0].MessageID = x'1000'
;; File #1 - Invalid Index
$SC_$CPU_DS_PF_TBL[0].FilterParams[0].Index = DS_DEST_FILE_CNT
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

;; Entry 2 - Invalid FilterType
$SC_$CPU_DS_PF_TBL[1].MessageID = x'1001'
;; File #1 - Invalid FilterType
$SC_$CPU_DS_PF_TBL[1].FilterParams[0].Index = 1
$SC_$CPU_DS_PF_TBL[1].FilterParams[0].FilterType = DS_BY_TIME + 1
$SC_$CPU_DS_PF_TBL[1].FilterParams[0].N_Value = 1
$SC_$CPU_DS_PF_TBL[1].FilterParams[0].X_Value = 1
$SC_$CPU_DS_PF_TBL[1].FilterParams[0].O_Value = 0
for j = 1 to DS_FILTERS_PER_PACKET-1 do
  $SC_$CPU_DS_PF_TBL[1].FilterParams[j].Index = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[1].FilterParams[j].FilterType = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[1].FilterParams[j].N_Value = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[1].FilterParams[j].X_Value = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[1].FilterParams[j].O_Value = DS_UNUSED
enddo

;; Entry 3 - Invalid N value
$SC_$CPU_DS_PF_TBL[2].MessageID = x'1002'
;; File #1 - Invalid N value
$SC_$CPU_DS_PF_TBL[2].FilterParams[0].Index = 2
$SC_$CPU_DS_PF_TBL[2].FilterParams[0].FilterType = DS_BY_COUNT
$SC_$CPU_DS_PF_TBL[2].FilterParams[0].N_Value = 2
$SC_$CPU_DS_PF_TBL[2].FilterParams[0].X_Value = 1
$SC_$CPU_DS_PF_TBL[2].FilterParams[0].O_Value = 0
for j = 1 to DS_FILTERS_PER_PACKET-1 do
  $SC_$CPU_DS_PF_TBL[2].FilterParams[j].Index = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[2].FilterParams[j].FilterType = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[2].FilterParams[j].N_Value = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[2].FilterParams[j].X_Value = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[2].FilterParams[j].O_Value = DS_UNUSED
enddo

;; Entry 4 - Invalid O value (O > X)
$SC_$CPU_DS_PF_TBL[3].MessageID = x'1003'
;; File #1 - Invalid O value (O > X)
$SC_$CPU_DS_PF_TBL[3].FilterParams[0].Index = 4
$SC_$CPU_DS_PF_TBL[3].FilterParams[0].FilterType = DS_BY_TIME
$SC_$CPU_DS_PF_TBL[3].FilterParams[0].N_Value = 1
$SC_$CPU_DS_PF_TBL[3].FilterParams[0].X_Value = 1
$SC_$CPU_DS_PF_TBL[3].FilterParams[0].O_Value = 2
for j = 2 to DS_FILTERS_PER_PACKET-1 do
  $SC_$CPU_DS_PF_TBL[3].FilterParams[j].Index = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[3].FilterParams[j].FilterType = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[3].FilterParams[j].N_Value = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[3].FilterParams[j].X_Value = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[3].FilterParams[j].O_Value = DS_UNUSED
enddo

;; Entry 5 -> Valid
$SC_$CPU_DS_PF_TBL[4].MessageID = x'1004'
;; File #1
$SC_$CPU_DS_PF_TBL[4].FilterParams[0].Index = 5
$SC_$CPU_DS_PF_TBL[4].FilterParams[0].FilterType = DS_BY_TIME
$SC_$CPU_DS_PF_TBL[4].FilterParams[0].N_Value = 1
$SC_$CPU_DS_PF_TBL[4].FilterParams[0].X_Value = 1
$SC_$CPU_DS_PF_TBL[4].FilterParams[0].O_Value = 0
for j = 1 to DS_FILTERS_PER_PACKET-1 do
  $SC_$CPU_DS_PF_TBL[4].FilterParams[j].Index = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[4].FilterParams[j].FilterType = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[4].FilterParams[j].N_Value = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[4].FilterParams[j].X_Value = DS_UNUSED
  $SC_$CPU_DS_PF_TBL[4].FilterParams[j].O_Value = DS_UNUSED
enddo

local maxEntry = DS_PACKETS_IN_FILTER_TABLE - 1
local maxFilterParams = DS_FILTERS_PER_PACKET - 1

;; Loop for the remaining entries defined in the table
for entry = 5 to maxEntry do
  $SC_$CPU_DS_PF_TBL[entry].MessageID = DS_UNUSED
  for j = 0 to DS_FILTERS_PER_PACKET-1 do
    $SC_$CPU_DS_PF_TBL[entry].FilterParams[j].Index = DS_UNUSED
    $SC_$CPU_DS_PF_TBL[entry].FilterParams[j].FilterType = DS_UNUSED
    $SC_$CPU_DS_PF_TBL[entry].FilterParams[j].N_Value = DS_UNUSED
    $SC_$CPU_DS_PF_TBL[entry].FilterParams[j].X_Value = DS_UNUSED
    $SC_$CPU_DS_PF_TBL[entry].FilterParams[j].O_Value = DS_UNUSED
  enddo
enddo

local endmnemonic = "$SC_$CPU_DS_PF_TBL[" & maxEntry & "].FilterParams["
endmnemonic = endmnemonic & maxFilterParams & "].O_Value"

s create_tbl_file_from_cvt(hostCPU,filterAppid,"Invalid Filter Table Description","ds_badfilter1.tbl",filterTblName,"$SC_$CPU_DS_PF_TBL_Description",endmnemonic)

;; Set the Table description text to a valid string
$SC_$CPU_DS_PF_TBL_Description  = "Reset No CDS Test"

s create_tbl_file_from_cvt(hostCPU,filterAppid,"Invalid Filter Table Entry #1 Index","ds_badfilter2.tbl",filterTblName,"$SC_$CPU_DS_PF_TBL_Description",endmnemonic)

;; Set the Index to a valid file
$SC_$CPU_DS_PF_TBL[0].FilterParams[0].Index = 0

s create_tbl_file_from_cvt(hostCPU,filterAppid,"Invalid Filter Table Entry #2 Type","ds_badfilter3.tbl",filterTblName,"$SC_$CPU_DS_PF_TBL_Description",endmnemonic)

;; Set the Filter Type back to valid
$SC_$CPU_DS_PF_TBL[1].FilterParams[0].FilterType = DS_BY_TIME

s create_tbl_file_from_cvt(hostCPU,filterAppid,"Invalid Filter Table Entry #3 N>X","ds_badfilter4.tbl",filterTblName,"$SC_$CPU_DS_PF_TBL_Description",endmnemonic)

;; Set the N Value to a valid number
$SC_$CPU_DS_PF_TBL[2].FilterParams[0].N_Value = 1

s create_tbl_file_from_cvt(hostCPU,filterAppid,"Invalid Filter Table Entry #4 O>X","ds_badfilter5.tbl",filterTblName,"$SC_$CPU_DS_PF_TBL_Description",endmnemonic)

%liv (log_procedure) = logging

write ";*********************************************************************"
write ";  End procedure $SC_$CPU_ds_badfiltertbls"
write ";*********************************************************************"
ENDPROC
