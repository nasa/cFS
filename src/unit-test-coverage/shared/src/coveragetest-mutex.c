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
#include "ut-osapi-mutex.h"

#include <overrides/string.h>

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_MutexAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_MutexAPI_Init();

    UtAssert_True(actual == expected, "OS_MutexAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_MutSemCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemCreate (uint32 *sem_id, const char *sem_name, uint32 options)
     */
    int32 expected = OS_SUCCESS;
    uint32 objid = 0xFFFFFFFF;
    int32 actual = OS_MutSemCreate(&objid, "UT", 0);

    UtAssert_True(actual == expected, "OS_MutSemCreate() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "objid (%lu) != 0", (unsigned long)objid);

    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate(NULL, NULL, 0), OS_INVALID_POINTER);
    UT_SetForceFail(UT_KEY(OCS_strlen), 10 + OS_MAX_API_NAME);
    OSAPI_TEST_FUNCTION_RC(OS_MutSemCreate(&objid, "UT", 0), OS_ERR_NAME_TOO_LONG);
}

void Test_OS_MutSemDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemDelete (uint32 sem_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_MutSemDelete(1);

    UtAssert_True(actual == expected, "OS_MutSemDelete() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_MutSemGive(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGive ( uint32 sem_id )
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_MutSemGive(1);

    UtAssert_True(actual == expected, "OS_MutSemGive() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_MutSemTake(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemTake ( uint32 sem_id )
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_MutSemTake(1);

    UtAssert_True(actual == expected, "OS_MutSemTake() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_MutSemGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGetIdByName (uint32 *sem_id, const char *sem_name)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    uint32 objid = 0;

    UT_SetForceFail(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_MutSemGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_MutSemGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "OS_MutSemGetIdByName() objid (%lu) != 0", (unsigned long)objid);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual = OS_MutSemGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_MutSemGetIdByName() (%ld) == %ld",
            (long)actual, (long)expected);

    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetIdByName(NULL, NULL), OS_INVALID_POINTER);

}

void Test_OS_MutSemGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_MutSemGetInfo (uint32 sem_id, OS_mut_sem_prop_t *mut_prop)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    OS_mut_sem_prop_t prop;
    uint32 local_index = 1;
    OS_common_record_t utrec;
    OS_common_record_t *rptr = &utrec;

    memset(&utrec, 0, sizeof(utrec));
    utrec.creator = 111;
    utrec.name_entry = "ABC";
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &local_index, sizeof(local_index), false);
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &rptr, sizeof(rptr), false);
    actual = OS_MutSemGetInfo(1, &prop);

    UtAssert_True(actual == expected, "OS_MutSemGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(prop.creator == 111, "prop.creator (%lu) == 111",
            (unsigned long)prop.creator);
    UtAssert_True(strcmp(prop.name, "ABC") == 0, "prop.name (%s) == ABC",
            prop.name);

    OSAPI_TEST_FUNCTION_RC(OS_MutSemGetInfo(0, NULL), OS_INVALID_POINTER);

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
    ADD_TEST(OS_MutexAPI_Init);
    ADD_TEST(OS_MutSemCreate);
    ADD_TEST(OS_MutSemDelete);
    ADD_TEST(OS_MutSemGive);
    ADD_TEST(OS_MutSemTake);
    ADD_TEST(OS_MutSemGetIdByName);
    ADD_TEST(OS_MutSemGetInfo);
}





