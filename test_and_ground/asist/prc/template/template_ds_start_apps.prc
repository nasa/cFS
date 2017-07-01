proc $sc_$cpu_ds_start_apps (step_num)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Purpose:
;	The purpose of this procedure is to startup the CFS Data Storage (DS)
;	and test (TST_DS) applications if they are not already running.
;
; History:
;  29SEP09 WFM	Initial development of this proc
;  08DEC10 WFM	Added variable for application name and ram directory name
;  31JAN17 WFM	Updated for DS 2.5.0.0 using CPU1 for commanding and added a
;		hostCPU variable to start apps on
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Turn off logging for the includes
local logging = %liv (log_procedure)
%liv (log_procedure) = FALSE

#include "ut_statusdefs.h"
#include "ut_cfe_info.h"
#include "cfe_platform_cfg.h"
#include "cfe_es_events.h"
#include "ds_events.h"
#include "tst_ds_events.h"

%liv (log_procedure) = logging

local file_index
local found_app1, found_app2
local stream1, stream2
local subStepNum = 1
local DSAppName = "DS"  
local ramDir = "RAM:0"
local hostCPU = "$CPU"

write ";*********************************************************************"
write "; Step ",step_num, ".1: Determine if the applications are running."
write ";*********************************************************************"
start get_file_to_cvt (ramDir, "cfe_es_app_info.log", "$sc_$cpu_es_app_info.log", hostCPU)

found_app1 = FALSE
found_app2 = FALSE

;Loop thru the table looking for the CS and TST_CS
for file_index = 1 to CFE_ES_MAX_APPLICATIONS do
  if ($SC_$CPU_ES_ALE[file_index].ES_AL_AppName = DSAppName) then
    found_app1 = TRUE
  elseif ($SC_$CPU_ES_ALE[file_index].ES_AL_AppName = "TST_DS") then
    found_app2 = TRUE
  endif
enddo

if ((found_app1 = TRUE) AND (found_app2 = TRUE)) then
  write "The Applications are running. Setup will be skipped!!!"
  goto procterm
else
  write "At least one application is not running. They will be started."
  wait 10
endif

;; If TST_DS is the only app running, then you must stop TST_DS
local cmdCtr
if (found_app1 = FALSE) AND (found_app2 = TRUE) then
  cmdCtr = $SC_$CPU_ES_CMDPC + 1

  /$SC_$CPU_ES_DELETEAPP Application="TST_DS"
  wait 5

  ut_tlmwait  $SC_$CPU_ES_CMDPC, {cmdCtr}
  if (UT_TW_Status = UT_Success) then
    write "<*> Passed - TST_DS app stop command sent properly."
  else
    write "<!> Failed - TST_DS app stop command did not increment CMDPC."
  endif

  found_app2 = FALSE
endif

;; Increment the subStep
subStepNum = 2

;  Load the applications
;; Only perform this step if the DS app was not running
if (found_app1 = FALSE) then
  write ";*********************************************************************"
  write ";  Step ",step_num, ".2: Load and start the Data Storage application"
  write ";*********************************************************************"
  ut_setupevents "$SC", "$CPU", "CFE_ES", CFE_ES_START_INF_EID, "INFO", 1
  ut_setupevents "$SC", "$CPU", {DSAppName}, DS_INIT_EID, "INFO", 2

  s load_start_app (DSAppName,hostCPU,"DS_AppMain")

  ; Wait for app startup events
  ut_tlmwait  $SC_$CPU_find_event[2].num_found_messages, 1, 70
  IF (UT_TW_Status = UT_Success) THEN
    if ($SC_$CPU_num_found_messages = 1) then
      write "<*> Passed - DS Application Started"
    else
      write "<!> Failed - CFE_ES start Event Message for DS not received."
    endif
  else
    write "<!> Failed - DS Application start Event Message not received."
    goto procerror
  endif

  ;; CPU1 is the default
  stream1 = x'08B8'
  stream2 = x'08B9'

  /$SC_$CPU_TO_ADDPACKET STREAM=stream1 PKT_SIZE=X'0' PRIORITY=X'0' RELIABILITY=X'0' BUFLIMIT=x'4'
  wait 2
  /$SC_$CPU_TO_ADDPACKET STREAM=stream2 PKT_SIZE=X'0' PRIORITY=X'0' RELIABILITY=X'0' BUFLIMIT=x'4'

  subStepNum = 3
endif

;; If the TST_DS application is not running, start it
if (found_app2 = FALSE) then
  write ";*********************************************************************"
  write "; Step ",step_num, ".",subStepNum,": Load and start the TST_DS application"
  write ";*********************************************************************"
  ut_setupevents "$SC", "$CPU", "CFE_ES", CFE_ES_START_INF_EID, "INFO", 1
  ut_setupevents "$SC", "$CPU", "TST_DS", TST_DS_INIT_INF_EID, "INFO", 2

  s load_start_app ("TST_DS",hostCPU,"TST_DS_AppMain")

  ; Wait for app startup events
  ut_tlmwait  $SC_$CPU_find_event[2].num_found_messages, 1
  IF (UT_TW_Status = UT_Success) THEN
    if ($SC_$CPU_num_found_messages = 1) then
      write "<*> Passed - TST_DS Application Started"
    else
      write "<!> Failed - CFE_ES start Event Message for TST_DS not received."
      write "Event Message count = ",$SC_$CPU_num_found_messages
    endif
  else
    write "<!> Failed - TST_DS Application start Event Message not received."
    goto procerror
  endif

  ;; CPU1 is the default
  stream2 = x'0942'

  /$SC_$CPU_TO_ADDPACKET STREAM=stream2 PKT_SIZE=X'0' PRIORITY=X'0' RELIABILITY=X'0' BUFLIMIT=x'4'

  wait 10
endif

goto procterm

procerror:
     Write "There was a problem with this procedure"

procterm:
    Write "Procedure completed!!!"

endproc
