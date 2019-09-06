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
#include "ut-osapi-heap.h"


void Test_OS_HeapGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_HeapGetInfo(OS_heap_prop_t *heap_prop)
     */
    OS_heap_prop_t heap_prop;
    int32 expected = OS_SUCCESS;
    int32 actual   = OS_HeapGetInfo(&heap_prop);

    UtAssert_True(actual == expected, "OS_HeapGetInfo() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_HeapGetInfo(NULL);

    UtAssert_True(actual == expected, "OS_HeapGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);

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
    ADD_TEST(OS_HeapGetInfo);
}




