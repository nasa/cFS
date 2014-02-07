#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "osal-core-test.h"


/* OS Constructs */

int TestTasks (void);
void InitializeTaskIds (void);
void InitializeQIds (void);
void InitializeBinIds(void);
void InitializeMutIds(void);
int TestQueues(void);
int TestBinaries(void);
int TestMutexes(void);
int TestGetInfos(void);

/* *************************************** MAIN ************************************** */

void OS_Application_Startup(void)
{
    int status;
    int totalfailures = 0;

    OS_API_Init();
    
    OS_printf("*********************************************************\n");
    status = TestTasks();
    OS_printf("Number of failures in TestTasks = %d\n",status);
    totalfailures += status;
    OS_printf("---------------------------------------------------------\n");
    
    status = TestQueues();
    OS_printf("Number of failures in TestQueues = %d\n", status);
    totalfailures += status;
    OS_printf("---------------------------------------------------------\n");

    status = TestBinaries();
    OS_printf("Number of failures in TestBinaries = %d\n",status);
    totalfailures += status;
    OS_printf("---------------------------------------------------------\n");

    status = TestMutexes();
    OS_printf("Number of failures in TestMutexes = %d\n",status);
    totalfailures += status;
    OS_printf("---------------------------------------------------------\n");

    status = TestGetInfos();
    OS_printf("Number of failures in TestGetInfos = %d\n",status);
    totalfailures += status;
   
    OS_printf("*********************************************************\n");
    OS_printf("Total Failures = %d\n",totalfailures);
    OS_printf("*********************************************************\n");
     
    OS_printf("Test Complete: On a Desktop System, hit Control-C to return to command shell\n");

    return;

} /* end OS_Application Startup */


/* ********************************************** TASKS******************************* */
int TestTasks(void)
{
    int status;
    int failTaskCreatecount = 0;
    int failTaskDeletecount = 0;
    int failGetIdcount = 0;
    int totalfail = -999;
    
   /* OS_TaskRegister(); */

    /* Testing Creating up to OS_MAX_TASKS (20), plus one more */
    InitializeTaskIds();
    
    status = OS_TaskCreate( &task_0_id, "Task 0", task_0, task_0_stack, 
                            TASK_0_STACK_SIZE, TASK_0_PRIORITY, 0);
    
    OS_printf("Create Status = %d, Id = %d\n",status,task_0_id);
    OS_printf("Status after creating this task: %d\n",status);
    OS_printf("Id after creating this task: %d\n",task_0_id);
    
    
    if (status != PASS)
        failTaskCreatecount++;
    
    status = OS_TaskCreate( &task_1_id, "Task 1", task_1, task_1_stack, 
                            TASK_1_STACK_SIZE, TASK_1_PRIORITY, 0);
    
    OS_printf("Create Status = %d, Id = %d\n",status,task_1_id);
    OS_printf("Status after creating this task: %d\n",status);
    OS_printf("Id after creating this task: %d\n",task_1_id);
   
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_2_id, "Task 2", task_2, task_2_stack, 
            TASK_2_STACK_SIZE, TASK_2_PRIORITY, 0);
    
    OS_printf("Create Status = %d, Id = %d\n",status,task_2_id);
    OS_printf("Status after creating this task: %d\n",status);
    OS_printf("Id after creating this task: %d\n",task_2_id);
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_3_id, "Task 3", task_3, task_3_stack, 
            TASK_3_STACK_SIZE, TASK_3_PRIORITY, 0);
   
    OS_printf("Create Status = %d, Id = %d\n",status,task_3_id);
    OS_printf("Status after creating this task: %d\n",status);
    OS_printf("Id after creating this task: %d\n",task_3_id);    
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_4_id, "Task 4", task_4, task_4_stack, 
                            TASK_4_STACK_SIZE, TASK_4_PRIORITY, 0);
    
    OS_printf("Create Status = %d, Id = %d\n",status,task_4_id);
    OS_printf("Status after creating this task: %d\n",status);
    OS_printf("Id after creating this task: %d\n",task_4_id);    
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_5_id, "Task 5", task_5, task_5_stack,
                            TASK_5_STACK_SIZE, TASK_5_PRIORITY, 0);
    
    OS_printf("Create Status = %d, Id = %d\n",status,task_5_id);
    OS_printf("Status after creating this task: %d\n",status);
    OS_printf("Id after creating this task: %d\n",task_5_id);
   
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_6_id, "Task 6", task_6, task_6_stack, 
                            TASK_6_STACK_SIZE, TASK_6_PRIORITY, 0);
    OS_printf("Create Status = %d, Id = %d\n",status,task_6_id);
    OS_printf("Status after creating this task: %d\n",status);
    OS_printf("Id after creating this task: %d\n",task_6_id);
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_7_id, "Task 7", task_7, task_7_stack, 
                            TASK_7_STACK_SIZE, TASK_7_PRIORITY, 0);
   
    OS_printf("Create Status = %d, Id = %d\n",status,task_7_id);
    OS_printf("Status after creating this task: %d\n",status);
    OS_printf("Id after creating this task: %d\n",task_7_id);
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_8_id, "Task 8", task_8, task_8_stack, 
                            TASK_8_STACK_SIZE, TASK_8_PRIORITY, 0);
     
     OS_printf("Create Status = %d, Id = %d\n",status,task_8_id);
     OS_printf("Status after creating this task: %d\n",status);
     OS_printf("Id after creating this task: %d\n",task_8_id);
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_9_id, "Task 9", task_9, task_9_stack, 
                            TASK_9_STACK_SIZE, TASK_9_PRIORITY, 0);
    
     OS_printf("Create Status = %d, Id = %d\n",status,task_9_id);
     OS_printf("Status after creating this task: %d\n",status);
     OS_printf("Id after creating this task: %d\n",task_9_id);
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_10_id, "Task 10", task_10, task_10_stack, 
                            TASK_10_STACK_SIZE, TASK_10_PRIORITY, 0);
    
      OS_printf("Create Status = %d, Id = %d\n",status,task_10_id);
      OS_printf("Status after creating this task: %d\n",status);
      OS_printf("Id after creating this task: %d\n",task_10_id);
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_11_id, "Task 11", task_11, task_11_stack, 
                            TASK_11_STACK_SIZE, TASK_11_PRIORITY, 0);
    
      OS_printf("Create Status = %d, Id = %d\n",status,task_11_id);
      OS_printf("Status after creating this task: %d\n",status);
      OS_printf("Id after creating this task: %d\n",task_11_id);
    
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_12_id, "Task 12", task_12, task_12_stack, 
                            TASK_12_STACK_SIZE, TASK_12_PRIORITY, 0);

     OS_printf("Create Status = %d, Id = %d\n",status,task_12_id);
     OS_printf("Status after creating this task: %d\n",status);
     OS_printf("Id after creating this task: %d\n",task_12_id);
     
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_13_id, "Task 13", task_13, task_13_stack, 
                            TASK_13_STACK_SIZE, TASK_13_PRIORITY, 0);
     
     OS_printf("Create Status = %d, Id = %d\n",status,task_13_id);
     OS_printf("Status after creating this task: %d\n",status);
     OS_printf("Id after creating this task: %d\n",task_13_id);
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_14_id, "Task 14", task_14, task_14_stack, 
                            TASK_14_STACK_SIZE, TASK_14_PRIORITY, 0);
    
     OS_printf("Create Status = %d, Id = %d\n",status,task_14_id);
     OS_printf("Status after creating this task: %d\n",status);
     OS_printf("Id after creating this task: %d\n",task_14_id);
   
    if (status != PASS)
        failTaskCreatecount++;
        
    status = OS_TaskCreate( &task_15_id, "Task 15", task_15, task_15_stack, 
                            TASK_15_STACK_SIZE, TASK_15_PRIORITY, 0);
    
      OS_printf("Create Status = %d, Id = %d\n",status,task_15_id);
      OS_printf("Status after creating this task: %d\n",status);
      OS_printf("Id after creating this task: %d\n",task_15_id);
    
    if (status != PASS)
        failTaskCreatecount++;


    status = OS_TaskCreate( &task_16_id, "Task 16", task_16, task_16_stack, 
                            TASK_16_STACK_SIZE, TASK_16_PRIORITY, 0);
     
      OS_printf("Create Status = %d, Id = %d\n",status,task_16_id);
      OS_printf("Status after creating this task: %d\n",status);
      OS_printf("Id after creating this task: %d\n",task_16_id);
     
    
    if (status != PASS)
        failTaskCreatecount++;


    status = OS_TaskCreate( &task_17_id, "Task 17", task_17, task_17_stack, 
                            TASK_17_STACK_SIZE, TASK_17_PRIORITY, 0);
    
      OS_printf("Create Status = %d, Id = %d\n",status,task_17_id);
      OS_printf("Status after creating this task: %d\n",status);
      OS_printf("Id after creating this task: %d\n",task_17_id);
     
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_18_id, "Task 18", task_18, task_18_stack, 
                            TASK_18_STACK_SIZE, TASK_18_PRIORITY, 0);
    
      OS_printf("Create Status = %d, Id = %d\n",status,task_18_id);
      OS_printf("Status after creating this task: %d\n",status);
      OS_printf("Id after creating this task: %d\n",task_18_id);
     
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_19_id, "Task 19", task_19, task_19_stack, 
                            TASK_19_STACK_SIZE, TASK_19_PRIORITY, 0);
     
      OS_printf("Create Status = %d, Id = %d\n",status,task_19_id);
      OS_printf("Status after creating this task: %d\n",status);
      OS_printf("Id after creating this task: %d\n",task_19_id);
     
    
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_20_id, "Task 20", task_20, task_20_stack, 
                            TASK_20_STACK_SIZE, TASK_20_PRIORITY, 0);
    
      OS_printf("Create Status = %d, Id = %d\n",status,task_20_id);
      OS_printf("Status after creating this task: %d\n",status);
      OS_printf("Id after creating this task: %d\n",task_20_id);
     
    
    if (status == PASS)
        failTaskCreatecount++;
    
    /* Testing the Deletions of all the tasks we have created */
    
    if (OS_TaskDelete(task_0_id) != PASS)
        failTaskDeletecount++;
        
    
    if (OS_TaskDelete(task_1_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_2_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_3_id) != PASS)
        failTaskDeletecount++;
    
    if (OS_TaskDelete(task_4_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_5_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_6_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_7_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_8_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_9_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_10_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_11_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_12_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_13_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_14_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_15_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_16_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_17_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_18_id) != PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_19_id) != PASS)
        failTaskDeletecount++;

    /* This Task was never created successfully */
    if (OS_TaskDelete(task_20_id) == PASS)
        failTaskDeletecount++;
    
    /* These next few tasks were deleted already, testing a "redelete" */
    
    if (OS_TaskDelete(task_1_id) == PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_2_id) == PASS)
        failTaskDeletecount++;

    if (OS_TaskDelete(task_3_id) == PASS)
        failTaskDeletecount++;

    /* Creating some more tasks for testing name functions */
    

    InitializeTaskIds();
    /* Create Task 0 again */
    status = OS_TaskCreate( &task_0_id, "Task 0", task_0, task_0_stack, 
                            TASK_0_STACK_SIZE, TASK_0_PRIORITY, 0);
      /*OS_printf("Create Status = %d, Id = %d\n",status,task_0_id); */
    if (status != PASS)
        failTaskCreatecount++;
    
  /* Try and create another "Task 0", should fail as we already have one named "Task 0" */
    status = OS_TaskCreate( &task_1_id, "Task 0", task_0, task_0_stack, 
                            TASK_0_STACK_SIZE, TASK_0_PRIORITY, 0);
     /* OS_printf("Create Status = %d, Id = %d\n",status,task_1_id); */
    if (status == PASS)
        failTaskCreatecount++;

    status = OS_TaskCreate( &task_2_id, "Task 2", task_2, task_2_stack,
                            TASK_2_STACK_SIZE, TASK_2_PRIORITY, 0);
    /*  OS_printf("Create Status = %d, Id = %d\n",status,task_2_id); */
    if (status != PASS)
        failTaskCreatecount++;
    
    status = OS_TaskCreate( &task_3_id, "Task 3", task_3, task_3_stack,
                            TASK_3_STACK_SIZE, TASK_3_PRIORITY, 0);
    /*  OS_printf("Create Status = %d, Id = %d\n",status,task_3_id); */
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_TaskGetIdByName(&task_0_id, "Task 0");
    /* OS_printf("Satus after Getting the id of \"Task 0\":%d,%d \n\n",status,task_0_id); */
    /*first newly created task should have id == 0*/
    if (status != PASS || task_0_id != 0)    
        failGetIdcount++;
    
    status = OS_TaskGetIdByName(&task_1_id, "Task 1");
    /*OS_printf("Satus after Getting the id of \"Task 1\":%d,%d \n\n",status,task_1_id);*/
    if (status == PASS)
        failGetIdcount++;

    status = OS_TaskGetIdByName(&task_2_id, "Task 2");
    /* OS_printf("Satus after Getting the id of \"Task 2\":%d,%d \n\n",status,task_2_id);*/
    if (status != PASS || task_2_id != 1)   
        failGetIdcount++;
    
    status = OS_TaskGetIdByName(&task_3_id, "Task 3");
    /* OS_printf("Satus after Getting the id of \"Task 3\":%d,%d \n\n",status,task_3_id); */
    if (status != PASS || task_3_id != 2)    
        failGetIdcount++;

    if (OS_TaskDelete(task_0_id) != PASS)
        failTaskDeletecount++;
    if (OS_TaskDelete(task_1_id) == PASS)
        failTaskDeletecount++;
    
    if (OS_TaskDelete(task_2_id) != PASS)
        failTaskDeletecount++;
    
    if (OS_TaskDelete(task_3_id) != PASS)
        failTaskDeletecount++;
    

    if (failTaskCreatecount != 0)
        OS_printf("Task Create Failed Count: %d\n", failTaskCreatecount);

    if (failTaskDeletecount != 0)
        OS_printf("Task Delete Failed Count: %d\n", failTaskDeletecount);


    if (failGetIdcount != 0)
        OS_printf("Task Get ID Failed Count: %d\n", failGetIdcount);

    totalfail = failTaskCreatecount + failTaskDeletecount + failGetIdcount;
    
    return totalfail;

}/* end TestTasks */


/* ************************************************************************************ */

int TestQueues(void)
{
    int status;
    int failQCreatecount = 0;
    int failQDeletecount = 0;
    int failQGetIdcount = 0;
    int totalfailures = 0;

    InitializeQIds();
    status = OS_QueueCreate( &msgq_0, "q 0", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 0: %d,%d\n",status,msgq_0); */
    if (status != PASS)
        failQCreatecount++;
    
    status = OS_QueueCreate( &msgq_1, "q 1", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 1: %d,%d\n",status,msgq_1); */
    if (status != PASS)
        failQCreatecount++;
    
    status = OS_QueueCreate( &msgq_2, "q 2", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 2: %d,%d\n",status,msgq_2); */
    if (status != PASS)
        failQCreatecount++;
    
    status = OS_QueueCreate( &msgq_3, "q 3", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 3: %d,%d\n",status,msgq_3); */
    if (status != PASS)
        failQCreatecount++;
    
    status = OS_QueueCreate( &msgq_4, "q 4", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 4: %d,%d\n",status,msgq_4); */
    if (status != PASS)
        failQCreatecount++;
    
    status = OS_QueueCreate( &msgq_5, "q 5", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 5: %d,%d\n",status,msgq_5); */
    if (status != PASS)
        failQCreatecount++;
    
    status = OS_QueueCreate( &msgq_6, "q 6", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 6: %d,%d\n",status,msgq_6); */
    if (status != PASS)
        failQCreatecount++;
    
    status = OS_QueueCreate( &msgq_7, "q 7", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 7: %d,%d\n",status,msgq_7); */
    if (status != PASS)
        failQCreatecount++;
    
    status = OS_QueueCreate( &msgq_8, "q 8", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 8: %d,%d\n",status,msgq_8); */
    if (status != PASS)
        failQCreatecount++;

    status = OS_QueueCreate( &msgq_9, "q 9", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 9: %d,%d\n",status,msgq_9); */
    if (status != PASS)
        failQCreatecount++;
    
    /* This one should fail, it is queue number 11 */
    status = OS_QueueCreate( &msgq_10, "q 10", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 10: %d, %d\n",status,msgq_10); */
    if (status == PASS)
        failQCreatecount++;

    
 /*     Trying now to Delete all of the Queues created. */
    
    status = OS_QueueDelete(msgq_0);
     /* OS_printf("Status after Deleting q 0: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    status = OS_QueueDelete(msgq_1);
     /* OS_printf("Status after Deleting q 1: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    status = OS_QueueDelete(msgq_2);
     /* OS_printf("Status after Deleting q 2: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    status = OS_QueueDelete(msgq_3);
     /* OS_printf("Status after Deleting q 3: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    status = OS_QueueDelete(msgq_4);
     /* OS_printf("Status after Deleting q 4: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    status = OS_QueueDelete(msgq_5);
     /* OS_printf("Status after Deleting q 5: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    status = OS_QueueDelete(msgq_6);
     /* OS_printf("Status after Deleting q 6: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    status = OS_QueueDelete(msgq_7);
     /* OS_printf("Status after Deleting q 7: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    status = OS_QueueDelete(msgq_8);
     /* OS_printf("Status after Deleting q 8: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    status = OS_QueueDelete(msgq_9);
     /* OS_printf("Status after Deleting q 9: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    /* This one should have never gotten created, so it */
    /* shouldn't get deleted */
    status = OS_QueueDelete(msgq_10);
     /* OS_printf("Status after Deleting q 10: %d\n",status); */
    if (status == PASS)
        failQDeletecount++;

/*     Create Some more Queues for trying to get the id by name */

    InitializeQIds();
    status = OS_QueueCreate( &msgq_0, "q 0", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 0: %d,%d\n",status,msgq_0);*/
    if (status != PASS)
        failQCreatecount++;
    
    /* This one should fail */
    status = OS_QueueCreate( &msgq_1, "q 0", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 0 again: %d,%d\n",status,msgq_1); */
    if (status == PASS)
        failQCreatecount++;
    
    status = OS_QueueCreate( &msgq_2, "q 2", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 2: %d,%d\n",status,msgq_2); */
    if (status != PASS)
        failQCreatecount++;
    
    status = OS_QueueCreate( &msgq_3, "q 3", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 3: %d,%d\n",status,msgq_3); */
    if (status != PASS)
        failQCreatecount++;

/*    
* Now that the Queues are created, its time to see if we can find
* the propper ID by the name of the queue;
*/
    status = OS_QueueGetIdByName(&msgq_0,"q 0");
    if (status != PASS || msgq_0 != 0)
        failQGetIdcount++;

    status = OS_QueueGetIdByName(&msgq_1,"q 1");
    if (status == PASS)
        failQGetIdcount++;

    status = OS_QueueGetIdByName(&msgq_2,"q 2");
    if (status != PASS || msgq_2 != 1)
        failQGetIdcount++;

    status = OS_QueueGetIdByName(&msgq_3,"q 3");
    if (status != PASS || msgq_3 != 2)
        failQGetIdcount++;

    /* Time to Delete the Queues we just created */

    status = OS_QueueDelete(msgq_0);
     /* OS_printf("Status after Deleting q 0 : %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    status = OS_QueueDelete(msgq_1);
     /* OS_printf("Status after Deleting q 1: %d\n",status); */
    if (status == PASS)
        failQDeletecount++;

    status = OS_QueueDelete(msgq_2);
     /* OS_printf("Status after Deleting q 2: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    status = OS_QueueDelete(msgq_3);
     /* OS_printf("Status after Deleting q 3: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;
    
    totalfailures = failQCreatecount + failQDeletecount + failQGetIdcount;
    
    if (totalfailures != 0)
    {
        OS_printf("Queue Create Failed Count: %d\n",failQCreatecount);
        OS_printf("Queue Delete Failed Count: %d\n",failQDeletecount);
        OS_printf("Queue Get ID Failed Count: %d\n",failQGetIdcount);
    }
    
    return totalfailures;
}/* end TestQueues */

/* *************************************************************************** */
int TestBinaries(void)
{
    int failBinCreatecount = 0;
    int failBinDeletecount = 0;
    int failBinGetIdcount = 0;
    int totalfailures = 0;
    int status;
    InitializeBinIds();

    status = OS_BinSemCreate( &bin_0,"Bin 0",1,0);
    /*  OS_printf("Status after creating: %d,%d\n",status,bin_0); */ 
    if (status != OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_1,"Bin 1",1,0);
    /*  OS_printf("Status after creating: %d,%d\n",status,bin_1); */
    if (status != OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_2,"Bin 2",1,0);
      /* OS_printf("Status after creating: %d,%d\n",status,bin_2); */
    if (status != OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_3,"Bin 3",1,0);
      /* OS_printf("Status after creating: %d,%d\n",status,bin_3); */
    if (status != OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_4,"Bin 4",1,0);
      /* OS_printf("Status after creating: %d,%d\n",status,bin_4); */
    if (status != OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_5,"Bin 5",1,0);
      /* OS_printf("Status after creating: %d,%d\n",status,bin_5); */
    if (status != OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_6,"Bin 6",1,0);
      /* OS_printf("Status after creating: %d,%d\n",status,bin_6); */
    if (status != OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_7,"Bin 7",1,0);
      /* OS_printf("Status after creating: %d,%d\n",status,bin_7); */
    if (status != OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_8,"Bin 8",1,0);
      /* OS_printf("Status after creating: %d,%d\n",status,bin_8); */
    if (status != OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_9,"Bin 9",1,0);
      /* OS_printf("Status after creating: %d,%d\n",status,bin_9); */
    if (status != OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_10,"Bin 10",1,0);
      /* OS_printf("Status after creating: %d,%d\n",status,bin_10); */
    if (status == OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemDelete(bin_0);
      /* OS_printf("Status after deleteing:%d\n",status); */ 
    if (status != OS_SUCCESS)
        failBinDeletecount++;

    status = OS_BinSemDelete(bin_1);
      /* OS_printf("Status after deleteing:%d\n",status); */ 
    if (status != OS_SUCCESS)
        failBinDeletecount++;

    status = OS_BinSemDelete(bin_2);
      /* OS_printf("Status after deleteing:%d\n",status); */ 
    if (status != OS_SUCCESS)
        failBinDeletecount++;

    status = OS_BinSemDelete(bin_3);
      /* OS_printf("Status after deleteing:%d\n",status); */ 
    if (status != OS_SUCCESS)
        failBinDeletecount++;

    status = OS_BinSemDelete(bin_4);
      /* OS_printf("Status after deleteing:%d\n",status); */ 
    if (status != OS_SUCCESS)
        failBinDeletecount++;

    status = OS_BinSemDelete(bin_5);
      /* OS_printf("Status after deleteing:%d\n",status); */ 
    if (status != OS_SUCCESS)
        failBinDeletecount++;

    status = OS_BinSemDelete(bin_6);
      /* OS_printf("Status after deleteing:%d\n",status); */ 
    if (status != OS_SUCCESS)
        failBinDeletecount++;
    status = OS_BinSemDelete(bin_7);
      /* OS_printf("Status after deleteing:%d\n",status); */ 
    if (status != OS_SUCCESS)
        failBinDeletecount++;

    status = OS_BinSemDelete(bin_8);
      /* OS_printf("Status after deleteing:%d\n",status); */ 
    if (status != OS_SUCCESS)
        failBinDeletecount++;

    status = OS_BinSemDelete(bin_9);
      /* OS_printf("Status after deleteing:%d\n",status); */ 
    if (status != OS_SUCCESS)
        failBinDeletecount++;

    status = OS_BinSemDelete(bin_10);
      /* OS_printf("Status after deleteing:%d\n",status); */ 
    if (status == OS_SUCCESS)
        failBinDeletecount++;

/*
* Now Create a few extra semaphores
*  to test  GetIdByName
*/

    InitializeBinIds();
    status = OS_BinSemCreate( &bin_0,"Bin 0",OS_SEM_FULL,0);
    /* OS_printf("Status after creating: %d,%d\n",status,bin_0); */
    if (status != OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_1,"Bin 0",OS_SEM_FULL,0);
    /* OS_printf("Status after creating: %d,%d\n",status,bin_1); */
    if (status == OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_2,"Bin 2",OS_SEM_EMPTY,0);
     /* OS_printf("Status after creating: %d,%d\n",status,bin_2);  */
    if (status != OS_SUCCESS)
        failBinCreatecount++;

    status = OS_BinSemCreate( &bin_3,"Bin 3",OS_SEM_EMPTY,0);
     /* OS_printf("Status after creating: %d,%d\n",status,bin_3); */
    if (status != OS_SUCCESS)
        failBinCreatecount++;



    
    status = OS_BinSemGetIdByName(&bin_0,"Bin 0");
      /* OS_printf("Status after GETID: %d,%d\n",status,bin_0); */
    if (status != OS_SUCCESS || bin_0 != 0)
        failBinGetIdcount++;
    
    status = OS_BinSemGetIdByName(&bin_1,"Bin 1");
     /* OS_printf("Status after GETID: %d,%d\n",status,bin_1); */

    if (status == OS_SUCCESS)
        failBinGetIdcount++;
    
    status = OS_BinSemGetIdByName(&bin_2,"Bin 2");
    /* OS_printf("Status after GETID: %d,%d\n",status,bin_2); */ 

    if (status != OS_SUCCESS || bin_2 != 1)
        failBinGetIdcount++;
    
    status = OS_BinSemGetIdByName(&bin_3,"Bin 3");
     /* OS_printf("Status after GETID: %d,%d\n",status,bin_3); */
    if (status != OS_SUCCESS || bin_3 != 2)
        failBinGetIdcount++;
     
    status = OS_BinSemDelete(bin_0);
     /* OS_printf("Status after deleteing:%d\n",status); */
    if (status != OS_SUCCESS)
        failBinDeletecount++;

    /* this one was never created */
    status = OS_BinSemDelete(bin_1);
     /* OS_printf("Status after deleteing:%d\n",status); */
    if (status == OS_SUCCESS)
        failBinDeletecount++;

    if (failBinCreatecount != 0)
        OS_printf("Bin Sem Create Failed Count %d\n", failBinCreatecount);

    if (failBinDeletecount != 0)
        OS_printf("Bin Sem Delete Failed Count %d\n", failBinDeletecount);

    if (failBinGetIdcount != 0)
        OS_printf("Bin Sem Get ID Failed Count %d\n", failBinGetIdcount);

    
    totalfailures = failBinCreatecount + failBinDeletecount + failBinGetIdcount;
    
    return totalfailures;

}/* end TestBinaries */


/* ************************************************************************************ */
int TestMutexes(void)
{
    int failMutCreatecount = 0;
    int failMutDeletecount = 0;
    int failMutGetIdcount = 0;
    int totalfailures = 0;
    int status;
    
    InitializeMutIds();

    status = OS_MutSemCreate( &mut_0,"Mut 0",0);
    /* OS_printf("Status after creating Mut 0: %d,%d\n",status,mut_0);  */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_1,"Mut 1",0);
    /*  OS_printf("Status after creating Mut 1: %d,%d\n",status,mut_1);  */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_2,"Mut 2",0);
    /*  OS_printf("Status after creating Mut 2: %d,%d\n",status,mut_2);  */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_3,"Mut 3",0);
    /*  OS_printf("Status after creating Mut 3: %d,%d\n",status,mut_3); */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_4,"Mut 4",0);
    /*  OS_printf("Status after creating Mut 4: %d,%d\n",status,mut_4);  */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_5,"Mut 5",0);
    /*   OS_printf("Status after creating Mut 5: %d,%d\n",status,mut_5);  */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_6,"Mut 6",0);
    /*  OS_printf("Status after creating Mut 6: %d,%d\n",status,mut_6);  */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_7,"Mut 7",0);
    /*  OS_printf("Status after creating Mut 7: %d,%d\n",status,mut_7);  */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_8,"Mut 8",0);
    /*  OS_printf("Status after creating Mut 8: %d,%d\n",status,mut_8);  */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_9,"Mut 9",0);
    /*  OS_printf("Status after creating Mut 9: %d,%d\n",status,mut_9);  */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_10,"Mut 10",0);
    /*  OS_printf("Status after creating Mut 10: %d,%d\n",status,mut_10);  */
    if (status == OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemDelete(mut_0);
    /*  OS_printf("Status after deleteing Mut 0:%d\n",status);   */
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    status = OS_MutSemDelete(mut_1);
    /*  OS_printf("Status after deleteing Mut 1:%d\n",status);  */
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    status = OS_MutSemDelete(mut_2);
    /*  OS_printf("Status after deleteing Mut 2:%d\n",status);  */
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    status = OS_MutSemDelete(mut_3);
    /*  OS_printf("Status after deleteing Mut 3:%d\n",status);  */
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    status = OS_MutSemDelete(mut_4);
    /*  OS_printf("Status after deleteing Mut 4:%d\n",status);  */
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    status = OS_MutSemDelete(mut_5);
    /*  OS_printf("Status after deleteing Mut 5:%d\n",status);  */
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    status = OS_MutSemDelete(mut_6);
    /*  OS_printf("Status after deleteing Mut 6:%d\n",status);  */
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    status = OS_MutSemDelete(mut_7);
    /*  OS_printf("Status after deleteing Mut 7:%d\n",status);  */
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    status = OS_MutSemDelete(mut_8);
    /*  OS_printf("Status after deleteing Mut 8:%d\n",status); */ 
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    status = OS_MutSemDelete(mut_9);
    /*  OS_printf("Status after deleteing Mut 9:%d\n",status);  */
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    status = OS_MutSemDelete(mut_10);
    /*  OS_printf("Status after deleteing Mut 10:%d\n",status); */
    if (status == OS_SUCCESS)
        failMutDeletecount++;
/*
* Now Create a few extra semaphores
*  to test  GetIdByName
*/
    InitializeMutIds();
    status = OS_MutSemCreate( &mut_0,"Mut 0",0);
    /*  OS_printf("Status after creating Mut 0: %d,%d\n",status,mut_0);  */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_1,"Mut 0",0);
    /*  OS_printf("Status after creating Mut 0 again: %d,%d\n",status,mut_1); */
    if (status == OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_2,"Mut 2",0);
    /*  OS_printf("Status after creating Mut 2: %d,%d\n",status,mut_2); */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemCreate( &mut_3,"Mut 3",0);
    /*  OS_printf("Status after creating Mut 3: %d,%d\n",status,mut_3); */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    status = OS_MutSemGetIdByName(&mut_0,"Mut 0");
    if (status != OS_SUCCESS || mut_0 != 0)
        failMutGetIdcount++;
    
    status = OS_MutSemGetIdByName(&mut_1,"Mut 1");
    if (status == OS_SUCCESS)
        failMutGetIdcount++;
    
    status = OS_MutSemGetIdByName(&mut_2,"Mut 2");
    if (status != OS_SUCCESS || mut_2 != 1)
        failMutGetIdcount++;
    
    status = OS_MutSemGetIdByName(&mut_3,"Mut 3");
    if (status != OS_SUCCESS || mut_3 != 2)
        failMutGetIdcount++;
    
    status = OS_MutSemDelete(mut_0);
    /*  OS_printf("Status after deleteing Mut 0:%d\n",status);  */
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    /* this one was never created*/
    status = OS_MutSemDelete(mut_1);
    /*  OS_printf("Status after deleteing Mut 1:%d\n",status);  */
    if (status == OS_SUCCESS)
        failMutDeletecount++;

    status = OS_MutSemDelete(mut_2);
    /*  OS_printf("Status after deleteing Mut 2:%d\n",status);  */
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    status = OS_MutSemDelete(mut_3);
    /*  OS_printf("Status after deleteing Mut 3:%d\n",status); */ 
    if (status != OS_SUCCESS)
        failMutDeletecount++;
  
    if (failMutCreatecount != 0)
        OS_printf("Mutex Create Failed Count %d\n", failMutCreatecount);

    if (failMutDeletecount != 0)
        OS_printf("Mutex Delete Failed Count %d\n", failMutDeletecount);

    if (failMutGetIdcount != 0)
        OS_printf("Mutex Get ID Failed Count %d\n", failMutGetIdcount);

    totalfailures = failMutCreatecount + failMutDeletecount + failMutGetIdcount;
    
    return totalfailures;

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
int TestGetInfos(void)
{
    int status;
    int failTaskCreatecount = 0;
    int failTaskDeletecount = 0;
    int failQCreatecount = 0;
    int failQDeletecount = 0;
    int failBinCreatecount = 0;
    int failBinDeletecount = 0;
    int failMutCreatecount = 0;
    int failMutDeletecount = 0;
    int failGetInfocount = 0;
    int totalfailures = 0;

    OS_task_prop_t      task_prop;
    OS_queue_prop_t     queue_prop;
    OS_bin_sem_prop_t   bin_prop;
    OS_mut_sem_prop_t   mut_prop;
        
    /* first step is to create an object to to get the properties of */
    
    status = OS_TaskCreate( &task_0_id, "Task 0", task_0, task_0_stack, 
                            TASK_0_STACK_SIZE, TASK_0_PRIORITY, 0);
    if (status != PASS)
        failTaskCreatecount++;

    status = OS_QueueCreate( &msgq_0, "q 0", MSGQ_DEPTH, MSGQ_SIZE, 0);
     /* OS_printf("Status after Creating q 0: %d,%d\n",status,msgq_0); */
    if (status != PASS)
        failQCreatecount++;

    status = OS_BinSemCreate( &bin_0,"Bin 0",1,0);
     /* OS_printf("Status after creating: %d,%d\n",status,bin_0); */
    if (status != OS_SUCCESS)
        failBinCreatecount++;

  status = OS_MutSemCreate( &mut_0,"Mut 0",0);
     /* OS_printf("Status after creating: %d,%d\n",status,mut_0); */
    if (status != OS_SUCCESS)
        failMutCreatecount++;

    /* Next Step is to get the properties of the objects */
    
    status = OS_TaskGetInfo(task_0_id, &task_prop);
    if (status != OS_SUCCESS)
        failGetInfocount++;
    status = OS_QueueGetInfo(msgq_0, &queue_prop);
    if (status != OS_SUCCESS)
        failGetInfocount++;

    status = OS_BinSemGetInfo(bin_0, &bin_prop);
    if (status != OS_SUCCESS)
        failGetInfocount++;

    status = OS_MutSemGetInfo(mut_0, &mut_prop);
    if (status != OS_SUCCESS)
        failGetInfocount++;

    if (OS_TaskDelete(task_0_id) != PASS)
        failTaskDeletecount++;

    status = OS_QueueDelete(msgq_0);
     /* OS_printf("Status after Deleting q 0: %d\n",status); */
    if (status != PASS)
        failQDeletecount++;

    
    status = OS_BinSemDelete(bin_0);
     /* OS_printf("Status after deleteing:%d\n",status); */
    if (status != OS_SUCCESS)
        failBinDeletecount++;


    status = OS_MutSemDelete(mut_0);
     /* OS_printf("Status after deleteing:%d\n",status); */
    if (status != OS_SUCCESS)
        failMutDeletecount++;

    totalfailures = failMutDeletecount +  failBinDeletecount  + failQDeletecount   + 
                    failTaskDeletecount + failMutCreatecount  + failBinCreatecount + 
                    failQCreatecount    + failTaskCreatecount + failGetInfocount;

    return totalfailures;

}

