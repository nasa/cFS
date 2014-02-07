/*================================================================================*
** File:  ut_oscore_countsem_test.c
** Owner: Alan Cudmore
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_countsem_test.h"
#include "ut_oscore_test_platforms.h"

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
** Syntax: OS_CountSemCreate
** Purpose: Creates a counting semaphore
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name passed in has already been used
**          OS_ERR_NO_FREE_IDS if there are no more free counting semaphore ids
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_create_test()
{
    int            i;
    UT_OsApiInfo_t apiInfo;
    int32          res = 0, idx = 0;
    const char*    testDesc = NULL;
    uint32         count_sem_ids[OS_MAX_COUNT_SEMAPHORES+1];
    char           sem_name[OS_MAX_API_NAME];
    char           long_sem_name[OS_MAX_API_NAME+5];
    uint32         test_setup_invalid = 0;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemCreate(&count_sem_ids[0], "Good", 1, 0 );
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_count_sem_create_test_exit_tag;
    }

    /* Clean up */
    res = OS_CountSemDelete(count_sem_ids[0]);

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg-1";

    res = OS_CountSemCreate(NULL, "CountSem1", 1, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Null-pointer-arg-2";

    res = OS_CountSemCreate(&count_sem_ids[0], NULL, 1, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    memset(long_sem_name, 'X', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name)-1] = '\0';
    res = OS_CountSemCreate(&count_sem_ids[0], long_sem_name, 1, 0);
    if (res == OS_ERR_NAME_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 Initial-count-too-high";

    res = OS_CountSemCreate(&count_sem_ids[0], "CountSem1", SEM_VALUE_MAX + 1, 0);
    if (res == OS_INVALID_SEM_VALUE)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#5 No-free-IDs";

    /* Setup */
    for ( i = 0; i< OS_MAX_COUNT_SEMAPHORES; i++ )
    {
        memset(sem_name, '\0', sizeof(sem_name));
        UT_os_sprintf(sem_name, "COUNTSEM%d", i);
        res = OS_CountSemCreate(&count_sem_ids[i], sem_name, 1, 0);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#5 No-free-IDs - Count Sem Create failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
            test_setup_invalid = 1;
            break;
        }
    }

    if ( test_setup_invalid == 0 )
    {
        res = OS_CountSemCreate(&count_sem_ids[OS_MAX_COUNT_SEMAPHORES], "OneTooMany", 1, 0);
        if (res == OS_ERR_NO_FREE_IDS)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

    /* Reset test environment */
    for ( i = 0; i< OS_MAX_COUNT_SEMAPHORES; i++ )
    {
        res = OS_CountSemDelete(count_sem_ids[i]); /* Ignore errors, does not matter here */
    }

    /*-----------------------------------------------------*/
    testDesc = "#6 Duplicate-name";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_ids[0], "DUPLICATE", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#6 Duplicate-name - Count Sem Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)
    }
    else
    {
        res = OS_CountSemCreate(&count_sem_ids[1], "DUPLICATE", 1, 0);
        if (res == OS_ERR_NAME_TAKEN)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        /* Reset test environment */
        res = OS_CountSemDelete(count_sem_ids[0]);
        res = OS_CountSemDelete(count_sem_ids[1]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#7 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#8 Nominal";

    res = OS_CountSemCreate(&count_sem_ids[0], "Good", 1, 0);
    if ( res == OS_SUCCESS )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    res = OS_CountSemDelete(count_sem_ids[0]);

UT_os_count_sem_create_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_CountSemCreate", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_CountSemDelete
** Purpose: Deletes a counting semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid counting semaphore id
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_delete_test()
{
    UT_OsApiInfo_t apiInfo;
    int32          res = 0, idx = 0;
    const char*    testDesc = NULL;
    uint32         count_sem_id;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemDelete(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_count_sem_delete_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_CountSemDelete(99999);
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
    res = OS_CountSemCreate(&count_sem_id, "DeleteTest", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Count Sem Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_CountSemDelete(count_sem_id);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

UT_os_count_sem_delete_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_CountSemDelete", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_CountSemGive
** Purpose: Gives back a counting semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid counting semaphore id
**          OS_SEM_FAILURE if the semaphore was not previously init'd or is not in the
**                         array of semaphores defined by the system
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_give_test()
{
    UT_OsApiInfo_t apiInfo;
    int32          res = 0, idx = 0;
    const char*    testDesc = NULL;
    uint32         count_sem_id;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemGive(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_count_sem_give_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_CountSemGive(99999);
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
    res = OS_CountSemCreate(&count_sem_id, "GiveTest", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Count Sem Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_CountSemGive(count_sem_id);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_CountSemDelete(count_sem_id);
    }

UT_os_count_sem_give_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_CountSemGive", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_CountSemTake
** Purpose: Reserves a counting semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid counting semaphore id
**          OS_SEM_FAILURE if the semaphore was not previously init'd
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_take_test()
{
    UT_OsApiInfo_t apiInfo;
    int32          res = 0, idx = 0;
    const char*    testDesc = NULL;
    uint32         count_sem_id;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemTake(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_count_sem_take_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_CountSemTake(99999);
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
    res = OS_CountSemCreate(&count_sem_id, "TakeTest", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Count Sem Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_CountSemTake(count_sem_id);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        res = OS_CountSemDelete(count_sem_id);
    }

UT_os_count_sem_take_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_CountSemTake", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_CountSemTimedWait
** Purpose: Reserves a counting semaphore with a timeout
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid counting semaphore id
**          OS_SEM_FAILURE if the OS call failed
**          OS_SEM_TIMEOUT if the semaphore was not relinquished in time
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_timed_wait_test()
{
    UT_OsApiInfo_t apiInfo;
    int32          res = 0, idx = 0;
    const char*    testDesc = NULL;
    uint32         count_sem_id;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemTimedWait(0,1000);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_count_sem_timed_wait_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_CountSemTimedWait(99999, 1000);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#3 Sem-take-timed-out";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_id, "TimedWait", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Sem-take-timed-out - Count Sem Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_CountSemTake(count_sem_id);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#3 Sem-take-timed-out - Count Sem Take failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        }
        else
        {
            res = OS_CountSemTimedWait(count_sem_id, 1000);
            if ( res == OS_SEM_TIMEOUT )
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
            else
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        }
        res = OS_CountSemDelete(count_sem_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_id, "TimedWait", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#4 Nominal - Count Sem Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_CountSemTimedWait(count_sem_id, 1000);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_CountSemDelete(count_sem_id);
    }

UT_os_count_sem_timed_wait_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_CountSemTimedWait", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_CountSemGetIdByName
** Purpose: Returns the id of a given counting semaphore name
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the name passed in was not found in the counting
**                                semaphore table
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_get_id_by_name_test()
{
    UT_OsApiInfo_t apiInfo;
    int32          res = 0, idx = 0;
    const char*    testDesc = NULL;
    uint32         count_sem_id;
    char           long_sem_name[OS_MAX_API_NAME+5];

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemGetIdByName(0, "InvalidName");
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_count_sem_get_id_by_name_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-pointer-arg-1";

    res = OS_CountSemGetIdByName(NULL, "InvalidName");
    if ( res == OS_INVALID_POINTER )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg-2";

    res = OS_CountSemGetIdByName(&count_sem_id, NULL);
    if ( res == OS_INVALID_POINTER )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    memset(long_sem_name, 'Y', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name)-1] = '\0';
    res = OS_CountSemGetIdByName(&count_sem_id, long_sem_name);
    if ( res == OS_ERR_NAME_TOO_LONG )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-not-found";

    res = OS_CountSemGetIdByName(&count_sem_id, "NameNotFound");
    if ( res == OS_ERR_NAME_NOT_FOUND )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_id, "GetIDByName", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Nominal - Count Sem Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_CountSemGetIdByName(&count_sem_id, "GetIDByName");
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_CountSemDelete(count_sem_id);
    }

UT_os_count_sem_get_id_by_name_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_CountSemGetIdByName", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_CountSemGetInfo
** Purpose: Returns semaphore information about the given binary semaphore id
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_count_sem_get_info_test()
{
    UT_OsApiInfo_t     apiInfo;
    int32              res = 0, idx = 0;
    const char*        testDesc = NULL;
    uint32             count_sem_id;
    OS_count_sem_prop_t  count_sem_prop;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemGetInfo(0,&count_sem_prop);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_count_sem_get_info_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_CountSemGetInfo(99999, &count_sem_prop);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_id, "GetInfo", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#2 Invalid-pointer-arg - Count Sem Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_CountSemGetInfo(count_sem_id, NULL);
        if ( res == OS_INVALID_POINTER )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_CountSemDelete(count_sem_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_id, "GetInfo", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Count Sem Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_CountSemGetInfo(count_sem_id, &count_sem_prop);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_CountSemDelete(count_sem_id);
    }

UT_os_count_sem_get_info_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_CountSemGetInfo", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*================================================================================*
** End of File: ut_oscore_countsem_test.c
**================================================================================*/
