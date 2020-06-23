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
 * Filename: timer-add-api-test.c
 *
 * Purpose: This file contains functional tests for "osapi-timer"
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

#define NUMBER_OF_TIMERS 4
#define TASK_1_ID         1
#define TASK_1_STACK_SIZE 4096
#define TASK_1_PRIORITY   101

OS_time_t        StartTime;
OS_time_t        EndTime;
uint32           TimerStart[NUMBER_OF_TIMERS] = {1000, 2000000, 3000000, 4000000 };
uint32           TimerInterval[NUMBER_OF_TIMERS] = {500000, 400000, 800000, 600000 };

uint32 TimerTestTaskStack[TASK_1_STACK_SIZE];
uint32 timer_counter[NUMBER_OF_TIMERS];


void counter_func(uint32 timer_id , void *arg)
{
   uint32 *counter = arg;
   ++(*counter);
}

void null_func(uint32 timer_id , void *arg)
{

}

/* *************************************** MAIN ************************************** */

void TestTimerAddApi(void)
{
    /*
     * Test Case For:
     * int32 OS_TimerAdd(uint32 *timer_id, const char *timer_name, uint32 timebase_ref_id, OS_ArgCallback_t  callback_ptr, void *callback_arg)
     */

    int32    actual;
    int32    expected;
    int32    tbc_ret_val;
    int32    tbs_ret_val;
    uint32   timer_id;
    uint32   time_base_id;
    int      i = 0;
    int32    TimerStatus[NUMBER_OF_TIMERS];
    uint32   TimerID[NUMBER_OF_TIMERS];
    char     TimerName[NUMBER_OF_TIMERS][20] = {"TIMER1","TIMER2","TIMER3","TIMER4"};
    uint32   microsecs;

   /* Create and set the TimeBase obj and verify success */

    tbc_ret_val = OS_TimeBaseCreate( &time_base_id, "TimeBase", 0);
    expected = OS_SUCCESS; 
    UtAssert_True(tbc_ret_val == expected, "OS_TimeBaseCreate() (%ld) == OS_SUCCESS", (long)tbc_ret_val);

    tbs_ret_val = OS_TimeBaseSet(time_base_id, 10000, 10000); /* ms */
    expected = OS_SUCCESS; 
    UtAssert_True(tbs_ret_val == expected, "OS_TimeBaseSet() (%ld) == OS_SUCCESS", (long)tbs_ret_val);


   for ( i = 0; i < NUMBER_OF_TIMERS; i++ )
   {
      TimerStatus[i] = OS_TimerAdd(&TimerID[i], TimerName[i], time_base_id, &counter_func, &timer_counter[i]);
      UtAssert_True(TimerStatus[i] == OS_SUCCESS, "Timer %d Created RC=%d ID=%d", i, (int)TimerStatus[i], (int)TimerID[i]);

   }

   /* Sample the clock now, before starting any timer */
   OS_GetLocalTime(&StartTime);
   for ( i = 0; i < NUMBER_OF_TIMERS; i++ )
   {
      /*
       * to ensure that all timers are started as closely as possible,
       * this just stores the result and does not assert/printf now
       */
      TimerStatus[i]  =  OS_TimerSet(TimerID[i], TimerStart[i], TimerInterval[i]);
   }

   /*
    * Now the actual OS_TimerSet() return code can be checked.
    */
   for ( i = 0; i < NUMBER_OF_TIMERS; i++ )
   {
       UtAssert_True(TimerStatus[i] == OS_SUCCESS, "Timer %d programmed RC=%d", i, (int)TimerStatus[i]);
   }

   /*
   ** Let the main thread sleep 
   */     
   UtPrintf("Starting Delay loop.\n");
   for (i = 0 ; i < 30; i++ )
   {
      /* 
      ** Even though this sleep call is for 1 second,
      ** the sigalarm timer for the 1hz will keep waking
      ** it up. Keep that in mind when setting the timer down
      ** to something very small.
      */
      OS_TaskDelay(1000);
   }

   OS_GetLocalTime(&EndTime);

   for ( i = 0; i < NUMBER_OF_TIMERS; i++ )
   {
       TimerStatus[i] =  OS_TimerDelete(TimerID[i]);
   }

   for ( i = 0; i < NUMBER_OF_TIMERS; i++ )
   {
       UtAssert_True(TimerStatus[i] == OS_SUCCESS, "Timer %d delete RC=%d. Count total = %d",
               i, (int)TimerStatus[i], (int)timer_counter[i]);
   }

   /*
    * Time limited test 
    */
   microsecs = 1000000 * (EndTime.seconds - StartTime.seconds);
   if (EndTime.microsecs < StartTime.microsecs)
   {
      microsecs -= StartTime.microsecs - EndTime.microsecs;
   }
   else
   {
      microsecs += EndTime.microsecs - StartTime.microsecs;
   }

   /* Make sure the ratio of the timers are OK */
   for ( i = 0; i < NUMBER_OF_TIMERS; i++ )
   {
      /*
       * Expect one tick after the start time (i.e. first tick)
       * Plus one tick for every interval that occurred during the test
       */
      expected = 1 + (microsecs - TimerStart[i]) / TimerInterval[i];
      UtAssert_True(expected > 0, "Expected ticks = %u", (unsigned int)expected);

      /*
       * Since all these counts are affected by test system load,
       * allow for some fudge factor before declaring failure
       */
      UtAssert_True(timer_counter[i] >= (expected - 3), "Timer %d count >= %d", (int)i, (int)(expected - 3));
      UtAssert_True(timer_counter[i] <= (expected + 3), "Timer %d count <= %d", (int)i, (int)(expected + 3));
   }

    /* Test nominal inputs */
    expected = OS_SUCCESS; 
    actual = OS_TimerAdd(&timer_id, "Timer", time_base_id, null_func, NULL);
    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_SUCCESS", (long)actual);

    /* Test invalid inputs */
    expected = OS_INVALID_POINTER;
    actual = OS_TimerAdd(NULL, "Timer", time_base_id, null_func, NULL);
    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_ERR_INVALID_ID;
    actual = OS_TimerAdd(&timer_id, "Timer", 1, null_func, NULL);
    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_ERR_INVALID_ID", (long)actual);

    expected = OS_TIMER_ERR_INVALID_ARGS;
    actual = OS_TimerAdd(&timer_id, "Timer",time_base_id , NULL, NULL);
    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_TIMER_ERR_INVALID_ARGS", (long)actual);

    expected = OS_ERR_NAME_TAKEN;
    actual = OS_TimerAdd(&timer_id, "Timer", time_base_id, null_func, NULL);
    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_ERR_NAME_TAKEN", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_TimerAdd(&timer_id, 0, time_base_id, null_func, NULL);
    UtAssert_True(actual == expected, "OS_TimerAdd() (%ld) == OS_INVALID_POINTER", (long)actual);


} /* end TestTimerAddApi */


void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(TestTimerAddApi, NULL, NULL, "TestTimerAddApi");
}

