/* pull in the OSAL configuration */
#include "stub-map-to-real.h"
#include "osconfig.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "ostimer.c"
#include "ut-ostimer.h"

/*
 * Stub objects to satisfy linking requirements -
 */

OS_timecb_internal_record_t OS_timecb_table[OS_MAX_TIMERS];
OS_timebase_internal_record_t OS_timebase_table[OS_MAX_TIMEBASES];

OS_common_record_t OS_stub_timebase_table[OS_MAX_TIMEBASES];
OS_common_record_t OS_stub_timecb_table[OS_MAX_TIMERS];

OS_common_record_t * const OS_global_timebase_table   = OS_stub_timebase_table;
OS_common_record_t * const OS_global_timecb_table     = OS_stub_timecb_table;


OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false
      };

int32 Osapi_Internal_CallSigWaitFunc(uint32 local_id)
{
    return OS_VxWorks_SigWait(local_id);
}

int Osapi_Internal_CallHelperTaskFunc(int arg)
{
    return OS_VxWorks_TimeBaseTask(arg);
}

void Osapi_Internal_SetTimeBaseRegState(uint32 local_id, bool is_success)
{
    /* Mimic the setting of the Reg state global, which
     * is typically done by the task after spawning
     */
    if (is_success)
    {
        OS_impl_timebase_table[local_id].timer_state = OS_TimerRegState_SUCCESS;
    }
    else
    {
        OS_impl_timebase_table[local_id].timer_state = OS_TimerRegState_ERROR;
    }
}

void Osapi_Internal_ResetState(void)
{
    memset(OS_timecb_table, 0, sizeof(OS_timecb_table));
    memset(OS_timebase_table, 0, sizeof(OS_timebase_table));
    memset(OS_stub_timebase_table, 0, sizeof(OS_stub_timebase_table));
    memset(OS_stub_timecb_table, 0, sizeof(OS_stub_timecb_table));
}

void Osapi_Internal_Setup(uint32 local_id, int signo)
{
    static int FAKE_TASK;
    static int FAKE_SEM;

    OS_impl_timebase_table[local_id].assigned_signal = signo;
    OS_impl_timebase_table[local_id].handler_task = &FAKE_TASK;
    OS_impl_timebase_table[local_id].handler_mutex = &FAKE_SEM;
}

