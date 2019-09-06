/* pull in the OSAL configuration */
#include "stub-map-to-real.h"
#include "osconfig.h"
#include "ut-osapi-task.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osapi-task.c"

/* Stub objects to satisfy linking requirements -
 */
OS_common_record_t OS_stub_task_table[OS_MAX_TASKS];
OS_common_record_t * const OS_global_task_table   = OS_stub_task_table;
