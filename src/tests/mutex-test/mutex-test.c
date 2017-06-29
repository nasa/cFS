/*
** Binary Semaphore Flush Test
*/
#include <stdio.h>
#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* Define setup and check functions for UT assert */
void MutexSetup(void);
void MutexCheck(void);

#define TASK_STACK_SIZE  4096
#define TASK_1_PRIORITY  100
#define TASK_2_PRIORITY  110
#define TASK_3_PRIORITY  120

uint32 task_1_stack[TASK_STACK_SIZE];
uint32 task_1_id; 
uint32 task_1_failures;

uint32 task_2_stack[TASK_STACK_SIZE];
uint32 task_2_id; 
uint32 task_2_failures;

uint32 task_3_stack[TASK_STACK_SIZE];
uint32 task_3_id; 
uint32 task_3_failures;

uint32 mut_sem_id;
uint32 shared_obj_owner;

int    counter = 0;

void task_1(void)
{
    uint32             status;

    OS_printf("Starting task 1\n");
    OS_TaskRegister();

    while (1)
    {
       status = OS_TaskDelay(100);
       status = OS_MutSemTake(mut_sem_id);
       if ( status != OS_SUCCESS )
       {
          ++task_1_failures;
          OS_printf("TASK 1:Error calling OS_MutSemTake\n");
       }
       else
       {
          OS_printf("TASK 1 Has the Mutex\n");
       }

       OS_printf("TASK 1   Doing some work: %d\n", (int)counter++);
       if (shared_obj_owner != 0)
       {
          ++task_1_failures;
       }

       /* Hold the resource for a while and check if any interference occurs */
       shared_obj_owner = 1;
       for(status = 0; status < 100000; ++status)
       {
          if (shared_obj_owner != 1)
          {
             ++task_1_failures;
             shared_obj_owner = 1;
          }
       }
       shared_obj_owner = 0;

       status = OS_MutSemGive(mut_sem_id);
       if ( status != OS_SUCCESS )
       {
          ++task_1_failures;
          OS_printf("TASK 1:Error calling OS_MutSemGive\n");
       }
       else
       {
          OS_printf("TASK 1 Released the Mutex\n");
       }
   }
}

void task_2(void)
{
    uint32             status;

    OS_printf("Starting task 2\n");
    OS_TaskRegister();

    while (1)
    {
       status = OS_TaskDelay(200);

       status = OS_MutSemTake(mut_sem_id);
       if ( status != OS_SUCCESS )
       {
          ++task_2_failures;
          OS_printf("TASK 2:Error calling OS_MutSemTake\n");
       }
       else
       {
          OS_printf("TASK 2 Has the Mutex\n");
       }

       OS_printf("TASK 2:   Doing some work: %d\n", (int)counter++);
       if (shared_obj_owner != 0)
       {
          ++task_2_failures;
       }


       /* Hold the resource for a while and check if any interference occurs */
       shared_obj_owner = 2;
       for(status = 0; status < 100000; ++status)
       {
          if (shared_obj_owner != 2)
          {
             ++task_2_failures;
             shared_obj_owner = 2;
          }
       }
       shared_obj_owner = 0;

       status = OS_MutSemGive(mut_sem_id);
       if ( status != OS_SUCCESS )
       {
          ++task_2_failures;
          OS_printf("TASK 2:Error calling OS_MutSemGive\n");
       }
       else
       {
          OS_printf("TASK 2 Released the Mutex\n");
       }
    }
}

void task_3(void)
{
    uint32             status;

    OS_printf("Starting task 3\n");
    OS_TaskRegister();

    while (1)
    {
 
       status = OS_TaskDelay(300);
       status = OS_MutSemTake(mut_sem_id);
       if ( status != OS_SUCCESS )
       {
          ++task_3_failures;
          OS_printf("TASK 3:Error calling OS_MutSemTake\n");
       }
       else
       {
          OS_printf("TASK 3 Has the Mutex\n");
       }

       OS_printf("TASK 3:   Doing some work: %d\n", (int)counter++);
       if (shared_obj_owner != 0)
       {
          ++task_3_failures;
       }

       /* Hold the resource for a while and check if any interference occurs */
       shared_obj_owner = 3;
       for(status = 0; status < 100000; ++status)
       {
          if (shared_obj_owner != 3)
          {
             ++task_3_failures;
             shared_obj_owner = 3;
          }
       }
       shared_obj_owner = 0;

       status = OS_MutSemGive(mut_sem_id);
       if ( status != OS_SUCCESS )
       {
          ++task_3_failures;
          OS_printf("TASK 3:Error calling OS_MutSemGive\n");
       }
       else
       {
          OS_printf("TASK 3 Released the Mutex\n");
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
    UtTest_Add(MutexCheck, MutexSetup, NULL, "MutexTest");
}

void MutexSetup(void)
{
   uint32 status;

   task_1_failures = 0;
   task_2_failures = 0;
   task_3_failures = 0;
   shared_obj_owner = 0;

   /*
   ** Create the mutex 
   */
   status = OS_MutSemCreate( &mut_sem_id, "MutSem1", 0);
   UtAssert_True(status == OS_SUCCESS, "MutSem1 create Id=%u Rc=%d", (unsigned int)mut_sem_id, (int)status);

   /*
   ** Test the mutex to see if it supports nesting 
   */
   status = OS_MutSemTake(mut_sem_id);
   UtAssert_True(status == OS_SUCCESS, "OS_MutSemTake 1 Rc=%d", (int)status);

   status = OS_MutSemTake(mut_sem_id);
   UtAssert_True(status == OS_SUCCESS, "OS_MutSemTake 2 Rc=%d", (int)status);

   status = OS_MutSemGive(mut_sem_id);
   UtAssert_True(status == OS_SUCCESS, "OS_MutSemGive 2 Rc=%d", (int)status);

   status = OS_MutSemGive(mut_sem_id);
   UtAssert_True(status == OS_SUCCESS, "OS_MutSemGive 1 Rc=%d", (int)status);

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
    * Time limited execution
    */
   while (counter < 100)
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

void MutexCheck(void)
{
    /* None of the tasks should have any failures in their own counters */
    UtAssert_True(task_1_failures == 0, "Task 1 failures = %u",(unsigned int)task_1_failures);
    UtAssert_True(task_2_failures == 0, "Task 2 failures = %u",(unsigned int)task_2_failures);
    UtAssert_True(task_3_failures == 0, "Task 3 failures = %u",(unsigned int)task_3_failures);
}

