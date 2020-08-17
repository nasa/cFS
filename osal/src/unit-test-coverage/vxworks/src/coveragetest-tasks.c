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

/**
 * \file     coveragetest-tasks.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-tasks.h"

#include "os-shared-task.h"
#include "os-shared-idmap.h"

#include <OCS_stdlib.h>

/*
 * A chunk of memory usable as a heap for malloc() emulation
 */
unsigned long TestHeap[4096];
int TestGlobalSem;

void Test_OS_VxWorks_TaskAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_TaskAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(UT_Call_OS_VxWorks_TaskAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_VxWorksEntry(void)
{
    /*
     * Test Case For:
     * static int OS_VxWorksEntry(int arg)
     */
    OSAPI_TEST_FUNCTION_RC(UT_TaskTest_CallEntryPoint(0),OS_SUCCESS);
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
    UT_TaskTest_SetImplTaskId(0, OCS_taskIdSelf());
    OSAPI_TEST_FUNCTION_RC(OS_TaskMatch_Impl(0), OS_SUCCESS);
    UT_TaskTest_SetImplTaskId(0, (OCS_TASK_ID)0);
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
    TaskTcb = UT_TaskTest_GetTaskTcb(1);
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

void Test_OS_TaskValidateSystemData_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_TaskValidateSystemData_Impl(const void *sysdata, uint32 sysdata_size)
     */
    OCS_TASK_ID test_sys_id;

    memset(&test_sys_id, 'x', sizeof(test_sys_id));

    OSAPI_TEST_FUNCTION_RC(OS_TaskValidateSystemData_Impl(&test_sys_id, sizeof(test_sys_id)), OS_SUCCESS);
    OSAPI_TEST_FUNCTION_RC(OS_TaskValidateSystemData_Impl(NULL, sizeof(test_sys_id)), OS_INVALID_POINTER);
    OSAPI_TEST_FUNCTION_RC(OS_TaskValidateSystemData_Impl(&test_sys_id, sizeof(test_sys_id)-1), OS_INVALID_POINTER);
}

void Test_OS_TaskIdMatchSystemData_Impl(void)
{
    /*
     * Test Case For:
     * bool OS_TaskIdMatchSystemData_Impl(void *ref, uint32 local_id, const OS_common_record_t *obj)
     */
    OCS_TASK_ID test_sys_id;

    memset(&test_sys_id, 'x', sizeof(test_sys_id));

    UT_TaskTest_SetImplTaskId(0, test_sys_id);
    OSAPI_TEST_FUNCTION_RC(OS_TaskIdMatchSystemData_Impl(&test_sys_id, 0, NULL), true);

    memset(&test_sys_id, 'y', sizeof(test_sys_id));
    OSAPI_TEST_FUNCTION_RC(OS_TaskIdMatchSystemData_Impl(&test_sys_id, 0, NULL), false);
}


/* ------------------- End of test cases --------------------------------------*/

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);
    memset(OS_task_table, 0, sizeof(OS_task_table));
    memset(OS_global_task_table, 0, sizeof(OS_common_record_t) * OS_MAX_TASKS);

    memset(UT_Ref_OS_impl_task_table, 0, UT_Ref_OS_impl_task_table_SIZE);
}

/*
 * Osapi_Test_Teardown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_Test_Teardown(void)
{

}

/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
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
    ADD_TEST(OS_TaskValidateSystemData_Impl);
    ADD_TEST(OS_TaskIdMatchSystemData_Impl);
}


