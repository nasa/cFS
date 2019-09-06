/* pull in the OSAL configuration */
#include "stub-map-to-real.h"
#include "osconfig.h"
#include "ut-osapi-timebase.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osapi-timebase.c"

/* Stub objects to satisfy linking requirements -
 * the time module may look up entities in the timebase table
 */
OS_timecb_internal_record_t OS_timecb_table[OS_MAX_TIMERS];
OS_common_record_t OS_stub_timebase_table[OS_MAX_TIMEBASES];
OS_common_record_t OS_stub_timecb_table[OS_MAX_TIMERS];

OS_common_record_t * const OS_global_timebase_table   = OS_stub_timebase_table;
OS_common_record_t * const OS_global_timecb_table     = OS_stub_timecb_table;


OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false
      };

