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
#include "ut-osapi-queue.h"

#include <overrides/string.h>

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_QueueAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_QueueAPI_Init();

    UtAssert_True(actual == expected, "OS_QueueAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_QueueCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueCreate (uint32 *queue_id, const char *queue_name, uint32 queue_depth, uint32 data_size, uint32 flags)
     */
    int32 expected = OS_SUCCESS;
    uint32 objid = 0xFFFFFFFF;
    int32 actual = OS_QueueCreate(&objid, "UT", 0, 0,0);

    UtAssert_True(actual == expected, "OS_QueueCreate() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "objid (%lu) != 0", (unsigned long)objid);

    /* test error cases */
    expected = OS_INVALID_POINTER;
    actual = OS_QueueCreate(NULL, "UT", 0, 0,0);
    UtAssert_True(actual == expected, "OS_QueueCreate() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_MAX_API_NAME);
    expected = OS_ERR_NAME_TOO_LONG;
    actual = OS_QueueCreate(&objid, "UT", 0, 0,0);
    UtAssert_True(actual == expected, "OS_QueueCreate() (%ld) == OS_ERR_NAME_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));
}

void Test_OS_QueueDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueDelete (uint32 queue_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_QueueDelete(1);

    UtAssert_True(actual == expected, "OS_QueueDelete() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_QueueGet(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueGet (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    uint32 actual_size;
    char Buf[4];

    actual = OS_QueueGet(1, Buf, sizeof(Buf), &actual_size, 0);

    UtAssert_True(actual == expected, "OS_QueueGet() (%ld) == OS_SUCCESS", (long)actual);

    /* test error cases */
    expected = OS_INVALID_POINTER;
    actual = OS_QueueGet(1, NULL, sizeof(Buf), &actual_size, 0);
    UtAssert_True(actual == expected, "OS_QueueGet() (%ld) == OS_INVALID_POINTER", (long)actual);

    OS_queue_table[1].max_size = sizeof(Buf) + 10;
    expected = OS_QUEUE_INVALID_SIZE;
    actual = OS_QueueGet(1, Buf, sizeof(Buf), &actual_size, 0);
    UtAssert_True(actual == expected, "OS_QueueGet() (%ld) == OS_QUEUE_INVALID_SIZE", (long)actual);
}


void Test_OS_QueuePut(void)
{
    /*
     * Test Case For:
     * int32 OS_QueuePut (uint32 queue_id, const void *data, uint32 size, uint32 flags)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    const char Data[4] = "xyz";

    actual = OS_QueuePut(1, Data, sizeof(Data), 0);

    UtAssert_True(actual == expected, "OS_QueuePut() (%ld) == OS_SUCCESS", (long)actual);

    /* test error cases */
    expected = OS_INVALID_POINTER;
    actual = OS_QueuePut(1, NULL, sizeof(Data), 0);
    UtAssert_True(actual == expected, "OS_QueuePut() (%ld) == OS_INVALID_POINTER", (long)actual);

}


void Test_OS_QueueGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueGetIdByName (uint32 *queue_id, const char *queue_name)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    uint32 objid = 0;

    UT_SetForceFail(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_QueueGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_QueueGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "OS_QueueGetIdByName() objid (%lu) != 0", (unsigned long)objid);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual = OS_QueueGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_QueueGetIdByName() (%ld) == %ld",
            (long)actual, (long)expected);


    expected = OS_INVALID_POINTER;
    actual = OS_QueueGetIdByName(NULL, "NF");
    UtAssert_True(actual == expected, "OS_QueueGetIdByName() (%ld) == OS_INVALID_POINTER", (long)actual);
}

void Test_OS_QueueGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_QueueGetInfo (uint32 queue_id, OS_queue_prop_t *queue_prop)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    OS_queue_prop_t queue_prop;
    uint32 local_index = 1;
    OS_common_record_t utrec;
    OS_common_record_t *rptr = &utrec;

    memset(&utrec, 0, sizeof(utrec));
    utrec.creator = 111;
    utrec.name_entry = "ABC";
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &local_index, sizeof(local_index), false);
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &rptr, sizeof(rptr), false);
    actual = OS_QueueGetInfo(1, &queue_prop);

    UtAssert_True(actual == expected, "OS_QueueGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(queue_prop.creator == 111, "queue_prop.creator (%lu) == 111",
            (unsigned long)queue_prop.creator);
    UtAssert_True(strcmp(queue_prop.name, "ABC") == 0, "queue_prop.name (%s) == ABC",
            queue_prop.name);

    expected = OS_INVALID_POINTER;
    actual = OS_QueueGetInfo(1, NULL);
    UtAssert_True(actual == expected, "OS_QueueGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);
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
    ADD_TEST(OS_QueueAPI_Init);
    ADD_TEST(OS_QueueCreate);
    ADD_TEST(OS_QueueDelete);
    ADD_TEST(OS_QueueGet);
    ADD_TEST(OS_QueuePut);
    ADD_TEST(OS_QueueGetIdByName);
    ADD_TEST(OS_QueueGetInfo);
}





