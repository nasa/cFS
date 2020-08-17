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
 * \file     coveragetest-bsd-select.c
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-portable-coveragetest.h"
#include "ut-adaptor-portable-posix-io.h"
#include "os-shared-select.h"

#include <OCS_sys_select.h>

void Test_OS_SelectSingle_Impl(void)
{
    /* Test Case For:
     * int32 OS_SelectSingle_Impl(uint32 stream_id, uint32 *SelectFlags, int32 msecs)
     */
    uint32 SelectFlags;
    uint32 StreamID;
    struct OCS_timespec nowtime;
    struct OCS_timespec latertime;

    StreamID = 0;
    UT_PortablePosixIOTest_Set_Selectable(0, false);
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (StreamID, &SelectFlags, 0), OS_ERR_OPERATION_NOT_SUPPORTED);
    UT_PortablePosixIOTest_Set_Selectable(0, true);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (StreamID, &SelectFlags, 0), OS_SUCCESS);
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (StreamID, &SelectFlags, -1), OS_SUCCESS);
    SelectFlags = 0;
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (StreamID, &SelectFlags, 0), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_select), 0);
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    nowtime.tv_sec = 1;
    nowtime.tv_nsec = 500000000;
    latertime.tv_sec = 10;
    latertime.tv_nsec = 0;
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime),&nowtime, sizeof(nowtime), false);
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime),&latertime, sizeof(latertime), false);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (StreamID, &SelectFlags, 999), OS_ERROR_TIMEOUT);

    UT_SetForceFail(UT_KEY(OCS_select), -1);
    SelectFlags = OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
    nowtime.tv_sec = 1;
    nowtime.tv_nsec = 0;
    latertime.tv_sec = 2;
    latertime.tv_nsec = 600000000;
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime),&nowtime, sizeof(nowtime), false);
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime),&latertime, sizeof(latertime), false);
    OSAPI_TEST_FUNCTION_RC(OS_SelectSingle_Impl, (StreamID, &SelectFlags, 2100), OS_ERROR);
} /* end OS_SelectSingle_Impl */


void Test_OS_SelectMultiple_Impl(void)
{
    /* Test Case For:
     * int32 OS_SelectMultiple_Impl(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs)
     */
    OS_FdSet ReadSet;
    OS_FdSet WriteSet;

    memset(&ReadSet, 0, sizeof(ReadSet));
    memset(&WriteSet, 0xff, sizeof(WriteSet));
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, 0), OS_SUCCESS);
    memset(&ReadSet, 0xff, sizeof(ReadSet));
    memset(&WriteSet, 0, sizeof(WriteSet));
    UT_SetForceFail(UT_KEY(OCS_select), 0);
    OSAPI_TEST_FUNCTION_RC(OS_SelectMultiple_Impl, (&ReadSet, &WriteSet, 1), OS_ERROR_TIMEOUT);
} /* end OS_SelectMultiple_Impl */



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
    ADD_TEST(OS_SelectSingle_Impl);
    ADD_TEST(OS_SelectMultiple_Impl);
}




