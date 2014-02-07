/*
** example1.c
**
** This is an example OSAL Application. This Application creates three tasks, 
** and passes messages back and forth using queues
*/

#include <stdio.h>
#include "common_types.h"
#include "osapi.h"


/* Task 1 */

#define TASK_1_ID         1
#define TASK_1_STACK_SIZE 1024
#define TASK_1_PRIORITY   101

uint32 task_1_stack[TASK_1_STACK_SIZE];

void task_1(void);


/* Task 2 */

#define TASK_2_ID         2
#define TASK_2_STACK_SIZE 1024
#define TASK_2_PRIORITY   102

uint32 task_2_stack[TASK_2_STACK_SIZE];

void task_2(void);


/* Task 3 */

#define TASK_3_ID         3
#define TASK_3_STACK_SIZE 1024
#define TASK_3_PRIORITY   103

uint32 task_3_stack[TASK_3_STACK_SIZE];

void task_3(void);


/* OS Constructs */

#define MSGQ_ID           1
#define MSGQ_DEPTH        50
#define MSGQ_SIZE         4

#define MUTEX_ID          1

uint32 task_1_id, task_2_id, task_3_id;
uint32 mutex_id, msgq_id;

/* Global Data */

uint32 shared_resource_x;


/* ********************** MAIN **************************** */

void OS_Application_Startup(void)
{
  uint32 status;

  OS_API_Init();

  OS_printf("********If You see this, we got into OS_Application_Startup****\n");

  status = OS_QueueCreate( &msgq_id, "MsgQ", MSGQ_DEPTH, MSGQ_SIZE, 0);
  if ( status != OS_SUCCESS )
  {
     OS_printf("Error creating Message Queue\n");
  }

  status = OS_MutSemCreate( &mutex_id, "Mutex", 0);
  if ( status != OS_SUCCESS )
  {
     OS_printf("Error creating mutex\n");
  }
  else
  {
     OS_printf("MutexSem ID = %d\n",mutex_id);
  }

  status = OS_TaskCreate( &task_1_id, "Task 1", task_1, task_1_stack, TASK_1_STACK_SIZE, TASK_1_PRIORITY, 0);
  if ( status != OS_SUCCESS )
  {
     OS_printf("Error creating Task 1\n");
  }

  status = OS_TaskCreate( &task_2_id, "Task 2", task_2, task_2_stack, TASK_2_STACK_SIZE, TASK_2_PRIORITY, 0);
  if ( status != OS_SUCCESS )
  {
     OS_printf("Error creating Task 2\n");
  }

  status = OS_TaskCreate( &task_3_id, "Task 3", task_3, task_3_stack, TASK_3_STACK_SIZE, TASK_3_PRIORITY, 0);
  if ( status != OS_SUCCESS )
  {
     OS_printf("Error creating Task 3\n");
  }


}

/* ********************** TASK 1 **************************** */

void task_1(void)
{
    uint32 status;

    OS_printf("Starting task 1\n");

    OS_TaskRegister();

    while(1)
    {
        status = OS_MutSemTake(mutex_id);
        if ( status != OS_SUCCESS )
        {
           OS_printf("TASK 1:Error calling OS_MutSemTake with mutex_id = %d\n",mutex_id);
        }

        shared_resource_x = task_1_id;

        status = OS_QueuePut(msgq_id, (void*)&shared_resource_x, sizeof(uint32), 0);
        if ( status != OS_SUCCESS )
        {
           OS_printf("TASK 1:Error calling OS_QueuePut ( 1 )\n");
        }

        shared_resource_x = task_1_id;

        status = OS_QueuePut(msgq_id, (void*)&shared_resource_x, sizeof(uint32), 0);
        if ( status != OS_SUCCESS )
        {
           OS_printf("TASK 1:Error calling OS_QueuePut ( 2 )\n");
        }

        status = OS_MutSemGive(mutex_id);
        if ( status != OS_SUCCESS )
        {
           OS_printf("TASK 1:Error calling OS_MutSemGive\n");
        }
        
        OS_TaskDelay(100);
    }
}


/* ********************** TASK 2 **************************** */

void task_2(void)
{
    uint32 status;

    OS_printf("Starting task 2\n");

    OS_TaskRegister();

    while(1)
    {
        status = OS_MutSemTake(mutex_id);
        if ( status != OS_SUCCESS )
        {
           OS_printf("TASK 2:Error calling OS_MutSemTake\n");
        }

        shared_resource_x = task_2_id;

        status = OS_QueuePut(msgq_id, (void*)&shared_resource_x, sizeof(uint32), 0);
        if ( status != OS_SUCCESS )
        {
           OS_printf("TASK 2:Error calling OS_QueuePut (1)\n");
        }
            
        OS_TaskDelay(150);

        shared_resource_x = task_2_id;

        status = OS_QueuePut(msgq_id, (void*)&shared_resource_x, sizeof(uint32), 0);
        if ( status != OS_SUCCESS )
        {
           OS_printf("TASK 2:Error calling OS_QueuePut (2)\n");
        }

        status = OS_MutSemGive(mutex_id);
        if ( status != OS_SUCCESS )
        {
           OS_printf("TASK 2:Error calling OS_MutSemGive\n");
        }

        OS_TaskDelay(500);
    }  
}


/* ********************** TASK 3 **************************** */

void task_3(void)
{   
    
    uint32 data_received;
    uint32 data_size;
    uint32 status;

    OS_printf("Starting task 3\n");

    OS_TaskRegister();

    while(1)
    {
        status = OS_QueueGet(msgq_id, (void*)&data_received, MSGQ_SIZE, &data_size, OS_PEND);
   
        if (status == OS_SUCCESS)
        {
            OS_printf("TASK 3: Received - %d\n", (int)data_received+1);
        } 
        else
        {
            OS_printf("TASK 3: Error calling OS_QueueGet\n");
        }
    }
}
