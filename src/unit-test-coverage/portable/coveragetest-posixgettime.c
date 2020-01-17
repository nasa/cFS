/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

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

#include <utassert.h>
#include <uttest.h>
#include <utstubs.h>

#include <osapi.h>
#include <os-impl.h>

#include <OCS_time.h>

#define OSAPI_TEST_FUNCTION_RC(func,exp)   \
{                                               \
    int32 rcexp = exp;                          \
    int32 rcact = func;                    \
    UtAssert_True(rcact == rcexp, "%s (%ld) == %s (%ld)", \
        #func, (long)rcact, #exp, (long)rcexp);      \
}


void Test_OS_GetLocalTime_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_GetLocalTime_Impl(OS_time_t *time_struct)
     */
    OS_time_t timeval;
    timeval.seconds = 1;
    timeval.microsecs = 1;
    OSAPI_TEST_FUNCTION_RC(OS_GetLocalTime_Impl(&timeval), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_clock_gettime), -1);
    OSAPI_TEST_FUNCTION_RC(OS_GetLocalTime_Impl(&timeval), OS_ERROR);
}

void Test_OS_SetLocalTime_Impl(void)
{
    /*
     * Test Case For:
     * int32 OS_SetLocalTime_Impl(const OS_time_t *time_struct)
     */
    OS_time_t timeval;
    timeval.seconds = 1;
    timeval.microsecs = 1;
    OSAPI_TEST_FUNCTION_RC(OS_SetLocalTime_Impl(&timeval), OS_SUCCESS);

    UT_SetForceFail(UT_KEY(OCS_clock_settime), -1);
    OSAPI_TEST_FUNCTION_RC(OS_SetLocalTime_Impl(&timeval), OS_ERROR);
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

/* UtTest_Setup
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_GetLocalTime_Impl);
    ADD_TEST(OS_SetLocalTime_Impl);
}


