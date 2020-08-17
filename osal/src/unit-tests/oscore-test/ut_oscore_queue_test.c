/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

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
    int i = 0;
    int32 res = 0;
    const char* testDesc;
    uint32  queue_id = 0;
    uint32  queue_id2 = 0;
    char    queue_name[UT_OS_NAME_BUFF_SIZE];
    char    long_queue_name[UT_OS_NAME_BUFF_SIZE];
    uint32  test_setup_invalid = 0;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_QueueCreate(&queue_id, "Good", 10, 4, 0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_queue_create_test_exit_tag;
    }
    /* Clean up */
    OS_QueueDelete(queue_id);

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg-1";

    res = OS_QueueCreate(NULL, "Queue1", 10, 4, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Null-pointer-arg-2";

    res = OS_QueueCreate(&queue_id, NULL, 10, 4, 0);
    if (res == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    memset(long_queue_name, 'X', sizeof(long_queue_name));
    long_queue_name[sizeof(long_queue_name)-1] = '\0';
    res = OS_QueueCreate(&queue_id, long_queue_name, 10, 4, 0);
    if (res == OS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

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
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
            test_setup_invalid = 1;
            break;
        }
    }

    if ( test_setup_invalid == 0 )
    {
        res = OS_QueueCreate(&queue_id, "OneTooMany", 10, 4, 0);
        if (res == OS_ERR_NO_FREE_IDS)
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }

    /* Reset test environment */
    OS_DeleteAllObjects();

    /*-----------------------------------------------------*/
    testDesc = "#5 Duplicate-name";

    /* Setup */
    res = OS_QueueCreate(&queue_id2, "DUPLICATE", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        UT_OS_TEST_RESULT( "Queue Create failed", UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_QueueCreate(&queue_id, "DUPLICATE", 10, 4, 0);
        if (res == OS_ERR_NAME_TAKEN)
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Reset test environment */
        res = OS_QueueDelete(queue_id2);
    }

    /*-----------------------------------------------------*/
    testDesc = "#6 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#7 Nominal";

    res = OS_QueueCreate(&queue_id, "Good", 10, 4, 0);
    if ( res == OS_SUCCESS )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    res = OS_QueueDelete(queue_id);

UT_os_queue_create_test_exit_tag:
    return;
    
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
    int32 res = 0;
    const char* testDesc;
    uint32  queue_id;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_QueueDelete(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_queue_delete_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_QueueDelete(99999);
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
    res = OS_QueueCreate(&queue_id, "DeleteTest", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_QueueDelete(queue_id);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }

UT_os_queue_delete_test_exit_tag:
    return;
    
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
    int32              res = 0;
    const char*        testDesc;
    uint32             queue_id;
    uint32             queue_data_out;
    uint32             queue_data_in;
    uint32             size_copied;
    uint32             data_size;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_QueueGet(0, (void *)&queue_data_in, 4, &size_copied, OS_CHECK);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_queue_get_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_QueueGet(99999, (void *)&queue_data_in, 4, &size_copied, OS_CHECK);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg-1";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueGet", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#2 Invalid-pointer-arg-1 - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_QueueGet(queue_id, NULL, 4, &size_copied, OS_CHECK);
        if ( res == OS_INVALID_POINTER )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 Invalid-pointer-arg-2";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueGet", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Invalid-pointer-arg-2 - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_QueueGet(queue_id, (void *)&queue_data_in, 4, NULL, OS_CHECK);
        if ( res == OS_INVALID_POINTER )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

       res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#4 Queue-empty";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueEmpty", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#4 Queue-empty - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_QueueGet(queue_id, (void *)&queue_data_in, 4, &data_size, OS_CHECK);
        if ( res == OS_QUEUE_EMPTY )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#5 Queue-timed-out";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueTimeout", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Queue-timed-out - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_QueueGet(queue_id, (void *)&queue_data_in, 4, &data_size, 2);
        if ( res == OS_QUEUE_TIMEOUT )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#6 Invalid-queue-size";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueuePut", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#6 Invalid-queue-size - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        queue_data_out = 0x11223344;
        res = OS_QueuePut(queue_id, (void *)&queue_data_out, 2, 0);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#6 Invalid-queue-size - Queue Put failed";
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
        }
        else
        {
            res = OS_QueueGet(queue_id, (void *)&queue_data_in, 3, &data_size, OS_CHECK);
            if ( res == OS_QUEUE_INVALID_SIZE )
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
            else
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        }
        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#7 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#8 Nominal Pend";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueGet", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#8 Nominal Pend - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        queue_data_out = 0x11223344;
        res = OS_QueuePut(queue_id, (void *)&queue_data_out, 4, 0);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#8 Nominal Pend - Queue Put failed";
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
        }
        else
        {
            res = OS_QueueGet(queue_id, (void *)&queue_data_in, 4, &data_size, OS_PEND);
            if ( res == OS_SUCCESS )
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
            else
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        }
        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#9 Nominal timeout";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueGet", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#9 Nominal timeout - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        queue_data_out = 0x11223344;
        res = OS_QueuePut(queue_id, (void *)&queue_data_out, 4, 0);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#9 Nominal timeout - Queue Put failed";
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
        }
        else
        {
            res = OS_QueueGet(queue_id, (void *)&queue_data_in, 4, &data_size, 20);
            if ( res == OS_SUCCESS )
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
            else
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        }
        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#10 Nominal check";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueGet", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#10 Nominal check - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        queue_data_out = 0x11223344;
        res = OS_QueuePut(queue_id, (void *)&queue_data_out, 4, 0);
        if ( res != OS_SUCCESS )
        {
            testDesc = "#10 Nominal check - Queue Put failed";
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
        }
        else
        {
            res = OS_QueueGet(queue_id, (void *)&queue_data_in, 4, &data_size, OS_CHECK);
            if ( res == OS_SUCCESS )
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
            else
                UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        }
        res = OS_QueueDelete(queue_id);
    }

UT_os_queue_get_test_exit_tag:
    return;
    
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
    int32              res = 0;
    const char*        testDesc;
    uint32             queue_id;
    uint32             queue_data_out = 0;
    int                i;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_QueuePut(0, (void *)&queue_data_out, 4, 0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_queue_put_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_QueuePut(99999, (void *)&queue_data_out, 4, 0);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueuePut", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#2 Invalid-pointer-arg - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_QueuePut(queue_id, NULL, 4, 0);
        if ( res == OS_INVALID_POINTER )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#4 Queue-full";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueuePut", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#4 Queue-full - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
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
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "QueueGet", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Nominal - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        queue_data_out = 0x11223344;
        res = OS_QueuePut(queue_id, (void *)&queue_data_out, 4, 0);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
        res = OS_QueueDelete(queue_id);
    }

UT_os_queue_put_test_exit_tag:
    return;
    
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
    int32 res = 0;
    const char* testDesc;
    uint32  queue_id;
    char  long_queue_name[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_QueueGetIdByName(0, "InvalidName");
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_queue_get_id_by_name_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-pointer-arg-1";

    res = OS_QueueGetIdByName(NULL, "InvalidName");
    if ( res == OS_INVALID_POINTER )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg-2";

    res = OS_QueueGetIdByName(&queue_id, NULL);
    if ( res == OS_INVALID_POINTER )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    memset(long_queue_name, 'Y', sizeof(long_queue_name));
    long_queue_name[sizeof(long_queue_name)-1] = '\0';
    res = OS_QueueGetIdByName(&queue_id, long_queue_name);
    if ( res == OS_ERR_NAME_TOO_LONG )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-not-found";

    res = OS_QueueGetIdByName(&queue_id, "NameNotFound");
    if ( res == OS_ERR_NAME_NOT_FOUND )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "GetIDByName", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Nominal - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_QueueGetIdByName(&queue_id, "GetIDByName");
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_QueueDelete(queue_id);
    }

UT_os_queue_get_id_by_name_test_exit_tag:
    return;
    
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
    int32              res = 0;
    const char*        testDesc;
    uint32             queue_id;
    OS_queue_prop_t    queue_prop;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_QueueGetInfo(0,&queue_prop);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_queue_get_info_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_QueueGetInfo(99999, &queue_prop);
    if ( res == OS_ERR_INVALID_ID )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "GetInfo", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#2 Invalid-pointer-arg - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_QueueGetInfo(queue_id, NULL);
        if ( res == OS_INVALID_POINTER )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_QueueDelete(queue_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_QueueCreate(&queue_id, "GetInfo", 10, 4, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Queue Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_QueueGetInfo(queue_id, &queue_prop);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_QueueDelete(queue_id);
    }

UT_os_queue_get_info_test_exit_tag:
    return;
    
}

/*================================================================================*
** End of File: ut_oscore_queue_test.c
**================================================================================*/
