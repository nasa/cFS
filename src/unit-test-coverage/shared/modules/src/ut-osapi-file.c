/* pull in the OSAL configuration */
#include "stub-map-to-real.h"
#include "osconfig.h"
#include "ut-osapi-file.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osapi-file.c"

/*
 * Stub objects to satisfy linking requirements -
 */
OS_common_record_t OS_stub_file_table[OS_MAX_NUM_OPEN_FILES];

OS_common_record_t * const OS_global_stream_table   = OS_stub_file_table;

OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false
      };

