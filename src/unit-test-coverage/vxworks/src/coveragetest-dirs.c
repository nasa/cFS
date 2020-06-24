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
 * \file     coveragetest-dirs.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-dirs.h"

#include "os-shared-dir.h"

#include <OCS_stdlib.h>
#include <OCS_taskLib.h>
#include <OCS_dirent.h>
#include <OCS_unistd.h>
#include <OCS_stat.h>

void Test_OS_VxWorks_DirAPI_Impl_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_VxWorks_DirAPI_Impl_Init(void)
     */
    OSAPI_TEST_FUNCTION_RC(UT_Call_OS_VxWorks_DirAPI_Impl_Init(), OS_SUCCESS);
}

void Test_OS_DirCreate_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirCreate_Impl(const char *local_path, uint32 access)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirCreate_Impl("dir", 0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_mkdir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirCreate_Impl("dir", 0), OS_ERROR);
}

void Test_OS_DirOpen_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirOpen_Impl(uint32 local_id, const char *local_path)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirOpen_Impl(0, "dir"), OS_SUCCESS);
    UT_SetForceFail(UT_KEY(OCS_opendir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirOpen_Impl(0, "dir"), OS_ERROR);
}

void Test_OS_DirClose_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirClose_Impl(uint32 local_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirClose_Impl(0), OS_SUCCESS);
}

void Test_OS_DirRead_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirRead_Impl(uint32 local_id, os_dirent_t *dirent)
     */
    os_dirent_t dirent_buff;

    OSAPI_TEST_FUNCTION_RC(OS_DirRead_Impl(0, &dirent_buff), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_readdir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirRead_Impl(0, &dirent_buff), OS_ERROR);

}

void Test_OS_DirRewind_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirRewind_Impl(uint32 local_id)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirRewind_Impl(0), OS_SUCCESS);
}

void Test_OS_DirRemove_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_DirRemove_Impl(const char *local_path)
     */
    OSAPI_TEST_FUNCTION_RC(OS_DirRemove_Impl("dir"), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_rmdir), -1);
    OSAPI_TEST_FUNCTION_RC(OS_DirRemove_Impl("dir"), OS_ERROR);
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
    ADD_TEST(OS_VxWorks_DirAPI_Impl_Init);
    ADD_TEST(OS_DirCreate_Impl);
    ADD_TEST(OS_DirOpen_Impl);
    ADD_TEST(OS_DirClose_Impl);
    ADD_TEST(OS_DirRead_Impl);
    ADD_TEST(OS_DirRewind_Impl);
    ADD_TEST(OS_DirRemove_Impl);
}


