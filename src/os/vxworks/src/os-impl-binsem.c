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
 * \file     os-impl-binsem.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"

#include "os-impl-binsem.h"
#include "os-shared-binsem.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/


/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

/* Tables where the OS object information is stored */
OS_impl_binsem_internal_record_t   OS_impl_bin_sem_table   [OS_MAX_BIN_SEMAPHORES];


/****************************************************************************************
                             BINARY SEMAPHORE API
****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_BinSemAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_BinSemAPI_Impl_Init(void)
{
    memset(OS_impl_bin_sem_table, 0, sizeof(OS_impl_bin_sem_table));
    return (OS_SUCCESS);
} /* end OS_VxWorks_BinSemAPI_Impl_Init */


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
    SEM_ID tmp_sem_id;

    /* Initialize VxWorks Semaphore.
     * The memory for this sem is statically allocated. */
    tmp_sem_id = semBInitialize(OS_impl_bin_sem_table[sem_id].bmem, SEM_Q_PRIORITY, sem_initial_value);

    /* check if semBInitialize failed */
    if(tmp_sem_id == (SEM_ID)0)
    {
        OS_DEBUG("semBInitialize() - vxWorks errno %d\n",errno);
        return OS_SEM_FAILURE;
    }

    OS_impl_bin_sem_table[sem_id].vxid = tmp_sem_id;
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
    /*
     * As the memory for the sem is statically allocated, delete is a no-op.
     */
    OS_impl_bin_sem_table[sem_id].vxid = 0;
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
    /* Use common routine */
    return OS_VxWorks_GenericSemGive(OS_impl_bin_sem_table[sem_id].vxid);
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
    /* Flush VxWorks Semaphore */
    if(semFlush(OS_impl_bin_sem_table[sem_id].vxid) != OK)
    {
        OS_DEBUG("semFlush() - vxWorks errno %d\n",errno);
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
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
    /* Use common routine */
    return OS_VxWorks_GenericSemTake(OS_impl_bin_sem_table[sem_id].vxid, WAIT_FOREVER);

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
    return OS_VxWorks_GenericSemTake(OS_impl_bin_sem_table[sem_id].vxid, OS_Milli2Ticks(msecs));
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
    /* VxWorks has no API for obtaining the current value of a semaphore */
    return OS_SUCCESS;
} /* end OS_BinSemGetInfo_Impl */


