/*
** Queue read timeout test
*/
#include <stdio.h>
#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* Define setup and check functions for UT assert */
void QueueTimeoutSetup(void);
void QueueTimeoutCheck(void);

#define MSGQ_ID           1
#define MSGQ_DEPTH        50
#define MSGQ_SIZE         4

/* Task 1 */
#define TASK_1_ID         1
#define TASK_1_STACK_SIZE 1024
#define TASK_1_PRIORITY   101
#define TASK_2_STACK_SIZE 1024
#define TASK_2_PRIORITY   50

uint32 task_1_stack[TASK_1_STACK_SIZE];
uint32 task_1_id; 
uint32 task_1_failures;
uint32 task_1_timeouts;
uint32 task_1_messages;
uint32 task_2_stack[TASK_2_STACK_SIZE];
uint32 task_2_id;
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
           ++task_1_messages;
           OS_printf("TASK 1: Recieved a message on the queue\n");
        }
        else if ( status == OS_QUEUE_TIMEOUT )
        {
           ++task_1_timeouts;
           OS_printf("TASK 1: Timeout on Queue! Timer counter = %u\n", (unsigned int)timer_counter);
        }
        else
        {
           ++task_1_failures;
            OS_printf("TASK 1: Queue Get error!\n");
        }
    }
}

void task_2(void)
{
   OS_TaskRegister();
   /*
    * Time limited execution
    */

   while (timer_counter < 100)
   {
      OS_TaskDelay(500);
   }

   OS_ApplicationShutdown(TRUE);
   OS_TaskExit();
}

void QueueTimeoutCheck(void)
{
    int32 status;
    uint32 limit;

    status  =  OS_TimerDelete(timer_id);
    UtAssert_True(status == OS_SUCCESS, "Timer delete Rc=%d", (int)status);
    status = OS_TaskDelete(task_1_id);
    UtAssert_True(status == OS_SUCCESS, "Task 1 delete Rc=%d", (int)status);

    /* None of the tasks should have any failures in their own counters */
    UtAssert_True(task_1_failures == 0, "Task 1 failures = %u",(unsigned int)task_1_failures);

    /*
     * Since nothing currently sends messages, message count should be zero,
     * and timer counter =~ 10 + ( 10 x task_1_timeouts )
     */
    UtAssert_True(task_1_messages == 0, "Task 1 messages = %u",(unsigned int)task_1_messages);

    limit = (timer_counter / 10);
    UtAssert_True(task_1_timeouts <= limit, "Task 1 timeouts %u <= %u",
            (unsigned int)task_1_timeouts, (unsigned int)limit);

    limit = ((timer_counter - 20) / 12);
    UtAssert_True(task_1_timeouts >= limit, "Task 1 timeouts %u >= %u",
            (unsigned int)task_1_timeouts, (unsigned int)limit);

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
    UtTest_Add(QueueTimeoutCheck, QueueTimeoutSetup, NULL, "QueueTimeoutTest");
}

void QueueTimeoutSetup(void)
{
   int32             status;
   uint32 accuracy;

   task_1_failures = 0;
   task_1_messages = 0;
   task_1_timeouts = 0;

   status = OS_QueueCreate( &msgq_id, "MsgQ", MSGQ_DEPTH, MSGQ_SIZE, 0);
   UtAssert_True(status == OS_SUCCESS, "MsgQ create Id=%u Rc=%d", (unsigned int)msgq_id, (int)status);

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


   OS_IdleLoop();
}
