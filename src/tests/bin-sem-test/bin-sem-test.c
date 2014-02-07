/*
** Binary semaphore Producer/Consumer test
*/
#include <stdio.h>
#include "common_types.h"
#include "osapi.h"

/* Task 1 */
#define TASK_1_ID         1
#define TASK_1_STACK_SIZE 1024
#define TASK_1_PRIORITY   101

#define TIMER_ENTRY 0x001
#define TIMER_EXIT  0x002
#define TASK_ENTRY  0x003
#define TASK_EXIT   0x004

uint32 task_1_stack[TASK_1_STACK_SIZE];
uint32 task_1_id; 
uint32 bin_sem_id;

uint32 timer_counter;
uint32 timer_id;
uint32 timer_start = 10000;
uint32 timer_interval = 100000; /* 1000 = 1000 hz, 10000 == 100 hz */
uint32 timer_accuracy;

int    counter = 0;

void TimerFunction(uint32 timer_id)
{
   int32              status;

#ifndef _RTEMS_OS_
   OS_bin_sem_prop_t  bin_sem_prop;
#endif
  
   timer_counter++;

   status = OS_BinSemGive(bin_sem_id);

   /* 
   ** Note: The timer callback function in RTEMS runs as an ISR
   **       so calls that block cannot be made.
   */
   #ifndef _RTEMS_OS_ 
      status = OS_BinSemGetInfo (bin_sem_id, &bin_sem_prop);
      if ( bin_sem_prop.value > 1 )
      {
         OS_printf("Error: Binary sem value > 1 ( in timer):%d !\n",(int)bin_sem_prop.value);
         exit(-1);
      }
      else if ( bin_sem_prop.value < -1 )
      {
         OS_printf("Error: Binary sem value < -1 ( in timer):%d !\n",(int)bin_sem_prop.value);
         exit(-1);
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
           exit(-1);
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
           if ( bin_sem_prop.value > 1 )
           {
                OS_printf("Error: Binary sem value > 1 ( in task):%d !\n",(int)bin_sem_prop.value);
                exit(-1);
           }
           else if ( bin_sem_prop.value < -1 )
           {
                OS_printf("Error: Binary sem value < -1 ( in task):%d !\n",(int)bin_sem_prop.value);
                exit(-1);
           }
        }

    }
}

void OS_Application_Startup(void)
{
   uint32             status;
   OS_bin_sem_prop_t  bin_sem_prop;

   OS_API_Init();

   OS_printf("OS Application Startup\n");

   /*
   ** Create the binary semaphore
   */
   status = OS_BinSemCreate( &bin_sem_id, "BinSem1", 1, 0);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error creating Binary Sem\n");
   }
   else
   {
      status = OS_BinSemGetInfo (bin_sem_id, &bin_sem_prop);
      OS_printf("Binary Sem ID = %d, value = %d\n", (int)bin_sem_id, (int)bin_sem_prop.value);
   }

   /*
   ** Create a timer
   */
   status = OS_TimerCreate(&timer_id, "Timer 1", &timer_accuracy, &(TimerFunction));
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error creating OS Timer\n");
   }
   else
   {
      OS_printf("Timer ID = %d\n", (int)bin_sem_id);
      OS_printf("Timer Accuracy = %d microseconds \n",(int)timer_accuracy);
   }

   /*
   ** Take the semaphore so the value is 0 and the next SemTake call should block
   */
   status = OS_BinSemTake(bin_sem_id);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error calling OS_BinSemTake with bin_sem_id = %d\n",(int)bin_sem_id);
   }
   else
   {
      status = OS_BinSemGetInfo (bin_sem_id, &bin_sem_prop);
      OS_printf("Initial Binary Sem Take: value = %d\n", (int)bin_sem_prop.value);
   }

   /*
   ** Create the "consumer" task.
   */
   status = OS_TaskCreate( &task_1_id, "Task 1", task_1, task_1_stack, TASK_1_STACK_SIZE, TASK_1_PRIORITY, 0);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error creating Task 1\n");
   }
   else
   {
      OS_printf("Created Task 1\n");
   }

   /*
   ** Start the timer
   */
   status  =  OS_TimerSet(timer_id, timer_start, timer_interval);
   if ( status != OS_SUCCESS )
   {
       OS_printf("Error calling OS_TimerSet: ID = %d\n", (int)timer_id);
   }
   else
   {
       OS_printf("Timer programmed\n");
   }

}
