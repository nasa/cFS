/* pull in the OSAL configuration */
#include "stub-map-to-real.h"
#include "osconfig.h"
#include "ut-osapi.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osapi.c"

/* Stub objects to satisfy linking requirements -
 */
OS_common_record_t OS_stub_task_table[OS_MAX_TASKS];
OS_common_record_t * const OS_global_task_table   = OS_stub_task_table;
OS_common_record_t OS_stub_queue_table[OS_MAX_QUEUES];
OS_common_record_t * const OS_global_queue_table   = OS_stub_queue_table;
OS_common_record_t OS_stub_console_table[OS_MAX_QUEUES];
OS_common_record_t * const OS_global_console_table   = OS_stub_console_table;

OS_queue_internal_record_t OS_queue_table[OS_MAX_QUEUES];
OS_task_internal_record_t OS_task_table[OS_MAX_TASKS];
OS_console_internal_record_t OS_console_table[OS_MAX_CONSOLES];

OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false
      };


void Osapi_Internal_ResetState(void)
{
    memset(&OS_SharedGlobalVars, 0, sizeof(OS_SharedGlobalVars));
    memset(OS_queue_table, 0, sizeof(OS_queue_table));
    memset(OS_task_table, 0, sizeof(OS_task_table));
    memset(OS_console_table, 0, sizeof(OS_console_table));
    memset(OS_stub_queue_table, 0, sizeof(OS_stub_queue_table));
    memset(OS_stub_task_table, 0, sizeof(OS_stub_task_table));
    memset(OS_stub_console_table, 0, sizeof(OS_stub_console_table));
}

void Osapi_Internal_SetImplTableMutex(uint32 idtype, OCS_SEM_ID vxid)
{
    VX_MUTEX_TABLE[idtype].vxid = vxid;
}

void Osapi_Internal_SetImplTaskId(uint32 local_id, OCS_TASK_ID TaskId)
{
    OS_impl_task_table[local_id].vxid = TaskId;
}

/*
 * Because the task entry point is declared "static",
 * in order for the UT to invoke it there must be a non-static
 * way to get access to it.
 */
int Osapi_Internal_CallEntryPoint(int arg)
{
    return OS_VxWorksEntry(arg);
}

void Osapi_Internal_CallConsoleTask_Entry(int arg)
{
    OS_ConsoleTask_Entry(arg);
}

OCS_WIND_TCB* Osapi_Internal_GetTaskTcb(uint32 local_id)
{
    return &OS_impl_task_table[local_id].tcb;
}

void Osapi_Internal_SetConsoleAsync(uint32 local_id, bool is_async)
{
    OS_impl_console_table[local_id].is_async = is_async;
}

