/*================================================================================*
** File:  ut_oscore_countsem_test.c
** Owner: Alan Cudmore
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_countsem_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

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
    int32          res = 0;
    const char*    testDesc;
    uint32         count_sem_ids[OS_MAX_COUNT_SEMAPHORES+1];
    char           sem_name[UT_OS_NAME_BUFF_SIZE];
    char           long_sem_name[UT_OS_NAME_BUFF_SIZE];
    uint32         test_setup_invalid = 0;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemCreate(&count_sem_ids[0], "Good", 1, 0 );
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_count_sem_create_test_exit_tag;
    }

    /* Clean up */
    OS_CountSemDelete(count_sem_ids[0]);

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg-1";

    res = OS_CountSemCreate(NULL, "CountSem1", 1, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Null-pointer-arg-2";

    res = OS_CountSemCreate(&count_sem_ids[0], NULL, 1, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    memset(long_sem_name, 'X', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name)-1] = '\0';
    res = OS_CountSemCreate(&count_sem_ids[0], long_sem_name, 1, 0);
    if (res == OS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Initial-count-too-high";

    /*
     * FIXME: This test can only be done if the OS defines a specific "SEM_VALUE_MAX"
     * The OSAL should define this for itself, but it currently does not.
     *  (This macro is not currently defined in RTEMS)
     */
#ifdef SEM_VALUE_MAX
    res = OS_CountSemCreate(&count_sem_ids[0], "CountSem1", ((uint32)SEM_VALUE_MAX) + 1, 0);
    if (res == OS_INVALID_SEM_VALUE)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
#else
    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
#endif

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
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
            test_setup_invalid = 1;
            break;
        }
    }

    if ( test_setup_invalid == 0 )
    {
        res = OS_CountSemCreate(&count_sem_ids[OS_MAX_COUNT_SEMAPHORES], "OneTooMany", 1, 0);
        if (res == OS_ERR_NO_FREE_IDS)
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }

    /* Reset test environment */
    OS_DeleteAllObjects();

    /*-----------------------------------------------------*/
    testDesc = "#6 Duplicate-name";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_ids[0], "DUPLICATE", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#6 Duplicate-name - Count Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);
    }
    else
    {
        res = OS_CountSemCreate(&count_sem_ids[1], "DUPLICATE", 1, 0);
        if (res == OS_ERR_NAME_TAKEN)
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Reset test environment */
        OS_CountSemDelete(count_sem_ids[0]);
        OS_CountSemDelete(count_sem_ids[1]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#7 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#8 Nominal";

    res = OS_CountSemCreate(&count_sem_ids[0], "Good", 1, 0);
    if ( res == OS_SUCCESS )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    res = OS_CountSemDelete(count_sem_ids[0]);

UT_os_count_sem_create_test_exit_tag:
    return;
    
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
    int32          res = 0;
    const char*    testDesc;
    uint32         count_sem_id;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemDelete(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_count_sem_delete_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_CountSemDelete(99999);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_id, "DeleteTest", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Count Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_CountSemDelete(count_sem_id);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }

UT_os_count_sem_delete_test_exit_tag:
    return;
    
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
    int32          res = 0;
    const char*    testDesc;
    uint32         count_sem_id;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemGive(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_count_sem_give_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_CountSemGive(99999);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_id, "GiveTest", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Count Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_CountSemGive(count_sem_id);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_CountSemDelete(count_sem_id);
    }

UT_os_count_sem_give_test_exit_tag:
    return;
    
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
    int32          res = 0;
    const char*    testDesc;
    uint32         count_sem_id;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemTake(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_count_sem_take_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_CountSemTake(99999);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_id, "TakeTest", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Count Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_CountSemTake(count_sem_id);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        res = OS_CountSemDelete(count_sem_id);
    }

UT_os_count_sem_take_test_exit_tag:
    return;
    
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
    int32          res = 0;
    const char*    testDesc;
    uint32         count_sem_id;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemTimedWait(0,1000);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_count_sem_timed_wait_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_CountSemTimedWait(99999, 1000);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Sem-take-timed-out";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_id, "TimedWait", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Sem-take-timed-out - Count Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_CountSemTake(count_sem_id);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#3 Sem-take-timed-out - Count Sem Take failed";
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
        }
        else
        {
            res = OS_CountSemTimedWait(count_sem_id, 1000);
            if ( res == OS_SEM_TIMEOUT )
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
            else
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
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
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_CountSemTimedWait(count_sem_id, 1000);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_CountSemDelete(count_sem_id);
    }

UT_os_count_sem_timed_wait_test_exit_tag:
    return;
    
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
    int32          res = 0;
    const char*    testDesc;
    uint32         count_sem_id;
    char           long_sem_name[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemGetIdByName(0, "InvalidName");
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_count_sem_get_id_by_name_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-pointer-arg-1";

    res = OS_CountSemGetIdByName(NULL, "InvalidName");
    if ( res == OS_INVALID_POINTER )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg-2";

    res = OS_CountSemGetIdByName(&count_sem_id, NULL);
    if ( res == OS_INVALID_POINTER )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    memset(long_sem_name, 'Y', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name)-1] = '\0';
    res = OS_CountSemGetIdByName(&count_sem_id, long_sem_name);
    if ( res == OS_ERR_NAME_TOO_LONG )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-not-found";

    res = OS_CountSemGetIdByName(&count_sem_id, "NameNotFound");
    if ( res == OS_ERR_NAME_NOT_FOUND )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_id, "GetIDByName", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Nominal - Count Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_CountSemGetIdByName(&count_sem_id, "GetIDByName");
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_CountSemDelete(count_sem_id);
    }

UT_os_count_sem_get_id_by_name_test_exit_tag:
    return;
    
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
    int32              res = 0;
    const char*        testDesc;
    uint32             count_sem_id;
    OS_count_sem_prop_t  count_sem_prop;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_CountSemGetInfo(0,&count_sem_prop);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_count_sem_get_info_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_CountSemGetInfo(99999, &count_sem_prop);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_id, "GetInfo", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#2 Invalid-pointer-arg - Count Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_CountSemGetInfo(count_sem_id, NULL);
        if ( res == OS_INVALID_POINTER )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_CountSemDelete(count_sem_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_CountSemCreate(&count_sem_id, "GetInfo", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Count Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_CountSemGetInfo(count_sem_id, &count_sem_prop);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_CountSemDelete(count_sem_id);
    }

UT_os_count_sem_get_info_test_exit_tag:
    return;
    
}

/*================================================================================*
** End of File: ut_oscore_countsem_test.c
**================================================================================*/
