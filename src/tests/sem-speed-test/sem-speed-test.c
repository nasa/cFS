/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/*
** Semaphore Speed Test
**
** This is a simple way to gauge the performance of the
** OS/kernel task switching and semaphore implementations
** on a given machine.
**
** This implements a flip-flop between two tasks using
** two semaphores.  Task 1 waits for semaphore 1 and
** gives semaphore 2, while task 2 waits for semaphore 2
** and gives semaphore 1.
**
** The two tasks will run continuously, each pending
** on each other and swapping between them, for 5 seconds.
**
** At the end of the test, the total number of "work"
** cycles for each task is indicated.  Higher numbers
** indicate better performance.
**
**
*/
#include <stdio.h>
#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* Define setup and test functions for UT assert */
void SemSetup(void);
void SemRun(void);

/*
 * A macro for semaphore operation depending on type.
 *
 * This resolves to a function name like e.g. OS_BinSemTake.
 */
#define SEMCALL(type,op)    OS_ ## type ## Sem ## op

/*
 * This test works with either binary or counting
 * semaphores.  To switch between them, set this
 * to either "Bin" or "Count" without quotes.
 */
#define SEMOP(op)           SEMCALL(Count,op)

#define TASK_PRIORITY  50

uint32 task_1_id; 
uint32 task_1_work;

uint32 task_2_id; 
uint32 task_2_work;

uint32 sem_id_1;
uint32 sem_id_2;

void task_1(void)
{
    uint32             status;

    OS_printf("Starting task 1\n");
    OS_TaskRegister();

    while(1)
    {
       status = SEMOP(Take)(sem_id_1);
       if ( status != OS_SUCCESS )
       {
          OS_printf("TASK 1: Error calling SemTake 1: %d\n", (int)status);
          break;
       }

       ++task_1_work;

       status = SEMOP(Give)(sem_id_2);
       if ( status != OS_SUCCESS )
       {
          OS_printf("TASK 1: Error calling SemGive 2: %d\n", (int)status);
          break;
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
       status = SEMOP(Take)(sem_id_2);
       if ( status != OS_SUCCESS )
       {
          OS_printf("TASK 2: Error calling SemTake 2: %d\n", (int)status);
          break;
       }

       ++task_2_work;

       status = SEMOP(Give)(sem_id_1);
       if ( status != OS_SUCCESS )
       {
          OS_printf("TASK 2: Error calling SemGive 1: %d\n", (int)status);
          break;
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
    UtTest_Add(SemRun, SemSetup, NULL, "SemSpeedTest");
}

void SemSetup(void)
{
   uint32 status;

   task_1_work = 0;
   task_2_work = 0;

   /*
   ** Create the Bin semaphore
   */
   status = SEMOP(Create)( &sem_id_1, "Sem1", 0, 0);
   UtAssert_True(status == OS_SUCCESS, "Sem 1 create Id=%u Rc=%d", (unsigned int)sem_id_1, (int)status);
   status = SEMOP(Create)( &sem_id_2, "Sem2", 0, 0);
   UtAssert_True(status == OS_SUCCESS, "Sem 2 create Id=%u Rc=%d", (unsigned int)sem_id_2, (int)status);

   /*
   ** Create the tasks
   */
   status = OS_TaskCreate( &task_1_id, "Task 1", task_1, NULL, 4096, TASK_PRIORITY, 0);
   UtAssert_True(status == OS_SUCCESS, "Task 1 create Id=%u Rc=%d", (unsigned int)task_1_id, (int)status);

   status = OS_TaskCreate( &task_2_id, "Task 2", task_2, NULL, 4096, TASK_PRIORITY, 0);
   UtAssert_True(status == OS_SUCCESS, "Task 2 create Id=%u Rc=%d", (unsigned int)task_2_id, (int)status);

   /* A small delay just to allow the tasks
    * to start and pend on the sem */
   OS_TaskDelay(10);
}

void SemRun(void)
{
    int32 status;

    /* Give the initial sem that starts the loop */
    SEMOP(Give)(sem_id_1);

    /* Time Limited Execution */
    OS_TaskDelay(5000);

    /*
    ** Delete resources
    */
    status = OS_TaskDelete( task_1_id );
    UtAssert_True(status == OS_SUCCESS, "Task 1 delete Rc=%d", (int)status);

    status = OS_TaskDelete( task_2_id );
    UtAssert_True(status == OS_SUCCESS, "Task 2 delete Rc=%d", (int)status);

    status = SEMOP(Delete)( sem_id_1 );
    UtAssert_True(status == OS_SUCCESS, "Sem 1 delete Rc=%d", (int)status);
    status = SEMOP(Delete)( sem_id_2 );
    UtAssert_True(status == OS_SUCCESS, "Sem 2 delete Rc=%d", (int)status);


    /* Task 1 and 2 should have both executed */
    UtAssert_True(task_1_work != 0, "Task 1 work counter = %u", (unsigned int)task_1_work);
    UtAssert_True(task_2_work != 0, "Task 2 work counter = %u", (unsigned int)task_2_work);
}

