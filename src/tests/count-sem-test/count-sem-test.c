/*
** Counting Semaphore Test
*/
#include <stdio.h>
#include "common_types.h"
#include "osapi.h"

#define TASK_STACK_SIZE  4096
#define TASK_1_PRIORITY  100
#define TASK_2_PRIORITY  110
#define TASK_3_PRIORITY  120

uint32 task_1_stack[TASK_STACK_SIZE];
uint32 task_1_id; 

uint32 task_2_stack[TASK_STACK_SIZE];
uint32 task_2_id; 

uint32 task_3_stack[TASK_STACK_SIZE];
uint32 task_3_id; 

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
          OS_printf("TASK 1: Error calling OS_CountSemGive 1\n");
       }
       else
       {
          OS_printf("TASK 1: Counting Sem Give 1 complete\n");
       }

       OS_TaskDelay(500);

       OS_printf("TASK 1: Giving the counting semaphore 2\n");
       status = OS_CountSemGive(count_sem_id);
       if ( status != OS_SUCCESS )
       {
          OS_printf("TASK 1: Error calling OS_CountSemGive 2\n");
       }
       else
       {
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
          OS_printf("TASK 2: Error calling OS_CountSemTake\n");
       }
       else
       {
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
          OS_printf("TASK 3: Error calling OS_CountSemTake\n");
       }
       else
       {
          OS_printf("TASK 3: grabbed Counting Sem\n");
       }
    }
}

void OS_Application_Startup(void)
{
   uint32 status;
   int    i;

   OS_API_Init();

   OS_printf("OS Application Startup\n");

   /*
   ** Create the Counting semaphore
   */
   status = OS_CountSemCreate( &count_sem_id, "CountSem1", 2, 0);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error creating Count Sem\n");
   }
   else
   {
      OS_printf("Counting Sem ID = %d\n", (int)count_sem_id);
   }

   /*
   ** Take the semaphore so the value is 0 and the next SemTake call should block
   */
   for ( i = 0; i < 2; i++)
   {
      status = OS_CountSemTake(count_sem_id);
      if ( status != OS_SUCCESS )
      {
         OS_printf("Error calling OS_CountSemTake with count_sem_id = %d\n",(int)count_sem_id);
      }
      else
      {
         OS_printf("Take Counting Sem: %d\n",i);
      }
   }

   /*
   ** Create the tasks
   */
   status = OS_TaskCreate( &task_1_id, "Task 1", task_1, task_1_stack, TASK_STACK_SIZE, TASK_1_PRIORITY, 0);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error creating Task 1\n");
   }
   else
   {
      OS_printf("Created Task 1\n");
   }

   status = OS_TaskCreate( &task_2_id, "Task 2", task_2, task_2_stack, TASK_STACK_SIZE, TASK_2_PRIORITY, 0);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error creating Task 2\n");
   }
   else
   {
      OS_printf("Created Task 2\n");
   }
 
   status = OS_TaskCreate( &task_3_id, "Task 3", task_3, task_3_stack, TASK_STACK_SIZE, TASK_3_PRIORITY, 0);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error creating Task 3\n");
   }
   else
   {
      OS_printf("Created Task 3\n");
   }

   OS_printf("Main done!\n");

}

