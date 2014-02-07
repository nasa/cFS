/*================================================================================*
** File:  ut_oscore_mutex_test.c
** Owner: Alan Cudmore
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_mutex_test.h"

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
** Syntax: OS_MutSemCreate
** Purpose: Creates a mutex semaphore
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name passed in has already been used
**          OS_ERR_NO_FREE_IDS if there are no more free mutex ids
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_mut_sem_create_test()
{
    int i;
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;
    uint32  mut_sem_id;
    uint32  mut_sem_id2;
    char    sem_name[OS_MAX_API_NAME];
    char    long_sem_name[OS_MAX_API_NAME+5];
    uint32  test_setup_invalid = 0;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_MutSemCreate(&mut_sem_id, "Good", 0 );
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_mut_sem_create_test_exit_tag;
    }

    /* Clean up */
    res = OS_MutSemDelete(mut_sem_id);

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg-1";

    res = OS_MutSemCreate(NULL, "MutSem1", 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Null-pointer-arg-2";

    res = OS_MutSemCreate(&mut_sem_id, NULL, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    memset(long_sem_name, 'X', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name)-1] = '\0';
    res = OS_MutSemCreate(&mut_sem_id, long_sem_name, 0);
    if (res == OS_ERR_NAME_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 No-free-IDs";

    /* Setup */
    for ( i = 0; i< OS_MAX_MUTEXES; i++ )
    {
        memset(sem_name, '\0', sizeof(sem_name));
        UT_os_sprintf(sem_name, "MUTSEM%d",i);
        res = OS_MutSemCreate(&mut_sem_id, sem_name, 0);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#4 No-free-IDs - Mutex Create failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
            test_setup_invalid = 1;
            break;
        }
    }

    if ( test_setup_invalid == 0 )
    {
        res = OS_MutSemCreate(&mut_sem_id, "OneTooMany", 0);
        if (res == OS_ERR_NO_FREE_IDS)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

    /* Reset test environment */
    for ( i = 0; i< OS_MAX_MUTEXES; i++ )
    {
        res = OS_MutSemDelete(i); /* Ignore errors, does not matter here */
    }

    /*-----------------------------------------------------*/
    testDesc = "#5 Duplicate-name";

    /* Setup */
    res = OS_MutSemCreate(&mut_sem_id2, "DUPLICATE",  0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Duplicate-name - Mutex Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_MutSemCreate(&mut_sem_id, "DUPLICATE", 0);
        if (res == OS_ERR_NAME_TAKEN)
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        /* Reset test environment */
        res = OS_MutSemDelete(mut_sem_id2);
    }

    /*-----------------------------------------------------*/
    testDesc = "#6 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#7 Nominal";

    res = OS_MutSemCreate(&mut_sem_id, "Good", 0);
    if ( res == OS_SUCCESS )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    res = OS_MutSemDelete(mut_sem_id);

UT_os_mut_sem_create_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_MutSemCreate", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_MutSemDelete
** Purpose: Deletes a mutex semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid mutex id
**          OS_ERR_SEM_NOT_FULL if the mutex is empty
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_mut_sem_delete_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;
    uint32  mut_sem_id;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_MutSemDelete(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_mut_sem_delete_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_MutSemDelete(99999);
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
    res = OS_MutSemCreate(&mut_sem_id, "DeleteTest", 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Mutex Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_MutSemDelete(mut_sem_id);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

UT_os_mut_sem_delete_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_MutSemDelete", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_MutSemGive
** Purpose: Releases a mutex semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid mutex id
**          OS_SEM_FAILURE if the mutex was not previously init'd or is not in the
**                         array of semaphores defined by the system
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_mut_sem_give_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;
    uint32  mut_sem_id;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_MutSemGive(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_mut_sem_give_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_MutSemGive(99999);
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
    res = OS_MutSemCreate(&mut_sem_id, "GiveTest", 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Mutex Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_MutSemTake(mut_sem_id);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#3 Nominal - Mutex Take failed";
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        }
        else
        {
            res = OS_MutSemGive(mut_sem_id);
            if ( res == OS_SUCCESS )
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
            else
                UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

            res = OS_MutSemDelete(mut_sem_id);
        }
    }

UT_os_mut_sem_give_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_MutSemGive", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_MutSemTake
** Purpose: Allocates a mutex semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid mutex id
**          OS_SEM_FAILURE if the mutex was not previously init'd or is not in the
**                         array of semaphores defined by the system
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_mut_sem_take_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;
    uint32  mut_sem_id;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_MutSemTake(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_mut_sem_take_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_MutSemTake(99999);
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
    res = OS_MutSemCreate(&mut_sem_id, "TakeTest", 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Mutex Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_MutSemTake(mut_sem_id);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_MutSemGive(mut_sem_id);
        res = OS_MutSemDelete(mut_sem_id);
    }

UT_os_mut_sem_take_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_MutSemTake", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_MutSemGetIdByName
** Purpose: Returns the id of a given mutex semaphore name
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the name was not found in the mutex semaphore table
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_mut_sem_get_id_by_name_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;
    uint32  mut_sem_id;
    char        long_sem_name[OS_MAX_API_NAME+5];

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_MutSemGetIdByName(0,"InvalidName");
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_mut_sem_get_id_by_name_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-pointer-arg-1";

    res = OS_MutSemGetIdByName(NULL, "InvalidName");
    if ( res == OS_INVALID_POINTER )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg-2";

    res = OS_MutSemGetIdByName(&mut_sem_id, NULL);
    if ( res == OS_INVALID_POINTER )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    memset(long_sem_name, 'Y', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name)-1] = '\0';
    res = OS_MutSemGetIdByName(&mut_sem_id, long_sem_name);
    if ( res == OS_ERR_NAME_TOO_LONG )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-not-found";

    res = OS_MutSemGetIdByName(&mut_sem_id, "NameNotFound");
    if ( res == OS_ERR_NAME_NOT_FOUND )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    /* Setup */
    res = OS_MutSemCreate(&mut_sem_id, "GetIDByName", 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Nominal - Mutex Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_MutSemGetIdByName(&mut_sem_id, "GetIDByName");
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_MutSemDelete(mut_sem_id);
    }

UT_os_mut_sem_get_id_by_name_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_MutSemGetIdByName", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_MutSemGetInfo
** Purpose: Returns mutex semaphore information about a given mutex id
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_mut_sem_get_info_test()
{
    UT_OsApiInfo_t     apiInfo;
    int32              res = 0, idx = 0;
    const char*        testDesc = NULL;
    uint32             mut_sem_id;
    OS_mut_sem_prop_t  mut_sem_prop;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_MutSemGetInfo(0, &mut_sem_prop);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_mut_sem_get_info_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_MutSemGetInfo(99999, &mut_sem_prop);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg";

    res = OS_MutSemCreate(&mut_sem_id, "InvalidPtr", 0);
    if (res != OS_SUCCESS)
    {
        testDesc = "#2 Invalid-pointer-arg - Mutex Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_MutSemGetInfo(mut_sem_id, NULL);
        if ( res == OS_INVALID_POINTER )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

          OS_MutSemDelete(mut_sem_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_MutSemCreate(&mut_sem_id, "GetInfo", 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Mutex Create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }
    else
    {
        res = OS_MutSemGetInfo(mut_sem_id, &mut_sem_prop);
        if ( res == OS_SUCCESS )
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
        else
            UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

        res = OS_MutSemDelete(mut_sem_id);
    }

UT_os_mut_sem_get_info_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_MutSemGetInfo", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*================================================================================*
** End of File: ut_oscore_mutex_test.c
**================================================================================*/
