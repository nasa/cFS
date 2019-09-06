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
#include "ut-osapi-interrupts.h"

void dummy_inthandler(void)
{
}

/*****************************************************************************
 *
 * Test Case for OS_IntAttachHandler()
 * int32 OS_IntAttachHandler  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter)
 *
 *****************************************************************************/
void Test_OS_IntAttachHandler  (void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_IntAttachHandler(0, &dummy_inthandler, 0);

    UtAssert_True(actual == expected, "OS_IntAttachHandler() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_IntAttachHandler(0, NULL, 0);
    UtAssert_True(actual == expected, "OS_IntAttachHandler() (%ld) == OS_INVALID_POINTER", (long)actual);
}

/*****************************************************************************
 *
 * Test Case for Test_OS_IntLock()
 * int32 OS_IntLock(void)
 *
 *****************************************************************************/
void Test_OS_IntLock(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_IntLock();

    UtAssert_True(actual == expected, "OS_IntLock() (%ld) == OS_SUCCESS", (long)actual);
}

/*****************************************************************************
 *
 * Test Case for Test_OS_IntUnlock()
 * int32 OS_IntUnlock(int32 IntFlags)
 *
 *****************************************************************************/
void Test_OS_IntUnlock(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_IntUnlock(0);

    UtAssert_True(actual == expected, "OS_IntUnlock() (%ld) == OS_SUCCESS", (long)actual);
}

/*****************************************************************************
 *
 * Test Case for:
 * int32 OS_IntEnable(int32 Level)
 *
 *****************************************************************************/
void Test_OS_IntEnable(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_IntEnable(1);

    UtAssert_True(actual == expected, "OS_IntEnable() (%ld) == OS_SUCCESS", (long)actual);
}

/*****************************************************************************
 *
 * Test Case for:
 * int32 OS_IntDisable(int32 Level)
 *
 *****************************************************************************/
void Test_OS_IntDisable(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_IntDisable(1);

    UtAssert_True(actual == expected, "OS_IntDisable() (%ld) == OS_SUCCESS", (long)actual);
}

/*****************************************************************************
 *
 * Test Case for:
 * int32 OS_IntSetMask ( uint32 MaskSetting )
 *
 *****************************************************************************/
void Test_OS_IntSetMask (void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_IntSetMask(0x10);

    UtAssert_True(actual == expected, "OS_IntSetMask() (%ld) == OS_SUCCESS", (long)actual);
}

/*****************************************************************************
 *
 * Test Case for:
 * int32 OS_IntGetMask ( uint32 * MaskSettingPtr )
 *
 *****************************************************************************/
void Test_OS_IntGetMask (void)
{
    uint32 mask;
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_IntGetMask(&mask);

    UtAssert_True(actual == expected, "OS_IntGetMask() (%ld) == OS_SUCCESS", (long)actual);
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
    ADD_TEST(OS_IntAttachHandler);
    ADD_TEST(OS_IntLock);
    ADD_TEST(OS_IntUnlock);
    ADD_TEST(OS_IntEnable);
    ADD_TEST(OS_IntDisable);
    ADD_TEST(OS_IntSetMask);
    ADD_TEST(OS_IntGetMask);
}




