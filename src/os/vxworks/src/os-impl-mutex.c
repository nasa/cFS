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
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"

#include "os-impl-mutex.h"
#include "os-shared-mutex.h"

#include <errnoLib.h>

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

/* Console device */
OS_impl_mutsem_internal_record_t   OS_impl_mutex_table   [OS_MAX_MUTEXES];

/****************************************************************************************
                                  MUTEX API
****************************************************************************************/



/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_MutexAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_MutexAPI_Impl_Init(void)
{
    memset(OS_impl_mutex_table, 0, sizeof(OS_impl_mutex_table));
    return (OS_SUCCESS);
} /* end OS_VxWorks_MutexAPI_Impl_Init */


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
    SEM_ID tmp_sem_id;

    /* Initialize VxWorks Semaphore.
     * The memory for this sem is statically allocated. */
    tmp_sem_id = semMInitialize(OS_impl_mutex_table[sem_id].mmem, SEM_Q_PRIORITY | SEM_INVERSION_SAFE);

    if(tmp_sem_id == (SEM_ID)0)
    {
        OS_DEBUG("semMInitalize() - vxWorks errno %d\n",errno);
        return OS_SEM_FAILURE;
    }

    OS_impl_mutex_table[sem_id].vxid = tmp_sem_id;
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
    /*
     * As the memory for the sem is statically allocated, delete is a no-op.
     */
    OS_impl_mutex_table[sem_id].vxid = 0;
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
    /* Give VxWorks Semaphore */
    return OS_VxWorks_GenericSemGive(OS_impl_mutex_table[sem_id].vxid);
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
    /* Take VxWorks Semaphore */
    return OS_VxWorks_GenericSemTake(OS_impl_mutex_table[sem_id].vxid, WAIT_FOREVER);
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
    /* VxWorks provides no additional info */
    return OS_SUCCESS;

} /* end OS_MutSemGetInfo_Impl */

