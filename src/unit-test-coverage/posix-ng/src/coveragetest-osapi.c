/*
 * Filename: osapi_testcase_common.c
 *
 * Purpose: This file contains unit test cases for items in the "osapi-common" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include <utassert.h>
#include <uttest.h>
#include <utstubs.h>

#include <osapi.h>
#include <os-impl.h>

#include <overrides/stdlib.h>


void Test_OS_Lock_Global_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_Lock_Global_Impl(uint32 idtype)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_Lock_Global_Impl(1);

    UtAssert_True(actual == expected, "OS_Lock_Global_Impl() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_Unlock_Global_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_Unlock_Global_Impl(uint32 idtype)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_Unlock_Global_Impl(1);

    UtAssert_True(actual == expected, "OS_Unlock_Global_Impl() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_API_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_API_Impl_Init(uint32 idtype)
     */
}

void Test_OS_IdleLoop_Impl(void)
{
    /*
     * Test Case For:
     * void OS_IdleLoop_Impl()
     */
}

void Test_OS_ApplicationShutdown_Impl(void)
{
    /*
     * Test Case For:
     * void OS_ApplicationShutdown_Impl()
     */
}

void Test_OS_PthreadTaskEntry(void)
{
    /*
     * Test Case For:
     * void *OS_PthreadTaskEntry(void *arg)
     */
}

void Test_OS_Posix_GetSchedulerParams(void)
{
    /*
     * Test Case For:
     * bool OS_Posix_GetSchedulerParams(int sched_policy, POSIX_PriorityLimits_t *PriLim)
     */
}

void Test_OS_Posix_TaskAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_Posix_TaskAPI_Impl_Init(void)
     */
}

void Test_OS_Posix_InternalTaskCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_Posix_InternalTaskCreate_Impl(pthread_t *pthr, uint32 priority, size_t stacksz, PthreadFuncPtr_t entry, void *entry_arg)
     */
}

void Test_OS_TaskCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskCreate_Impl (uint32 task_id, uint32 flags)
     */
}

void Test_OS_TaskMatch_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskMatch_Impl(uint32 task_id)
     */
}

void Test_OS_TaskDelete_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskDelete_Impl (uint32 task_id)
     */
}

void Test_OS_TaskExit_Impl(void)
{
    /*
     * Test Case For:
     * void OS_TaskExit_Impl()
     */
}

void Test_OS_TaskDelay_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskDelay_Impl(uint32 millisecond)
     */
}

void Test_OS_TaskSetPriority_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskSetPriority_Impl (uint32 task_id, uint32 new_priority)
     */
}

void Test_OS_TaskRegister_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskRegister_Impl(uint32 global_task_id)
     */
}

void Test_OS_TaskGetId_Impl(void)
{
    /*
     * Test Case For:
     * uint32 OS_TaskGetId_Impl (void)
     */
}

void Test_OS_TaskGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskGetInfo_Impl (uint32 task_id, OS_task_prop_t *task_prop)
     */
}

void Test_OS_Posix_QueueAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_Posix_QueueAPI_Impl_Init(void)
     */
}

void Test_OS_QueueCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueCreate_Impl (uint32 queue_id, uint32 flags)
     */
}

void Test_OS_QueueDelete_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueDelete_Impl (uint32 queue_id)
     */
}

void Test_OS_QueueGet_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueGet_Impl (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout)
     */
}

void Test_OS_QueuePut_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_QueuePut_Impl (uint32 queue_id, const void *data, uint32 size, uint32 flags)
     */
}

void Test_OS_Posix_BinSemAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_Posix_BinSemAPI_Impl_Init(void)
     */
}

void Test_OS_BinSemCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemCreate_Impl (uint32 sem_id, uint32 initial_value, uint32 options)
     */
}

void Test_OS_BinSemDelete_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemDelete_Impl (uint32 sem_id)
     */
}

void Test_OS_BinSemGive_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemGive_Impl ( uint32 sem_id )
     */
}

void Test_OS_BinSemFlush_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemFlush_Impl (uint32 sem_id)
     */
}

void Test_OS_BinSemTake_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemTake_Impl ( uint32 sem_id )
     */
}

void Test_OS_BinSemTimedWait_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemTimedWait_Impl ( uint32 sem_id, uint32 msecs )
     */
}

void Test_OS_BinSemGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemGetInfo_Impl (uint32 sem_id, OS_bin_sem_prop_t *sem_prop)
     */
}

void Test_OS_Posix_CountSemAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_Posix_CountSemAPI_Impl_Init(void)
     */
}

void Test_OS_CountSemCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemCreate_Impl (uint32 sem_id, uint32 sem_initial_value, uint32 options)
     */
}

void Test_OS_CountSemDelete_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemDelete_Impl (uint32 sem_id)
     */
}

void Test_OS_CountSemGive_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemGive_Impl ( uint32 sem_id )
     */
}

void Test_OS_CountSemTake_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemTake_Impl ( uint32 sem_id )
     */
}

void Test_OS_CountSemTimedWait_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemTimedWait_Impl ( uint32 sem_id, uint32 msecs )
     */
}

void Test_OS_CountSemGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemGetInfo_Impl (uint32 sem_id, OS_count_sem_prop_t *count_prop)
     */
}

void Test_OS_Posix_MutexAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_Posix_MutexAPI_Impl_Init(void)
     */
}

void Test_OS_MutSemCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemCreate_Impl (uint32 sem_id, uint32 options)
     */
}

void Test_OS_MutSemDelete_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemDelete_Impl (uint32 sem_id)
     */
}

void Test_OS_MutSemGive_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGive_Impl ( uint32 sem_id )
     */
}

void Test_OS_MutSemTake_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemTake_Impl ( uint32 sem_id )
     */
}

void Test_OS_MutSemGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGetInfo_Impl (uint32 sem_id, OS_mut_sem_prop_t *mut_prop)
     */
}

void Test_OS_IntAttachHandler_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntAttachHandler_Impl  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter)
     */
}

void Test_OS_IntUnlock_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntUnlock_Impl (int32 IntLevel)
     */
}

void Test_OS_IntLock_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntLock_Impl ( void )
     */
}

void Test_OS_IntEnable_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntEnable_Impl(int32 Level)
     */
}

void Test_OS_IntDisable_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntDisable_Impl(int32 Level)
     */
}

void Test_OS_HeapGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_HeapGetInfo_Impl(OS_heap_prop_t *heap_prop)
     */
}

void Test_OS_IntSetMask_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntSetMask_Impl ( uint32 MaskSetting )
     */
}

void Test_OS_IntGetMask_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntGetMask_Impl ( uint32 * MaskSettingPtr )
     */
}

void Test_OS_CompAbsDelayTime(void)
{
    /*
     * Test Case For:
     * void  OS_CompAbsDelayTime( uint32 msecs, struct timespec * tm)
     */
}

void Test_OS_FPUExcAttachHandler_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FPUExcAttachHandler_Impl(uint32 ExceptionNumber, void * ExceptionHandler,
     *       int32 parameter)
     */
}

void Test_OS_FPUExcEnable_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FPUExcEnable_Impl(int32 ExceptionNumber)
     */
}

void Test_OS_FPUExcDisable_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FPUExcDisable_Impl(int32 ExceptionNumber)
     */
}

void Test_OS_FPUExcSetMask_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FPUExcSetMask_Impl(uint32 mask)
     */
}

void Test_OS_FPUExcGetMask_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FPUExcGetMask_Impl(uint32 *mask)
     */
}

/* ------------------- End of test cases --------------------------------------*/

/* Osapi_Task_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Task_Setup(void)
{
    UT_ResetState(0);
}

/*
 * Osapi_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_TearDown(void)
{

}


#define ADD_TEST(test) UtTest_Add((Test_ ## test), Osapi_Task_Setup, Osapi_TearDown, #test)

/* Osapi_AddTestCase_Tasks
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    ADD_TEST(OS_Lock_Global_Impl);
    ADD_TEST(OS_Unlock_Global_Impl);
    ADD_TEST(OS_API_Impl_Init);
    ADD_TEST(OS_IdleLoop_Impl);
    ADD_TEST(OS_ApplicationShutdown_Impl);
    ADD_TEST(OS_PthreadTaskEntry);
    ADD_TEST(OS_Posix_GetSchedulerParams);
    ADD_TEST(OS_Posix_TaskAPI_Impl_Init);
    ADD_TEST(OS_Posix_InternalTaskCreate_Impl);
    ADD_TEST(OS_TaskCreate_Impl);
    ADD_TEST(OS_TaskMatch_Impl);
    ADD_TEST(OS_TaskDelete_Impl);
    ADD_TEST(OS_TaskExit_Impl);
    ADD_TEST(OS_TaskDelay_Impl);
    ADD_TEST(OS_TaskSetPriority_Impl);
    ADD_TEST(OS_TaskRegister_Impl);
    ADD_TEST(OS_TaskGetId_Impl);
    ADD_TEST(OS_TaskGetInfo_Impl);
    ADD_TEST(OS_Posix_QueueAPI_Impl_Init);
    ADD_TEST(OS_QueueCreate_Impl);
    ADD_TEST(OS_QueueDelete_Impl);
    ADD_TEST(OS_QueueGet_Impl);
    ADD_TEST(OS_QueuePut_Impl);
    ADD_TEST(OS_Posix_BinSemAPI_Impl_Init);
    ADD_TEST(OS_BinSemCreate_Impl);
    ADD_TEST(OS_BinSemDelete_Impl);
    ADD_TEST(OS_BinSemGive_Impl);
    ADD_TEST(OS_BinSemFlush_Impl);
    ADD_TEST(OS_BinSemTake_Impl);
    ADD_TEST(OS_BinSemTimedWait_Impl);
    ADD_TEST(OS_BinSemGetInfo_Impl);
    ADD_TEST(OS_Posix_CountSemAPI_Impl_Init);
    ADD_TEST(OS_CountSemCreate_Impl);
    ADD_TEST(OS_CountSemDelete_Impl);
    ADD_TEST(OS_CountSemGive_Impl);
    ADD_TEST(OS_CountSemTake_Impl);
    ADD_TEST(OS_CountSemTimedWait_Impl);
    ADD_TEST(OS_CountSemGetInfo_Impl);
    ADD_TEST(OS_Posix_MutexAPI_Impl_Init);
    ADD_TEST(OS_MutSemCreate_Impl);
    ADD_TEST(OS_MutSemDelete_Impl);
    ADD_TEST(OS_MutSemGive_Impl);
    ADD_TEST(OS_MutSemTake_Impl);
    ADD_TEST(OS_MutSemGetInfo_Impl);
    ADD_TEST(OS_IntAttachHandler_Impl);
    ADD_TEST(OS_IntUnlock_Impl);
    ADD_TEST(OS_IntLock_Impl);
    ADD_TEST(OS_IntEnable_Impl);
    ADD_TEST(OS_IntDisable_Impl);
    ADD_TEST(OS_HeapGetInfo_Impl);
    ADD_TEST(OS_IntSetMask_Impl);
    ADD_TEST(OS_IntGetMask_Impl);
    ADD_TEST(OS_CompAbsDelayTime);
    ADD_TEST(OS_FPUExcAttachHandler_Impl);
    ADD_TEST(OS_FPUExcEnable_Impl);
    ADD_TEST(OS_FPUExcDisable_Impl);
    ADD_TEST(OS_FPUExcSetMask_Impl);
    ADD_TEST(OS_FPUExcGetMask_Impl);
}


