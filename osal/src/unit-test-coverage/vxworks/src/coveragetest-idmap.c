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
 * \file     coveragetest-idmap.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-idmap.h"

#include "os-shared-idmap.h"

#include <OCS_errno.h>
#include <OCS_objLib.h>

OCS_SEM TestGlobalSem;


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
    UT_IdMapTest_SetImplTableMutex(OS_OBJECT_TYPE_OS_TASK, (OCS_SEM_ID)0);
    OSAPI_TEST_FUNCTION_RC(OS_Lock_Global_Impl(OS_OBJECT_TYPE_OS_TASK), OS_ERROR);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_semTake)) == 0, "semTake() NOT called");

    UT_IdMapTest_SetImplTableMutex(OS_OBJECT_TYPE_OS_TASK, &TestGlobalSem);
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
    OSAPI_TEST_FUNCTION_RC(UT_Call_OS_VxWorks_TableMutex_Init(0), OS_SUCCESS);
    UT_SetForceFail(UT_KEY(OCS_semMInitialize), -1);
    OSAPI_TEST_FUNCTION_RC(UT_Call_OS_VxWorks_TableMutex_Init(OS_OBJECT_TYPE_OS_TASK), OS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_semMInitialize));
    OSAPI_TEST_FUNCTION_RC(UT_Call_OS_VxWorks_TableMutex_Init(OS_OBJECT_TYPE_OS_TASK), OS_SUCCESS);
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
    ADD_TEST(OS_Lock_Global_Impl);
    ADD_TEST(OS_Unlock_Global_Impl);
    ADD_TEST(OS_API_Impl_Init);
}


