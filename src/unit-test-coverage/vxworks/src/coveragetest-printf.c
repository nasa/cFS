/*
 * Filename: osapi_testcase_common.c
 *
 * Purpose: This file contains unit test cases for items in the "osfileapi-common" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-vxworks-coveragetest.h"
#include "ut-osapi.h"

#include <overrides/unistd.h>
#include <overrides/semLib.h>
#include <overrides/taskLib.h>
#include <overrides/errnoLib.h>
#include <overrides/stdio.h>

char TestConsoleBuffer[16];

void Test_OS_ConsoleWakeup_Impl(void)
{
    /*
     * Test Case For:
     * void OS_ConsoleWakeup_Impl(const char *string)
     */

    /* no return code - check for coverage */
    Osapi_Internal_SetConsoleAsync(0, true);
    OS_ConsoleWakeup_Impl(0);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_semGive)) == 1, "semGive() called in async mode");

    UT_SetForceFail(UT_KEY(OCS_semGive), -1);
    OS_ConsoleWakeup_Impl(0);

    Osapi_Internal_SetConsoleAsync(0, false);
    OS_console_table[0].WritePos = 1;
    OS_ConsoleWakeup_Impl(0);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_putchar)) == 1, "putchar() called in sync mode");
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
    Osapi_Internal_CallConsoleTask_Entry(0);
}

void Test_OS_ConsoleOutput_Impl(void)
{
    memset(TestConsoleBuffer, 'x', sizeof(TestConsoleBuffer));

    OS_console_table[0].WritePos = 4;
    OS_ConsoleOutput_Impl(0);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_putchar)) == 4, "putchar() called");

    OS_console_table[0].WritePos = 0;
    OS_ConsoleOutput_Impl(0);
    UtAssert_True(UT_GetStubCount(UT_KEY(OCS_putchar)) == sizeof(TestConsoleBuffer), "putchar() called");
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
    OS_console_table[0].BufBase = TestConsoleBuffer;
    OS_console_table[0].BufSize = sizeof(TestConsoleBuffer);
    OS_console_table[0].ReadPos = 0;
    OS_console_table[0].WritePos = 0;
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


#define ADD_TEST(test) UtTest_Add((Test_ ## test), Osapi_Task_Setup, Osapi_TearDown, #test)

/* Osapi_AddTestCase_Tasks
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    ADD_TEST(OS_ConsoleCreate_Impl);
    ADD_TEST(OS_ConsoleOutput_Impl);
    ADD_TEST(OS_ConsoleWakeup_Impl);
}


