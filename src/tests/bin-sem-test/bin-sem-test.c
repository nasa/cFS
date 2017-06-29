/*
** Binary semaphore Producer/Consumer test
*/
#include <stdio.h>
#include <stdlib.h>
#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* Define setup and check functions for UT assert */
void BinSemSetup(void);
void BinSemCheck(void);

/* Task 1 */
#define TASK_1_ID         1
#define TASK_1_STACK_SIZE 1024
#define TASK_1_PRIORITY   101
#define TASK_2_STACK_SIZE 1024
#define TASK_2_PRIORITY   50

#define TIMER_ENTRY 0x001
#define TIMER_EXIT  0x002
#define TASK_ENTRY  0x003
#define TASK_EXIT   0x004

uint32 task_1_stack[TASK_1_STACK_SIZE];
uint32 task_1_id;
uint32 task_1_failures;
uint32 task_2_stack[TASK_2_STACK_SIZE];
uint32 task_2_id;
uint32 timer_failures;

uint32 bin_sem_id;

uint32 timer_counter;
uint32 timer_id;
uint32 timer_start = 1000;
uint32 timer_interval = 10000; /* 1000 = 1000 hz, 10000 == 100 hz */
uint32 timer_accuracy;

int    counter = 0;

/*
 * Note that we should not call "printf" or anything
 * like that during a timer callback routine (may be ISR context)
 *
 * On RTEMS even a call to BinSemGetInfo has very ill effects.
 */
void TimerFunction(uint32 timer_id)
{
   int32              status;
  
   timer_counter++;

   status = OS_BinSemGive(bin_sem_id);
   if ( status != OS_SUCCESS )
   {
       ++timer_failures;
   }

#ifndef _RTEMS_OS_
   {
       OS_bin_sem_prop_t  bin_sem_prop;
       status = OS_BinSemGetInfo (bin_sem_id, &bin_sem_prop);
       if ( status != OS_SUCCESS )
       {
           ++timer_failures;
       }
       else if ( bin_sem_prop.value > 1 )
       {
           ++timer_failures;
       }
       else if ( bin_sem_prop.value < -1 )
       {
           ++timer_failures;
       }
   }
#endif

}

void task_1(void)
{
    uint32             status;
    OS_bin_sem_prop_t  bin_sem_prop;
    int                printf_counter = 0;

    OS_printf("Starting task 1\n");

    OS_TaskRegister();

    OS_printf("Delay for 1 second before starting\n");
    OS_TaskDelay(1000);

    while(1)
    {

        status = OS_BinSemTake(bin_sem_id);
        if ( status != OS_SUCCESS )
        {
           OS_printf("TASK 1:Error calling OS_BinSemTake\n");
           ++task_1_failures;
        }
        else
        {
           printf_counter++;
           counter++;
          
           if ( printf_counter > 100 ) 
           {
              OS_printf("TASK 1: counter:%d timer_counter:%d\n", (int)counter,(int)timer_counter);
              printf_counter = 0;
           }
           status = OS_BinSemGetInfo (bin_sem_id, &bin_sem_prop);
           if ( status != OS_SUCCESS )
           {
              OS_printf("TASK 1:Error calling OS_BinSemGetInfo\n");
              ++task_1_failures;
           }
           else if ( bin_sem_prop.value > 1 )
           {
                OS_printf("Error: Binary sem value > 1 ( in task):%d !\n",(int)bin_sem_prop.value);
                ++task_1_failures;
           }
           else if ( bin_sem_prop.value < -1 )
           {
                OS_printf("Error: Binary sem value < -1 ( in task):%d !\n",(int)bin_sem_prop.value);
                ++task_1_failures;
           }
        }

    }
}

void task_2(void)
{
   OS_TaskRegister();

   /*
    * Limit the amount of time the test runs
    */
   while(timer_counter < 1000)
   {
      OS_TaskDelay(100);
   }

   OS_ApplicationShutdown(TRUE);
   OS_TaskExit();
}

void BinSemCheck(void)
{
    uint32 status;

    status = OS_TimerDelete(timer_id);
    UtAssert_True(status == OS_SUCCESS, "OS_TimerDelete Rc=%d", (int)status);

    UtAssert_True(counter < timer_counter, "Task counter (%d) < timer counter (%d)", (int)counter, (int)timer_counter);
    UtAssert_True(task_1_failures == 0, "Task 1 failures = %u", (unsigned int)task_1_failures);
    UtAssert_True(timer_failures == 0, "Timer failures = %u", (unsigned int)timer_failures);
}


void OS_Application_Startup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(BinSemCheck, BinSemSetup, NULL, "BinSemTest");
}

void BinSemSetup(void)
{
    uint32             status;
    uint32             accuracy;
    OS_bin_sem_prop_t  bin_sem_prop;

    /* separate task failure counter because ut-assert is not reentrant */
    task_1_failures = 0;
    timer_failures = 0;

    /*
    ** Create the binary semaphore
    */
    status = OS_BinSemCreate( &bin_sem_id, "BinSem1", 1, 0);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 create Id=%u Rc=%d", (unsigned int)bin_sem_id, (int)status);

    status = OS_BinSemGetInfo (bin_sem_id, &bin_sem_prop);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 value=%d Rc=%d", (int)bin_sem_prop.value, (int)status);

    /*
    ** Take the semaphore so the value is 0 and the next SemTake call should block
    */
    status = OS_BinSemTake(bin_sem_id);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 take Rc=%d", (int)status);
    status = OS_BinSemGetInfo (bin_sem_id, &bin_sem_prop);
    UtAssert_True(status == OS_SUCCESS, "BinSem1 value=%d Rc=%d", (int)bin_sem_prop.value, (int)status);

    /*
    ** Create the "consumer" task.
    */
    status = OS_TaskCreate( &task_1_id, "Task 1", task_1, task_1_stack, TASK_1_STACK_SIZE, TASK_1_PRIORITY, 0);
    UtAssert_True(status == OS_SUCCESS, "Task 1 create Id=%u Rc=%d", (unsigned int)task_1_id, (int)status);

    /*
    ** Create the "producer" task.
    */
    status = OS_TaskCreate( &task_2_id, "Task 2", task_2, task_2_stack, TASK_2_STACK_SIZE, TASK_2_PRIORITY, 0);
    UtAssert_True(status == OS_SUCCESS, "Task 2 create Id=%u Rc=%d", (unsigned int)task_2_id, (int)status);

    /*
    ** Create a timer
    */
    status = OS_TimerCreate(&timer_id, "Timer 1", &accuracy, &(TimerFunction));
    UtAssert_True(status == OS_SUCCESS, "Timer 1 create Id=%u Rc=%d", (unsigned int)timer_id, (int)status);
    UtPrintf("Timer Accuracy = %u microseconds \n",(unsigned int)accuracy);

    /*
    ** Start the timer
    */
    status  =  OS_TimerSet(timer_id, timer_start, timer_interval);
    UtAssert_True(status == OS_SUCCESS, "Timer 1 set Rc=%d", (int)status);

    /*
     * Call OS_IdleLoop so the tasks and timers can run
     * Something must call OS_ApplicationShutdown when done which will continue the test
     */
    OS_IdleLoop();
}
