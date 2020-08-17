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
 * \file     coveragetest-console-bsp.c
 * \ingroup  portable
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-portable-coveragetest.h"
#include "os-shared-printf.h"

#include <OCS_stdio.h>
#include <OCS_bsp-impl.h>

#define TEST_BUFFER_LEN     16

const char TEST_BUF_INITIALIZER[1+TEST_BUFFER_LEN] = "abcdefghijklmnop";

void Test_OS_ConsoleOutput_Impl(void)
{
    char TestConsoleBspBuffer[TEST_BUFFER_LEN];
    char TestOutputBuffer[32];

    memcpy(TestConsoleBspBuffer, TEST_BUF_INITIALIZER, sizeof(TestConsoleBspBuffer));
    memset(TestOutputBuffer, 0, sizeof(TestOutputBuffer));

    OS_console_table[0].BufBase = TestConsoleBspBuffer;
    OS_console_table[0].BufSize = sizeof(TestConsoleBspBuffer);

    UT_SetDataBuffer(UT_KEY(OCS_OS_BSP_ConsoleOutput_Impl), TestOutputBuffer, sizeof(TestOutputBuffer), false);

    OS_console_table[0].WritePos = 4;
    OS_ConsoleOutput_Impl(0);
    UtAssert_True(strcmp(TestOutputBuffer, "abcd") == 0,
            "TestOutputBuffer (%s) == abcd", TestOutputBuffer);

    OS_console_table[0].WritePos = 2;
    OS_ConsoleOutput_Impl(0);
    UtAssert_True(strcmp(TestOutputBuffer, "abcdefghijklmnopab") == 0,
            "TestOutputBuffer (%s) == abcdefghijklmnopab", TestOutputBuffer);
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
    memset(OS_console_table, 0, sizeof(OS_console_table));
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
    ADD_TEST(OS_ConsoleOutput_Impl);
}


