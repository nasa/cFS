/*
** Queue read timeout test
*/
#include <stdio.h>
#include "common_types.h"
#include "osapi.h"

#define MSGQ_ID           1
#define MSGQ_DEPTH        50
#define MSGQ_SIZE         4

/* Task 1 */
#define TASK_1_ID         1
#define TASK_1_STACK_SIZE 1024
#define TASK_1_PRIORITY   101

uint32 task_1_stack[TASK_1_STACK_SIZE];
uint32 task_1_id; 
uint32 msgq_id;

uint32 timer_counter;
uint32 timer_id;
uint32 timer_start = 10000;
uint32 timer_interval = 100000; /* 1000 = 1000 hz, 10000 == 100 hz */
uint32 timer_accuracy;

void TimerFunction(uint32 timer_id)
{
   timer_counter++;
}

void task_1(void)
{
    uint32             status;
    uint32             data_received;
    uint32             data_size;

    OS_printf("Starting task 1\n");

    OS_TaskRegister();

    OS_printf("Delay for 1 second before starting\n");
    OS_TaskDelay(1000);

    while(1)
    {

        status = OS_QueueGet(msgq_id, (void*)&data_received, MSGQ_SIZE, &data_size, 1000);

        if ( status == OS_SUCCESS )
        {
           OS_printf("TASK 1: Recieved a message on the queue\n");
        }
        else if ( status == OS_QUEUE_TIMEOUT )
        {
           OS_printf("TASK 1: Timeout on Queue! Timer counter = %d\n", timer_counter);
        }
        else
        {
            OS_printf("TASK 1: Queue Get error!\n");
        }
    }
}

void OS_Application_Startup(void)
{
   uint32             status;

   OS_API_Init();

   OS_printf("OS Application Startup\n");

   status = OS_QueueCreate( &msgq_id, "MsgQ", MSGQ_DEPTH, MSGQ_SIZE, 0);
   if ( status != OS_SUCCESS )
   {
      OS_printf("Error creating Message Queue\n");
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
      OS_printf("Timer ID = %d\n", (int)timer_id);
      OS_printf("Timer Accuracy = %d microseconds \n",(int)timer_accuracy);
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
