/*================================================================================*
** File:  ut_oscore_task_test.c
** Owner: Alan Cudmore
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_task_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_TASK_STACK_SIZE  0x2000
#define UT_TASK_PRIORITY       111

/* This is not global in the OSAL */
#define MAX_PRIORITY  255    

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern UT_OsLogInfo_t g_logInfo;

extern int32 g_skipTestCase;
extern char* g_skipTestCaseResult;

extern char* g_task_names[UT_OS_TASK_LIST_LEN];
extern char  g_long_task_name[OS_MAX_API_NAME+5];

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

uint32 g_task_result = 0;
uint32 g_task_sync_sem = 0;
uint32 g_task_ids[UT_OS_TASK_LIST_LEN];
uint32 g_task_stacks[UT_OS_TASK_LIST_LEN][UT_TASK_STACK_SIZE];

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/* Test code template for testing a single OSAL API with multiple test cases */

#if 0
void UT_os_sample_test()
{
    /* Must declare these variables for each function. They can be renamed.
     * They're referenced in the macros used to track test cases and their results. */
    int32 idx = 0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc = NULL;

    /* Call this once at the beginning of the function to initialize the test variables. */
    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*
     * For each test case,
     *   1. Assign testDesc a brief description of the test
     *   2. Setup the test environment, if necessary
     *   3. Run the test
     *   4. Log result by calling UT_OS_SET_TEST_RESULT_MACRO
     *   4. Reset the test environment, if neccessary
     *
     * NOTE: "API not implemented" is always checked first but not
     *       being included as a test case.
     *       "Nominal" test case is always the last test case.
     *-----------------------------------------------------*/

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    /* TODO: Setup the test environment, if necessary */

    if (OS_xxx() == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_sample_test_exit_tag;
    }

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(NULL,...) == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#2 Name-too-long-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(aVeryLoooooongName) == OS_ERR_NAME_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(...) != OS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)

    /* TODO: Reset the test environment here, if necessary */

UT_os_sample_test_exit_tag:
    /* Call these macros at the very end of the function to close out the test variables
     * and get it added to the global list being tracked. */
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_xxx", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}
#endif

/*--------------------------------------------------------------------------------*/

void generic_test_task(void)
{
    int32 res=0, task_id=0;
    OS_task_prop_t task_prop;
    char text[UT_OS_MD_TEXT_LEN];

    OS_TaskRegister();

    task_id = OS_TaskGetId();
    res = OS_TaskGetInfo(task_id, &task_prop);
   
    memset(text, '\0', sizeof(text));
    UT_os_sprintf(text, "Starting GenericTask: %s, id: %d\n", task_prop.name, task_id);
    UT_OS_LOG_MACRO(text)

    while (1)
    {
        OS_TaskDelay(1000);
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskCreate
** Purpose: Creates a task
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_INVALID_PRIORITY if the priority passed in is bad
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name passed in has already been used
**          OS_ERR_NO_FREE_IDS if there no more task can be created
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_task_create_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 i=0, res=0, idx=0;
    const char* testDesc=NULL;
    char task_name[UT_OS_SM_TEXT_LEN];

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskCreate(&g_task_ids[0], g_task_names[0], generic_test_task, g_task_stacks[0],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_task_create_test_exit_tag;
    }

    /* Delay to let child task run */
    OS_TaskDelay(200);

    /* Reset test environment */
    res = OS_TaskDelete(g_task_ids[0]);

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg-1";

    res = OS_TaskCreate(NULL, g_task_names[1], generic_test_task, g_task_stacks[1],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Null-pointer-arg-2";

    res = OS_TaskCreate(&g_task_ids[2], NULL, generic_test_task, g_task_stacks[2],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Null-pointer-arg-3";

    res = OS_TaskCreate(&g_task_ids[3], g_task_names[3], NULL, g_task_stacks[3],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-too-long";

    res = OS_TaskCreate(&g_task_ids[4], g_long_task_name, generic_test_task, g_task_stacks[4],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res == OS_ERR_NAME_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#5 Invalid-priority";

    res = OS_TaskCreate(&g_task_ids[5], g_task_names[5], generic_test_task, g_task_stacks[5],
                        UT_TASK_STACK_SIZE, MAX_PRIORITY+1, 0);
    if (res == OS_ERR_INVALID_PRIORITY)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#6 No-free-IDs";

    if (g_skipTestCase == 6)
    {
        testDesc = "#6 No-free-IDs - Test case not applicable on platform";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, g_skipTestCaseResult)
    }
    else
    {
        /* Setup */
        for (i=0; i <= OS_MAX_TASKS; i++)
        {
            memset(task_name, '\0', sizeof(task_name));
            UT_os_sprintf(task_name, "CREATE_TASK%d", (int)i);
            res = OS_TaskCreate(&g_task_ids[i], task_name, generic_test_task, g_task_stacks[i],
                                UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
            if (res != OS_SUCCESS)
            {
                break;
            }
        }

        if ((res == OS_ERR_NO_FREE_IDS) && (i == OS_MAX_TASKS))
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        /* Delay to let child tasks run */
         OS_TaskDelay(500);

        /* Reset test environment */
        for (i=0; i < OS_MAX_TASKS; i++)
        {
           res = OS_TaskDelete(g_task_ids[i]);   /* Ignore errors, does not matter here */
        }
    }

    /*-----------------------------------------------------*/
    testDesc = "#7 Duplicate-name";

    /* Setup */
    res = OS_TaskCreate(&g_task_ids[7], g_task_names[7], generic_test_task, g_task_stacks[7],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#7 Duplicate-name - Task-Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_TaskCreate(&g_task_ids[8], g_task_names[7], generic_test_task, g_task_stacks[8],
                            UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
        if (res == OS_ERR_NAME_TAKEN)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        /* Delay to let child task run */
        OS_TaskDelay(200);

        /* Reset test environment */
        res = OS_TaskDelete(g_task_ids[7]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#8 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#9 Nominal";

    res = OS_TaskCreate(&g_task_ids[9], g_task_names[9], generic_test_task, g_task_stacks[9],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res == OS_SUCCESS)
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Delay to let child task run */
    OS_TaskDelay(200);

    /* Reset test environment */
    res = OS_TaskDelete(g_task_ids[9]);

UT_os_task_create_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TaskCreate", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskDelete
** Purpose: Deletes a task
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid task id
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_task_delete_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskDelete(99999);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_task_delete_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_TaskDelete(99999);
    if ( res == OS_ERR_INVALID_ID )
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_TaskCreate(&g_task_ids[3], g_task_names[3], generic_test_task, g_task_stacks[3],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#3 Nominal - Task-Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        /* Delay to let child task run */
        OS_TaskDelay(200);

        res = OS_TaskDelete(g_task_ids[3]);
        if (res == OS_SUCCESS)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

UT_os_task_delete_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TaskDelete", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskInstallDeleteHandler
** Purpose: Installs a callback function that is called when the task is deleted
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid task id
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void delete_handler_callback(void)
{
    UT_OS_LOG_MACRO("Task delete callback...\n")
}

/*--------------------------------------------------------------------------------*/

void delete_handler_test_task(void)
{
    int32 res=0, task_id=0;
    OS_task_prop_t task_prop;
    char text[UT_OS_MD_TEXT_LEN];

    OS_TaskRegister();

    task_id = OS_TaskGetId();
    res = OS_TaskGetInfo(task_id, &task_prop);

    memset(text, '\0', sizeof(text));
    UT_os_sprintf(text, "Starting DeleteTest Task: %s, id: %d\n", task_prop.name, task_id);
    UT_OS_LOG_MACRO(text)

    g_task_result = OS_TaskInstallDeleteHandler((void *)&delete_handler_callback);

    /* 
    ** Release the semaphore so the main function can record the results of the test
    ** and clean up
    */
    res = OS_BinSemGive(g_task_sync_sem);

    for (;;)
    {
        OS_TaskDelay(1000);
    }
}

/*--------------------------------------------------------------------------------*/

void UT_os_task_install_delete_handler_test(void)
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskInstallDeleteHandler((void *)&delete_handler_callback);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_task_install_delete_handler_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    if (g_skipTestCase == 1)
    {
        testDesc = "#1 Invalid-ID-arg - Test case not applicable on platform";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, g_skipTestCaseResult)
    }
    else
    {
        /*
        ** This test works because it is being called from the main task
        **  which should not be an official OSAL task
        */
        res = OS_TaskInstallDeleteHandler((void *)&delete_handler_callback);
        if ( res == OS_ERR_INVALID_ID )
           UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
           UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    /* Setup */
    res = OS_BinSemCreate(&g_task_sync_sem, "TaskSync", 1, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#2 Nominal - Bin-Sem-Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_BinSemTake(g_task_sync_sem);

        res = OS_TaskCreate(&g_task_ids[2], g_task_names[2], delete_handler_test_task, g_task_stacks[2],
                            UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
        if (res != OS_SUCCESS)
        {
            testDesc = "#2 Nominal - Task-Create-failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        }
        else
        {
            /* Wait for the task to finish the test */
            res = OS_BinSemTake(g_task_sync_sem);
            /* Delay to let child task run */
            OS_TaskDelay(500);

            res = OS_TaskDelete(g_task_ids[2]);

            if (g_task_result == OS_SUCCESS)
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
            else
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        }

         res = OS_BinSemDelete(g_task_sync_sem);
    }

UT_os_task_install_delete_handler_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TaskInstallDeleteHandler", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskExit
** Purpose: Allows a task to delete itself
** Parameters: To-be-filled-in
** Returns: Nothing
**--------------------------------------------------------------------------------*/
void exit_test_task(void)
{
    int32 res=0, task_id=0;
    OS_task_prop_t task_prop;
    char text[UT_OS_MD_TEXT_LEN];

    OS_TaskRegister();

    task_id = OS_TaskGetId();
    res = OS_TaskGetInfo(task_id, &task_prop);

    memset(text, '\0', sizeof(text));
    UT_os_sprintf(text, "Starting ExitTest Task: %s, id: %d\n", task_prop.name, task_id);
    UT_OS_LOG_MACRO(text)

    /*
    ** The parent task will check to see if this task is valid.
    ** As long as it has not successfully called OS_TaskExit, it should
    ** be valid.
    */
    while (1)
    {
        res = OS_BinSemGive(g_task_sync_sem);

        OS_TaskExit();
    }
}

/*--------------------------------------------------------------------------------*/

void UT_os_task_exit_test(void)
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    OS_task_prop_t task_prop;
    const char* testDesc = NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "#1 Nominal";

    /* Setup */
    res = OS_BinSemCreate(&g_task_sync_sem, "TaskSync", 1, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#1 Nominal - Bin-Sem-Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_BinSemTake(g_task_sync_sem);

        res = OS_TaskCreate(&g_task_ids[1], g_task_names[1], exit_test_task, g_task_stacks[1],
                            UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
        if (res != OS_SUCCESS)
        {
            testDesc = "#1 Nominal - Task-Create failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        }
        else
        {
            /* Wait for the task to finish the test */
            res = OS_BinSemTake(g_task_sync_sem);
            /* Delay to let the child task run */
            OS_TaskDelay(500);

            /*
            ** The only real way to tell if TaskExit ran is to check to see if the
            ** task ID is valid. It should not be valid
            */
            res = OS_TaskGetInfo(g_task_ids[1], &task_prop);
            if (res == OS_ERR_INVALID_ID)
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
            else
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

            res = OS_TaskDelete(g_task_ids[1]);  /* Won't hurt if its already deleted */
        }

        res = OS_BinSemDelete(g_task_sync_sem);
    }

    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TaskExit", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskDelay
** Purpose: Suspends a given task for the given period of milliseconds
** Parameters: To-be-filled-in
** Returns: OS_ERROR if sleep failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_task_delay_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskDelay(100);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_task_delay_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    res = OS_TaskDelay(100);
    if (res == OS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

UT_os_task_delay_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TaskDelay", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskSetPriority
** Purpose: Sets task priority for the given task id
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid task id
**          OS_ERR_INVALID_PRIORITY if the priority passed in is greater than the max allowed
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_task_set_priority_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskCreate(&g_task_ids[0], g_task_names[0], generic_test_task, g_task_stacks[0],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#0 API not implemented - Task-Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_task_set_priority_test_exit_tag;
    }
    else
    {
        res = OS_TaskSetPriority(g_task_ids[0], UT_TASK_PRIORITY);
        if (res == OS_ERR_NOT_IMPLEMENTED)
        {
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
            goto UT_os_task_set_priority_test_exit_tag;
        }

        /* Delay to let child task run */
        OS_TaskDelay(500);

        /* Clean up */
        res = OS_TaskDelete(g_task_ids[0]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_TaskSetPriority(99999, 100);
    if (res == OS_ERR_INVALID_ID)
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-priority";

    res = OS_TaskCreate(&g_task_ids[2], g_task_names[2], generic_test_task, g_task_stacks[2],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#2 Invalid-priority - Task-Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_TaskSetPriority(g_task_ids[2], MAX_PRIORITY+1);
        if (res == OS_ERR_INVALID_PRIORITY)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    
        /* Delay to let child task run */
        OS_TaskDelay(500);

        /* Reset test environment */
        res = OS_TaskDelete(g_task_ids[2]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    res = OS_TaskCreate(&g_task_ids[4], g_task_names[4], generic_test_task, g_task_stacks[4],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#4 Nominal - Task-Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_TaskSetPriority(g_task_ids[4], UT_TASK_PRIORITY-10);
        if (res == OS_SUCCESS)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    
        /* Delay to let child task run */
        OS_TaskDelay(500);

        /* Reset test environment */
        res = OS_TaskSetPriority(g_task_ids[4], UT_TASK_PRIORITY);
        res = OS_TaskDelete(g_task_ids[4]);
    }

UT_os_task_set_priority_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TaskSetPriority", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskRegister
** Purpose: Registers the task, performs application- and OS-specific inits
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid task id
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void register_test_task(void)
{
    int32 res=0, task_id=0;
    OS_task_prop_t task_prop;
    char text[UT_OS_MD_TEXT_LEN];

    g_task_result = OS_TaskRegister();

    task_id = OS_TaskGetId();
    res = OS_TaskGetInfo(task_id, &task_prop);

    memset(text, '\0', sizeof(text));
    UT_os_sprintf(text, "Starting RegisterTest Task: %s\n", task_prop.name);
    UT_OS_LOG_MACRO(text)

    /* 
    ** Release the semaphore so the main function can record the results of the test
    ** and clean up
    */
    res = OS_BinSemGive(g_task_sync_sem);

    for (;;)
    {
        OS_TaskDelay(1000);
    }
}

/*--------------------------------------------------------------------------------*/

void UT_os_task_register_test(void)
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskRegister();
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_task_register_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    if (g_skipTestCase == 1)
    {
        testDesc = "#1 Invalid-ID-arg - Test case not applicable on platform";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, g_skipTestCaseResult)
    }
    else
    {
        res = OS_TaskRegister();
        if (res == OS_ERR_INVALID_ID)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_BinSemCreate(&g_task_sync_sem, "TaskSync", 1, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#3 Nominal - Bin-Sem-Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_BinSemTake(g_task_sync_sem);

        res = OS_TaskCreate(&g_task_ids[3], g_task_names[3], register_test_task, g_task_stacks[3],
                            UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#3 Nominal - Task-Create failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        }
        else
        {
            /* Wait for the task to finish the test */
            res = OS_BinSemTake(g_task_sync_sem);
            /* Delay to let child task run */
            OS_TaskDelay(500);

            res = OS_TaskDelete(g_task_ids[3]);
            res = OS_BinSemDelete(g_task_sync_sem);

            if (g_task_result == OS_SUCCESS)
               UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
           else
               UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        }
    }

UT_os_task_register_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TaskRegister", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*/

void getid_test_task(void)
{
    int32 res=0, task_id=0;
    OS_task_prop_t task_prop;
    char text[UT_OS_LG_TEXT_LEN];

    OS_TaskRegister();

    task_id = OS_TaskGetId();
    res = OS_TaskGetInfo(task_id, &task_prop);

    memset(text, '\0', sizeof(text));
    UT_os_sprintf(text, "OS_TaskGetId() - #1 Nominal [This is the returned task Id=%d]\n",
    		            task_id);
    UT_OS_LOG_MACRO(text)

    while (1)
    {
        OS_TaskDelay(1000);
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskGetId
** Purpose: Returns the task id for the task that calls this routine
** Parameters: To-be-filled-in
** Returns: Task id of the task that calls this routine
**--------------------------------------------------------------------------------*/
void UT_os_task_get_id_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;
    char text[UT_OS_MD_TEXT_LEN];

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskGetId();
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_task_get_id_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Nominal";

    /* Setup */
    res = OS_TaskCreate(&g_task_ids[1], g_task_names[1], getid_test_task, g_task_stacks[1],
    		UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res != OS_SUCCESS)
    {
    	testDesc = "#1 Nominal - Task-Create failed";
    	UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
    	OS_TaskDelay(500);

    	memset(text, '\0', sizeof(text));
    	UT_os_sprintf(text, "OS_TaskGetId() - #1 Nominal [This is the expected task Id=%d]\n",
    			            g_task_ids[1]);
    	UT_OS_LOG_MACRO(text)

    	res = OS_TaskDelete(g_task_ids[1]);  /* Won't hurt if its already deleted */

    	UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, "#1 Nominal - Manual inspection required", UT_OS_MIR)
    }

UT_os_task_get_id_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TaskGetId", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskGetIdByName
** Purpose: Returns the task id of the given task name
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the name passed in is not in the task table
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_task_get_id_by_name_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskGetIdByName(0, "InvalidName");
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_task_get_id_by_name_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-pointer-arg-1";

    res = OS_TaskGetIdByName(NULL, "InvalidName");
    if (res == OS_INVALID_POINTER)
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg-2";

    res = OS_TaskGetIdByName(&g_task_ids[2], NULL);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    res = OS_TaskGetIdByName(&g_task_ids[3], g_long_task_name);
    if (res == OS_ERR_NAME_TOO_LONG)
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-not-found";

    res = OS_TaskGetIdByName(&g_task_ids[4], "NameNotFound");
    if ( res == OS_ERR_NAME_NOT_FOUND )
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    /* Setup */
    res = OS_TaskCreate(&g_task_ids[5], g_task_names[5], generic_test_task, g_task_stacks[5],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#5 Nominal - Task-Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_TaskGetIdByName(&g_task_ids[6], g_task_names[5]);
        if ((res == OS_SUCCESS) && (g_task_ids[5] == g_task_ids[6]))
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        OS_TaskDelay(500); /* Delay to let task run */
        res = OS_TaskDelete(g_task_ids[5]);
    }

UT_os_task_get_id_by_name_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TaskGetIdByName", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_TaskGetInfo
** Purpose: Returns task information about the given task id
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in is not a valid task id
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_task_get_info_test()
{
    int32 res=0, idx=0;
    UT_OsApiInfo_t apiInfo;
    OS_task_prop_t task_prop;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_TaskGetInfo(99999, &task_prop);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_task_get_info_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_TaskGetInfo(99999, &task_prop);
    if (res == OS_ERR_INVALID_ID)
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
       UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg";

    /* Setup */
    res = OS_TaskCreate(&g_task_ids[2], g_task_names[2], generic_test_task, g_task_stacks[2],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#2 Invalid-pointer-arg - Task-Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_TaskGetInfo(g_task_ids[2], NULL);
        if (res == OS_INVALID_POINTER)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        /* Delay to let child task run */
        OS_TaskDelay(500);

        /* Reset test environment */
        res = OS_TaskDelete(g_task_ids[2]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_TaskCreate(&g_task_ids[3], g_task_names[3], generic_test_task, g_task_stacks[3],
                        UT_TASK_STACK_SIZE, UT_TASK_PRIORITY, 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#3 Nominal - Task-Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_TaskGetInfo(g_task_ids[3], &task_prop);
        if (res == OS_SUCCESS)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        /* Delay to let child task run */
        OS_TaskDelay(500);

        /* Reset test environment */
        res = OS_TaskDelete(g_task_ids[3]);
    }

UT_os_task_get_info_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_TaskGetInfo", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*================================================================================*
** End of File: ut_oscore_task_test.c
**================================================================================*/
