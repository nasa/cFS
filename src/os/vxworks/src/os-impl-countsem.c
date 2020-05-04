/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     os-impl-countsem.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-countsem.h"
#include "os-shared-countsem.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/


/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/


/* Tables where the OS object information is stored */
OS_impl_countsem_internal_record_t OS_impl_count_sem_table [OS_MAX_COUNT_SEMAPHORES];

/****************************************************************************************
                             COUNTING SEMAPHORE API
****************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_CountSemAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_CountSemAPI_Impl_Init(void)
{
    memset(OS_impl_count_sem_table, 0, sizeof(OS_impl_count_sem_table));
    return (OS_SUCCESS);
} /* end OS_VxWorks_CountSemAPI_Impl_Init */



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
    SEM_ID tmp_sem_id;

    /* Initialize VxWorks Semaphore.
     * The memory for this sem is statically allocated. */
    tmp_sem_id = semCInitialize(OS_impl_count_sem_table[sem_id].cmem, SEM_Q_PRIORITY, sem_initial_value);

    /* check if semCInitialize failed */
    if(tmp_sem_id == (SEM_ID)0)
    {
        OS_DEBUG("semCInitialize() - vxWorks errno %d\n",errno);
        return OS_SEM_FAILURE;
    }

    OS_impl_count_sem_table[sem_id].vxid = tmp_sem_id;
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
    /*
     * As the memory for the sem is statically allocated, delete is a no-op.
     */
    OS_impl_count_sem_table[sem_id].vxid = 0;
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
int32 OS_CountSemGive_Impl (uint32 sem_id)
{
    /* Give VxWorks Semaphore */
    return OS_VxWorks_GenericSemGive(OS_impl_count_sem_table[sem_id].vxid);
} /* end OS_CountSemGive_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_CountSemTake_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemTake_Impl (uint32 sem_id)
{
    return OS_VxWorks_GenericSemTake(OS_impl_count_sem_table[sem_id].vxid, WAIT_FOREVER);
} /* end OS_CountSemTake_Impl */



/*----------------------------------------------------------------
 *
 * Function: OS_CountSemTimedWait_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CountSemTimedWait_Impl (uint32 sem_id, uint32 msecs)
{
    return OS_VxWorks_GenericSemTake(OS_impl_count_sem_table[sem_id].vxid,
            OS_Milli2Ticks(msecs));
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
    /* VxWorks does not provide an API to get the value */
    return OS_SUCCESS;

} /* end OS_CountSemGetInfo_Impl */

