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
#include "ut-osapi-fpu.h"

void dummy_fpu_exception_handler(void)
{
}

/*****************************************************************************
 *
 * Test Case for OS_FPUExcAttachHandler()
 * int32 OS_FPUExcAttachHandler   (uint32 ExceptionNumber, void * ExceptionHandler ,
                                 int32 parameter)
 *
 *
 *****************************************************************************/
void Test_OS_FPUExcAttachHandler(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_FPUExcAttachHandler(1, &dummy_fpu_exception_handler, 0);

    UtAssert_True(actual == expected, "OS_FPUExcAttachHandler() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_FPUExcAttachHandler(1, NULL, 0);
    UtAssert_True(actual == expected, "OS_FPUExcAttachHandler() (%ld) == OS_INVALID_POINTER", (long)actual);
}

/*****************************************************************************
 *
 * Test Case for OS_FPUExcEnable()
 * int32 OS_FPUExcEnable          (int32 ExceptionNumber)
 *
 *****************************************************************************/
void Test_OS_FPUExcEnable(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_FPUExcEnable(1);

    UtAssert_True(actual == expected, "OS_FPUExcEnable() (%ld) == OS_SUCCESS", (long)actual);
}

/*****************************************************************************
 *
 * Test Case for OS_FPUExcDisable()
 * int32 OS_FPUExcDisable         (int32 ExceptionNumber)
 *
 *****************************************************************************/
void Test_OS_FPUExcDisable(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_FPUExcDisable(1);

    UtAssert_True(actual == expected, "OS_FPUExcDisable() (%ld) == OS_SUCCESS", (long)actual);
}

/*****************************************************************************
 *
 * Test Case for OS_FPUExcSetMask()
 * int32 OS_FPUExcSetMask         (uint32 mask)
 *
 *****************************************************************************/
void Test_OS_FPUExcSetMask(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_FPUExcSetMask(0x10);

    UtAssert_True(actual == expected, "OS_FPUExcSetMask() (%ld) == OS_SUCCESS", (long)actual);
}

/*****************************************************************************
 *
 * Test Case for OS_FPUExcGetMask()
 * int32 OS_FPUExcGetMask         (uint32 *mask)
 *
 *****************************************************************************/
void Test_OS_FPUExcGetMask(void)
{
    uint32 mask;
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_FPUExcGetMask(&mask);

    UtAssert_True(actual == expected, "OS_FPUExcGetMask() (%ld) == OS_SUCCESS", (long)actual);
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
    ADD_TEST(OS_FPUExcAttachHandler);
    ADD_TEST(OS_FPUExcEnable);
    ADD_TEST(OS_FPUExcDisable);
    ADD_TEST(OS_FPUExcSetMask);
    ADD_TEST(OS_FPUExcGetMask);
}




