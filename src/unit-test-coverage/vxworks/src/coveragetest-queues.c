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
 * \file     coveragetest-queues.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-queues.h"
#include "os-shared-queue.h"

#include <OCS_msgQLib.h>
#include <OCS_errno.h>

void Test_OS_VxWorks_QueueAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_QueueAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(UT_Call_OS_VxWorks_QueueAPI_Impl_Init(), OS_SUCCESS);
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


/* ------------------- End of test cases --------------------------------------*/

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);

    memset(OS_queue_table, 0, sizeof(OS_queue_table));
    memset(OS_global_queue_table, 0, sizeof(OS_common_record_t) * OS_MAX_QUEUES);

    memset(UT_Ref_OS_impl_queue_table, 0, UT_Ref_OS_impl_queue_table_SIZE);
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
    ADD_TEST(OS_VxWorks_QueueAPI_Impl_Init);
    ADD_TEST(OS_QueueCreate_Impl);
    ADD_TEST(OS_QueueDelete_Impl);
    ADD_TEST(OS_QueueGet_Impl);
    ADD_TEST(OS_QueuePut_Impl);
    ADD_TEST(OS_QueueGetInfo_Impl);
}


