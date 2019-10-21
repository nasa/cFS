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
#include "ut-osapi-binsem.h"

#include <overrides/string.h>

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_BinSemAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_BinSemAPI_Init();

    UtAssert_True(actual == expected, "OS_BinSemAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_BinSemCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemCreate (uint32 *sem_id, const char *sem_name,
     *          uint32 sem_initial_value, uint32 options)
     */
    int32 expected = OS_SUCCESS;
    uint32 objid = 0xFFFFFFFF;
    int32 actual = OS_BinSemCreate(&objid, "UT", 0,0);

    UtAssert_True(actual == expected, "OS_BinSemCreate() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "objid (%lu) != 0", (unsigned long)objid);

    OSAPI_TEST_FUNCTION_RC(OS_BinSemCreate(NULL, NULL, 0, 0), OS_INVALID_POINTER);
    UT_SetForceFail(UT_KEY(OCS_strlen), 10 + OS_MAX_API_NAME);
    OSAPI_TEST_FUNCTION_RC(OS_BinSemCreate(&objid, "UT", 0, 0), OS_ERR_NAME_TOO_LONG);
}

void Test_OS_BinSemDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemDelete (uint32 sem_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_BinSemDelete(1);

    UtAssert_True(actual == expected, "OS_BinSemDelete() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_BinSemGive(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemGive ( uint32 sem_id )
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_BinSemGive(1);

    UtAssert_True(actual == expected, "OS_BinSemGive() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_BinSemTake(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemTake ( uint32 sem_id )
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_BinSemTake(1);

    UtAssert_True(actual == expected, "OS_BinSemTake() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_BinSemFlush(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemFlush (uint32 sem_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_BinSemFlush(1);

    UtAssert_True(actual == expected, "OS_BinSemFlush() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_BinSemTimedWait(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemTimedWait ( uint32 sem_id, uint32 msecs )
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_BinSemTimedWait(1,1);

    UtAssert_True(actual == expected, "OS_BinSemTimedWait() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_BinSemGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemGetIdByName (uint32 *sem_id, const char *sem_name)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    uint32 objid = 0;

    UT_SetForceFail(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_BinSemGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_BinSemGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "OS_BinSemGetIdByName() objid (%lu) != 0", (unsigned long)objid);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual = OS_BinSemGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_BinSemGetIdByName() (%ld) == %ld",
            (long)actual, (long)expected);

    OSAPI_TEST_FUNCTION_RC(OS_BinSemGetIdByName(NULL, NULL), OS_INVALID_POINTER);
}

void Test_OS_BinSemGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_BinSemGetInfo (uint32 sem_id, OS_bin_sem_prop_t *bin_prop)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    OS_bin_sem_prop_t prop;
    uint32 local_index = 1;
    OS_common_record_t utrec;
    OS_common_record_t *rptr = &utrec;

    memset(&utrec, 0, sizeof(utrec));
    utrec.creator = 111;
    utrec.name_entry = "ABC";
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &local_index, sizeof(local_index), false);
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &rptr, sizeof(rptr), false);
    actual = OS_BinSemGetInfo(1, &prop);

    UtAssert_True(actual == expected, "OS_BinSemGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(prop.creator == 111, "prop.creator (%lu) == 111",
            (unsigned long)prop.creator);
    UtAssert_True(strcmp(prop.name, "ABC") == 0, "prop.name (%s) == ABC",
            prop.name);


    OSAPI_TEST_FUNCTION_RC(OS_BinSemGetInfo(0, NULL), OS_INVALID_POINTER);
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
    ADD_TEST(OS_BinSemAPI_Init);
    ADD_TEST(OS_BinSemCreate);
    ADD_TEST(OS_BinSemDelete);
    ADD_TEST(OS_BinSemGive);
    ADD_TEST(OS_BinSemTake);
    ADD_TEST(OS_BinSemFlush);
    ADD_TEST(OS_BinSemTimedWait);
    ADD_TEST(OS_BinSemGetIdByName);
    ADD_TEST(OS_BinSemGetInfo);
}





