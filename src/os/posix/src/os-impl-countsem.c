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
 * \file     os-impl-countsem.c
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include "os-impl-countsem.h"
#include "os-shared-countsem.h"

/*
 * Added SEM_VALUE_MAX Define
 */
#ifndef SEM_VALUE_MAX
#define SEM_VALUE_MAX       (UINT32_MAX/2)
#endif

/* Tables where the OS object information is stored */
OS_impl_countsem_internal_record_t  OS_impl_count_sem_table     [OS_MAX_COUNT_SEMAPHORES];


/****************************************************************************************
                               COUNTING SEMAPHORE API
 ***************************************************************************************/

/*
 * Unlike binary semaphores, counting semaphores can use the standard POSIX semaphore facility.
 * This has the advantage of more correct behavior on "give" operations:
 *  - give may be done from a signal / ISR context
 *  - give should not cause an unexpected task switch nor should it ever block
 */

/*---------------------------------------------------------------------------------------
   Name: OS_Posix_CountSemAPI_Impl_Init

   Purpose: Initialize the Counting Semaphore data structures

---------------------------------------------------------------------------------------*/
int32 OS_Posix_CountSemAPI_Impl_Init(void)
{
   memset(OS_impl_count_sem_table, 0, sizeof(OS_impl_count_sem_table));
   return OS_SUCCESS;
} /* end OS_Posix_CountSemAPI_Impl_Init */


/*----------------------------------------------------------------
 *
 * Function: OS_CountSemCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemCreate_Impl (uint32 sem_id, uint32 sem_initial_value, uint32 options)
{
    if (sem_initial_value > SEM_VALUE_MAX)
    {
        return OS_INVALID_SEM_VALUE;
    }

    if (sem_init(&OS_impl_count_sem_table[sem_id].id, 0, sem_initial_value) < 0)
    {
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

} /* end OS_CountSemCreate_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_CountSemDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemDelete_Impl (uint32 sem_id)
{
    if (sem_destroy(&OS_impl_count_sem_table[sem_id].id) < 0)
    {
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

} /* end OS_CountSemDelete_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_CountSemGive_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemGive_Impl ( uint32 sem_id )
{
    if (sem_post(&OS_impl_count_sem_table[sem_id].id) < 0)
    {
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

} /* end OS_CountSemGive_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_CountSemTake_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemTake_Impl ( uint32 sem_id )
{
    if (sem_wait(&OS_impl_count_sem_table[sem_id].id) < 0)
    {
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
} /* end OS_CountSemTake_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_CountSemTimedWait_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemTimedWait_Impl ( uint32 sem_id, uint32 msecs )
{
   struct timespec ts;
   int result;

   /*
    ** Compute an absolute time for the delay
    */
   OS_Posix_CompAbsDelayTime(msecs, &ts);

   if (sem_timedwait(&OS_impl_count_sem_table[sem_id].id, &ts) == 0)
   {
       result = OS_SUCCESS;
   }
   else if (errno == ETIMEDOUT)
   {
       result = OS_SEM_TIMEOUT;
   }
   else
   {
       /* unspecified failure */
       result = OS_SEM_FAILURE;
   }

   return result;
} /* end OS_CountSemTimedWait_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_CountSemGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemGetInfo_Impl (uint32 sem_id, OS_count_sem_prop_t *count_prop)
{
    int sval;

    if (sem_getvalue(&OS_impl_count_sem_table[sem_id].id, &sval) < 0)
    {
        return OS_SEM_FAILURE;
    }

    /* put the info into the stucture */
    count_prop -> value = sval;
    return OS_SUCCESS;
} /* end OS_CountSemGetInfo_Impl */

