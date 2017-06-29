#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "osal-core-test.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* OS Constructs */

void TestTasks (void);
void InitializeTaskIds (void);
void InitializeQIds (void);
void InitializeBinIds(void);
void InitializeMutIds(void);
void TestQueues(void);
void TestBinaries(void);
void TestMutexes(void);
void TestGetInfos(void);

/* *************************************** MAIN ************************************** */
void OS_Application_Startup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }
    
    UtTest_Add(TestTasks, NULL, NULL, "TASK");
    UtTest_Add(TestQueues, NULL, NULL, "MSGQ");
    UtTest_Add(TestBinaries, NULL, NULL, "BSEM");
    UtTest_Add(TestMutexes, NULL, NULL, "MSEM");
    UtTest_Add(TestGetInfos, NULL, NULL, "INFO");
    
} /* end OS_Application Startup */


typedef struct
{
   uint32 task_id;
   uint32 task_stack[TASK_0_STACK_SIZE];
} TestTaskData;
/* ********************************************** TASKS******************************* */
void TestTasks(void)
{
    int status;
    char taskname[OS_MAX_API_NAME];
    int tasknum;
    uint32 saved_task0_id;
    static TestTaskData TaskData[OS_MAX_TASKS + 1];
    
   /* OS_TaskRegister(); */

    /* Testing Creating up to OS_MAX_TASKS, plus one more */
    memset(TaskData,0xFF,sizeof(TaskData));
    for (tasknum = 0; tasknum < (OS_MAX_TASKS + 1); ++tasknum)
    {
       snprintf(taskname,sizeof(taskname), "Task %d", tasknum);
       status = OS_TaskCreate( &TaskData[tasknum].task_id, taskname, task_0, TaskData[tasknum].task_stack,
                               TASK_0_STACK_SIZE, (250 - OS_MAX_TASKS) + tasknum, 0);

       UtDebug("Create %s Status = %d, Id = %d\n",taskname,(int)status,(int)TaskData[tasknum].task_id);

       UtAssert_True((tasknum < OS_MAX_TASKS && status == OS_SUCCESS) ||
             (tasknum >= OS_MAX_TASKS && status != OS_SUCCESS), "OS_TaskCreate, nominal");
    }
    
    /* Save off the task 0 ID for later */
    saved_task0_id = TaskData[0].task_id;

    /* Testing the Deletions of all the tasks we have created */
    for (tasknum = 0; tasknum < (OS_MAX_TASKS + 1); ++tasknum)
    {
       snprintf(taskname,sizeof(taskname), "Task %d", tasknum);
       status = OS_TaskDelete( TaskData[tasknum].task_id );

       UtDebug("Delete Status = %d, Id = %d\n",(int)status,(int)TaskData[tasknum].task_id);

       UtAssert_True((tasknum < OS_MAX_TASKS && status == OS_SUCCESS) ||
             (tasknum >= OS_MAX_TASKS && status != OS_SUCCESS), "OS_TaskDelete, nominal");
    }
    
    /* These next few tasks were deleted already, testing a "redelete" */
    UtAssert_True(OS_TaskDelete(TaskData[1].task_id) != OS_SUCCESS, "OS_TaskDelete, redelete 1");
    UtAssert_True(OS_TaskDelete(TaskData[2].task_id) != OS_SUCCESS, "OS_TaskDelete, redelete 2");
    UtAssert_True(OS_TaskDelete(TaskData[3].task_id) != OS_SUCCESS, "OS_TaskDelete, redelete 3");

    /* Creating some more tasks for testing name functions */
    

    InitializeTaskIds();
    /* Create Task 0 again */
    status = OS_TaskCreate( &task_0_id, "Task 0", task_0, task_0_stack, 
                            TASK_0_STACK_SIZE, TASK_0_PRIORITY, 0);
      /*UtDebug("Create Status = %d, Id = %d\n",status,task_0_id); */
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate, recreate 0");
    
  /* Try and create another "Task 0", should fail as we already have one named "Task 0" */
    status = OS_TaskCreate( &task_1_id, "Task 0", task_0, task_0_stack, 
                            TASK_0_STACK_SIZE, TASK_0_PRIORITY, 0);
    UtAssert_True(status != OS_SUCCESS, "OS_TaskCreate, dupe name 0");

    status = OS_TaskCreate( &task_2_id, "Task 2", task_2, task_2_stack,
                            TASK_2_STACK_SIZE, TASK_2_PRIORITY, 0);
    /*  UtDebug("Create Status = %d, Id = %d\n",status,task_2_id); */
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate, recreate 2");
    
    status = OS_TaskCreate( &task_3_id, "Task 3", task_3, task_3_stack,
                            TASK_3_STACK_SIZE, TASK_3_PRIORITY, 0);
    /*  UtDebug("Create Status = %d, Id = %d\n",status,task_3_id); */
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate, recreate 3");


    status = OS_TaskGetIdByName(&task_0_id, "Task 0");
    /* UtDebug("Satus after Getting the id of \"Task 0\":%d,%d \n\n",status,task_0_id); */
    /*first newly created task should have id == 0*/
    UtAssert_True(status == OS_SUCCESS, "OS_TaskGetIdByName, Task 0");
    
    status = OS_TaskGetIdByName(&task_1_id, "Task 1");
    /*UtDebug("Satus after Getting the id of \"Task 1\":%d,%d \n\n",status,task_1_id);*/
    UtAssert_True(status != OS_SUCCESS, "OS_TaskGetIdByName, Task 1");

    status = OS_TaskGetIdByName(&task_2_id, "Task 2");
    /* UtDebug("Satus after Getting the id of \"Task 2\":%d,%d \n\n",status,task_2_id);*/
    UtAssert_True(status == OS_SUCCESS, "OS_TaskGetIdByName, Task 2");
    
    status = OS_TaskGetIdByName(&task_3_id, "Task 3");
    /* UtDebug("Satus after Getting the id of \"Task 3\":%d,%d \n\n",status,task_3_id); */
    UtAssert_True(status == OS_SUCCESS, "OS_TaskGetIdByName, Task 3");

#ifdef OSAL_OPAQUE_OBJECT_IDS
    /*
     * This should fail - verifies that ID re-use protections are working.
     * This only works with the new OSALs
     */
    UtAssert_True(OS_TaskDelete(saved_task0_id) != OS_SUCCESS, "OS_TaskDelete, Old ID");
#else
    TaskData[0].task_id = saved_task0_id; /* squelch unused variable warning */
#endif

    UtAssert_True(OS_TaskDelete(task_0_id) == OS_SUCCESS, "OS_TaskDelete, Task 0");
    UtAssert_True(OS_TaskDelete(task_1_id) != OS_SUCCESS, "OS_TaskDelete, Task 1");
    UtAssert_True(OS_TaskDelete(task_2_id) == OS_SUCCESS, "OS_TaskDelete, Task 2");
    UtAssert_True(OS_TaskDelete(task_3_id) == OS_SUCCESS, "OS_TaskDelete, Task 3");
    
}/* end TestTasks */


/* ************************************************************************************ */

void TestQueues(void)
{
    int status;
    char qname[OS_MAX_API_NAME];
    int qnum;
    uint32 saved_queue0_id;
    static uint32 msgq_ids[OS_MAX_QUEUES + 1];

    InitializeQIds();
    memset(msgq_ids,0xFF,sizeof(msgq_ids));
    
    for (qnum = 0; qnum < (OS_MAX_QUEUES + 1); ++qnum)
    {
       snprintf(qname,sizeof(qname),"q %d", qnum);
       status = OS_QueueCreate( &msgq_ids[qnum], qname, MSGQ_DEPTH, MSGQ_SIZE, 0);

       UtAssert_True((qnum < OS_MAX_QUEUES && status == OS_SUCCESS) ||
             (qnum >= OS_MAX_QUEUES && status != OS_SUCCESS), "OS_QueueCreate, nominal");
    }
    
    saved_queue0_id = msgq_ids[0];
 /*     Trying now to Delete all of the Queues created. */
    
    for (qnum = 0; qnum < (OS_MAX_QUEUES + 1); ++qnum)
    {
       status = OS_QueueDelete( msgq_ids[qnum] );

       UtAssert_True((qnum < OS_MAX_QUEUES && status == OS_SUCCESS) ||
             (qnum >= OS_MAX_QUEUES && status != OS_SUCCESS), "OS_QueueDelete, nominal");
    }

/*     Create Some more Queues for trying to get the id by name */

    InitializeQIds();
    status = OS_QueueCreate( &msgq_0, "q 0", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* UtDebug("Status after Creating q 0: %d,%d\n",status,msgq_0);*/
    UtAssert_True(status == OS_SUCCESS, "OS_QueueCreate, recreate 0");
    
    /* This one should fail */
    status = OS_QueueCreate( &msgq_1, "q 0", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* UtDebug("Status after Creating q 0 again: %d,%d\n",status,msgq_1); */
    UtAssert_True(status != OS_SUCCESS, "OS_QueueCreate, dupe name 0");
    
    status = OS_QueueCreate( &msgq_2, "q 2", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* UtDebug("Status after Creating q 2: %d,%d\n",status,msgq_2); */
    UtAssert_True(status == OS_SUCCESS, "OS_QueueCreate, recreate 2");
    
    status = OS_QueueCreate( &msgq_3, "q 3", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* UtDebug("Status after Creating q 3: %d,%d\n",status,msgq_3); */
    UtAssert_True(status == OS_SUCCESS, "OS_QueueCreate, recreate 3");

    /*
     * Now that the Queues are created, its time to see if we can find
     * the propper ID by the name of the queue;
     */
    status = OS_QueueGetIdByName(&msgq_0,"q 0");
    UtAssert_True(status == OS_SUCCESS, "OS_QueueGetIdByName, q 0");

    status = OS_QueueGetIdByName(&msgq_1,"q 1");
    UtAssert_True(status != OS_SUCCESS, "OS_QueueGetIdByName, q 1");

    status = OS_QueueGetIdByName(&msgq_2,"q 2");
    UtAssert_True(status == OS_SUCCESS, "OS_QueueGetIdByName, q 2");

    status = OS_QueueGetIdByName(&msgq_3,"q 3");
    UtAssert_True(status == OS_SUCCESS, "OS_QueueGetIdByName, q 3");

#ifdef OSAL_OPAQUE_OBJECT_IDS
    /*
     * This should fail - verifies that ID re-use protections are working.
     */
    status = OS_QueueDelete(saved_queue0_id);
    UtAssert_True(status != OS_SUCCESS, "OS_QueueDelete, Old ID");
#else
    msgq_ids[0] = saved_queue0_id;
#endif

    /* Time to Delete the Queues we just created */

    status = OS_QueueDelete(msgq_0);
     /* UtDebug("Status after Deleting q 0 : %d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_QueueDelete, q 0");

    status = OS_QueueDelete(msgq_1);
     /* UtDebug("Status after Deleting q 1: %d\n",status); */
    UtAssert_True(status != OS_SUCCESS, "OS_QueueDelete, q 1");

    status = OS_QueueDelete(msgq_2);
     /* UtDebug("Status after Deleting q 2: %d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_QueueDelete, q 2");

    status = OS_QueueDelete(msgq_3);
     /* UtDebug("Status after Deleting q 3: %d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_QueueDelete, q 3");
    
}/* end TestQueues */

/* *************************************************************************** */
void TestBinaries(void)
{
    int status;
    char bname[OS_MAX_API_NAME];
    int bnum;
    uint32 saved_bin0_id;
    static uint32 binsem_ids[OS_MAX_BIN_SEMAPHORES + 1];


    memset(binsem_ids,0xFF,sizeof(binsem_ids));

    for (bnum = 0; bnum < (OS_MAX_BIN_SEMAPHORES + 1); ++bnum)
    {
       snprintf(bname,sizeof(bname),"Bin %d", bnum);
       status = OS_BinSemCreate( &binsem_ids[bnum], bname, 1, 0);

       UtAssert_True((bnum < OS_MAX_BIN_SEMAPHORES && status == OS_SUCCESS) ||
             (bnum >= OS_MAX_BIN_SEMAPHORES && status != OS_SUCCESS), "OS_BinSemCreate, nominal");
    }

    saved_bin0_id = binsem_ids[0];

    for (bnum = 0; bnum < (OS_MAX_BIN_SEMAPHORES + 1); ++bnum)
    {
       status = OS_BinSemDelete( binsem_ids[bnum] );

       UtAssert_True((bnum < OS_MAX_BIN_SEMAPHORES && status == OS_SUCCESS) ||
             (bnum >= OS_MAX_BIN_SEMAPHORES && status != OS_SUCCESS), "OS_BinSemDelete, nominal");
    }

    /*
     * Now Create a few extra semaphores
     *  to test  GetIdByName
     */
    InitializeBinIds();
    status = OS_BinSemCreate( &bin_0,"Bin 0",OS_SEM_FULL,0);
    /* UtDebug("Status after creating: %d,%d\n",status,bin_0); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemCreate, recreate 0");

    status = OS_BinSemCreate( &bin_1,"Bin 0",OS_SEM_FULL,0);
    /* UtDebug("Status after creating: %d,%d\n",status,bin_1); */
    UtAssert_True(status != OS_SUCCESS, "OS_BinSemCreate, dupe name 0");

    status = OS_BinSemCreate( &bin_2,"Bin 2",OS_SEM_EMPTY,0);
     /* UtDebug("Status after creating: %d,%d\n",status,bin_2);  */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemCreate, recreate 2");

    status = OS_BinSemCreate( &bin_3,"Bin 3",OS_SEM_EMPTY,0);
     /* UtDebug("Status after creating: %d,%d\n",status,bin_3); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemCreate, recreate 3");



    
    status = OS_BinSemGetIdByName(&bin_0,"Bin 0");
      /* UtDebug("Status after GETID: %d,%d\n",status,bin_0); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemGetIdByName, Bin 0");
    
    status = OS_BinSemGetIdByName(&bin_1,"Bin 1");
    /* UtDebug("Status after GETID: %d,%d\n",status,bin_1); */
    UtAssert_True(status != OS_SUCCESS, "OS_BinSemGetIdByName, Bin 1");
    
    status = OS_BinSemGetIdByName(&bin_2,"Bin 2");
    /* UtDebug("Status after GETID: %d,%d\n",status,bin_2); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemGetIdByName, Bin 2");
    
    status = OS_BinSemGetIdByName(&bin_3,"Bin 3");
     /* UtDebug("Status after GETID: %d,%d\n",status,bin_3); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemGetIdByName, Bin 3");

#ifdef OSAL_OPAQUE_OBJECT_IDS
    /*
     * This should fail - verifies that ID re-use protections are working.
     */
    status = OS_BinSemDelete(saved_bin0_id);
    UtAssert_True(status != OS_SUCCESS, "OS_BinSemDelete, Old ID");
#else
    binsem_ids[0] = saved_bin0_id;
#endif

    status = OS_BinSemDelete(bin_0);
     /* UtDebug("Status after deleteing:%d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemDelete, Bin 0");

    /* this one was never created */
    status = OS_BinSemDelete(bin_1);
     /* UtDebug("Status after deleteing:%d\n",status); */
    UtAssert_True(status != OS_SUCCESS, "OS_BinSemDelete, Bin 1");

    status = OS_BinSemDelete(bin_2);
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemDelete, Bin 2");
    
    status = OS_BinSemDelete(bin_3);
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemDelete, Bin 3");

}/* end TestBinaries */


/* ************************************************************************************ */
void TestMutexes(void)
{
    int status;
    char mname[OS_MAX_API_NAME];
    int mnum;
    uint32 saved_mut0_id;
    static uint32 mutex_ids[OS_MAX_MUTEXES + 1];


    memset(mutex_ids,0xFF,sizeof(mutex_ids));

    for (mnum = 0; mnum < (OS_MAX_MUTEXES + 1); ++mnum)
    {
       snprintf(mname,sizeof(mname),"Mut %d", mnum);
       status = OS_MutSemCreate( &mutex_ids[mnum], mname, 0);

       UtAssert_True((mnum < OS_MAX_MUTEXES && status == OS_SUCCESS) ||
             (mnum >= OS_MAX_MUTEXES && status != OS_SUCCESS), "OS_MutSemCreate, nominal");
    }
    
    saved_mut0_id = mutex_ids[0];

    for (mnum = 0; mnum < (OS_MAX_MUTEXES + 1); ++mnum)
    {
       status = OS_MutSemDelete( mutex_ids[mnum] );

       UtAssert_True((mnum < OS_MAX_MUTEXES && status == OS_SUCCESS) ||
             (mnum >= OS_MAX_MUTEXES && status != OS_SUCCESS), "OS_MutSemDelete, nominal");
    }

    /*
     * Now Create a few extra semaphores
     *  to test  GetIdByName
     */
    InitializeMutIds();
    status = OS_MutSemCreate( &mut_0,"Mut 0",0);
    /*  UtDebug("Status after creating Mut 0: %d,%d\n",status,mut_0);  */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate, recreate 0");

    status = OS_MutSemCreate( &mut_1,"Mut 0",0);
    /*  UtDebug("Status after creating Mut 0 again: %d,%d\n",status,mut_1); */
    UtAssert_True(status != OS_SUCCESS, "OS_MutSemCreate, dupe name 0");

    status = OS_MutSemCreate( &mut_2,"Mut 2",0);
    /*  UtDebug("Status after creating Mut 2: %d,%d\n",status,mut_2); */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate, recreate 2");

    status = OS_MutSemCreate( &mut_3,"Mut 3",0);
    /*  UtDebug("Status after creating Mut 3: %d,%d\n",status,mut_3); */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate, recreate 3");

    status = OS_MutSemGetIdByName(&mut_0,"Mut 0");
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemGetIdByName, Mut 0");
    
    status = OS_MutSemGetIdByName(&mut_1,"Mut 1");
    UtAssert_True(status != OS_SUCCESS, "OS_MutSemGetIdByName, Mut 1");
    
    status = OS_MutSemGetIdByName(&mut_2,"Mut 2");
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemGetIdByName, Mut 2");
    
    status = OS_MutSemGetIdByName(&mut_3,"Mut 3");
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemGetIdByName, Mut 3");

#ifdef OSAL_OPAQUE_OBJECT_IDS
    /*
     * This should fail - verifies that ID re-use protections are working.
     */
    status = OS_MutSemDelete(saved_mut0_id);
    UtAssert_True(status != OS_SUCCESS, "OS_MutSemDelete, Old ID");
#else
    mutex_ids[0] = saved_mut0_id;
#endif
    
    status = OS_MutSemDelete(mut_0);
    /*  UtDebug("Status after deleteing Mut 0:%d\n",status);  */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemDelete, Mut 0");

    /* this one was never created*/
    status = OS_MutSemDelete(mut_1);
    /*  UtDebug("Status after deleteing Mut 1:%d\n",status);  */
    UtAssert_True(status != OS_SUCCESS, "OS_MutSemDelete, Mut 1");

    status = OS_MutSemDelete(mut_2);
    /*  UtDebug("Status after deleteing Mut 2:%d\n",status);  */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemDelete, Mut 2");

    status = OS_MutSemDelete(mut_3);
    /*  UtDebug("Status after deleteing Mut 3:%d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemDelete, Mut 3");
  
} /* end TestMutexes */



/* These next several tasks simply initialize the ids to a number which
 * cannot occur in the system itself. This helps avoid confusion when a create
 * fails, and the previous value of the id is displayed 
 * */

/* ************************************************************************** */
void InitializeTaskIds(void)
{

task_0_id = 99;  task_1_id = 99;  task_2_id = 99;  task_3_id = 99; 
     task_4_id = 99;  task_5_id = 99;  task_6_id = 99;  task_7_id = 99; 
     task_8_id = 99;  task_9_id = 99; task_10_id = 99; task_11_id = 99;   
    task_12_id = 99; task_13_id = 99; task_14_id = 99; task_15_id = 99;
    task_16_id = 99; task_17_id = 99; task_18_id = 99; task_19_id = 99; 
    task_20_id = 99;

    return;
} /* end InitializeTaskIds */


/* **************************************************************************** */
void InitializeQIds(void)
{
    msgq_0 = 99; msgq_1 = 99; msgq_2 = 99; msgq_3 = 99; msgq_4 = 99; 
    msgq_5 = 99; msgq_6 = 99; msgq_7 = 99; msgq_8 = 99; msgq_9 = 99; msgq_10 = 99; 
    msgq_id = 99;
    
    return;
} /* end InitializeQIds */  


/* ***************************************************************************** */
void InitializeBinIds(void)
{
    bin_0 = 99; bin_1 = 99; bin_2 = 99; bin_3 = 99; bin_4 = 99; bin_5 = 99; 
    bin_6 = 99; bin_7 = 99; bin_8 = 99; bin_9 = 99; bin_10 = 99;
    return;
} /* end InitializeBinIds */


/* ***************************************************************************** */
void InitializeMutIds(void)
{
    mut_0 = 99; mut_1 = 99; mut_2 = 99; mut_3 = 99; mut_4 = 99; mut_5 = 99;
    mut_6 = 99; mut_7 = 99; mut_8 = 99; mut_9 = 99; mut_10 = 99;
    return;
} /* end InitializeMutIds */
/* ***************************************************************************** */
void TestGetInfos(void)
{
    int status;
    OS_task_prop_t      task_prop;
    OS_queue_prop_t     queue_prop;
    OS_bin_sem_prop_t   bin_prop;
    OS_mut_sem_prop_t   mut_prop;
        
    /* first step is to create an object to to get the properties of */
    
    status = OS_TaskCreate( &task_0_id, "Task 0", task_0, task_0_stack, 
                            TASK_0_STACK_SIZE, TASK_0_PRIORITY, 0);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskCreate");

    status = OS_QueueCreate( &msgq_0, "q 0", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* UtDebug("Status after Creating q 0: %d,%d\n",status,msgq_0); */
    UtAssert_True(status == OS_SUCCESS, "OS_QueueCreate");

    status = OS_BinSemCreate( &bin_0,"Bin 0",1,0);
     /* UtDebug("Status after creating: %d,%d\n",status,bin_0); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemCreate");

    status = OS_MutSemCreate( &mut_0,"Mut 0",0);
     /* UtDebug("Status after creating: %d,%d\n",status,mut_0); */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemCreate");

    /* Next Step is to get the properties of the objects */
    
    status = OS_TaskGetInfo(task_0_id, &task_prop);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskGetInfo");

    status = OS_QueueGetInfo(msgq_0, &queue_prop);
    UtAssert_True(status == OS_SUCCESS, "OS_QueueGetInfo");

    status = OS_BinSemGetInfo(bin_0, &bin_prop);
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemGetInfo");

    status = OS_MutSemGetInfo(mut_0, &mut_prop);
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemGetInfo");


    status = OS_TaskDelete(task_0_id);
    UtAssert_True(status == OS_SUCCESS, "OS_TaskDelete");

    status = OS_QueueDelete(msgq_0);
    UtAssert_True(status == OS_SUCCESS, "OS_QueueDelete");
     /* UtDebug("Status after Deleting q 0: %d\n",status); */
    
    status = OS_BinSemDelete(bin_0);
     /* UtDebug("Status after deleteing:%d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_BinSemDelete");


    status = OS_MutSemDelete(mut_0);
     /* UtDebug("Status after deleteing:%d\n",status); */
    UtAssert_True(status == OS_SUCCESS, "OS_MutSemDelete");
}

