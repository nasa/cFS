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
 * \file     coveragetest-binsem.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */


#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-binsem.h"

#include "os-shared-binsem.h"
#include "os-shared-idmap.h"

#include <OCS_errno.h>
#include <OCS_objLib.h>

void Test_OS_VxWorks_BinSemAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_BinSemAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(UT_Call_OS_VxWorks_BinSemAPI_Impl_Init(), OS_SUCCESS);
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

    UT_SetForceFail(UT_StubKey_GenericSemGive, OS_SEM_FAILURE);
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

    UT_SetForceFail(UT_StubKey_GenericSemTake, OS_SEM_FAILURE);
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

/* ------------------- End of test cases --------------------------------------*/

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);

    memset(OS_bin_sem_table, 0, sizeof(OS_bin_sem_table));
    memset(OS_global_bin_sem_table, 0, sizeof(OS_common_record_t) * OS_MAX_BIN_SEMAPHORES);

    memset(UT_Ref_OS_impl_bin_sem_table, 0, UT_Ref_OS_impl_bin_sem_table_SIZE);
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
    ADD_TEST(OS_VxWorks_BinSemAPI_Impl_Init);
    ADD_TEST(OS_BinSemCreate_Impl);
    ADD_TEST(OS_BinSemDelete_Impl);
    ADD_TEST(OS_BinSemGive_Impl);
    ADD_TEST(OS_BinSemFlush_Impl);
    ADD_TEST(OS_BinSemTake_Impl);
    ADD_TEST(OS_BinSemTimedWait_Impl);
    ADD_TEST(OS_BinSemGetInfo_Impl);
}


