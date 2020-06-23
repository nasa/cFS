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

/**
 * \file     os-impl-mutex.c
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 *      This file contains some of the OS APIs abstraction layer for RTEMS
 *      This has been tested against the current RTEMS 4.11 release branch
 *
 *      NOTE: This uses only the "Classic" RTEMS API.  It is intended to
 *      work on RTEMS targets that do not provide the POSIX API, i.e.
 *      when "--disable-posix" is given during the configuration stage.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-impl-mutex.h"

#include "os-shared-mutex.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/


/*
 * Define all of the RTEMS semaphore attributes
 *    In RTEMS, a MUTEX is defined as a binary semaphore
 *    It allows nested locks, priority wait order, and supports priority inheritance
 */

#define OSAL_MUTEX_ATTRIBS      (RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | RTEMS_INHERIT_PRIORITY)


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/* Tables where the OS object information is stored */
OS_impl_mutex_internal_record_t    OS_impl_mutex_table       [OS_MAX_MUTEXES];


/****************************************************************************************
                                  MUTEX API
 ***************************************************************************************/



/*----------------------------------------------------------------
 *
 * Function: OS_Rtems_MutexAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Rtems_MutexAPI_Impl_Init(void)
{
    memset(OS_impl_mutex_table, 0, sizeof(OS_impl_mutex_table));
    return (OS_SUCCESS);
} /* end OS_Rtems_MutexAPI_Impl_Init */


/*----------------------------------------------------------------
 *
 * Function: OS_MutSemCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemCreate_Impl (uint32 sem_id, uint32 options)
{
    rtems_status_code   status;
    rtems_name          r_name;

    /*
    ** Try to create the mutex
    */
    r_name = OS_global_mutex_table[sem_id].active_id;
    status = rtems_semaphore_create ( r_name, 1,
                                      OSAL_MUTEX_ATTRIBS ,
                                      0,
                                      &OS_impl_mutex_table[sem_id].id );

    if ( status != RTEMS_SUCCESSFUL )
    {
        OS_DEBUG("Unhandled semaphore_create error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

} /* end OS_MutSemCreate_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_MutSemDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemDelete_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_delete( OS_impl_mutex_table[sem_id].id);
    if (status != RTEMS_SUCCESSFUL)
    {
        /* clean up? */
        OS_DEBUG("Unhandled semaphore_delete error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

} /* end OS_MutSemDelete_Impl */



/*----------------------------------------------------------------
 *
 * Function: OS_MutSemGive_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemGive_Impl (uint32 sem_id)
{
    rtems_status_code status;

    /* Give the mutex */
    status = rtems_semaphore_release(OS_impl_mutex_table[sem_id].id);

    if(status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_release error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return  OS_SUCCESS;

} /* end OS_MutSemGive_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_MutSemTake_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemTake_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_obtain(OS_impl_mutex_table[sem_id].id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    if (status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_obtain error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

} /* end OS_MutSemTake_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_MutSemGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_MutSemGetInfo_Impl (uint32 sem_id, OS_mut_sem_prop_t *mut_prop)
{
    /* RTEMS provides no additional info */
    return OS_SUCCESS;

} /* end OS_MutSemGetInfo_Impl */

