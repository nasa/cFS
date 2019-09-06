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
#include "ut-osapi-errors.h"

void Test_OS_GetErrorName(void)
{
    /*
     * Test Case For:
     * int32 OS_GetErrorName(int32 error_num, os_err_name_t* err_name);
     */
    os_err_name_t err_name;
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_GetErrorName(OS_ERROR, &err_name);

    UtAssert_True(actual == expected, "OS_GetErrorName(%s) (%ld) == OS_SUCCESS", "OS_ERROR", (long)actual);
    UtAssert_True(strcmp(err_name,"OS_ERROR") == 0, "string(%s) == OS_ERROR", err_name);

    expected = OS_ERROR;
    actual   = OS_GetErrorName(-555555, &err_name);
    UtAssert_True(actual == expected, "OS_GetErrorName(%s) (%ld) == OS_SUCCESS", err_name, (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_GetErrorName(-555555, NULL);
    UtAssert_True(actual == expected, "OS_GetErrorName(NULL) (%ld) == OS_SUCCESS", (long)actual);
}

/* Osapi_Task_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Task_Setup(void)
{
    UT_ResetState(0);
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
    ADD_TEST(OS_GetErrorName);
}




