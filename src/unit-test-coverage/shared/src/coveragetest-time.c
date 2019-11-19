/*
 * Filename: osapi_testcase_timer.c
 *
 * Purpose: This file contains unit test cases for items in the "osapi-timer" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-shared-coveragetest.h"
#include "ut-osapi-time.h"

#include <overrides/string.h>

static uint32 UT_TimerCount = 0;
static uint32 UT_TimerArgCount = 0;

void UT_TimerCallback(uint32 timer_id)
{
    ++UT_TimerCount;
}
void UT_TimerArgCallback(uint32 object_id, void *arg)
{
    ++UT_TimerArgCount;
}

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_TimerCbAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerCbAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_TimerCbAPI_Init();

    UtAssert_True(actual == expected, "OS_TimerCbAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_TimerAdd(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerAdd(uint32 *timer_id, const char *timer_name, uint32 timebase_ref_id, OS_ArgCallback_t  callback_ptr, void *callback_arg)
     */
    int32 expected = OS_SUCCESS;
    uint32 objid = 0xFFFFFFFF;
    char arg = 'a';
    int32 actual = OS_TimerAdd(&objid, "UT", 1, UT_TimerArgCallback, &arg);

    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_SUCCESS", (long)actual);

    /* test error cases */
    expected = OS_INVALID_POINTER;
    actual = OS_TimerAdd(NULL, "UT", 1, UT_TimerArgCallback, &arg);
    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_MAX_API_NAME);
    expected = OS_ERR_NAME_TOO_LONG;
    actual = OS_TimerAdd(&objid, "UT", 1, UT_TimerArgCallback, &arg);
    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_ERR_NAME_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual = OS_TimerAdd(&objid, "UT", 1, NULL, &arg);
    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    UT_SetForceFail(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_TimerAdd(&objid, "UT", 1, UT_TimerArgCallback, &arg);
    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_TaskGetId_Impl));

    UT_SetForceFail(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    expected = OS_ERROR;
    actual = OS_TimerAdd(&objid, "UT", 1, UT_TimerArgCallback, &arg);
    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_ERROR", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdGetById));


    UT_SetForceFail(UT_KEY(OS_ObjectIdAllocateNew), OS_ERROR);
    expected = OS_ERROR;
    actual = OS_TimerAdd(&objid, "UT", 1, UT_TimerArgCallback, &arg);
    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_ERROR", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdAllocateNew));
}

void Test_OS_TimerCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerCreate(uint32 *timer_id, const char *timer_name, uint32 *accuracy, OS_TimerCallback_t  callback_ptr)
     */
    int32 expected = OS_SUCCESS;
    uint32 objid = 0xFFFFFFFF;
    uint32 local_id = 0xFFFFFFFF;
    uint32 accuracy = 0xFFFFFFFF;
    int32 actual = OS_TimerCreate(&objid, "UT", &accuracy, UT_TimerCallback);

    UtAssert_True(actual == expected, "OS_TimerCreate() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "objid (%lu) != 0", (unsigned long)objid);

    OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_OS_TIMECB, objid, &local_id);

    UtAssert_True(OS_timecb_table[local_id].callback_ptr != NULL,
            "OS_timecb_table[%lu].callback_ptr (%p) != NULL",
            (unsigned long)local_id,
            OS_timecb_table[local_id].callback_ptr);

    UT_TimerCount = 0;
    OS_timecb_table[local_id].callback_ptr(objid, OS_timecb_table[local_id].callback_arg);

    UtAssert_True(UT_TimerCount == 1,
            "UT_TimerCount (%lu) == 1",
            (unsigned long)UT_TimerCount);

    /* additional cases to exercise error paths */
    expected = OS_INVALID_POINTER;
    actual = OS_TimerCreate(&objid, NULL, NULL, NULL);
    UtAssert_True(actual == expected, "OS_TimerSet() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual = OS_TimerCreate(&objid, "UT", &accuracy, NULL);
    UtAssert_True(actual == expected, "OS_TimerSet() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);


    UT_SetForceFail(UT_KEY(OS_TimeBaseCreate), OS_ERROR);
    expected = OS_ERROR;
    actual = OS_TimerCreate(&objid, "UT", &accuracy, UT_TimerCallback);
    UtAssert_True(actual == expected, "OS_TimerCreate() (%ld) == OS_ERROR", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_TimeBaseCreate));

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_MAX_API_NAME);
    expected = OS_ERR_NAME_TOO_LONG;
    actual = OS_TimerCreate(&objid, "UT", &accuracy, UT_TimerCallback);
    UtAssert_True(actual == expected, "OS_TimerCreate() (%ld) == OS_ERR_NAME_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));


}


void Test_OS_TimerSet(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
     */
    int32 expected = OS_ERROR;
    int32 actual = OS_TimerSet(1, 0, 0);
    UtAssert_True(actual == expected, "OS_TimerSet() (%ld) == OS_ERROR", (long)actual);

    expected = OS_SUCCESS;
    actual = OS_TimerSet(1, 0, 1);
    UtAssert_True(actual == expected, "OS_TimerSet() (%ld) == OS_SUCCESS", (long)actual);

    OS_timecb_table[2].timebase_ref = 0;
    OS_timecb_table[2].flags = TIMECB_FLAG_DEDICATED_TIMEBASE;
    OS_global_timebase_table[0].active_id = 2;
    actual = OS_TimerSet(2, 0, 1);
    UtAssert_True(actual == expected, "OS_TimerSet() (%ld) == OS_SUCCESS", (long)actual);
    memset(OS_timecb_table, 0, sizeof(OS_timecb_table));

    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual = OS_TimerSet(2, UINT32_MAX, UINT32_MAX);
    UtAssert_True(actual == expected, "OS_TimerSet() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    UT_SetForceFail(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_TimerSet(2, 0, 1);
    UtAssert_True(actual == expected, "OS_TimerSet() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_TaskGetId_Impl));
}


void Test_OS_TimerDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerDelete(uint32 timer_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_TimerDelete(1);

    UtAssert_True(actual == expected, "OS_TimerDelete() (%ld) == OS_SUCCESS", (long)actual);

    OS_timecb_table[1].timebase_ref = 0;
    OS_timecb_table[2].timebase_ref = 0;
    OS_timecb_table[2].next_ref = 1;
    OS_timecb_table[1].next_ref = 1;
    OS_timebase_table[0].first_cb = 2;
    actual = OS_TimerDelete(2);
    UtAssert_True(actual == expected, "OS_TimerDelete() (%ld) == OS_SUCCESS", (long)actual);

    OS_timebase_table[0].first_cb = 1;
    actual = OS_TimerDelete(1);
    UtAssert_True(actual == expected, "OS_TimerDelete() (%ld) == OS_SUCCESS", (long)actual);

    /* verify deletion of the dedicated timebase objects
     * these are implicitly created as part of timer creation for API compatibility */
    OS_TimeBaseCreate(&OS_global_timebase_table[0].active_id,"ut",NULL);
    OS_timecb_table[1].flags = TIMECB_FLAG_DEDICATED_TIMEBASE;
    OS_timecb_table[1].timebase_ref = 0;
    actual = OS_TimerDelete(1);
    UtAssert_True(actual == expected, "OS_TimerDelete() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(UT_GetStubCount(UT_KEY(OS_TimeBaseDelete)) == 1, "OS_TimerDelete() invoked OS_TimeBaseDelete()");

    memset(OS_timecb_table, 0, sizeof(OS_timecb_table));
    memset(OS_timebase_table, 0, sizeof(OS_timebase_table));

    UT_SetForceFail(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_TimerDelete(2);
    UtAssert_True(actual == expected, "OS_TimerDelete() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_TaskGetId_Impl));

}


void Test_OS_TimerGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerGetIdByName (uint32 *timer_id, const char *timer_name)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    uint32 objid = 0;

    UT_SetForceFail(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_TimerGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_TimerGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "OS_TimerGetIdByName() objid (%lu) != 0", (unsigned long)objid);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual = OS_TimerGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_TimerGetIdByName() (%ld) == %ld",
            (long)actual, (long)expected);

    expected = OS_INVALID_POINTER;
    actual = OS_TimerGetIdByName(NULL, "NF");
    UtAssert_True(actual == expected, "OS_TimerGetIdByName() (%ld) == %ld",
            (long)actual, (long)expected);

    UT_SetForceFail(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_TimerGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_TimerGetIdByName() (%ld) == %ld",
            (long)actual, (long)expected);
    UT_ClearForceFail(UT_KEY(OS_TaskGetId_Impl));

}


void Test_OS_TimerGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerGetInfo (uint32 timer_id, OS_timer_prop_t *timer_prop)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    OS_timer_prop_t timer_prop;
    uint32 local_index = 1;
    OS_common_record_t utrec;
    OS_common_record_t *rptr = &utrec;

    memset(&utrec, 0, sizeof(utrec));
    utrec.creator = 111;
    utrec.name_entry = "ABC";
    OS_timecb_table[1].interval_time = 2222;
    OS_timecb_table[1].timebase_ref = 0;
    OS_timebase_table[0].accuracy_usec = 3333;
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &local_index, sizeof(local_index), false);
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &rptr, sizeof(rptr), false);
    actual = OS_TimerGetInfo(1, &timer_prop);

    UtAssert_True(actual == expected, "OS_TimerGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(strcmp(timer_prop.name, "ABC") == 0, "timer_prop.name (%s) == ABC",
            timer_prop.name);
    UtAssert_True(timer_prop.interval_time == 2222,
            "timer_prop.interval_time (%lu) == 2222",
            (unsigned long)timer_prop.interval_time);
    UtAssert_True(timer_prop.accuracy == 3333,
            "timer_prop.accuracy (%lu) == 3333",
            (unsigned long)timer_prop.accuracy);

    expected = OS_INVALID_POINTER;
    actual = OS_TimerGetInfo(1, NULL);
    UtAssert_True(actual == expected, "OS_TimerGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_TimerGetInfo(1, &timer_prop);
    UtAssert_True(actual == expected, "OS_TimerGetInfo() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
    UT_ClearForceFail(UT_KEY(OS_TaskGetId_Impl));

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
    ADD_TEST(OS_TimerCbAPI_Init);
    ADD_TEST(OS_TimerAdd);
    ADD_TEST(OS_TimerCreate);
    ADD_TEST(OS_TimerSet);
    ADD_TEST(OS_TimerDelete);
    ADD_TEST(OS_TimerGetIdByName);
    ADD_TEST(OS_TimerGetInfo);
}





