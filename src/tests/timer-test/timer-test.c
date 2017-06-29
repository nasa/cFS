/*
** timertest.c
**
** This program is an OSAL sample that tests the OSAL timer functions. 
**
*/

#include <stdio.h>
#include <unistd.h>

#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"


#define NUMBER_OF_TIMERS 4

#define TASK_1_ID         1
#define TASK_1_STACK_SIZE 4096
#define TASK_1_PRIORITY   101

void TimerTestSetup(void);
void TimerTestTask(void);
void TimerTestCheck(void);

OS_time_t        StartTime;
OS_time_t        EndTime;
uint32           TimerStart[NUMBER_OF_TIMERS] = {1000, 2000000, 3000000, 4000000 };
uint32           TimerInterval[NUMBER_OF_TIMERS] = {500000, 400000, 800000, 600000 };

uint32 TimerTestTaskStack[TASK_1_STACK_SIZE];
int32 timer_counter[NUMBER_OF_TIMERS];
uint32 timer_idlookup[OS_MAX_TIMERS];

/*
** Test timer function.
** Note: For some Host OSs, this is the equivalent of an ISR, so the calls available are limited.
** For example, Linux and vxWorks can call functions like printf, but RTEMS cannot.
*/
void test_func(uint32 timer_id)
{
   timer_counter[timer_idlookup[timer_id]]++;
}


/* ********************** MAIN **************************** */

void OS_Application_Startup(void)
{

  if (OS_API_Init() != OS_SUCCESS)
  {
      UtAssert_Abort("OS_API_Init() failed");
  }

  /*
   * Register the timer test setup and check routines in UT assert
   */
  UtTest_Add(TimerTestCheck, TimerTestSetup, NULL, "TimerTest");
}

void TimerTestSetup(void)
{
    int32  status;
    uint32 TimerTestTaskId;

    /*
     * In the new versions of OSAL, timers do NOT work in the "main" thread,
     * so we must create a task to handle them.
     */
    status = OS_TaskCreate( &TimerTestTaskId, "Task 1", TimerTestTask, TimerTestTaskStack, TASK_1_STACK_SIZE, TASK_1_PRIORITY, 0);
    UtAssert_True(status == OS_SUCCESS, "Timer Test Task Created RC=%d", (int)status);

    /*
     * Invoke OS_IdleLoop() so that the task/timers can run
     *
     * OS_IdleLoop() will return once TimerTestTask calls OS_ApplicationShutdown
     *
     * It is important to note that UT Assert does NOT officially
     * support multi-threaded testing.  HOWEVER, the architecture of
     * this test ensures that the "TimerTestCheck" will NOT execute until
     * "TimerTestTask" gets to the end of its run.
     *
     * Therefore it is OK to use UT asserts within both functions.
     */
    OS_IdleLoop();
}

void TimerTestTask(void)
{
   
   int              i = 0;
   int32            TimerStatus;
   uint32           TimerID[NUMBER_OF_TIMERS];
   char             TimerName[NUMBER_OF_TIMERS][20] = {"TIMER1","TIMER2","TIMER3","TIMER4"};
   uint32           ClockAccuracy;


   for ( i = 0; i < NUMBER_OF_TIMERS; i++ )
   {
      TimerStatus = OS_TimerCreate(&TimerID[i], TimerName[i], &ClockAccuracy, &(test_func));
      UtAssert_True(TimerStatus == OS_SUCCESS, "Timer %d Created RC=%d ID=%d", i, (int)TimerStatus, (int)TimerID[i]);

      UtPrintf("Timer %d Accuracy = %d microseconds \n",i ,(int)ClockAccuracy);

      TimerStatus  =  OS_TimerSet(TimerID[i], TimerStart[i], TimerInterval[i]);
      UtAssert_True(TimerStatus == OS_SUCCESS, "Timer %d programmed RC=%d", i, (int)TimerStatus);

      timer_idlookup[TimerID[i]] = i;
   }


   /*
   ** Let the main thread sleep 
   */     
   UtPrintf("Starting Delay loop.\n");
   OS_GetLocalTime(&StartTime);
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
       TimerStatus =  OS_TimerDelete(TimerID[i]);
       UtAssert_True(TimerStatus == OS_SUCCESS, "Timer %d delete RC=%d. Count total = %d",
               i, (int)TimerStatus, (int)timer_counter[i]);
   }

   OS_ApplicationShutdown(TRUE);
   OS_TaskExit();
}

void TimerTestCheck(void)
{
    uint32           microsecs;
    int32           expected;
    uint32           i;

   /*
    * Time limited test - check and exit
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
      expected = (microsecs - TimerStart[i]) / TimerInterval[i];
      /*
       * Since all these counts are affected by test system load,
       * allow for some fudge factor before declaring failure
       */
      UtAssert_True(timer_counter[i] >= (expected - 3), "Timer %d count >= %d", (int)i, (int)(expected - 3));
      UtAssert_True(timer_counter[i] <= (expected + 3), "Timer %d count <= %d", (int)i, (int)(expected + 3));
   }
}

