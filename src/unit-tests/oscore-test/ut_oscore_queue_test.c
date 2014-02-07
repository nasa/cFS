/*================================================================================*
** File:  ut_oscore_queue_test.c
** Owner: Alan Cudmore
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_queue_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern UT_OsLogInfo_t g_logInfo;

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

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
     * NOTE: "Not implemented" is always checked first but not
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

/*--------------------------------------------------------------------------------*
** Syntax: OS_QueueCreate
** Purpose: Creates a queue in the OS
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name has already been used on a created queue
**          OS_ERR_NO_FREE_IDS if there are no more free queue ids
**          OS_ERROR if the OS create call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_queue_create_test()
{
    int i,j;
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;
    uint32  queue_id;
    uint32  queue_id2;
    char    queue_name[OS_MAX_API_NAME];
    char    long_queue_name[OS_MAX_API_NAME+5];
    uint32  test_setup_invalid = 0;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_QueueCreate(&queue_id, "Good", 10, 4, 0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_queue_create_test_exit_tag;
    }
    /* Clean up */
    res = OS_QueueDelete(queue_id);

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg-1";

    res = OS_QueueCreate(NULL, "Queue1", 10, 4, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Null-pointer-arg-2";

    res = OS_QueueCreate(&queue_id, NULL, 10, 4, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    memset(long_queue_name, 'X', sizeof(long_queue_name));
    long_queue_name[sizeof(long_queue_name)-1] = '\0';
    res = OS_QueueCreate(&queue_id, long_queue_name, 10, 4, 0);
    if (res == OS_ERR_NAME_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 No-free-IDs";

    /* Setup */
    for ( i = 0; i< OS_MAX_QUEUES; i++ )
    {
        memset(queue_name, '\0', sizeof(queue_name));
        UT_os_sprintf(queue_name, "QUEUE%d",i);
        res = OS_QueueCreate(&queue_id, queue_name, 2, 4, 0);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#4 No-free-IDs - Queue Create failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
            test_setup_invalid = 1;
            break;
        }
    }

    if ( test_setup_invalid == 0 )
    {
        res = OS_QueueCreate(&queue_id, "OneTooMany", 10, 4, 0);
        if (res == OS_ERR_NO_FREE_IDS)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

    /* Reset test environment */
    for ( i = 0; i< OS_MAX_QUEUES; i++ )
    {
        res = OS_QueueDelete(i); /* Ignore errors, does not matter here */
    }

    /*-----------------------------------------------------*/
    testDesc = "#5 Duplicate-name";

    /* Setup */
    res = OS_QueueCreate(&queue_id2, "DUPLICATE", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, "Queue Create failed", UT_OS_TSF)
    }
    else
    {
        res = OS_QueueCreate(&queue_id, "DUPLICATE", 10, 4, 0);
        if (res == OS_ERR_NAME_TAKEN)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        /* Reset test environment */
        res = OS_QueueDelete(queue_id2);
    }

    /*-----------------------------------------------------*/
    testDesc = "#6 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#7 Nominal";

    res = OS_QueueCreate(&queue_id, "Good", 10, 4, 0);
    if ( res == OS_SUCCESS )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    res = OS_QueueDelete(queue_id);

UT_os_queue_create_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_QueueCreate", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_QueueDelete
** Purpose: Deletes a queue in the OS
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in does not exist
**          OS_ERROR if the OS call to delete the queue failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_queue_delete_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;
    uint32  queue_id;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_QueueDelete(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_queue_delete_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_QueueDelete(99999);
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
    res = OS_QueueCreate(&queue_id, "DeleteTest", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_QueueDelete(queue_id);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

UT_os_queue_delete_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_QueueDelete", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_QueueGet
** Purpose: Retrieves a data item from an existing queue
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in does not exist
**          OS_QUEUE_EMPTY if the queue has no message on it to be received
**          OS_QUEUE_TIMEOUT if the timeout was OS_PEND and the time expired
**          OS_QUEUE_INVALID_SIZE if the size copied from the queue is incorrect
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_queue_get_test()
{
    UT_OsApiInfo_t     apiInfo;
    int32              res = 0, idx = 0;
    const char*        testDesc = NULL;
    uint32             queue_id;
    uint32             queue_data_out;
    uint32             queue_data_in;
    uint32             size_copied;
    uint32             data_size;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_QueueGet(0, (void *)&queue_data_in, 4, &size_copied, OS_CHECK);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_queue_get_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_QueueGet(99999, (void *)&queue_data_in, 4, &size_copied, OS_CHECK);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg-1";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueGet", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#2 Invalid-pointer-arg-1 - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_QueueGet(queue_id, NULL, 4, &size_copied, OS_CHECK);
        if ( res == OS_INVALID_POINTER )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 Invalid-pointer-arg-2";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueGet", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Invalid-pointer-arg-2 - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_QueueGet(queue_id, (void *)&queue_data_in, 4, NULL, OS_CHECK);
        if ( res == OS_INVALID_POINTER )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

       res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#4 Queue-empty";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueEmpty", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#4 Queue-empty - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_QueueGet(queue_id, (void *)&queue_data_in, 4, &data_size, OS_CHECK);
        if ( res == OS_QUEUE_EMPTY )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#5 Queue-timed-out";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueTimeout", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Queue-timed-out - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_QueueGet(queue_id, (void *)&queue_data_in, 4, &data_size, 2);
        if ( res == OS_QUEUE_TIMEOUT )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#6 Invalid-queue-size";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueuePut", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#6 Invalid-queue-size - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        queue_data_out = 0x11223344;
        res = OS_QueuePut(queue_id, (void *)&queue_data_out, 2, 0);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#6 Invalid-queue-size - Queue Put failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        }
        else
        {
            res = OS_QueueGet(queue_id, (void *)&queue_data_in, 3, &data_size, OS_CHECK);
            if ( res == OS_QUEUE_INVALID_SIZE )
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
            else
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        }
        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#7 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#8 Nominal";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueGet", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#8 Nominal - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        queue_data_out = 0x11223344;
        res = OS_QueuePut(queue_id, (void *)&queue_data_out, 4, 0);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#8 Nominal - Queue Put failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        }
        else
        {
            res = OS_QueueGet(queue_id, (void *)&queue_data_in, 4, &data_size, OS_PEND);
            if ( res == OS_SUCCESS )
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
            else
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        }
        res = OS_QueueDelete(queue_id);
    }

UT_os_queue_get_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_QueueGet", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_QueuePut
** Purpose: Sends data on an existing queue
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in does not exist
**          OS_QUEUE_FULL if the queue could not accept another message
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_queue_put_test()
{
    UT_OsApiInfo_t     apiInfo;
    int32              res = 0, idx = 0;
    const char*        testDesc = NULL;
    uint32             queue_id;
    uint32             queue_data_out;
    int                i;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_QueuePut(0, (void *)&queue_data_out, 4, 0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_queue_put_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_QueuePut(99999, (void *)&queue_data_out, 4, 0);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueuePut", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#2 Invalid-pointer-arg - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_QueuePut(queue_id, NULL, 4, 0);
        if ( res == OS_INVALID_POINTER )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#4 Queue-full";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueuePut", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#4 Queue-full - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        queue_data_out = 0x11223344;
        for ( i = 0; i < 100; i++ )
        {
            res = OS_QueuePut(queue_id, (void *)&queue_data_out, 4, 0);
            if ( res == OS_QUEUE_FULL )
                break;
        }

        if ( res == OS_QUEUE_FULL )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueGet", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Nominal - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        queue_data_out = 0x11223344;
        res = OS_QueuePut(queue_id, (void *)&queue_data_out, 4, 0);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        res = OS_QueueDelete(queue_id);
    }

UT_os_queue_put_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_QueuePut", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_QueueGetIdByName
** Purpose: Returns the queue id of a given queue name
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the name of id pointers are null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the name was not found in the queue table
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_queue_get_id_by_name_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;
    uint32  queue_id;
    char  long_queue_name[OS_MAX_API_NAME+5];

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_QueueGetIdByName(0, "InvalidName");
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_queue_get_id_by_name_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-pointer-arg-1";

    res = OS_QueueGetIdByName(NULL, "InvalidName");
    if ( res == OS_INVALID_POINTER )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg-2";

    res = OS_QueueGetIdByName(&queue_id, NULL);
    if ( res == OS_INVALID_POINTER )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    memset(long_queue_name, 'Y', sizeof(long_queue_name));
    long_queue_name[sizeof(long_queue_name)-1] = '\0';
    res = OS_QueueGetIdByName(&queue_id, long_queue_name);
    if ( res == OS_ERR_NAME_TOO_LONG )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-not-found";

    res = OS_QueueGetIdByName(&queue_id, "NameNotFound");
    if ( res == OS_ERR_NAME_NOT_FOUND )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "GetIDByName", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Nominal - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_QueueGetIdByName(&queue_id, "GetIDByName");
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_QueueDelete(queue_id);
    }

UT_os_queue_get_id_by_name_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_QueueGetIdByName", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_QueueGetInfo
** Purpose: Returns queue information about the given queue id
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in does not exist
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_queue_get_info_test()
{
    UT_OsApiInfo_t     apiInfo;
    int32              res = 0, idx = 0;
    const char*        testDesc = NULL;
    uint32             queue_id;
    OS_queue_prop_t    queue_prop;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_QueueGetInfo(0,&queue_prop);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_queue_get_info_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_QueueGetInfo(99999, &queue_prop);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "GetInfo", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#2 Invalid-pointer-arg - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_QueueGetInfo(queue_id, NULL);
        if ( res == OS_INVALID_POINTER )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "GetInfo", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Queue Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_QueueGetInfo(queue_id, &queue_prop);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_QueueDelete(queue_id);
    }

UT_os_queue_get_info_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_QueueGetInfo", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*================================================================================*
** End of File: ut_oscore_queue_test.c
**================================================================================*/
