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
** File:  ut_oscore_binsem_test.c
** Owner: Alan Cudmore
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_binsem_test.h"

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
** Syntax: OS_BinSemCreate
** Purpose: Creates a binary semaphore
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name passed in has already been used
**          OS_ERR_NO_FREE_IDS if there are no more free binary semaphore ids
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_create_test()
{
    int i;
    char  sem_name[UT_OS_NAME_BUFF_SIZE];
    char  long_sem_name[UT_OS_NAME_BUFF_SIZE];
    uint32  sem_ids[OS_MAX_BIN_SEMAPHORES+1];

    /*-----------------------------------------------------*/
    if (!UT_IMPL(OS_BinSemCreate(&sem_ids[0], "Good", 1, 0))) return;
    UT_TEARDOWN(OS_BinSemDelete(sem_ids[0]));

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_BinSemCreate(NULL, "BinSem1", 1, 0), OS_INVALID_POINTER, "null pointer arg 1");

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_BinSemCreate(&sem_ids[0], NULL, 1, 0), OS_INVALID_POINTER, "null pointer arg 2");

    /*-----------------------------------------------------*/
    memset(long_sem_name, 'X', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name)-1] = '\0';
    UT_RETVAL(OS_BinSemCreate(&sem_ids[0], long_sem_name, 1, 0), OS_ERR_NAME_TOO_LONG, "name too long");

    /*-----------------------------------------------------*/
    /* Setup */
    for ( i = 0; i< OS_MAX_BIN_SEMAPHORES; i++ )
    {
       memset(sem_name, '\0', sizeof(sem_name));
       UT_os_sprintf(sem_name, "BINSEM%d", i);

       if(!UT_SETUP(OS_BinSemCreate(&sem_ids[i], sem_name, 1, 0)))
       {
          break;
       }
    }

    if ( i == OS_MAX_BIN_SEMAPHORES ) /* setup was successful */
    {
       UT_RETVAL(OS_BinSemCreate(&sem_ids[OS_MAX_BIN_SEMAPHORES], "OneTooMany", 1, 0), OS_ERR_NO_FREE_IDS, "no free ids");
    }

    /* Reset test environment */
    OS_DeleteAllObjects();

    /*-----------------------------------------------------*/
    if(UT_SETUP(OS_BinSemCreate(&sem_ids[0], "DUPLICATE", 1, 0)))
    {
       UT_RETVAL(OS_BinSemCreate(&sem_ids[0], "DUPLICATE", 1, 0), OS_ERR_NAME_TAKEN, "duplicate name");

       /* Reset test environment */
       UT_TEARDOWN(OS_BinSemDelete(sem_ids[0]));
    }

    /*-----------------------------------------------------*/
    UT_NOMINAL(OS_BinSemCreate(&sem_ids[0], "Good", 1, 0));

    /* Reset test environment */
    UT_TEARDOWN(OS_BinSemDelete(sem_ids[0]));
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemDelete
** Purpose: Deletes a binary semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in does not exist
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_delete_test()
{
    uint32  bin_sem_id;

    /*-----------------------------------------------------*/
    if (!UT_IMPL(OS_BinSemDelete(0))) return;

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_BinSemDelete(99999), OS_ERR_INVALID_ID, "invalid id arg");

    /*-----------------------------------------------------*/
    if(UT_SETUP(OS_BinSemCreate(&bin_sem_id, "DeleteTest", 1, 0)))
    {
       UT_NOMINAL(OS_BinSemDelete(bin_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemFlush
** Purpose: Releases all the tasks waiting on the given semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SEM_FAILURE if the semaphore was not previously init'd or is not in the
**                         array of semaphores defined by the system
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_flush_test()
{
    uint32  bin_sem_id;

    /*-----------------------------------------------------*/
    if (!UT_IMPL(OS_BinSemFlush(0))) return;

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_BinSemFlush(99999), OS_ERR_INVALID_ID, "invalid id arg");

    /*----------------------------------------------------*/
    if(UT_SETUP(OS_BinSemCreate(&bin_sem_id, "FlushTest", 1, 0)))
    {
       UT_NOMINAL(OS_BinSemFlush(bin_sem_id));
       UT_TEARDOWN(OS_BinSemDelete(bin_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemGive
** Purpose: Gives back a binary semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SEM_FAILURE if the semaphore was not previously init'd or is not in the
**                         array of semaphores defined by the system
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_give_test()
{
    uint32  bin_sem_id;

    /*-----------------------------------------------------*/
    if (!UT_IMPL(OS_BinSemGive(0))) return;

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_BinSemGive(99999), OS_ERR_INVALID_ID, "invalid id arg");

    /*-----------------------------------------------------*/
    if (UT_SETUP(OS_BinSemCreate(&bin_sem_id, "GiveTest", 1, 0)))
    {
       UT_NOMINAL(OS_BinSemGive(bin_sem_id));
       UT_TEARDOWN(OS_BinSemDelete(bin_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemTake
** Purpose: Reserves a binary semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SEM_FAILURE if the semaphore was not previously init'd or is not in the
**                         array of semaphores defined by the system
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_take_test()
{
    uint32  bin_sem_id;

    /*-----------------------------------------------------*/
    if (!UT_IMPL(OS_BinSemTake(0))) return;

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_BinSemTake(99999), OS_ERR_INVALID_ID, "invalid id arg");

    /*-----------------------------------------------------*/
    if (UT_SETUP(OS_BinSemCreate(&bin_sem_id, "TakeTest", 1, 0)))
    {
       UT_NOMINAL(OS_BinSemTake(bin_sem_id));
       UT_TEARDOWN(OS_BinSemDelete(bin_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemTimedWait
** Purpose: Reserves a binary semaphore with a timeout
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SEM_FAILURE if the OS call failed
**          OS_SEM_TIMEOUT if the semaphore was not relinquished in time
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_timed_wait_test()
{
    uint32  bin_sem_id;

    /*-----------------------------------------------------*/
    if (!UT_IMPL(OS_BinSemTimedWait(0, 1000))) return;

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_BinSemTimedWait(99999, 1000), OS_ERR_INVALID_ID, "invalid id arg");

    /*-----------------------------------------------------*/
    if(UT_SETUP(OS_BinSemCreate(&bin_sem_id, "TimedWait", 1, 0))
       && UT_SETUP(OS_BinSemTake(bin_sem_id)))
    {
       UT_RETVAL(OS_BinSemTimedWait(bin_sem_id, 1000), OS_SEM_TIMEOUT, "semtake timed out");
       UT_TEARDOWN(OS_BinSemDelete(bin_sem_id));
    }

    /*-----------------------------------------------------*/
    if(UT_SETUP(OS_BinSemCreate(&bin_sem_id, "TimedWait", 1, 0)))
    {
        UT_NOMINAL(OS_BinSemTimedWait(bin_sem_id, 1000));
        UT_TEARDOWN(OS_BinSemDelete(bin_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemGetIdByName
** Purpose: Returns the id of a given binary semaphore name
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the name passed in was not found in the binary
**                                semaphore table
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_get_id_by_name_test()
{
    uint32  bin_sem_id;
    char long_sem_name[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    if (!UT_IMPL(OS_BinSemGetIdByName(NULL, "InvalidName"))) return;

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_BinSemGetIdByName(NULL, "InvalidName"), OS_INVALID_POINTER, "invalid ptr arg 1");

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_BinSemGetIdByName(&bin_sem_id, NULL), OS_INVALID_POINTER, "invalid ptr arg 2");

    /*-----------------------------------------------------*/
    memset(long_sem_name, 'Y', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name)-1] = '\0';
    UT_RETVAL(OS_BinSemGetIdByName(&bin_sem_id, long_sem_name), OS_ERR_NAME_TOO_LONG, "name too long");

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_BinSemGetIdByName(&bin_sem_id, "NameNotFound"), OS_ERR_NAME_NOT_FOUND, "name not found");

    /*-----------------------------------------------------*/
    if(UT_SETUP(OS_BinSemCreate(&bin_sem_id, "GetIDByName", 1, 0)))
    {
       UT_NOMINAL(OS_BinSemGetIdByName(&bin_sem_id, "GetIDByName"));
       UT_TEARDOWN(OS_BinSemDelete(bin_sem_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemGetInfo
** Purpose: Returns semaphore information about the given binary semaphore id
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SUCCESS if succeeded
** Test #1: TBD
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_get_info_test()
{
    uint32             bin_sem_id;
    OS_bin_sem_prop_t  bin_sem_prop;

    /*-----------------------------------------------------*/
    if (!UT_IMPL(OS_BinSemGetInfo(0, &bin_sem_prop))) return;

    /*-----------------------------------------------------*/
    UT_RETVAL(OS_BinSemGetInfo(99999, &bin_sem_prop), OS_ERR_INVALID_ID, "invalid id");

    /*-----------------------------------------------------*/
    if(UT_SETUP(OS_BinSemCreate(&bin_sem_id, "GetInfo", 1, 0)))
    {
        UT_RETVAL(OS_BinSemGetInfo(bin_sem_id, NULL),OS_INVALID_POINTER, "invalid ptr");
        UT_TEARDOWN(OS_BinSemDelete(bin_sem_id));
    }

    /*-----------------------------------------------------*/
    if(UT_SETUP(OS_BinSemCreate(&bin_sem_id, "GetInfo", 1, 0)))
    {
        UT_NOMINAL(OS_BinSemGetInfo(bin_sem_id, &bin_sem_prop));
        UT_TEARDOWN(OS_BinSemDelete(bin_sem_id));
    }
}

/*================================================================================*
** End of File: ut_oscore_binsem_test.c
**================================================================================*/
