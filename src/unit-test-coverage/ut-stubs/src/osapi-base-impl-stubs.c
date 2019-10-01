/**
 * @file impl-stubs.c
 *
 * UT assert stubs for low-level implementation functions.
 * These are prototyped in the "os-impl.h" file within the shared API
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"
#include "os-impl.h"

const OS_ErrorTable_Entry_t OS_IMPL_ERROR_NAME_TABLE[] =
{
        { -4444, "UT_ERROR" },
        { 0, NULL }
};


UT_DEFAULT_STUB(OS_API_Impl_Init, (uint32 idtype))

void  OS_IdleLoop_Impl               (void)
{
    UT_DEFAULT_IMPL(OS_IdleLoop_Impl);
}

void  OS_ApplicationShutdown_Impl    (void)
{
    UT_DEFAULT_IMPL(OS_ApplicationShutdown_Impl);
}

/*
** Function prototypes for routines implemented in OS-specific layers
*/

/*
 * Table locking and unlocking for global objects can be done at the shared code
 * layer but the actual implementation is OS-specific
 */
UT_DEFAULT_STUB(OS_Lock_Global_Impl,(uint32 idtype))
UT_DEFAULT_STUB(OS_Unlock_Global_Impl,(uint32 idtype))



/*
** Task API
*/
UT_DEFAULT_STUB(OS_TaskMatch_Impl,(uint32 task_id))
UT_DEFAULT_STUB(OS_TaskCreate_Impl,(uint32 task_id, uint32 flags))
UT_DEFAULT_STUB(OS_TaskDelete_Impl,(uint32 task_id))
void OS_TaskExit_Impl               (void)
{
    UT_DEFAULT_IMPL(OS_TaskExit_Impl);
}

UT_DEFAULT_STUB(OS_TaskDelay_Impl,(uint32 millisecond))
UT_DEFAULT_STUB(OS_TaskSetPriority_Impl,(uint32 task_id, uint32 new_priority))
uint32 OS_TaskGetId_Impl            (void)
{
    return UT_DEFAULT_IMPL(OS_TaskGetId_Impl);
}
UT_DEFAULT_STUB(OS_TaskGetInfo_Impl,(uint32 task_id, OS_task_prop_t *task_prop))
UT_DEFAULT_STUB(OS_TaskRegister_Impl,(uint32 global_task_id))

/*
** Message Queue API
*/

/*
** Queue Create now has the Queue ID returned to the caller.
*/
UT_DEFAULT_STUB(OS_QueueCreate_Impl,(uint32 queue_id, uint32 flags))
UT_DEFAULT_STUB(OS_QueueDelete_Impl,(uint32 queue_id))
UT_DEFAULT_STUB(OS_QueueGet_Impl,(uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout))
UT_DEFAULT_STUB(OS_QueuePut_Impl,(uint32 queue_id, const void *data, uint32 size, uint32 flags))
UT_DEFAULT_STUB(OS_QueueGetInfo_Impl,(uint32 queue_id, OS_queue_prop_t *queue_prop))

/*
** Semaphore API
*/

UT_DEFAULT_STUB(OS_BinSemCreate_Impl,(uint32 sem_id, uint32 sem_initial_value, uint32 options))
UT_DEFAULT_STUB(OS_BinSemFlush_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_BinSemGive_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_BinSemTake_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_BinSemTimedWait_Impl,(uint32 sem_id, uint32 msecs))
UT_DEFAULT_STUB(OS_BinSemDelete_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_BinSemGetInfo_Impl,(uint32 sem_id, OS_bin_sem_prop_t *bin_prop))

UT_DEFAULT_STUB(OS_CountSemCreate_Impl,(uint32 sem_id, uint32 sem_initial_value, uint32 options))
UT_DEFAULT_STUB(OS_CountSemGive_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_CountSemTake_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_CountSemTimedWait_Impl,(uint32 sem_id, uint32 msecs))
UT_DEFAULT_STUB(OS_CountSemDelete_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_CountSemGetInfo_Impl,(uint32 sem_id, OS_count_sem_prop_t *count_prop))

/*
** Mutex API
*/

UT_DEFAULT_STUB(OS_MutSemCreate_Impl,(uint32 sem_id, uint32 options))
UT_DEFAULT_STUB(OS_MutSemGive_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_MutSemTake_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_MutSemDelete_Impl,(uint32 sem_id))
UT_DEFAULT_STUB(OS_MutSemGetInfo_Impl,(uint32 sem_id, OS_mut_sem_prop_t *mut_prop))

/*
** Console output API (printf)
*/
void  OS_ConsoleWakeup_Impl(uint32 local_id)
{
    UT_DEFAULT_IMPL(OS_ConsoleWakeup_Impl);
}
int32 OS_ConsoleCreate_Impl(uint32 local_id)
{
    return UT_DEFAULT_IMPL(OS_ConsoleCreate_Impl);
}
void  OS_ConsoleOutput_Impl(uint32 local_id)
{
    UT_DEFAULT_IMPL(OS_ConsoleOutput_Impl);
}

/*
** Shared memory API
*/
UT_DEFAULT_STUB(OS_ShMemInit_Impl,(void))
UT_DEFAULT_STUB(OS_ShMemCreate_Impl,(uint32 *Id, uint32 NBytes, char* SegName))
UT_DEFAULT_STUB(OS_ShMemSemTake_Impl,(uint32 Id))
UT_DEFAULT_STUB(OS_ShMemSemGive_Impl,(uint32 Id))
UT_DEFAULT_STUB(OS_ShMemAttach_Impl,(uint32 * Address, uint32 Id))
UT_DEFAULT_STUB(OS_ShMemGetIdByName_Impl,(uint32 *ShMemId, const char *SegName ))


/*
** Call to exit the running application
** Normally embedded applications run forever, but for debugging purposes
** (unit testing for example) this is needed in order to end the test
*/
void OS_ApplicationExit_Impl(int32 Status)
{
    UT_DEFAULT_IMPL(OS_ApplicationExit_Impl);
}

/*
 * Interrupt API low-level handlers
 */
UT_DEFAULT_STUB(OS_IntAttachHandler_Impl,(uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter))
UT_DEFAULT_STUB(OS_IntUnlock_Impl,(int32 IntLevel))
UT_DEFAULT_STUB(OS_IntLock_Impl,( void ))
UT_DEFAULT_STUB(OS_IntEnable_Impl,(int32 Level))
UT_DEFAULT_STUB(OS_IntDisable_Impl,(int32 Level))
UT_DEFAULT_STUB(OS_IntSetMask_Impl,( uint32 MaskSetting ))
UT_DEFAULT_STUB(OS_IntGetMask_Impl,( uint32 * MaskSettingPtr ))

/*
 * Heap API low-level handler
 */
UT_DEFAULT_STUB(OS_HeapGetInfo_Impl,(OS_heap_prop_t *heap_prop))

/*
 * FPU API low-level handlers
 */
UT_DEFAULT_STUB(OS_FPUExcAttachHandler_Impl,(uint32 ExceptionNumber, void * ExceptionHandler,int32 parameter))
UT_DEFAULT_STUB(OS_FPUExcEnable_Impl,(int32 ExceptionNumber))
UT_DEFAULT_STUB(OS_FPUExcDisable_Impl,(int32 ExceptionNumber))
UT_DEFAULT_STUB(OS_FPUExcSetMask_Impl,(uint32 mask))
UT_DEFAULT_STUB(OS_FPUExcGetMask_Impl,(uint32 *mask))

