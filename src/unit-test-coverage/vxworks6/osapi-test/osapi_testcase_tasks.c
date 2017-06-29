/*
 * Filename: osapi_testcase_tasks.c
 *
 * Purpose: This file contains unit test cases
 *
 * Notes:
 *
 * Modification History:
 *   06/25/2015, A. Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include "osapi_testcase.h"



/* Prototypes for non-exported functions */



/* -------------------- Special Test Case Variables ------------------------- */
static boolean deleteFncCalled = FALSE;
static OsApi_Adaptor_OS_task_record_t  *osTaskRecord_ptr;


/* -------------------------------------------------------------------------- */
/* Utility functions */

/* Dummy functions for OSAL calls */
void deleteFnc(void)
{
    /* Task delete hook */
    deleteFncCalled += 1;
}



/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  OS_API_Init Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_API_Init_ModTblInit_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_OSMODULETABLEINIT_INDEX, OS_ERROR, 1);

    /* Execute Test */
    actual = OS_API_Init();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

void Test_OS_API_Init_TimerApiInit_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_OSTIMERAPIINIT_INDEX, OS_ERROR, 1);

    /* Execute Test */
    actual = OS_API_Init();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

void Test_OS_API_Init_TaskTableSem_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMMCREATE_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_API_Init();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

void Test_OS_API_Init_QueueTableSem_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMMCREATE_INDEX, 0, 2);

    /* Execute Test */
    actual = OS_API_Init();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

void Test_OS_API_Init_BinTableSem_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMMCREATE_INDEX, 0, 3);

    /* Execute Test */
    actual = OS_API_Init();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

void Test_OS_API_Init_CountTableSem_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMMCREATE_INDEX, 0, 4);

    /* Execute Test */
    actual = OS_API_Init();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

void Test_OS_API_Init_MutTableSem_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SEMMCREATE_INDEX, 0, 5);

    /* Execute Test */
    actual = OS_API_Init();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

void Test_OS_API_Init_OsFsInit_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_OSFSINIT_INDEX, OS_ERROR, 1);

    /* Execute Test */
    actual = OS_API_Init();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

void Test_OS_API_Init_TaskSpawn_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_TASKSPAWN_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_API_Init();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
}

void Test_OS_API_Init_MsgQCreate_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_MSGQCREATE_INDEX, 0, 1);

    /* Execute Test */
    actual = OS_API_Init();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
    UtAssert_True(getNTaskDeleteForce() == 1, "taskDeleteForce called");
}

void Test_OS_API_Init_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;
    uint32 ii      = 0;

    /* Setup Inputs */

    /* Execute Test */
    actual = OS_API_Init();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(getNTaskDeleteForce() == 0, "taskDeleteForce not called");
    for (ii = 0; ii < OS_MAX_TASKS; ++ii)
    {
        UtAssert_True(OsApi_Adaptor_getTaskTableEntry(ii)->free == TRUE,
                      "OS_task_table entry is free");
    }
    for (ii = 0; ii < OS_MAX_QUEUES; ++ii)
    {
        UtAssert_True(OsApi_Adaptor_getQueueTableEntry(ii)->free == TRUE,
                      "OS_queue_table entry is free");
    }
    for (ii = 0; ii < OS_MAX_BIN_SEMAPHORES; ++ii)
    {
        UtAssert_True(OsApi_Adaptor_getBinSemTableEntry(ii)->free == TRUE,
                      "OS_bin_sem_table entry is free");
    }
    for (ii = 0; ii < OS_MAX_COUNT_SEMAPHORES; ++ii)
    {
        UtAssert_True(OsApi_Adaptor_getCountSemTableEntry(ii)->free == TRUE,
                      "OS_count_sem_table entry is free");
    }
    for (ii = 0; ii < OS_MAX_MUTEXES; ++ii)
    {
        UtAssert_True(OsApi_Adaptor_getMutSemTableEntry(ii)->free == TRUE,
                      "OS_mut_sem_table_entry is free");
    }
}


/*******************************************************************************
**
**  OS_TaskCreate Tests
**
*******************************************************************************/
void dummyFnc(void)
{
    return;
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskCreate_NullName(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id    = 1;
    char *task_name   = NULL;
    uint32 stack      = 1;
    uint32 stack_size = 1;
    uint32 priority   = 1;
    uint32 flags      = 1;

    /* Execute Test */
    actual = OS_TaskCreate(&task_id, task_name, dummyFnc, &stack, stack_size, priority, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 0, "No OS_TASK_TABLE_SEM taken");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskCreate_NullFnc(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id    = 1;
    char *task_name   = "test";
    uint32 stack      = 1;
    uint32 stack_size = 1;
    uint32 priority   = 1;
    uint32 flags      = 1;

    /* Execute Test */
    actual = OS_TaskCreate(&task_id, task_name, NULL, &stack, stack_size, priority, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 0, "No OS_TASK_TABLE_SEM taken");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskCreate_NullId(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 *task_id   = NULL;
    char *task_name   = "test";
    uint32 stack      = 1;
    uint32 stack_size = 1;
    uint32 priority   = 1;
    uint32 flags      = 1;

    /* Execute Test */
    actual = OS_TaskCreate(task_id, task_name, dummyFnc, &stack, stack_size, priority, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 0, "No OS_TASK_TABLE_SEM taken");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskCreate_NameTooLong(void)
{
    int32 expected = OS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id    = 1;
    char *task_name   = "test";
    uint32 stack      = 1;
    uint32 stack_size = 1;
    uint32 priority   = 1;
    uint32 flags      = 1;

    OsApi_SetReturnCode(OSAPI_STRLEN_INDEX, OS_MAX_API_NAME, 1);

    /* Execute Test */
    actual = OS_TaskCreate(&task_id, task_name, dummyFnc, &stack, stack_size, priority, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TOO_LONG");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 0, "No OS_TASK_TABLE_SEM taken");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskCreate_PriorityError(void)
{
    int32 expected = OS_ERR_INVALID_PRIORITY;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id    = 1;
    char *task_name   = "test";
    uint32 stack      = 1;
    uint32 stack_size = 1;
    uint32 priority   = MAX_PRIORITY + 1;
    uint32 flags      = 1;

    /* Execute Test */
    actual = OS_TaskCreate(&task_id, task_name, dummyFnc, &stack, stack_size, priority, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_PRIORITY");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 0, "No OS_TASK_TABLE_SEM taken");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskCreate_NoFreeIds(void)
{
    int32 expected = OS_ERR_NO_FREE_IDS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id    = 1;
    char *task_name   = "test";
    uint32 stack      = 1;
    uint32 stack_size = 1;
    uint32 priority   = 1;
    uint32 flags      = 1;

    /* Execute Test */
    actual = OS_TaskCreate(&task_id, task_name, dummyFnc, &stack, stack_size, priority, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NO_FREE_IDS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 1, "semaphore taken once");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskCreate_NameTaken(void)
{
    int32 expected = OS_ERR_NAME_TAKEN;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id    = 1;
    char *task_name   = "test";
    uint32 stack      = 1;
    uint32 stack_size = 1;
    uint32 priority   = 1;
    uint32 flags      = 1;

    OsApi_Adaptor_setTaskTableEntry(0, TRUE, 0, "", 0, 0, 0, NULL);
    OsApi_Adaptor_setTaskTableEntry(1, FALSE, 0, "bad_task_name", 0, 0, 0, NULL);
    OsApi_Adaptor_setTaskTableEntry(OS_MAX_TASKS - 1, FALSE, 0, task_name, 0, 0, 0, NULL);

    /* Execute Test */
    actual = OS_TaskCreate(&task_id, task_name, dummyFnc, &stack, stack_size, priority, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TAKEN");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 1, "semaphore taken once");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskCreate_SpawnError(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id    = 1;
    char *task_name   = "test";
    uint32 stack      = 1;
    uint32 stack_size = 1;
    uint32 priority   = 1;
    uint32 flags      = OS_FP_ENABLED;

    OsApi_Adaptor_setTaskTableEntry(0, TRUE, 0, "", 0, 0, 0, NULL);
    OsApi_SetReturnCode(OSAPI_TASKSPAWN_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_TaskCreate(&task_id, task_name, dummyFnc, &stack, stack_size, priority, flags);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
    UtAssert_True(OsApi_Adaptor_getTaskTableEntry(0)->free == TRUE,
                  "task table entry still free");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) >= 1, "semaphore taken >1");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskCreate_FpEnabled(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 taskIdIdx   = OS_MAX_TASKS - 1;
    uint32 task_id     = 1;
    char *task_name    = "test";
    uint32 stack       = 2;
    uint32 stack_size  = 3;
    uint32 priority    = 4;
    uint32 flags       = OS_FP_ENABLED;
    uint32 taskSpawnId = 21;
    uint32 creatorId   = 9;
    uint32 creatorIndex = 2;

    OsApi_Adaptor_setTaskTableEntry(taskIdIdx, TRUE, 0, "", 0, 0, 0, NULL);
    OsApi_SetReturnCode(OSAPI_TASKSPAWN_INDEX, taskSpawnId, 1);
    OsApi_SetReturnCode(OSAPI_TASKIDSELF_INDEX, creatorId, 1);
    OsApi_Adaptor_setTaskTableEntry(creatorIndex, FALSE, creatorId, "", 0, 0, 0, NULL);

    /* Execute Test */
    actual = OS_TaskCreate(&task_id, task_name, dummyFnc, &stack, stack_size, priority, flags);

    /* Verify Outputs */
    osTaskRecord_ptr = OsApi_Adaptor_getTaskTableEntry(taskIdIdx);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(getTaskSpawnFlags() == VCS_VX_FP_TASK, "taskSpawn called with expected flags");
    UtAssert_StrCmp(task_name, osTaskRecord_ptr->name, "task name == expected");
    UtAssert_True(osTaskRecord_ptr->free == FALSE, "task entry not free");
    UtAssert_True(osTaskRecord_ptr->id == taskSpawnId, "task id == expected");
    UtAssert_True(task_id == taskIdIdx, "input task id == expected");
    UtAssert_True(osTaskRecord_ptr->creator == creatorIndex, "creator ID == input");
    UtAssert_True(osTaskRecord_ptr->stack_size == stack_size, "stack size == input");
    UtAssert_True(osTaskRecord_ptr->priority == priority, "priority == input");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) >= 1, "semaphore taken >1");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskCreate_FpNotEnabled(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 taskIdIdx   = 7;
    uint32 task_id     = 5;
    char *task_name    = "test";
    uint32 stack       = 4;
    uint32 stack_size  = 3;
    uint32 priority    = 2;
    uint32 flags       = 0;
    uint32 taskSpawnId = 101;
    uint32 creatorId   = 15;
    uint32 creatorIndex = 5;

    OsApi_Adaptor_setTaskTableEntry(taskIdIdx, TRUE, 0, "", 0, 0, 0, NULL);
    OsApi_SetReturnCode(OSAPI_TASKSPAWN_INDEX, taskSpawnId, 1);
    OsApi_SetReturnCode(OSAPI_TASKIDSELF_INDEX, creatorId, 1);
    OsApi_Adaptor_setTaskTableEntry(creatorIndex, FALSE, creatorId, "", 0, 0, 0, NULL);

    /* Execute Test */
    actual = OS_TaskCreate(&task_id, task_name, dummyFnc, &stack, stack_size, priority, flags);

    /* Verify Outputs */
    osTaskRecord_ptr = OsApi_Adaptor_getTaskTableEntry(taskIdIdx);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(getTaskSpawnFlags() == 0, "taskSpawn called with expected flags");
    UtAssert_StrCmp(task_name, osTaskRecord_ptr->name, "task name == expected");
    UtAssert_True(osTaskRecord_ptr->free == FALSE, "task entry not free");
    UtAssert_True(osTaskRecord_ptr->id == taskSpawnId, "task id == expected");
    UtAssert_True(task_id == taskIdIdx, "input task id == expected");
    UtAssert_True(osTaskRecord_ptr->creator == creatorIndex, "creator ID == input");
    UtAssert_True(osTaskRecord_ptr->stack_size == stack_size, "stack size == input");
    UtAssert_True(osTaskRecord_ptr->priority == priority, "priority == input");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) >= 1, "semaphore taken >1");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_TaskDelete Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TaskDelete_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = OS_MAX_TASKS;

    /* Execute Test */
    actual = OS_TaskDelete(task_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 1, "semaphore taken once");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskDelete_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = OS_MAX_TASKS - 1;
    OsApi_Adaptor_setTaskTableEntry(task_id, TRUE, 1, "testTask", 2, 3, 4, deleteFnc);

    /* Execute Test */
    actual = OS_TaskDelete(task_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 1, "semaphore taken once");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskDelete_DelHookCall(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = 8;
    OsApi_Adaptor_setTaskTableEntry(task_id, FALSE, 1, "testTask", 2, 3, 4, deleteFnc);

    deleteFncCalled = FALSE;
    OsApi_SetReturnCode(OSAPI_TASKDELETE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_TaskDelete(task_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(deleteFncCalled == TRUE, "delete function hook called");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 1, "semaphore taken once");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskDelete_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = 13;

    OsApi_SetReturnCode(OSAPI_TASKDELETE_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_TaskDelete(task_id);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) >= 1, "semaphore taken >1");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskDelete_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = 6;
    OsApi_Adaptor_setTaskTableEntry(task_id, FALSE, 1, "testTask", 2, 3, 4, deleteFnc);

    OsApi_SetReturnCode(OSAPI_TASKDELETE_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_TaskDelete(task_id);

    /* Verify Outputs */
    osTaskRecord_ptr = OsApi_Adaptor_getTaskTableEntry(task_id);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_StrCmp("", osTaskRecord_ptr->name, "task name == expected");
    UtAssert_True(osTaskRecord_ptr->free == TRUE, "task entry is free");
    UtAssert_True(osTaskRecord_ptr->id == UNINITIALIZED, "task id == expected");
    UtAssert_True(osTaskRecord_ptr->creator == UNINITIALIZED, "creator ID == input");
    UtAssert_True(osTaskRecord_ptr->stack_size == UNINITIALIZED, "stack size == input");
    UtAssert_True(osTaskRecord_ptr->priority == UNINITIALIZED, "priority == input");
    UtAssert_True(osTaskRecord_ptr->delete_hook_pointer == NULL, "priority == input");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) >= 1, "semaphore taken >1");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_TaskExit Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TaskExit_Success(void)
{
    int32 expected = TRUE;

    /* Setup Inputs */
    uint32 task_id = 2;
    OsApi_Adaptor_setOsTaskKey(task_id);
    OsApi_Adaptor_setTaskTableEntry(task_id, FALSE, 1, "testTask", 2, 3, 4, deleteFnc);

    OsApi_SetReturnCode(OSAPI_EXIT_INDEX, 0, 1);

    /* Execute Test */
    OS_TaskExit();

    /* Verify Outputs */
    osTaskRecord_ptr = OsApi_Adaptor_getTaskTableEntry(task_id);
    UtAssert_True(getTaskExitCalled() == expected, "taskExit called");
    UtAssert_StrCmp("", osTaskRecord_ptr->name, "task name == expected");
    UtAssert_True(osTaskRecord_ptr->free == TRUE, "task entry is free");
    UtAssert_True(osTaskRecord_ptr->id == UNINITIALIZED, "task id == expected");
    UtAssert_True(osTaskRecord_ptr->creator == UNINITIALIZED, "creator ID == expected");
    UtAssert_True(osTaskRecord_ptr->stack_size == UNINITIALIZED, "stack size == expected");
    UtAssert_True(osTaskRecord_ptr->priority == UNINITIALIZED, "priority == expected");
    UtAssert_True(osTaskRecord_ptr->delete_hook_pointer == NULL, "priority == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_TaskDelay Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TaskDelay_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_SYSCLKRATEGET_INDEX, 1000000, 1);
    OsApi_SetReturnCode(OSAPI_TASKDELAY_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_TaskDelay(1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
    UtAssert_True(getTaskDelayTicks() == 1000, "sys_ticks == expected");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskDelay_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_SetReturnCode(OSAPI_TASKDELAY_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_TaskDelay(1);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  OS_TaskSetPriority Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TaskSetPriority_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id  = OS_MAX_TASKS;
    uint32 priority = 7;

    /* Execute Test */
    actual = OS_TaskSetPriority(task_id, priority);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskSetPriority_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id  = OS_MAX_TASKS - 1;
    uint32 priority = 7;
    OsApi_Adaptor_setTaskTableEntry(task_id, TRUE, 0, "", 0, 0, 0, NULL);


    /* Execute Test */
    actual = OS_TaskSetPriority(task_id, priority);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskSetPriority_PriInvalid(void)
{
    int32 expected = OS_ERR_INVALID_PRIORITY;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id  = 8;
    uint32 priority = MAX_PRIORITY + 1;

    /* Execute Test */
    actual = OS_TaskSetPriority(task_id, priority);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_PRIORITY");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskSetPriority_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id  = 8;
    uint32 priority = MAX_PRIORITY;
    OsApi_Adaptor_setTaskTableEntry(task_id, FALSE, 0, "", 0, 0, 1, NULL);

    OsApi_SetReturnCode(OSAPI_TASKPRIORITYSET_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_TaskSetPriority(task_id, priority);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
    UtAssert_True(OsApi_Adaptor_getTaskTableEntry(task_id)->priority == 1, "priority unchanged");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskSetPriority_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id  = 8;
    uint32 priority = MAX_PRIORITY;
    OsApi_Adaptor_setTaskTableEntry(task_id, FALSE, 0, "", 0, 0, 1, NULL);

    OsApi_SetReturnCode(OSAPI_TASKPRIORITYSET_INDEX, OS_SUCCESS, 1);

    /* Execute Test */
    actual = OS_TaskSetPriority(task_id, priority);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_TaskRegister Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TaskRegister_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = 4;
    OsApi_SetReturnCode(OSAPI_TASKIDSELF_INDEX, task_id, 1);

    /* Execute Test */
    actual = OS_TaskRegister();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskRegister_Error(void)
{
    int32 expected = OS_ERROR;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = 4;
    OsApi_Adaptor_setOsTaskKey(77);
    OsApi_Adaptor_setTaskTableEntry(1, FALSE, task_id, "", 0, 0, 1, NULL);
    OsApi_SetReturnCode(OSAPI_TASKIDSELF_INDEX, task_id, 1);
    OsApi_SetReturnCode(OSAPI_TASKVARADD_INDEX, VCS_ERROR, 1);

    /* Execute Test */
    actual = OS_TaskRegister();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERROR");
    UtAssert_True(OsApi_Adaptor_getOsTaskKey() == 77, "OS_task_key unchanged");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskRegister_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 tableIdx = 1;
    uint32 task_id = 4;
    OsApi_Adaptor_setOsTaskKey(77);
    OsApi_Adaptor_setTaskTableEntry(tableIdx, FALSE, task_id, "", 0, 0, 1, NULL);
    OsApi_SetReturnCode(OSAPI_TASKIDSELF_INDEX, task_id, 1);
    OsApi_SetReturnCode(OSAPI_TASKVARADD_INDEX, VCS_OK, 1);

    /* Execute Test */
    actual = OS_TaskRegister();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(OsApi_Adaptor_getOsTaskKey() == tableIdx, "OS_task_key == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_TaskGetId Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TaskGetId(void)
{
    int32 expected = 10;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setOsTaskKey(expected);

    /* Execute Test */
    actual = OS_TaskGetId();

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == expected");
}


/*******************************************************************************
**
**  OS_TaskGetIdByName Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TaskGetIdByName_NullId(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 *task_id = NULL;
    const char *task_name = "testTaskName";

    /* Execute Test */
    actual = OS_TaskGetIdByName(task_id, task_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 0, "No OS_TASK_TABLE_SEM taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskGetIdByName_NullName(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = 1;
    const char *task_name = NULL;

    /* Execute Test */
    actual = OS_TaskGetIdByName(&task_id, task_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 0, "No OS_TASK_TABLE_SEM taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskGetIdByName_NameLong(void)
{
    int32 expected = OS_ERR_NAME_TOO_LONG;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = 1;
    const char *task_name = "testTaskName";

    OsApi_SetReturnCode(OSAPI_STRLEN_INDEX, OS_MAX_API_NAME, 1);

    /* Execute Test */
    actual = OS_TaskGetIdByName(&task_id, task_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_TOO_LONG");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 0, "No OS_TASK_TABLE_SEM taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskGetIdByName_NameErr(void)
{
    int32 expected = OS_ERR_NAME_NOT_FOUND;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = 1;
    const char *task_name = "testTaskName";

    /* Execute Test */
    actual = OS_TaskGetIdByName(&task_id, task_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_NAME_NOT_FOUND");
    UtAssert_True(task_id == 1, "task_id unchanged");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskGetIdByName_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id_idx = 5;
    uint32 task_id = 1;
    const char *task_name = "testTaskName";
    OsApi_Adaptor_setTaskTableEntry(0, FALSE, 0, "", 0, 0, 0, NULL);
    OsApi_Adaptor_setTaskTableEntry(0, TRUE, 0, "", 0, 0, 0, NULL); /* for full MCDC coverage */
    OsApi_Adaptor_setTaskTableEntry(task_id_idx, FALSE, 0, (char *)task_name, 0, 0, 0, NULL);

    /* Execute Test */
    actual = OS_TaskGetIdByName(&task_id, task_name);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(task_id == task_id_idx, "task_id updated");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_TaskGetInfo Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TaskGetInfo_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = OS_MAX_TASKS;
    static OS_task_prop_t task_prop;

    /* Execute Test */
    actual = OS_TaskGetInfo(task_id, &task_prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskGetInfo_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = OS_MAX_TASKS - 1;
    static OS_task_prop_t task_prop;
    OsApi_Adaptor_setTaskTableEntry(task_id, TRUE, 0, "", 0, 0, 0, NULL);

    /* Execute Test */
    actual = OS_TaskGetInfo(task_id, &task_prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskGetInfo_NullProp(void)
{
    int32 expected = OS_INVALID_POINTER;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = 1;
    OS_task_prop_t *task_prop = NULL;

    /* Execute Test */
    actual = OS_TaskGetInfo(task_id, task_prop);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_INVALID_POINTER");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskGetInfo_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 task_id = 1;
    static OS_task_prop_t task_prop;
    OsApi_Adaptor_setTaskTableEntry(task_id, FALSE, 1, "testTask", 2, 3, 4, deleteFnc);

    /* Execute Test */
    actual = OS_TaskGetInfo(task_id, &task_prop);

    /* Verify Outputs */
    osTaskRecord_ptr = OsApi_Adaptor_getTaskTableEntry(task_id);
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(task_prop.creator == osTaskRecord_ptr->creator, "creator == expected");
    UtAssert_True(task_prop.stack_size == osTaskRecord_ptr->stack_size, "stack_size == expected");
    UtAssert_True(task_prop.priority == osTaskRecord_ptr->priority, "priority == expected");
    UtAssert_True(task_prop.OStask_id == osTaskRecord_ptr->id, "OStask_id == expected");
    UtAssert_StrCmp(task_prop.name, osTaskRecord_ptr->name, "name == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/*******************************************************************************
**
**  OS_TaskInstallDeleteHandler Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TaskInstallDeleteHandler_IdInvalid(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setOsTaskKey(OS_MAX_TASKS);

    /* Execute Test */
    actual = OS_TaskInstallDeleteHandler(dummyFnc);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == 0, "No semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskInstallDeleteHandler_IdIsFree(void)
{
    int32 expected = OS_ERR_INVALID_ID;
    int32 actual   = 99;

    /* Setup Inputs */
    OsApi_Adaptor_setOsTaskKey(OS_MAX_TASKS - 1);
    OsApi_Adaptor_setTaskTableEntry(OS_MAX_TASKS - 1, TRUE, 0, "", 0, 0, 0, NULL);

    /* Execute Test */
    actual = OS_TaskInstallDeleteHandler(dummyFnc);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_ERR_INVALID_ID");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TaskInstallDeleteHandler_Success(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual   = 99;

    /* Setup Inputs */
    uint32 idx = 1;
    OsApi_Adaptor_setOsTaskKey(idx);

    /* Execute Test */
    actual = OS_TaskInstallDeleteHandler(dummyFnc);

    /* Verify Outputs */
    UtAssert_True(actual == expected, "actual == OS_SUCCESS");
    UtAssert_True(OsApi_Adaptor_getTaskTableEntry(idx)->delete_hook_pointer == dummyFnc,
                  "delete_hook_pointer == expected");

    /* Verify Semaphore Usage */
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) > 0, "semaphore taken");
    UtAssert_True(getNSemTake(OS_TASK_TABLE_SEM) == getNSemGive(OS_TASK_TABLE_SEM),
                  "nSemTake == nSemGive");
}


/* ------------------- End of test cases --------------------------------------*/


/* Osapi_Task_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Task_Setup(void)
{
    Osapi_Setup();

}



/* Osapi_AddTestCase_Tasks
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void Osapi_AddTestCase_Tasks(void)
{
    /* OS_API_Init Tests */
    ADD_TEST(Test_OS_API_Init_ModTblInit_Error,        Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_API_Init_TimerApiInit_Error,      Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_API_Init_TaskTableSem_Error,      Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_API_Init_QueueTableSem_Error,     Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_API_Init_BinTableSem_Error,       Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_API_Init_CountTableSem_Error,     Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_API_Init_MutTableSem_Error,       Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_API_Init_OsFsInit_Error,          Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_API_Init_TaskSpawn_Error,         Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_API_Init_MsgQCreate_Error,        Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_API_Init_Success,                 Osapi_Task_Setup, Osapi_TearDown);

    /* OS_TaskCreate Tests */
    ADD_TEST(Test_OS_TaskCreate_NullName,              Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskCreate_NullFnc,               Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskCreate_NullId,                Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskCreate_NameTooLong,           Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskCreate_PriorityError,         Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskCreate_NoFreeIds,             Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskCreate_NameTaken,             Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskCreate_SpawnError,            Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskCreate_FpEnabled,             Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskCreate_FpNotEnabled,          Osapi_Task_Setup, Osapi_TearDown);

    /* OS_TaskDelete Tests */
    ADD_TEST(Test_OS_TaskDelete_IdInvalid,             Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskDelete_IdIsFree,              Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskDelete_DelHookCall,           Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskDelete_Error,                 Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskDelete_Success,               Osapi_Task_Setup, Osapi_TearDown);

    /* OS_TaskExit Tests */
    ADD_TEST(Test_OS_TaskExit_Success,                 Osapi_Task_Setup, Osapi_TearDown);

    /* OS_TaskDelay Tests */
    ADD_TEST(Test_OS_TaskDelay_Error,                  Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskDelay_Success,                Osapi_Task_Setup, Osapi_TearDown);

    /* OS_TaskSetPriority Tests */
    ADD_TEST(Test_OS_TaskSetPriority_IdInvalid,        Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskSetPriority_IdIsFree,         Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskSetPriority_PriInvalid,       Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskSetPriority_Error,            Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskSetPriority_Success,          Osapi_Task_Setup, Osapi_TearDown);

    /* OS_TaskRegister Tests */
    ADD_TEST(Test_OS_TaskRegister_IdInvalid,           Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskRegister_Error,               Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskRegister_Success,             Osapi_Task_Setup, Osapi_TearDown);

    /* OS_TaskGetId Tests */
    ADD_TEST(Test_OS_TaskGetId,                        Osapi_Task_Setup, Osapi_TearDown);

    /* OS_TaskGetIdByName Tests */
    ADD_TEST(Test_OS_TaskGetIdByName_NullId,           Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskGetIdByName_NullName,         Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskGetIdByName_NameLong,         Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskGetIdByName_NameErr,          Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskGetIdByName_Success,          Osapi_Task_Setup, Osapi_TearDown);

    /* OS_TaskGetInfo Tests */
    ADD_TEST(Test_OS_TaskGetInfo_IdInvalid,            Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskGetInfo_IdIsFree,             Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskGetInfo_NullProp,             Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskGetInfo_Success,              Osapi_Task_Setup, Osapi_TearDown);

    /* OS_TaskInstallDeleteHandler Tests */
    ADD_TEST(Test_OS_TaskInstallDeleteHandler_IdInvalid,Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskInstallDeleteHandler_IdIsFree, Osapi_Task_Setup, Osapi_TearDown);
    ADD_TEST(Test_OS_TaskInstallDeleteHandler_Success,  Osapi_Task_Setup, Osapi_TearDown);
}




