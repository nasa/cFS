/*
 * Filename: osapi_testcase_common.c
 *
 * Purpose: This file contains unit test cases for items in the "osloader-common" file
 *
 * Notes:
 *
 */

#ifndef _OS_VXWORKS_COVERAGETEST_H_
#define _OS_VXWORKS_COVERAGETEST_H_

/*
 * Includes
 */

#include <utassert.h>
#include <uttest.h>
#include <utstubs.h>
#include <os-impl.h>


#define OSAPI_TEST_FUNCTION_RC(func,exp)        \
{                                               \
    int32 rcexp = exp;                          \
    int32 rcact = func;                         \
    UtAssert_True(rcact == rcexp, "%s (%ld) == %s (%ld)",   \
        #func, (long)rcact, #exp, (long)rcexp);             \
}

#define ADD_TEST(test) UtTest_Add((Test_ ## test), Osapi_Task_Setup, Osapi_TearDown, #test)

/* Osapi_Task_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Task_Setup(void);
void Osapi_TearDown(void);

#endif

