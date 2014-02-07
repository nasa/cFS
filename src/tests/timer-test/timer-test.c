/*
** timertest.c
**
** This program is an OSAL sample that tests the OSAL timer functions. 
**
*/

#include <stdio.h>
#include <unistd.h>

#include "osapi.h"

#define NUMBER_OF_TIMERS 4

#define TASK_1_ID         1
#define TASK_1_STACK_SIZE 4096
#define TASK_1_PRIORITY   101

void TimerTestTask(void);

uint32 TimerTestTaskStack[TASK_1_STACK_SIZE];
uint32 timer_counter[NUMBER_OF_TIMERS];

/*
** Test timer function.
** Note: For some Host OSs, this is the equivalent of an ISR, so the calls available are limited.
** For example, Linux and vxWorks can call functions like printf, but RTEMS cannot.
*/
void test_func(uint32 timer_id)
{
   timer_counter[timer_id]++;
}


/* ********************** MAIN **************************** */

void OS_Application_Startup(void)
{
  int32  status;
  uint32 TimerTestTaskId;

  OS_API_Init();

  status = OS_TaskCreate( &TimerTestTaskId, "Task 1", TimerTestTask, TimerTestTaskStack, TASK_1_STACK_SIZE, TASK_1_PRIORITY, 0);
  if ( status != OS_SUCCESS )
  {
     OS_printf("Error creating Timer Test Task\n");
  }

}

void TimerTestTask(void)
{
   
   int              i = 0;
   int32            TimerStatus;
   uint32           TimerID[NUMBER_OF_TIMERS];
   char             TimerName[NUMBER_OF_TIMERS][20] = {"TIMER1","TIMER2","TIMER3","TIMER4"};
   uint32           TimerStart[NUMBER_OF_TIMERS] = {1000, 2000000, 3000000, 4000000 };
   uint32           TimerInterval[NUMBER_OF_TIMERS] = {500000, 4000000, 4000000, 4000000 }; 
   uint32           ClockAccuracy;


   for ( i = 0; i < NUMBER_OF_TIMERS; i++ )
   {
      TimerStatus = OS_TimerCreate(&TimerID[i], TimerName[i], &ClockAccuracy, &(test_func));
      if ( TimerStatus != OS_SUCCESS )
      {
         OS_printf("Error creating timer# %d: %d\n",i ,(int)TimerStatus);       
      }   
      else
      {
         OS_printf("Timer %d Created\n", i);
      }

      OS_printf("Timer %d Accuracy = %d microseconds \n",i ,(int)ClockAccuracy);

      TimerStatus  =  OS_TimerSet(TimerID[i], TimerStart[i], TimerInterval[i]);
      if ( TimerStatus != OS_SUCCESS )
      {
         OS_printf("Error calling OS_TimerSet: ID = %d\n", (int)TimerID[i]);
      }
      else
      {  
         OS_printf("Timer %d programmed\n", i);
      }

   }


   /*
   ** Let the main thread sleep 
   */     
   OS_printf("Starting Delay loop.\n");
   for (i = 0 ; i < 15; i++ )
   {
      /* 
      ** Even though this sleep call is for 1 second,
      ** the sigalarm timer for the 1hz will keep waking
      ** it up. Keep that in mind when setting the timer down
      ** to something very small.
      */
      OS_TaskDelay(1000);
   }

   for ( i = 0; i < NUMBER_OF_TIMERS; i++ )
   {
                 
      TimerStatus =  OS_TimerDelete(TimerID[i]);
      if ( TimerStatus != OS_SUCCESS )
      {
         OS_printf("Error calling OS_TimerDelete for timer %d: ID = %d\n", i, (int)TimerID[i]);
      }
      else
      {
         OS_printf("Timer %d deleted. Count total = %d\n",i, (int)timer_counter[i]);
      }
 
   }
   OS_printf("Hit control-c to end test on a desktop system.\n");

}

