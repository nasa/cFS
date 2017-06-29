/*
** Counting Semaphore Test
*/
#include <stdio.h>
#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* Define setup and check functions for UT assert */
void CountSemSetup(void);
void CountSemCheck(void);

#define TASK_STACK_SIZE  4096
#define TASK_1_PRIORITY  100
#define TASK_2_PRIORITY  110
#define TASK_3_PRIORITY  120

uint32 task_1_stack[TASK_STACK_SIZE];
uint32 task_1_id; 
uint32 task_1_failures;
uint32 task_1_work;

uint32 task_2_stack[TASK_STACK_SIZE];
uint32 task_2_id; 
uint32 task_2_failures;
uint32 task_2_work;

uint32 task_3_stack[TASK_STACK_SIZE];
uint32 task_3_id; 
uint32 task_3_failures;
uint32 task_3_work;

uint32 count_sem_id;

void task_1(void)
{
    uint32             status;

    OS_printf("Starting task 1\n");
    OS_TaskRegister();

    while(1)
    {
       OS_TaskDelay(2000);

       OS_printf("TASK 1: Giving the counting semaphore 1\n");
       status = OS_CountSemGive(count_sem_id);
       if ( status != OS_SUCCESS )
       {
          ++task_1_failures;
          OS_printf("TASK 1: Error calling OS_CountSemGive 1\n");
       }
       else
       {
          ++task_1_work;
          OS_printf("TASK 1: Counting Sem Give 1 complete\n");
       }

       OS_TaskDelay(500);

       OS_printf("TASK 1: Giving the counting semaphore 2\n");
       status = OS_CountSemGive(count_sem_id);
       if ( status != OS_SUCCESS )
       {
          ++task_1_failures;
          OS_printf("TASK 1: Error calling OS_CountSemGive 2\n");
       }
       else
       {
          ++task_1_work;
          OS_printf("TASK 1: Counting Sem Give 2 complete\n");
       }

    }
}

void task_2(void)
{
    uint32             status;

    OS_printf("Starting task 2\n");
    OS_TaskRegister();

    while(1)
    {
       OS_TaskDelay(1000);

       OS_printf("TASK 2: Waiting on the semaphore\n");
       status = OS_CountSemTake(count_sem_id);
       if ( status != OS_SUCCESS )
       {
          ++task_2_failures;
          OS_printf("TASK 2: Error calling OS_CountSemTake\n");
       }
       else
       {
          ++task_2_work;
          OS_printf("TASK 2: grabbed Counting Sem\n");
       }

    }
}

void task_3(void)
{
    uint32             status;

    OS_printf("Starting task 3\n");
    OS_TaskRegister();

    while(1)
    {
       OS_TaskDelay(1000);

       OS_printf("TASK 3: Waiting on the semaphore\n");
       status = OS_CountSemTake(count_sem_id);
       if ( status != OS_SUCCESS )
       {
          ++task_3_failures;
          OS_printf("TASK 3: Error calling OS_CountSemTake\n");
       }
       else
       {
          ++task_3_work;
          OS_printf("TASK 3: grabbed Counting Sem\n");
       }
    }
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
    UtTest_Add(CountSemCheck, CountSemSetup, NULL, "CountSemTest");
}

void CountSemSetup(void)
{
   uint32 status;
   int    i;

   task_1_failures = 0;
   task_2_failures = 0;
   task_3_failures = 0;
   task_1_work = 0;
   task_2_work = 0;
   task_3_work = 0;

   /*
   ** Create the Counting semaphore
   */
   status = OS_CountSemCreate( &count_sem_id, "CountSem1", 2, 0);
   UtAssert_True(status == OS_SUCCESS, "CountSem1 create Id=%u Rc=%d", (unsigned int)count_sem_id, (int)status);

   /*
   ** Take the semaphore so the value is 0 and the next SemTake call should block
   */
   for ( i = 0; i < 2; i++)
   {
      status = OS_CountSemTake(count_sem_id);
      UtAssert_True(status == OS_SUCCESS, "CountSem1 take Rc=%d", (int)status);
   }

   /*
   ** Create the tasks
   */
   status = OS_TaskCreate( &task_1_id, "Task 1", task_1, task_1_stack, TASK_STACK_SIZE, TASK_1_PRIORITY, 0);
   UtAssert_True(status == OS_SUCCESS, "Task 1 create Id=%u Rc=%d", (unsigned int)task_1_id, (int)status);

   status = OS_TaskCreate( &task_2_id, "Task 2", task_2, task_2_stack, TASK_STACK_SIZE, TASK_2_PRIORITY, 0);
   UtAssert_True(status == OS_SUCCESS, "Task 2 create Id=%u Rc=%d", (unsigned int)task_2_id, (int)status);
 
   status = OS_TaskCreate( &task_3_id, "Task 3", task_3, task_3_stack, TASK_STACK_SIZE, TASK_3_PRIORITY, 0);
   UtAssert_True(status == OS_SUCCESS, "Task 3 create Id=%u Rc=%d", (unsigned int)task_3_id, (int)status);

   /*
    * Time-limited execution
    */
   while(task_1_work < 10)
   {
      OS_TaskDelay(100);
   }

   /*
    * Delete the tasks
    */
   status = OS_TaskDelete(task_1_id);
   UtAssert_True(status == OS_SUCCESS, "Task 1 delete Rc=%d", (int)status);
   status = OS_TaskDelete(task_2_id);
   UtAssert_True(status == OS_SUCCESS, "Task 2 delete Rc=%d", (int)status);
   status = OS_TaskDelete(task_3_id);
   UtAssert_True(status == OS_SUCCESS, "Task 3 delete Rc=%d", (int)status);
}

void CountSemCheck(void)
{
    /* Task 2 and 3 should have both executed */
    UtAssert_True(task_2_work != 0, "Task 2 work counter = %u", (unsigned int)task_2_work);
    UtAssert_True(task_3_work != 0, "Task 3 work counter = %u", (unsigned int)task_3_work);

    /*
     * The sum of task 2 and task 3 work (consumer) cannot be greater than task 1 (the producer)
     * Add a fudge factor of +/- 2 to help avoid false failures due to scheduling
     */
    UtAssert_True((task_2_work + task_3_work) <= (task_1_work + 2), "Task 2+3 work < %u", (unsigned int)(task_1_work + 2));
    UtAssert_True((task_2_work + task_3_work) >= (task_1_work - 2), "Task 2+3 work > %u", (unsigned int)(task_1_work - 2));

    /* None of the tasks should have any failures in their own counters */
    UtAssert_True(task_1_failures == 0, "Task 1 failures = %u",(unsigned int)task_1_failures);
    UtAssert_True(task_2_failures == 0, "Task 2 failures = %u",(unsigned int)task_2_failures);
    UtAssert_True(task_3_failures == 0, "Task 3 failures = %u",(unsigned int)task_3_failures);
}

