/* pull in the OSAL configuration */
#include "stub-map-to-real.h"
#include "osconfig.h"
#include "ut-osapi-printf.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osapi-printf.c"

OS_common_record_t OS_stub_console_table[OS_MAX_TASKS];
OS_common_record_t * const OS_global_console_table   = OS_stub_console_table;

OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false
      };

