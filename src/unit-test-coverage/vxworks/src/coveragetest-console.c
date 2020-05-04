/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     coveragetest-console.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include "os-vxworks-coveragetest.h"
#include "ut-adaptor-console.h"
#include "os-shared-printf.h"

#include <OCS_unistd.h>
#include <OCS_semLib.h>
#include <OCS_taskLib.h>
#include <OCS_errnoLib.h>
#include <OCS_stdio.h>

void Test_OS_ConsoleWakeup_Impl(void)
{
    /*
     * Test Case For:
     * void OS_ConsoleWakeup_Impl(const char *string)
     */

    /* no return code - check for coverage */
    UT_ConsoleTest_SetConsoleAsync(0, true);
    OS_ConsoleWakeup_Impl(0);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_semGive)) == 1, "semGive() called in async mode");

    UT_SetForceFail(UT_KEY(OCS_semGive), -1);
    OS_ConsoleWakeup_Impl(0);

    UT_ConsoleTest_SetConsoleAsync(0, false);
    OS_console_table[0].WritePos = 1;
    OS_ConsoleWakeup_Impl(0);
    UtAssert_True(UT_GetStubCount(UT_KEY(OS_ConsoleOutput_Impl)) == 1, "OS_ConsoleOutput_Impl() called in sync mode");
}

void Test_OS_ConsoleCreate_Impl(void)
{
    OSAPI_TEST_FUNCTION_RC(OS_ConsoleCreate_Impl(0), OS_SUCCESS);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_taskSpawn)) == 1, "taskSpawn() called");

    UT_SetForceFail(UT_KEY(OCS_semCInitialize), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_ConsoleCreate_Impl(0), OS_SEM_FAILURE);
    UT_ClearForceFail(UT_KEY(OCS_semCInitialize));

    UT_SetForceFail(UT_KEY(OCS_taskSpawn), OCS_ERROR);
    OSAPI_TEST_FUNCTION_RC(OS_ConsoleCreate_Impl(0), OS_ERROR);
    UT_ClearForceFail(UT_KEY(OCS_taskSpawn));

    OSAPI_TEST_FUNCTION_RC(OS_ConsoleCreate_Impl(OS_MAX_CONSOLES + 1), OS_ERR_NOT_IMPLEMENTED);

    /* Also call the actual console task, to get coverage on it.
     * This task has an infinite loop, which only exits if semTake fails */
    UT_SetDeferredRetcode(UT_KEY(OCS_semTake), 2, OCS_ERROR);
    UT_ConsoleTest_TaskEntry(0);
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
    memset(UT_Ref_OS_impl_console_table, 0, UT_Ref_OS_impl_console_table_SIZE);
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


#define ADD_TEST(test) UtTest_Add((Test_ ## test), Osapi_Test_Setup, Osapi_Test_Teardown, #test)

/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_ConsoleCreate_Impl);
    ADD_TEST(OS_ConsoleWakeup_Impl);
}


