/*
** Binary Semaphore Flush Test
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

uint32 mut_sem_id;

int    counter = 0;

void task_1(void)
{
    uint32             status;

    OS_printf("Starting task 1\n");
    OS_TaskRegister();

    while (1)
    {
       status = OS_TaskDelay(1000); 
       status = OS_MutSemTake(mut_sem_id);
       if ( status != OS_SUCCESS )
       {
          OS_printf("TASK 1:Error calling OS_MutSemTake\n");
       }
       else
       {
          OS_printf("TASK 1 Has the Mutex\n");
       }

       OS_printf("TASK 1   Doing some work: %d\n", (int)counter++);

       status = OS_MutSemGive(mut_sem_id);
       if ( status != OS_SUCCESS )
       {
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
       status = OS_TaskDelay(1000);

       status = OS_MutSemTake(mut_sem_id);
       if ( status != OS_SUCCESS )
       {
          OS_printf("TASK 2:Error calling OS_MutSemTake\n");
       }
       else
       {
          OS_printf("TASK 2 Has the Mutex\n");
       }

       OS_printf("TASK 2:   Doing some work: %d\n", (int)counter++);

       status = OS_MutSemGive(mut_sem_id);
       if ( status != OS_SUCCESS )
       {
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
 
       status = OS_TaskDelay(1000);
       status = OS_MutSemTake(mut_sem_id);
       if ( status != OS_SUCCESS )
       {
          OS_printf("TASK 3:Error calling OS_MutSemTake\n");
       }
       else
       {
          OS_printf("TASK 3 Has the Mutex\n");
       }

       OS_printf("TASK 3:   Doing some work: %d\n", (int)counter++);

       status = OS_MutSemGive(mut_sem_id);
       if ( status != OS_SUCCESS )
       {
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
   uint32 status;

   OS_API_Init();

   OS_printf("OS Application Startup\n");

   /*
   ** Create the mutex 
   */
   status = OS_MutSemCreate( &mut_sem_id, "BinSem1", 0);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error creating Mutex\n");
   }
   else
   {
      OS_printf("Mutex ID = %d\n", (int)mut_sem_id);
   }

   /*
   ** Test the mutex to see if it supports nesting 
   */
   status = OS_MutSemTake(mut_sem_id);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error calling OS_MutSemTake with mut_sem_id = %d\n",(int)mut_sem_id);
   }

   OS_printf("Initial Mutex Take -- trying nested call\n");

   status = OS_MutSemTake(mut_sem_id);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error calling Nested OS_MutSemTake with mut_sem_id = %d\n",(int)mut_sem_id);
   }

   OS_printf("OK, inside nested Mutex call!\n");

   status = OS_MutSemGive(mut_sem_id);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error calling OS_MutSemGive with mut_sem_id = %d\n",(int)mut_sem_id);
   }

   OS_printf("Did first Mutex Give ( inner most nest )\n");

   status = OS_MutSemGive(mut_sem_id);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error calling OS_MutSemGive with mut_sem_id = %d\n",(int)mut_sem_id);
   }

   OS_printf("Did second Mutex Give ( outer nest )\n");

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

