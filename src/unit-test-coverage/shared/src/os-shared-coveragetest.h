/*
 * Filename: os-shared-coveragetest.h
 *
 * Purpose: Common header file for all "shared" layer coverage test modules
 */

#ifndef _OS_SHARED_COVERAGETEST_H_
#define _OS_SHARED_COVERAGETEST_H_

/*
 * Includes
 */

#include <utassert.h>
#include <uttest.h>
#include <utstubs.h>
#include <osapi.h>
#include <os-impl.h>

/*
 * Macro to call a function and check its int32 return code
 */
#define OSAPI_TEST_FUNCTION_RC(func,exp)        \
{                                               \
    int32 rcexp = exp;                          \
    int32 rcact = func;                         \
    UtAssert_True(rcact == rcexp, "%s (%ld) == %s (%ld)",   \
        #func, (long)rcact, #exp, (long)rcexp);             \
}

/*
 * Macro to add a test case to the list of tests to execute
 */
#define ADD_TEST(test) UtTest_Add((Test_ ## test),Osapi_Task_Setup,Osapi_TearDown, #test)

/*
 * Setup function prior to every test
 */
void Osapi_Task_Setup(void);

/*
 * Teardown function after every test
 */
void Osapi_TearDown(void);

#endif

