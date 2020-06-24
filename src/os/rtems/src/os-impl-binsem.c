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
 * \file     os-impl-binsem.c
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
#include "os-impl-binsem.h"
#include "os-shared-binsem.h"
#include "os-shared-idmap.h"


/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/*
 * Define all of the RTEMS semaphore attributes
 * 3. OSAL Binary Semaphore attributes
 *    This is a simple binary semaphore used for synchronization. It does not
 *    allow nested calls ( nor should it ) It should not be used for mutual exclusion.
 */

#define OSAL_BINARY_SEM_ATTRIBS (RTEMS_SIMPLE_BINARY_SEMAPHORE | RTEMS_PRIORITY)



/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/
/*  tables for the properties of objects */


/* Tables where the OS object information is stored */
OS_impl_binsem_internal_record_t    OS_impl_bin_sem_table       [OS_MAX_BIN_SEMAPHORES];


/****************************************************************************************
                                  SEMAPHORE API
 ***************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_Rtems_BinSemAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Rtems_BinSemAPI_Impl_Init(void)
{
    memset(OS_impl_bin_sem_table, 0, sizeof(OS_impl_bin_sem_table));
    return (OS_SUCCESS);
} /* end OS_Rtems_BinSemAPI_Impl_Init */


/*----------------------------------------------------------------
 *
 * Function: OS_BinSemCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemCreate_Impl (uint32 sem_id, uint32 sem_initial_value, uint32 options)
{
    rtems_status_code status;
    rtems_name        r_name;

    /*
    ** RTEMS task names are 4 byte integers.
    ** It is convenient to use the OSAL ID in here, as we know it is already unique
    ** and trying to use the real name would be less than useful (only 4 chars)
    */
    r_name = OS_global_bin_sem_table[sem_id].active_id;

    /* Check to make sure the sem value is going to be either 0 or 1 */
    if (sem_initial_value > 1)
    {
        sem_initial_value = 1;
    }

    /* Create RTEMS Semaphore */
    status = rtems_semaphore_create( r_name, sem_initial_value,
                                     OSAL_BINARY_SEM_ATTRIBS,
                                     0,
                                     &(OS_impl_bin_sem_table[sem_id].id));

    /* check if Create failed */
    if ( status != RTEMS_SUCCESSFUL )
    {
        OS_DEBUG("Unhandled semaphore_create error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

} /* end OS_BinSemCreate_Impl */



/*----------------------------------------------------------------
 *
 * Function: OS_BinSemDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemDelete_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_delete(OS_impl_bin_sem_table[sem_id].id);
    if(status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_delete error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

} /* end OS_BinSemDelete_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_BinSemGive_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemGive_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_release(OS_impl_bin_sem_table[sem_id].id);
    if(status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_release error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return  OS_SUCCESS;
} /* end OS_BinSemGive_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_BinSemFlush_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemFlush_Impl (uint32 sem_id)
{
    rtems_status_code status;

    /* Give Semaphore */
    status = rtems_semaphore_flush(OS_impl_bin_sem_table[sem_id].id);
    if(status != RTEMS_SUCCESSFUL)
    {
        OS_DEBUG("Unhandled semaphore_flush error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

	return  OS_SUCCESS;

} /* end OS_BinSemFlush_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_BinSemTake_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemTake_Impl (uint32 sem_id)
{
    rtems_status_code status;

    status = rtems_semaphore_obtain(OS_impl_bin_sem_table[sem_id].id, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    /*
    ** If the semaphore is flushed, this function will return
    ** RTEMS_UNSATISFIED. If this happens, the OSAL does not want to return
    ** an error, it would be inconsistent with the other ports
    **
    ** I currently do not know of any other reasons this call would return
    **  RTEMS_UNSATISFIED, so I think it is OK.
    */
    if ( status != RTEMS_SUCCESSFUL && status != RTEMS_UNSATISFIED )
    {
        OS_DEBUG("Unhandled semaphore_obtain error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

} /* end OS_BinSemTake_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_BinSemTimedWait_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemTimedWait_Impl (uint32 sem_id, uint32 msecs)
{
    rtems_status_code status;
    uint32            TimeInTicks;

    TimeInTicks = OS_Milli2Ticks(msecs);

    status  = 	rtems_semaphore_obtain(OS_impl_bin_sem_table[sem_id].id, RTEMS_WAIT, TimeInTicks) ;

    if ( status == RTEMS_TIMEOUT )
    {
        return OS_SEM_TIMEOUT;
    }

    /* See BinSemWait regarding UNSATISFIED */
    if ( status != RTEMS_SUCCESSFUL && status != RTEMS_UNSATISFIED )
    {
        OS_DEBUG("Unhandled semaphore_obtain error: %s\n",rtems_status_text(status));
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;

} /* end OS_BinSemTimedWait_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_BinSemGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemGetInfo_Impl (uint32 sem_id, OS_bin_sem_prop_t *bin_prop)
{
    /* RTEMS has no API for obtaining the current value of a semaphore */
    return OS_SUCCESS;
} /* end OS_BinSemGetInfo_Impl */

