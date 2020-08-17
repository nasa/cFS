/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * Filename: idmap-api-test.c
 *
 * Purpose: This file contains functional tests for "osapi-idmap"
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"


uint32 task_id;
uint32 queue_id;
uint32 count_sem_id;
uint32 bin_sem_id;
uint32 mutex_id1;
uint32 mutex_id2;
uint32 mutex_id3;
uint32 time_base_id;

#define UT_EXIT_LOOP_MAX    100

/* *************************************** MAIN ************************************** */

typedef struct
{
    uint32 TaskCount;
    uint32 QueueCount;
    uint32 CountSemCount;
    uint32 BinSemCount;
    uint32 MutexCount;
    uint32 TimeBaseCount;
    uint32 OtherCount;
} Test_OS_ObjTypeCount_t;

static void ObjTypeCounter(uint32 object_id, void *arg)
{
    Test_OS_ObjTypeCount_t *count = arg;

    switch(OS_IdentifyObject(object_id))
    {
    case OS_OBJECT_TYPE_OS_TASK:
        ++count->TaskCount;
        break;
    case OS_OBJECT_TYPE_OS_QUEUE:
        ++count->QueueCount;
        break;
    case OS_OBJECT_TYPE_OS_COUNTSEM:
        ++count->CountSemCount;
        break;
    case OS_OBJECT_TYPE_OS_BINSEM:
        ++count->BinSemCount;
        break;
    case OS_OBJECT_TYPE_OS_MUTEX:
        ++count->MutexCount;
        break;
    case OS_OBJECT_TYPE_OS_TIMEBASE:
        ++count->TimeBaseCount;
        break;
    default:
        ++count->OtherCount;
        break;
    }
}

/*
* A void test function that creates an object for testing
*/
void Test_Void_Fn(void)
{
    uint32 bin_sem_id_my_task;
    OS_BinSemCreate( &bin_sem_id_my_task, "BinSemTaskMyTask", 1, 0);
    OS_TaskDelay(5);

} /* end Test_Void_Fn */

void TestIdMapApi_Setup(void)
{
    uint32 loopcnt;
    int32 status;
    OS_task_prop_t taskprop;

    /*
     * Create all allowed objects
     */
    status = OS_TaskCreate( &task_id, "Task", Test_Void_Fn, NULL, 4096, 50, 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_QueueCreate( &queue_id, "Queue", 5, 5, 0);
    UtAssert_True(status == OS_SUCCESS, "OS_QueueCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_CountSemCreate( &count_sem_id, "CountSem", 1, 0);
    UtAssert_True(status == OS_SUCCESS, "OS_CountSemCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_BinSemCreate( &bin_sem_id, "BinSem", 1, 0);
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_MutSemCreate( &mutex_id1, "Mutex1", 0);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_MutSemCreate( &mutex_id2, "Mutex2", 0);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_MutSemCreate( &mutex_id3, "Mutex3", 0);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate() (%ld) == OS_SUCCESS", (long)status);
    status = OS_TimeBaseCreate( &time_base_id, "TimeBase", 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)status);

    /* Looping delay in parent task to wait for child task to exit */
    loopcnt = 0;
    while ((OS_TaskGetInfo(task_id, &taskprop) == OS_SUCCESS) && (loopcnt < UT_EXIT_LOOP_MAX))
    {
       OS_TaskDelay(10);
       loopcnt++;
    }
    UtAssert_True(loopcnt < UT_EXIT_LOOP_MAX, "Task exited after %ld iterations", (long)loopcnt);
}
/* *************************************** MAIN ************************************** */

void TestIdMapApi(void)
{
    int32 expected;
    int32 actual;
    uint32 TestArrayIndex; 
    uint32 TestMutex1Index; 
    uint32 TestMutex2Index; 
    Test_OS_ObjTypeCount_t Count;

    /*
     * NOTE: The following objects were not created and tested:
     * OS_OBJECT_TYPE_OS_STREAM
     * OS_OBJECT_TYPE_OS_DIR
     * OS_OBJECT_TYPE_OS_TIMECB
     * OS_OBJECT_TYPE_OS_FILESYS
     * OS_OBJECT_TYPE_OS_CONSOLE
     * OS_OBJECT_TYPE_USER
     */

    /*
     * Test Case For:
     * int32 OS_IdentifyObject(void)
     */

    /*
     * Test with nominal values
     */
    expected = OS_OBJECT_TYPE_OS_TASK;
    actual   = OS_IdentifyObject(task_id); 
    UtAssert_True(actual == expected, "OS_IdentifyObject() (%ld) == %ld", (long)actual, (long)expected); 

    expected = OS_OBJECT_TYPE_OS_QUEUE;
    actual   = OS_IdentifyObject(queue_id); 
    UtAssert_True(actual == expected, "OS_IdentifyObject() (%ld) == %ld", (long)actual, (long)expected); 

    expected = OS_OBJECT_TYPE_OS_COUNTSEM;
    actual   = OS_IdentifyObject(count_sem_id); 
    UtAssert_True(actual == expected, "OS_IdentifyObject() (%ld) == %ld", (long)actual, (long)expected); 

    expected = OS_OBJECT_TYPE_OS_BINSEM;
    actual   = OS_IdentifyObject(bin_sem_id); 
    UtAssert_True(actual == expected, "OS_IdentifyObject() (%ld) == %ld", (long)actual, (long)expected); 

    expected = OS_OBJECT_TYPE_OS_MUTEX;
    actual   = OS_IdentifyObject(mutex_id1); 
    UtAssert_True(actual == expected, "OS_IdentifyObject() (%ld) == %ld", (long)actual, (long)expected);   

    expected = OS_OBJECT_TYPE_OS_TIMEBASE;
    actual   = OS_IdentifyObject(time_base_id); 
    UtAssert_True(actual == expected, "OS_IdentifyObject() (%ld) == %ld", (long)actual, (long)expected);  

    /*
     * Test with extreme cases using min and max values
     * Note: There are no asserts, checks or expected values 
     * here.  The only check is that the function doesn't return
     * an error when called
     */
    OS_IdentifyObject(0x00000); 
    OS_IdentifyObject(0xFFFFFFFF); 

    /*
     * Test Case For:
     * int32 OS_ConvertToArrayIndex(void)
     */
    expected = OS_SUCCESS;

    /*
     * Check different id types and verify array indices
     * Each Object Type index is added to an array index of its own type
     * Each object type is checked once, and MUTEX is checked twice to 
     * verify multiple indices
     */

    /*
     * Test with nominal values
     */
    actual   = OS_ConvertToArrayIndex(task_id, &TestArrayIndex);
    UtAssert_True(actual == expected , "OS_ConvertToArrayIndex() (%ld) == %ld ", (long)actual, (long)expected );
    UtAssert_True(TestArrayIndex >= 0 && TestArrayIndex < OS_MAX_TASKS , "0 < TestArrayIndex(%lu)  <= OS_MAX_TASKS", (long)TestArrayIndex);

    actual   = OS_ConvertToArrayIndex(queue_id, &TestArrayIndex);
    UtAssert_True(actual == expected , "OS_ConvertToArrayIndex() (%ld) == %ld ", (long)actual, (long)expected );
    UtAssert_True(TestArrayIndex >=0 && TestArrayIndex < OS_MAX_QUEUES , "0 < TestArrayIndex(%lu)  <= OS_MAX_QUEUES", (long)TestArrayIndex);

    actual   = OS_ConvertToArrayIndex(count_sem_id, &TestArrayIndex);
    UtAssert_True(actual == expected , "OS_ConvertToArrayIndex() (%ld) == %ld ", (long)actual, (long)expected );
    UtAssert_True(TestArrayIndex >= 0 && TestArrayIndex < OS_MAX_COUNT_SEMAPHORES , "0 < TestArrayIndex(%lu)  <= OS_MAX_COUNT_SEMAPHORES", (long)TestArrayIndex);

    actual   = OS_ConvertToArrayIndex(bin_sem_id, &TestArrayIndex);
    UtAssert_True(actual == expected , "OS_ConvertToArrayIndex() (%ld) == %ld ", (long)actual, (long)expected );
    UtAssert_True(TestArrayIndex >= 0 && TestArrayIndex < OS_MAX_BIN_SEMAPHORES , "0 < TestArrayIndex(%lu)  <= OS_MAX_BIN_SEMAPHORES", (long)TestArrayIndex);

    actual   = OS_ConvertToArrayIndex(mutex_id1, &TestMutex1Index);
    UtAssert_True(actual == expected , "OS_ConvertToArrayIndex() (%ld) == %ld ", (long)actual, (long)expected );
    UtAssert_True(TestMutex1Index >= 0 && TestMutex1Index < OS_MAX_MUTEXES , "0 < TestMutex1Index(%lu)  <= OS_MAX_MUTEXES", (long)TestMutex1Index);

    actual   = OS_ConvertToArrayIndex(mutex_id2, &TestMutex2Index);
    UtAssert_True(actual == expected , "OS_ConvertToArrayIndex() (%ld) == %ld ", (long)actual, (long)expected );
    UtAssert_True(TestMutex2Index >= 0 && TestMutex2Index < OS_MAX_MUTEXES , "0 < TestMutex2Index(%lu)  <= OS_MAX_MUTEXES", (long)TestMutex2Index);
    UtAssert_True(TestMutex1Index != TestMutex2Index , "TestMutex1Index(%lu) !=  TestMutex2Index(%lu)", (long)TestMutex1Index, (long)TestMutex2Index );

    actual   = OS_ConvertToArrayIndex(time_base_id, &TestArrayIndex);
    UtAssert_True(actual == expected , "OS_ConvertToArrayIndex() (%ld) == %ld ", (long)actual, (long)expected );
    UtAssert_True(TestArrayIndex >= 0 && TestArrayIndex < OS_MAX_TIMEBASES , "0 < TestArrayIndex(%lu)  <= OS_MAX_TIMEBASES", (long)TestArrayIndex);

    /*
     * Test with extreme cases using invalid inputs and checking
     * for an error return code 
     */
    actual   = OS_ConvertToArrayIndex(0x0000, &TestArrayIndex);
    expected = OS_ERR_INVALID_ID;
    UtAssert_True(actual == expected , "OS_ConvertToArrayIndex() (%ld) == %ld ", (long)actual, (long)expected );

    actual   = OS_ConvertToArrayIndex(0xFFFFFFFF, &TestArrayIndex);
    expected = OS_ERR_INVALID_ID;
    UtAssert_True(actual == expected , "OS_ConvertToArrayIndex() (%ld) == %ld ", (long)actual, (long)expected );

    /*
     * Test Case For:
     * void OS_ForEachObject (uint32 creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg);
     */
    memset(&Count, 0, sizeof(Count));

    OS_ForEachObject (0, &ObjTypeCounter, &Count);

    /* Verify Outputs */
    UtAssert_True(Count.TaskCount == 0, "OS_ForEachObject() TaskCount (%lu) == 0", (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 1, "OS_ForEachObject() QueueCount (%lu) == 1", (unsigned long)Count.QueueCount);
    UtAssert_True(Count.CountSemCount == 1, "OS_ForEachObject() CountSemCount (%lu) == 1", (unsigned long)Count.CountSemCount);
    UtAssert_True(Count.BinSemCount == 2, "OS_ForEachObject() BinSemCount (%lu) == 2", (unsigned long)Count.BinSemCount);
    UtAssert_True(Count.MutexCount == 3, "OS_ForEachObject() MutexCount (%lu) == 3", (unsigned long)Count.MutexCount);
    UtAssert_True(Count.TimeBaseCount == 1, "OS_ForEachObject() TimeBaseCount (%lu) == 1", (unsigned long)Count.TimeBaseCount);

    /*
     * Use current task as an input
     */
    memset(&Count, 0, sizeof(Count));
    OS_ForEachObject (task_id, &ObjTypeCounter, &Count);

    /* Verify Output */
    UtAssert_True(Count.BinSemCount == 1, "OS_ForEachObject() BinSemCount MyTask (%lu) == 1", (unsigned long)Count.BinSemCount);

    /*
     * Delete all created objects, and verify that the count is now zero
     */
    memset(&Count, 0, sizeof(Count));
    OS_DeleteAllObjects();
    OS_ForEachObject (0, &ObjTypeCounter, &Count);

    /* Verify Outputs */
    UtAssert_True(Count.TaskCount == 0, "OS_ForEachObject() TaskCount After Delete (%lu) == 0", (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 0, "OS_ForEachObject() QueueCount After Delete (%lu) == 0", (unsigned long)Count.QueueCount);
    UtAssert_True(Count.CountSemCount == 0, "OS_ForEachObject() CountSemCount After Delete (%lu) == 0", (unsigned long)Count.CountSemCount);
    UtAssert_True(Count.BinSemCount == 0, "OS_ForEachObject() BinSemCount After Delete (%lu) == 0", (unsigned long)Count.BinSemCount);
    UtAssert_True(Count.MutexCount == 0, "OS_ForEachObject() MutexCount After Delete (%lu) == 0", (unsigned long)Count.MutexCount);
    UtAssert_True(Count.TimeBaseCount == 0, "OS_ForEachObject() TimeBaseCount After Delete (%lu) == 0", (unsigned long)Count.TimeBaseCount);

    /*
     * Pass an invalid input, and verify that object counts are not increased
     */
    OS_ForEachObject (0xFFFFFFFF, &ObjTypeCounter, &Count); 

    /* Verify Outputs */
    UtAssert_True(Count.TaskCount == 0, "OS_ForEachObject() TaskCount Invalid Input (%lu) == 0", (unsigned long)Count.TaskCount);
    UtAssert_True(Count.QueueCount == 0, "OS_ForEachObject() QueueCount Invalid Input (%lu) == 0", (unsigned long)Count.QueueCount);
    UtAssert_True(Count.CountSemCount == 0, "OS_ForEachObject() CountSemCount Invalid Input (%lu) == 0", (unsigned long)Count.CountSemCount);
    UtAssert_True(Count.BinSemCount == 0, "OS_ForEachObject() BinSemCount Invalid Input (%lu) == 0", (unsigned long)Count.BinSemCount);
    UtAssert_True(Count.MutexCount == 0, "OS_ForEachObject() MutexCount Invalid Input (%lu) == 0", (unsigned long)Count.MutexCount);
    UtAssert_True(Count.TimeBaseCount == 0, "OS_ForEachObject() TimeBaseCount Invalid Input (%lu) == 0", (unsigned long)Count.TimeBaseCount);




} /* end TestIdMapApi */


void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(TestIdMapApi, TestIdMapApi_Setup, NULL, "TestIdMapApi");
}

