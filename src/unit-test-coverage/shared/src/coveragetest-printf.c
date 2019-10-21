/*
 * Filename: osapi_testcase_common.c
 *
 * Purpose: This file contains unit test cases for items in the "osapi-common" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-shared-coveragetest.h"
#include "ut-osapi-printf.h"

#include <overrides/stdio.h>

char TestConsoleBuffer[16];

void Test_OS_ConsoleAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_ConsoleAPI_Init(void)
     */
    uint32 CallCount = 0;
    uint32 local_id = 0;

    UT_SetDataBuffer(UT_KEY(OS_ObjectIdAllocateNew), &local_id, sizeof(local_id), false);

    /* call for coverage */
    OS_ConsoleAPI_Init();

    CallCount = UT_GetStubCount(UT_KEY(OS_ConsoleCreate_Impl));
    UtAssert_True(CallCount == 1, "OS_ConsoleCreate_Impl() call count (%lu) == 1", (unsigned long)CallCount);
}


void Test_OS_printf(void)
{
    /*
     * Test Case For:
     * void OS_printf( const char *String, ...);
     * void OS_printf_disable(void);
     * void OS_printf_enable(void);
     */
    uint32 CallCount = 0;

    /* catch case where OS_printf called before init */
    OS_SharedGlobalVars.PrintfConsoleId = 0;
    OS_SharedGlobalVars.Initialized = false;
    OS_printf("UnitTest1");
    UtAssert_True(OS_console_table[0].WritePos == 0, "WritePos (%lu) >= 0", (unsigned long)OS_console_table[0].WritePos);

    /* because printf is disabled, the call count should _not_ increase here */
    OS_SharedGlobalVars.Initialized = true;
    OS_printf_disable();
    OS_printf("UnitTest2");
    UtAssert_True(OS_console_table[0].WritePos == 0, "WritePos (%lu) >= 0", (unsigned long)OS_console_table[0].WritePos);

    /* normal case */
    OS_printf_enable();
    OS_printf("UnitTest3");
    CallCount = UT_GetStubCount(UT_KEY(OS_ConsoleWakeup_Impl));
    UtAssert_True(CallCount == 1, "OS_ConsoleWakeup_Impl() call count (%lu) == 1", (unsigned long)CallCount);
    UtAssert_True(OS_console_table[0].WritePos >= 9, "WritePos (%lu) >= 9", (unsigned long)OS_console_table[0].WritePos);

    /* print a long string that does not fit in the 16-char buffer */
    OS_printf_enable();
    OS_printf("UnitTest4BufferLengthExceeded");

    /* test writing with a non-empty console name */
    strncpy(OS_console_table[0].device_name,"ut",sizeof(OS_console_table[0].device_name)-1);
    OS_printf("UnitTest5");

    /*
     * For coverage, exercise different paths depending on the return value
     */
    UT_SetForceFail(UT_KEY(OCS_vsnprintf), -1);
    OS_printf("UnitTest6");

    UT_SetForceFail(UT_KEY(OCS_vsnprintf), OS_BUFFER_SIZE+10);
    OS_printf("UnitTest7");
}

/* Osapi_Task_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Task_Setup(void)
{
    UT_ResetState(0);
    memset(OS_console_table, 0, sizeof(OS_console_table));
    OS_console_table[0].BufBase = TestConsoleBuffer;
    OS_console_table[0].BufSize = sizeof(TestConsoleBuffer);
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

/*
 * Register the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    ADD_TEST(OS_ConsoleAPI_Init);
    ADD_TEST(OS_printf);
}




