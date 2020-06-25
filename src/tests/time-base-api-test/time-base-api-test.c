/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * Filename: time-base-api-test.c
 *
 * Purpose: This file contains functional tests for "osapi-timebase"
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

static uint32 UT_TimerSync(uint32 timer_id)
{
    OS_TaskDelay(1);
    return 1;
}


/* *************************************** MAIN ************************************** */

void TestTimeBaseApi(void)
{
    int32 expected;
    int32 actual; 
    int32 TimeBaseNum;
    int32 tbc_results[OS_MAX_TIMEBASES];
    uint32 freerun;
    uint32 objid;
    uint32 time_base_id; 
    uint32 time_base_id2; 
    uint32 tb_id[OS_MAX_TIMEBASES];
    char overMaxTimeBase[12];
    char TimeBaseIter[OS_MAX_TIMEBASES][12];
    OS_timebase_prop_t timebase_prop;

    /*
     * Test Case For:
     * int32 OS_TimeBaseCreate(uint32 *timer_id, const char *timebase_name, OS_TimerSync_t external_sync)
     */

    /* Test for nominal inputs */
    expected = OS_SUCCESS;

    actual= OS_TimeBaseCreate(&time_base_id, "TimeBaseA", 0);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_TimeBaseCreate(&time_base_id2, "TimeBaseB", NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)actual);

    actual = OS_TimeBaseCreate(&time_base_id, "TimeBaseC", UT_TimerSync);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)actual);

    /* Test for nominal, max/min cases */
    objid = 0xFFFFFFFF;
    actual = OS_TimeBaseCreate(&objid, "TimeBaseD", 0);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)actual);


    /* Checking for OS_MAX_TIMEBASES */
    for ( int i = 0; i < OS_MAX_TIMEBASES; i++ )
    {
        snprintf(TimeBaseIter[i], 12, "TimeBase%d", i);
        tbc_results[i] = OS_TimeBaseCreate(&tb_id[i], TimeBaseIter[i], 0);
        UtAssert_True(tbc_results[i] == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)actual);

        OS_TimeBaseDelete(tb_id[i]);
    }


    /* Test for invalid inputs */
    expected = OS_INVALID_POINTER;
    actual = OS_TimeBaseCreate(NULL, NULL, NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_TimeBaseCreate(NULL, "TimeBase6", 0);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_TimeBaseCreate(&time_base_id, NULL, 0);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_ERR_NAME_TAKEN;
    actual= OS_TimeBaseCreate(&time_base_id, "TimeBaseA", 0);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_ERR_NAME_TAKEN", (long)actual);

    /* Checking OS_MAX_TIMEBASES + 1 */
    for ( int i = 0; i < OS_MAX_TIMEBASES; i++ )
    {
        snprintf(TimeBaseIter[i], sizeof(TimeBaseIter[i]), "TimeBase%d", i);
        tbc_results[i] = OS_TimeBaseCreate(&tb_id[i], TimeBaseIter[i], 0);
    }
    TimeBaseNum = OS_MAX_TIMEBASES+1;
    snprintf(overMaxTimeBase, sizeof(overMaxTimeBase), "TimeBase%d", (int)TimeBaseNum);
    expected = OS_ERR_NO_FREE_IDS;
    actual= OS_TimeBaseCreate(&time_base_id, "overMaxTimeBase", 0);
    UtAssert_True(actual == expected, "OS_TimeBaseCreate() (%ld) == OS_ERR_NO_FREE_IDS", (long)actual);



    /*
     * Test Case For:
     * int32 OS_TimeBaseSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
     */

    /* Test for nominal inputs */
    expected = OS_SUCCESS;
    actual = OS_TimeBaseSet(time_base_id, 1000, 1000);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_SUCCESS;
    actual = OS_TimeBaseSet(time_base_id, 0, 0);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_SUCCESS", (long)actual);

    /* Test for invalid inputs */
    /* overflow on input */
    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual = OS_TimeBaseSet(time_base_id, UINT32_MAX, UINT32_MAX);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual = OS_TimeBaseSet(time_base_id, -1000, -1000);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual = OS_TimeBaseSet(time_base_id, 1000, -1000);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual = OS_TimeBaseSet(time_base_id, -1000, 1000);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    expected = OS_ERR_INVALID_ID;
    actual = OS_TimeBaseSet(0, 1000, 1000);
    UtAssert_True(actual == expected, "OS_TimeBaseSet() (%ld) == OS_ERR_INVALID_ID", (long)actual);


    /*
     * Test Case For:
     * int32 OS_TimeBaseDelete(uint32 timer_id)
     */

    /* Test for nominal inputs */
    expected = OS_SUCCESS;
    actual = OS_TimeBaseDelete(time_base_id);
    UtAssert_True(actual == expected, "OS_TimeBaseDelete() (%ld) == OS_SUCCESS", (long)actual);

    /* Test for invalid inputs */
    expected = OS_ERR_INVALID_ID;
    actual = OS_TimeBaseDelete(0x0000000);
    UtAssert_True(actual == expected, "OS_TimeBaseDelete() (%ld) == OS_ERR_INVALID_ID", (long)actual);

    expected = OS_ERR_INVALID_ID;
    actual = OS_TimeBaseDelete(0xFFFFFFF);
    UtAssert_True(actual == expected, "OS_TimeBaseDelete() (%ld) == OS_ERR_INVALID_ID", (long)actual);

    /*
     * Test Case For:
     * int32 OS_TimeBaseGetIdByName (uint32 *timer_id, const char *timebase_name)
     */

    /* Test for nominal inputs */
    /* Note: TimeBase2 was created above using TimeBaseCreate and id was set to time_base_id2 */
    expected = OS_SUCCESS;
    objid = 0;
    actual = OS_TimeBaseGetIdByName(&objid, "TimeBaseB");
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid == time_base_id2, "OS_TimeBaseGetIdByName() objid (%lu) Matches!", (unsigned long)objid);


    /* Test for invalid inputs */
    expected = OS_ERR_NAME_NOT_FOUND;
    objid = 0;
    actual = OS_TimeBaseGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == OS_ERR_NAME_NOT_FOUND",(long)actual);
    UtAssert_True(objid == 0, "OS_TimeBaseGetIdByName() objid (%lu) still 0", (unsigned long)objid);

    expected = OS_INVALID_POINTER;
    actual = OS_TimeBaseGetIdByName(NULL, NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseGetIdByName() (%ld) == OS_INVALID_POINTER", (long)actual);


    /*
     * Test Case For:
     * int32 OS_TimeBaseGetInfo (uint32 timebase_id, OS_timebase_prop_t *timebase_prop)
     */
    expected = OS_SUCCESS;

    /* Test for nominal inputs */
    /* Note: TimeBase2 was created above using TimeBaseCreate */

    /* Initializing timebase_prop values to something other than time_base_id2 to ensure they have changed */
    timebase_prop.creator = 1111;
    strncpy(timebase_prop.name, "ABC", sizeof(
timebase_prop.name));
    timebase_prop.nominal_interval_time = 2222;
    timebase_prop.freerun_time = 3333;
    timebase_prop.accuracy = 0;

    actual = OS_TimeBaseGetInfo(time_base_id2, &timebase_prop);

    UtAssert_True(actual == expected, "OS_TimeBaseGetInfo() (%ld) == OS_SUCCESS", (long)actual);

    UtAssert_True(timebase_prop.creator == 0, "timebase_prop.creator (%lu) == 0",
            (unsigned long)timebase_prop.creator);
    UtAssert_True(strcmp(timebase_prop.name, "TimeBaseB") == 0, "timebase_prop.name (%s) == TimeBase2",
            timebase_prop.name);
    UtAssert_True(timebase_prop.nominal_interval_time == 0,
            "timebase_prop.nominal_interval_time (%lu) == 0",
            (unsigned long)timebase_prop.nominal_interval_time);
    UtAssert_True(timebase_prop.freerun_time == 0,
            "timebase_prop.freerun_time (%lu) == 0",
            (unsigned long)timebase_prop.freerun_time);
    UtAssert_True(timebase_prop.accuracy >= 0,
            "timebase_prop.accuracy (%lu) >= 0",
            (unsigned long)timebase_prop.accuracy);

    /* Test for invalid inputs */
    expected = OS_ERR_INVALID_ID;
    actual = OS_TimeBaseGetInfo(1, &timebase_prop);
    UtAssert_True(actual == expected, "OS_TimeBaseGetInfo() (%ld) == OS_ERR_INVALID_ID", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_TimeBaseGetInfo(time_base_id2, NULL);
    UtAssert_True(actual == expected, "OS_TimeBaseGetInfo() (%ld) == OS_INVALID_POINTER", (long)actual);

    /*
     * Test Case For:
     * int32 OS_TimeBaseGetFreeRun     (uint32 timebase_id, uint32 *freerun_val)
     */

    /* Test for nominal inputs */
    /* Note: TimeBase2 was created above using TimeBaseCreate */
    expected = OS_SUCCESS;

    freerun = 0xFFFFFFFF;
    actual = OS_TimeBaseGetFreeRun(time_base_id2, &freerun);
    UtAssert_True(actual == expected, "OS_TimeBaseGetFreeRun() (%ld) == OS_SUCCESS", (long)actual);

    freerun = 0x0000000;
    actual = OS_TimeBaseGetFreeRun(time_base_id2, &freerun);
    UtAssert_True(actual == expected, "OS_TimeBaseGetFreeRun() (%ld) == OS_SUCCESS", (long)actual);

    /* Test for invalid inputs */
    expected = OS_ERR_INVALID_ID;
    freerun = 0xFFFFFFFF;
    actual = OS_TimeBaseGetFreeRun(1, &freerun);
    UtAssert_True(actual == expected, "OS_TimeBaseGetFreeRun() (%ld) == OS_SUCCESS", (long)actual);




} /* end TestTimeBaseApi */


void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(TestTimeBaseApi, NULL, NULL, "TestTimeBaseApi");
}

