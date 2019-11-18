/*
 * Filename: osapi_testcase_timebase.c
 *
 * Purpose: This file contains unit test cases for items in the "osapi-timebase" file
 *
 * Notes:
 *
 */


/*
 * Includes
 */

#include "os-shared-coveragetest.h"

#include "ut-osapi-timebase.h"

#include <overrides/string.h>

static uint32 TimerSyncCount = 0;
static uint32 TimerSyncRetVal = 0;
static uint32 TimeCB = 0;

static uint32 UT_TimerSync(uint32 timer_id)
{
    ++TimerSyncCount;
    return TimerSyncRetVal;
}

static int32 ClearObjectsHook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{
    OS_common_record_t *recptr = UserObj;

    if (CallCount >= 10)
    {
        memset(recptr, 0, sizeof(*recptr));
    }
    return StubRetcode;
}

static void UT_TimeCB(uint32 object_id, void *arg)
{
    ++TimeCB;
}

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_TimeBaseAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_TimeBaseAPI_Init();

    UtAssert_True(actual == expected, "OS_TimeBaseAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_TimeBaseCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseCreate(uint32 *timer_id, const char *timebase_name, OS_TimerSync_t external_sync)
     */
    int32 expected = OS_SUCCESS;
    uint32 objid = 0xFFFFFFFF;
    int32 actual;

    actual = OS_TimeBaseCreate(&objid, "UT1", UT_TimerSync);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_TimeBaseCreate(&objid, "UT2", NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)actual);

    /* test error paths */
    expected = OS_INVALID_POINTER;
    actual = OS_TimeBaseCreate(NULL, NULL, NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_MAX_API_NAME);
    expected = OS_ERR_NAME_TOO_LONG;
    actual = OS_TimeBaseCreate(&objid, "UT", UT_TimerSync);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_ERR_NAME_TOO_LONG", (long)actual);
    UT_ClearForceFail(UT_KEY(OCS_strlen));

    UT_SetForceFail(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_TimeBaseCreate(&objid, "UT", UT_TimerSync);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
}

void Test_OS_TimeBaseSet(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_TimeBaseSet(1, 1000, 1000);

    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_SUCCESS", (long)actual);

    /* test error paths: overflow on input */
    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual = OS_TimeBaseSet(1, UINT32_MAX, UINT32_MAX);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    /* test error paths */
    UT_SetForceFail(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_TimeBaseSet(1, 1000, 1000);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);

}

void Test_OS_TimeBaseDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseDelete(uint32 timer_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_TimeBaseDelete(1);

    UtAssert_True(actual == expected, "OS_TimeBaseDelete() (%ld) == OS_SUCCESS", (long)actual);

    /* test error paths */
    UT_SetForceFail(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_TimeBaseDelete(1);
    UtAssert_True(actual == expected, "OS_TimeBaseDelete() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
}

void Test_OS_TimeBaseGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseGetIdByName (uint32 *timer_id, const char *timebase_name)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    uint32 objid = 0;

    UT_SetForceFail(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_TimeBaseGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "OS_TimeBaseGetIdByName() objid (%lu) != 0", (unsigned long)objid);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdFindByName));

    expected = OS_ERR_NAME_NOT_FOUND;
    actual = OS_TimeBaseGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == %ld",
            (long)actual, (long)expected);


    /* test error paths */
    expected = OS_INVALID_POINTER;
    actual = OS_TimeBaseGetIdByName(NULL, NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_TimeBaseGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);
}

void Test_OS_TimeBaseGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseGetInfo (uint32 timebase_id, OS_timebase_prop_t *timebase_prop)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    OS_timebase_prop_t timebase_prop;
    uint32 local_index = 1;
    OS_common_record_t utrec;
    OS_common_record_t *rptr = &utrec;

    memset(&utrec, 0, sizeof(utrec));
    utrec.creator = 1111;
    utrec.name_entry = "ABC";
    OS_timebase_table[1].nominal_interval_time = 2222;
    OS_timebase_table[1].freerun_time = 3333;
    OS_timebase_table[1].accuracy_usec = 4444;
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &local_index, sizeof(local_index), false);
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &rptr, sizeof(rptr), false);
    actual = OS_TimeBaseGetInfo(1, &timebase_prop);

    UtAssert_True(actual == expected, "OS_TimeBaseGetInfo() (%ld) == OS_SUCCESS", (long)actual);

    UtAssert_True(timebase_prop.creator == 1111, "timebase_prop.creator (%lu) == 1111",
            (unsigned long)timebase_prop.creator);
    UtAssert_True(strcmp(timebase_prop.name, "ABC") == 0, "timebase_prop.name (%s) == ABC",
            timebase_prop.name);
    UtAssert_True(timebase_prop.nominal_interval_time == 2222,
            "timebase_prop.nominal_interval_time (%lu) == 2222",
            (unsigned long)timebase_prop.nominal_interval_time);
    UtAssert_True(timebase_prop.freerun_time == 3333,
            "timebase_prop.freerun_time (%lu) == 3333",
            (unsigned long)timebase_prop.freerun_time);
    UtAssert_True(timebase_prop.accuracy == 4444,
            "timebase_prop.accuracy (%lu) == 4444",
            (unsigned long)timebase_prop.accuracy);

    /* test error paths */
    expected = OS_INVALID_POINTER;
    actual = OS_TimeBaseGetInfo(1, NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OS_TaskGetId_Impl), 1 | (OS_OBJECT_TYPE_OS_TIMEBASE << OS_OBJECT_TYPE_SHIFT));
    expected = OS_ERR_INCORRECT_OBJ_STATE;
    actual = OS_TimeBaseGetInfo(1, &timebase_prop);
    UtAssert_True(actual == expected, "OS_TimeBaseGetInfo() (%ld) == OS_ERR_INCORRECT_OBJ_STATE", (long)actual);

}

void Test_OS_TimeBaseGetFreeRun(void)
{
    /*
     * Test Case For:
     * int32 OS_TimeBaseGetFreeRun     (uint32 timebase_id, uint32 *freerun_val)
     */
    int32 expected = OS_SUCCESS;
    uint32 freerun = 0xFFFFFFFF;
    int32 actual = OS_TimeBaseGetFreeRun(1, &freerun);

    UtAssert_True(actual == expected, "OS_TimeBaseGetFreeRun() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_TimeBase_CallbackThread(void)
{
    /*
     * Test Case For:
     * void OS_TimeBase_CallbackThread(uint32 timebase_id)
     */
    OS_common_record_t fake_record;
    OS_common_record_t *recptr = &fake_record;
    uint32 idbuf;

    memset(&fake_record, 0, sizeof(fake_record));
    fake_record.active_id = 2;

    OS_timebase_table[2].external_sync = UT_TimerSync;
    OS_timecb_table[0].wait_time = 2000;
    OS_timecb_table[0].callback_ptr = UT_TimeCB;
    TimerSyncCount = 0;
    TimerSyncRetVal = 0;
    TimeCB = 0;
    idbuf = 2;
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById),&idbuf, sizeof(idbuf), false);
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById),&recptr, sizeof(recptr), false);
    UT_SetHookFunction(UT_KEY(OS_TimeBaseLock_Impl), ClearObjectsHook, recptr);
    OS_TimeBase_CallbackThread(2);

    UtAssert_True(TimerSyncCount == 11, "TimerSyncCount (%lu) == 11", (unsigned long)TimerSyncCount);

    UT_ResetState(UT_KEY(OS_TimeBaseLock_Impl));
    TimerSyncCount = 0;
    TimerSyncRetVal = 1000;
    fake_record.active_id = 2;
    idbuf = 2;
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById),&idbuf, sizeof(idbuf), false);
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById),&recptr, sizeof(recptr), false);
    UT_SetHookFunction(UT_KEY(OS_TimeBaseLock_Impl), ClearObjectsHook, recptr);
    OS_TimeBase_CallbackThread(2);

    /* Check that the TimeCB function was called */
    UtAssert_True(TimeCB > 0, "TimeCB (%lu) > 0", (unsigned long)TimeCB);


    UT_SetForceFail(UT_KEY(OS_ObjectIdGetById), OS_ERROR);
    OS_TimeBase_CallbackThread(2);
}

void Test_OS_Tick2Micros(void)
{
    /*
     * Test Case For:
     * int32 OS_Tick2Micros (void)
     */
    OS_SharedGlobalVars.MicroSecPerTick = 5555;
    int32 actual = OS_Tick2Micros();

    UtAssert_True(actual == 5555, "OS_Tick2Micros() (%ld) == 5555", (long)actual);
}

void Test_OS_Milli2Ticks(void)
{
    /*
     * Test Case For:
     * int32 OS_Milli2Ticks(uint32 milli_seconds)
     */
    OS_SharedGlobalVars.TicksPerSecond = 500;
    int32 actual = OS_Milli2Ticks(5678);

    UtAssert_True(actual == 2839, "OS_Milli2Ticks() (%ld) == 2839", (long)actual);
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
    ADD_TEST(OS_TimeBaseAPI_Init);
    ADD_TEST(OS_TimeBaseCreate);
    ADD_TEST(OS_TimeBaseSet);
    ADD_TEST(OS_TimeBaseDelete);
    ADD_TEST(OS_TimeBaseGetIdByName);
    ADD_TEST(OS_TimeBaseGetInfo);
    ADD_TEST(OS_TimeBaseGetFreeRun);
    ADD_TEST(OS_TimeBase_CallbackThread);
    ADD_TEST(OS_Tick2Micros);
    ADD_TEST(OS_Milli2Ticks);
}





