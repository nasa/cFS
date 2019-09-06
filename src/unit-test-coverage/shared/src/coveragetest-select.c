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
#include "ut-osapi-select.h"

#include <overrides/string.h>

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_SelectSingle(void)
{
    /*
     * Test Case For:
     * int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs);
     */
    int32 expected = OS_SUCCESS;
    uint32 StateFlags = 0;
    int32 actual = OS_SelectSingle(1, &StateFlags, 0);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_SelectSingle() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_SelectMultiple(void)
{
    /*
     * Test Case For:
     * int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs);
     */
    OS_FdSet ReadSet;
    OS_FdSet WriteSet;
    int32 expected = OS_SUCCESS;
    int32 actual;

    OS_SelectFdZero(&ReadSet);
    OS_SelectFdZero(&WriteSet);
    actual = OS_SelectMultiple(&ReadSet, &WriteSet, 0);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_SelectMultiple() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_SelectFdAddClearOps(void)
{
    /*
     * Test Cases For:
     * int32 OS_SelectFdZero(OS_FdSet *Set);
     * int32 OS_SelectFdAdd(OS_FdSet *Set, uint32 objid);
     * int32 OS_SelectFdClear(OS_FdSet *Set, uint32 objid);
     * bool OS_SelectFdIsSet(OS_FdSet *Set, uint32 objid);
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    uint32 CallCount = 0;
    OS_FdSet UtSet;

    actual = OS_SelectFdZero(&UtSet);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "OS_SelectSingle() (%ld) == OS_SUCCESS", (long)actual);
    CallCount = UT_GetStubCount(UT_KEY(OCS_memset));
    UtAssert_True(CallCount == 1, "memset() call count (%lu) == 1", (unsigned long)CallCount);
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, 1), "OS_SelectFdIsSet(1) == false");
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, 2), "OS_SelectFdIsSet(2) == false");

    expected = OS_SUCCESS;
    actual = OS_SelectFdAdd(&UtSet, 1);
    UtAssert_True(actual == expected, "OS_SelectFdAdd() (%ld) == %ld", (long)actual, (long)expected);
    UtAssert_True(OS_SelectFdIsSet(&UtSet, 1), "OS_SelectFdIsSet(1) == true");
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, 2), "OS_SelectFdIsSet(2) == false");
    actual = OS_SelectFdAdd(&UtSet, 2);
    UtAssert_True(actual == expected, "OS_SelectFdAdd() (%ld) == %ld", (long)actual, (long)expected);
    UtAssert_True(OS_SelectFdIsSet(&UtSet, 1), "OS_SelectFdIsSet(1) == true");
    UtAssert_True(OS_SelectFdIsSet(&UtSet, 2), "OS_SelectFdIsSet(2) == true");

    actual = OS_SelectFdClear(&UtSet, 2);
    UtAssert_True(actual == expected, "OS_SelectFdClear() (%ld) == %ld", (long)actual, (long)expected);
    UtAssert_True(OS_SelectFdIsSet(&UtSet, 1), "OS_SelectFdIsSet(1) == true");
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, 2), "OS_SelectFdIsSet(2) == false");


    expected = -42;
    UT_SetForceFail(UT_KEY(OS_ObjectIdToArrayIndex), expected);
    actual = OS_SelectFdAdd(&UtSet, 2);
    UtAssert_True(actual == expected, "OS_SelectFdAdd() (%ld) == %ld", (long)actual, (long)expected);
    actual = OS_SelectFdClear(&UtSet, 1);
    UtAssert_True(actual == expected, "OS_SelectFdClear() (%ld) == %ld", (long)actual, (long)expected);
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, 1), "OS_SelectFdIsSet(1) == false");
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, 2), "OS_SelectFdIsSet(2) == false");

    UT_ClearForceFail(UT_KEY(OS_ObjectIdToArrayIndex));
    UtAssert_True(OS_SelectFdIsSet(&UtSet, 1), "OS_SelectFdIsSet(1) == true");
    UtAssert_True(!OS_SelectFdIsSet(&UtSet, 2), "OS_SelectFdIsSet(2) == false");
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
    ADD_TEST(OS_SelectFdAddClearOps);
    ADD_TEST(OS_SelectSingle);
    ADD_TEST(OS_SelectMultiple);
}





