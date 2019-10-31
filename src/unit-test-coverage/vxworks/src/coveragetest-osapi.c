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

#include "os-vxworks-coveragetest.h"
#include "ut-osapi.h"

#include <overrides/stdio.h>
#include <overrides/stdlib.h>
#include <overrides/errno.h>
#include <overrides/intLib.h>
#include <overrides/taskLib.h>
#include <overrides/errnoLib.h>
#include <overrides/semLib.h>
#include <overrides/msgQLib.h>
#include <overrides/memPartLib.h>

/*
 * A chunk of memory usable as a heap for malloc() emulation
 */
unsigned long TestHeap[4096];
int TestGlobalSem;

void Test_OS_Lock_Global_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_Lock_Global_Impl(uint32 idtype)
     */
    OSAPI_TEST_FUNCTION_RC(OS_Lock_Global_Impl(10000), OS_ERROR);

    /*
     * Confirm that if vxid is 0/NULL that the function returns error
     * and does not call semTake.
     */
    Osapi_Internal_SetImplTableMutex(OS_OBJECT_TYPE_OS_TASK, (OCS_SEM_ID)0);
    OSAPI_TEST_FUNCTION_RC(OS_Lock_Global_Impl(OS_OBJECT_TYPE_OS_TASK), OS_ERROR);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_semTake)) == 0, "semTake() NOT called");

    Osapi_Internal_SetImplTableMutex(OS_OBJECT_TYPE_OS_TASK, (OCS_SEM_ID)&TestGlobalSem);
    OSAPI_TEST_FUNCTION_RC(OS_Lock_Global_Impl(OS_OBJECT_TYPE_OS_TASK), OS_SUCCESS);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_semTake)) == 1, "semTake() called");

    UT_SetForceFail(UT_KEY(OCS_semTake), -1);
    OSAPI_TEST_FUNCTION_RC(OS_Lock_Global_Impl(OS_OBJECT_TYPE_OS_TASK), OS_ERROR);
}

void Test_OS_Unlock_Global_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_Unlock_Global_Impl(uint32 idtype)
     */
    OSAPI_TEST_FUNCTION_RC(OS_Unlock_Global_Impl(10000), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_Unlock_Global_Impl(0), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_TASK), OS_SUCCESS);
    UT_SetForceFail(UT_KEY(OCS_semGive), -1);
    OSAPI_TEST_FUNCTION_RC(OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_TASK), OS_ERROR);
}

void Test_OS_API_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_API_Impl_Init(uint32 idtype)
     */
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(0), OS_SUCCESS);
    UT_SetForceFail(UT_KEY(OCS_semMInitialize), -1);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_TASK), OS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_semMInitialize));
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_TASK), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_QUEUE), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_BINSEM), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_COUNTSEM), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_MUTEX), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_MODULE), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_TIMEBASE), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_STREAM), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_API_Impl_Init(OS_OBJECT_TYPE_OS_DIR), OS_SUCCESS);
}

void Test_OS_IdleLoop_Impl(void)
{
    /*
     * Test Case For:
     * void OS_IdleLoop_Impl()
     */
    /* just call the function for coverage */
    OS_IdleLoop_Impl();
}

void Test_OS_ApplicationShutdown_Impl(void)
{
    /*
     * Test Case For:
     * void OS_ApplicationShutdown_Impl()
     */
    /* just call the function for coverage */
    OS_ApplicationShutdown_Impl();
}

void Test_OS_VxWorks_TaskAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_TaskAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_TaskAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_VxWorksEntry(void)
{
    /*
     * Test Case For:
     * static int OS_VxWorksEntry(int arg)
     */
    OSAPI_TEST_FUNCTION_RC(Osapi_Internal_CallEntryPoint(0),OS_SUCCESS);
    UtAssert_True(UT_GetStubCount(UT_KEY(OS_TaskEntryPoint)) == 1, "OS_TaskEntryPoint() called");
}

void Test_OS_TaskCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskCreate_Impl (uint32 task_id, uint32 flags)
     */
    UT_SetDataBuffer(UT_KEY(OCS_malloc), TestHeap, sizeof(TestHeap), false);
    UT_SetDataBuffer(UT_KEY(OCS_free), TestHeap, sizeof(TestHeap), false);

    /* create task with stack size of 250 - this should invoke malloc() to get the stack.
     * The first call checks the failure path and ensures that a malloc failure gets handled */
    OS_task_table[0].stack_size = 250;
    UT_SetForceFail(UT_KEY(OCS_malloc), OS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_TaskCreate_Impl(0,0), OS_ERROR);

    UT_ClearForceFail(UT_KEY(OCS_malloc));
    OSAPI_TEST_FUNCTION_RC(OS_TaskCreate_Impl(0,OS_FP_ENABLED), OS_SUCCESS);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_malloc)) == 2, "malloc() called");
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_free)) == 0, "free() not called");
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_taskInit)) == 1, "taskInit() called");
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_taskActivate)) == 1, "taskActivate() called");

    /* create again with smaller stack - this should re-use existing buffer */
    OS_task_table[0].stack_size = 100;
    OSAPI_TEST_FUNCTION_RC(OS_TaskCreate_Impl(0,OS_FP_ENABLED), OS_SUCCESS);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_malloc)) == 2, "malloc() not called");
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_free)) == 0, "free() not called");
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_taskInit)) == 2, "taskInit() called");
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_taskActivate)) == 2, "taskActivate() called");

    /* create again with larger stack - this should free existing and malloc() new buffer */
    OS_task_table[0].stack_size = 400;
    OSAPI_TEST_FUNCTION_RC(OS_TaskCreate_Impl(0,OS_FP_ENABLED), OS_SUCCESS);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_malloc)) == 3, "malloc() called");
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_free)) == 1, "free() called");
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_taskInit)) == 3, "taskInit() called");
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_taskActivate)) == 3, "taskActivate() called");


    /* other failure modes */
    UT_SetForceFail(UT_KEY(OCS_taskInit), -1);
    OSAPI_TEST_FUNCTION_RC(OS_TaskCreate_Impl(0,0), OS_ERROR);

}

void Test_OS_TaskMatch_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskMatch_Impl(uint32 task_id)
     */
    Osapi_Internal_SetImplTaskId(0, OCS_taskIdSelf());
    OSAPI_TEST_FUNCTION_RC(OS_TaskMatch_Impl(0), OS_SUCCESS);
    Osapi_Internal_SetImplTaskId(0, (OCS_TASK_ID)0);
    OSAPI_TEST_FUNCTION_RC(OS_TaskMatch_Impl(0), OS_ERROR);
}

void Test_OS_TaskDelete_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskDelete_Impl (uint32 task_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_TaskDelete_Impl(0), OS_SUCCESS);

    /* failure mode */
    UT_SetForceFail(UT_KEY(OCS_taskDelete), -1);
    OSAPI_TEST_FUNCTION_RC(OS_TaskDelete_Impl(0), OS_ERROR);
}

void Test_OS_TaskExit_Impl(void)
{
    /*
     * Test Case For:
     * void OS_TaskExit_Impl()
     */
    /* just call the function for coverage */
    OS_TaskExit_Impl();
}

void Test_OS_TaskDelay_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskDelay_Impl(uint32 millisecond)
     */
    OSAPI_TEST_FUNCTION_RC(OS_TaskDelay_Impl(100), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_taskDelay), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_TaskDelay_Impl(100), OS_ERROR);
}

void Test_OS_TaskSetPriority_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskSetPriority_Impl (uint32 task_id, uint32 new_priority)
     */
    OSAPI_TEST_FUNCTION_RC(OS_TaskSetPriority_Impl(0, 100), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_taskPrioritySet), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_TaskSetPriority_Impl(0, 100), OS_ERROR);
}

void Test_OS_TaskRegister_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskRegister_Impl(uint32 global_task_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_TaskRegister_Impl(0), OS_SUCCESS);
}

void Test_OS_TaskGetId_Impl(void)
{
    /*
     * Test Case For:
     * uint32 OS_TaskGetId_Impl (void)
     */
    OCS_WIND_TCB *TaskTcb;

    OS_global_task_table[1].active_id = 0x12345;
    TaskTcb = Osapi_Internal_GetTaskTcb(1);
    UT_SetDataBuffer(UT_KEY(OCS_taskTcb), &TaskTcb, sizeof(TaskTcb), false);
    OSAPI_TEST_FUNCTION_RC(OS_TaskGetId_Impl(), 0x12345);
}

void Test_OS_TaskGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskGetInfo_Impl (uint32 task_id, OS_task_prop_t *task_prop)
     */
    OS_task_prop_t task_prop;
    memset(&task_prop, 0xEE, sizeof(task_prop));
    OSAPI_TEST_FUNCTION_RC(OS_TaskGetInfo_Impl(0,&task_prop), OS_SUCCESS);
}

void Test_OS_VxWorks_QueueAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_QueueAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_QueueAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_QueueCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueCreate_Impl (uint32 queue_id, uint32 flags)
     */
    OSAPI_TEST_FUNCTION_RC(OS_QueueCreate_Impl(0,0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_msgQCreate), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_QueueCreate_Impl(0,0), OS_ERROR);
}

void Test_OS_QueueDelete_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueDelete_Impl (uint32 queue_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_QueueDelete_Impl(0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_msgQDelete), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_QueueDelete_Impl(0), OS_ERROR);
}

void Test_OS_QueueGet_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueGet_Impl (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout)
     */
    char Data[16];
    uint32 ActSz;

    OSAPI_TEST_FUNCTION_RC(OS_QueueGet_Impl(0, &Data, sizeof(Data), &ActSz, OS_PEND), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_QueueGet_Impl(0, &Data, sizeof(Data), &ActSz, OS_CHECK), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_QueueGet_Impl(0, &Data, sizeof(Data), &ActSz, 100), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_msgQReceive), OCS_ERROR);
    OCS_errno = OCS_S_objLib_OBJ_TIMEOUT;
    OSAPI_TEST_FUNCTION_RC(OS_QueueGet_Impl(0, &Data, sizeof(Data), &ActSz, OS_CHECK), OS_QUEUE_TIMEOUT);
    OCS_errno = OCS_S_objLib_OBJ_UNAVAILABLE;
    OSAPI_TEST_FUNCTION_RC(OS_QueueGet_Impl(0, &Data, sizeof(Data), &ActSz, OS_CHECK), OS_QUEUE_EMPTY);
    OCS_errno = 0;
    OSAPI_TEST_FUNCTION_RC(OS_QueueGet_Impl(0, &Data, sizeof(Data), &ActSz, OS_CHECK), OS_ERROR);
}

void Test_OS_QueuePut_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_QueuePut_Impl (uint32 queue_id, const void *data, uint32 size, uint32 flags)
     */
    char Data[16] = "Test";
    OSAPI_TEST_FUNCTION_RC(OS_QueuePut_Impl(0, Data, sizeof(Data), 0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_msgQSend), OCS_ERROR);
    OCS_errno = OCS_S_objLib_OBJ_UNAVAILABLE;
    OSAPI_TEST_FUNCTION_RC(OS_QueuePut_Impl(0, Data, sizeof(Data), 0), OS_QUEUE_FULL);
    OCS_errno = 0;
    OSAPI_TEST_FUNCTION_RC(OS_QueuePut_Impl(0, Data, sizeof(Data), 0), OS_ERROR);
}

void Test_OS_QueueGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueGetInfo_Impl (uint32 queue_id, OS_queue_prop_t *queue_prop)
     */
    OS_queue_prop_t queue_prop;
    memset(&queue_prop, 0xEE, sizeof(queue_prop));
    OSAPI_TEST_FUNCTION_RC(OS_QueueGetInfo_Impl(0,&queue_prop), OS_SUCCESS);
}

void Test_OS_VxWorks_BinSemAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_BinSemAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_BinSemAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_BinSemCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemCreate_Impl (uint32 sem_id, uint32 initial_value, uint32 options)
     */
    OSAPI_TEST_FUNCTION_RC(OS_BinSemCreate_Impl(0,0,0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_semBInitialize), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemCreate_Impl(0,0,0), OS_SEM_FAILURE);
}

void Test_OS_BinSemDelete_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemDelete_Impl (uint32 sem_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_BinSemDelete_Impl(0), OS_SUCCESS);
}

void Test_OS_BinSemGive_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemGive_Impl ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_BinSemGive_Impl(0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_semGive), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemGive_Impl(0), OS_SEM_FAILURE);
}

void Test_OS_BinSemFlush_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemFlush_Impl (uint32 sem_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_BinSemFlush_Impl(0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_semFlush), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemFlush_Impl(0), OS_SEM_FAILURE);
}

void Test_OS_BinSemTake_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemTake_Impl ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_BinSemTake_Impl(0), OS_SUCCESS);
}

void Test_OS_BinSemTimedWait_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemTimedWait_Impl ( uint32 sem_id, uint32 msecs )
     */
    OSAPI_TEST_FUNCTION_RC(OS_BinSemTimedWait_Impl(0,100), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_semTake), OCS_ERROR);
    OCS_errno = OCS_S_objLib_OBJ_TIMEOUT;
    OSAPI_TEST_FUNCTION_RC(OS_BinSemTimedWait_Impl(0,100), OS_SEM_TIMEOUT);
    OCS_errno = 0;
    OSAPI_TEST_FUNCTION_RC(OS_BinSemTimedWait_Impl(0,100), OS_SEM_FAILURE);
}

void Test_OS_BinSemGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemGetInfo_Impl (uint32 sem_id, OS_bin_sem_prop_t *sem_prop)
     */
    OS_bin_sem_prop_t sem_prop;
    memset(&sem_prop, 0xEE, sizeof(sem_prop));
    OSAPI_TEST_FUNCTION_RC(OS_BinSemGetInfo_Impl(0,&sem_prop), OS_SUCCESS);
}

void Test_OS_VxWorks_CountSemAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_CountSemAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_CountSemAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_CountSemCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemCreate_Impl (uint32 sem_id, uint32 sem_initial_value, uint32 options)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CountSemCreate_Impl(0,0,0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_semCInitialize), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_CountSemCreate_Impl(0,0,0), OS_SEM_FAILURE);
}

void Test_OS_CountSemDelete_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemDelete_Impl (uint32 sem_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_CountSemDelete_Impl(0), OS_SUCCESS);
}

void Test_OS_CountSemGive_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemGive_Impl ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_CountSemGive_Impl(0), OS_SUCCESS);
}

void Test_OS_CountSemTake_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemTake_Impl ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_CountSemTake_Impl(0), OS_SUCCESS);
}

void Test_OS_CountSemTimedWait_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemTimedWait_Impl ( uint32 sem_id, uint32 msecs )
     */
    OSAPI_TEST_FUNCTION_RC(OS_CountSemTimedWait_Impl(0, 100), OS_SUCCESS);
}

void Test_OS_CountSemGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemGetInfo_Impl (uint32 sem_id, OS_count_sem_prop_t *count_prop)
     */
    OS_count_sem_prop_t count_prop;
    memset(&count_prop, 0xEE, sizeof(count_prop));
    OSAPI_TEST_FUNCTION_RC(OS_CountSemGetInfo_Impl(0, &count_prop), OS_SUCCESS);
}

void Test_OS_VxWorks_MutexAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_MutexAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(OS_VxWorks_MutexAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_MutSemCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemCreate_Impl (uint32 sem_id, uint32 options)
     */
    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate_Impl(0,0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_semMInitialize), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate_Impl(0,0), OS_SEM_FAILURE);
}

void Test_OS_MutSemDelete_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemDelete_Impl (uint32 sem_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_MutSemDelete_Impl(0), OS_SUCCESS);
}

void Test_OS_MutSemGive_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGive_Impl ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_MutSemGive_Impl(0), OS_SUCCESS);
}

void Test_OS_MutSemTake_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemTake_Impl ( uint32 sem_id )
     */
    OSAPI_TEST_FUNCTION_RC(OS_MutSemTake_Impl(0), OS_SUCCESS);
}

void Test_OS_MutSemGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGetInfo_Impl (uint32 sem_id, OS_mut_sem_prop_t *mut_prop)
     */
    OS_mut_sem_prop_t mut_prop;
    memset(&mut_prop, 0xEE, sizeof(mut_prop));
    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetInfo_Impl(0,&mut_prop), OS_SUCCESS);
}

void Test_OS_IntAttachHandler_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntAttachHandler_Impl  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter)
     */
    OSAPI_TEST_FUNCTION_RC(OS_IntAttachHandler_Impl(0,NULL,0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_intConnect), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_IntAttachHandler_Impl(0,NULL,0), OS_ERROR);
}

void Test_OS_IntUnlock_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntUnlock_Impl (int32 IntLevel)
     */
    OSAPI_TEST_FUNCTION_RC(OS_IntUnlock_Impl(0), OS_SUCCESS);
}

void Test_OS_IntLock_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntLock_Impl ( void )
     */
    UT_SetForceFail(UT_KEY(OCS_intLock), 0x1111);
    OSAPI_TEST_FUNCTION_RC(OS_IntLock_Impl(), 0x1111);
}

void Test_OS_IntEnable_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntEnable_Impl(int32 Level)
     */
    OSAPI_TEST_FUNCTION_RC(OS_IntEnable_Impl(0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_intEnable), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_IntEnable_Impl(0), OS_ERROR);
}

void Test_OS_IntDisable_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntDisable_Impl(int32 Level)
     */
    OSAPI_TEST_FUNCTION_RC(OS_IntDisable_Impl(0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_intDisable), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_IntDisable_Impl(0), OS_ERROR);
}

void Test_OS_HeapGetInfo_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_HeapGetInfo_Impl(OS_heap_prop_t *heap_prop)
     */
    OS_heap_prop_t heap_prop;

    memset(&heap_prop, 0xEE, sizeof(heap_prop));
    OSAPI_TEST_FUNCTION_RC(OS_HeapGetInfo_Impl(&heap_prop), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_memPartInfoGet), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_HeapGetInfo_Impl(&heap_prop), OS_ERROR);
}

void Test_OS_IntSetMask_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntSetMask_Impl ( uint32 MaskSetting )
     */
    OSAPI_TEST_FUNCTION_RC(OS_IntSetMask_Impl(0x1111), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_IntGetMask_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_IntGetMask_Impl ( uint32 * MaskSettingPtr )
     */
    uint32 Mask = 0x1111;
    OSAPI_TEST_FUNCTION_RC(OS_IntGetMask_Impl(&Mask), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_FPUExcAttachHandler_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FPUExcAttachHandler_Impl(uint32 ExceptionNumber, void * ExceptionHandler,
     *       int32 parameter)
     */
    OSAPI_TEST_FUNCTION_RC(OS_FPUExcAttachHandler_Impl(0, NULL, 0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_FPUExcEnable_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FPUExcEnable_Impl(int32 ExceptionNumber)
     */
    OSAPI_TEST_FUNCTION_RC(OS_FPUExcEnable_Impl(0), OS_SUCCESS);
}

void Test_OS_FPUExcDisable_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FPUExcDisable_Impl(int32 ExceptionNumber)
     */
    OSAPI_TEST_FUNCTION_RC(OS_FPUExcDisable_Impl(0), OS_SUCCESS);
}

void Test_OS_FPUExcSetMask_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FPUExcSetMask_Impl(uint32 mask)
     */
    OSAPI_TEST_FUNCTION_RC(OS_FPUExcSetMask_Impl(0), OS_ERR_NOT_IMPLEMENTED);
}

void Test_OS_FPUExcGetMask_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_FPUExcGetMask_Impl(uint32 *mask)
     */
    OSAPI_TEST_FUNCTION_RC(OS_FPUExcGetMask_Impl(0), OS_ERR_NOT_IMPLEMENTED);
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
    Osapi_Internal_ResetState();
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
    ADD_TEST(OS_VxWorks_TaskAPI_Impl_Init);
    ADD_TEST(OS_TaskCreate_Impl);
    ADD_TEST(OS_VxWorksEntry);
    ADD_TEST(OS_TaskMatch_Impl);
    ADD_TEST(OS_TaskDelete_Impl);
    ADD_TEST(OS_TaskExit_Impl);
    ADD_TEST(OS_TaskDelay_Impl);
    ADD_TEST(OS_TaskSetPriority_Impl);
    ADD_TEST(OS_TaskRegister_Impl);
    ADD_TEST(OS_TaskGetId_Impl);
    ADD_TEST(OS_TaskGetInfo_Impl);
    ADD_TEST(OS_VxWorks_QueueAPI_Impl_Init);
    ADD_TEST(OS_QueueCreate_Impl);
    ADD_TEST(OS_QueueDelete_Impl);
    ADD_TEST(OS_QueueGet_Impl);
    ADD_TEST(OS_QueuePut_Impl);
    ADD_TEST(OS_QueueGetInfo_Impl);
    ADD_TEST(OS_VxWorks_BinSemAPI_Impl_Init);
    ADD_TEST(OS_BinSemCreate_Impl);
    ADD_TEST(OS_BinSemDelete_Impl);
    ADD_TEST(OS_BinSemGive_Impl);
    ADD_TEST(OS_BinSemFlush_Impl);
    ADD_TEST(OS_BinSemTake_Impl);
    ADD_TEST(OS_BinSemTimedWait_Impl);
    ADD_TEST(OS_BinSemGetInfo_Impl);
    ADD_TEST(OS_VxWorks_CountSemAPI_Impl_Init);
    ADD_TEST(OS_CountSemCreate_Impl);
    ADD_TEST(OS_CountSemDelete_Impl);
    ADD_TEST(OS_CountSemGive_Impl);
    ADD_TEST(OS_CountSemTake_Impl);
    ADD_TEST(OS_CountSemTimedWait_Impl);
    ADD_TEST(OS_CountSemGetInfo_Impl);
    ADD_TEST(OS_VxWorks_MutexAPI_Impl_Init);
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
    ADD_TEST(OS_FPUExcAttachHandler_Impl);
    ADD_TEST(OS_FPUExcEnable_Impl);
    ADD_TEST(OS_FPUExcDisable_Impl);
    ADD_TEST(OS_FPUExcSetMask_Impl);
    ADD_TEST(OS_FPUExcGetMask_Impl);
}


