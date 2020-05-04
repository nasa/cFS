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
 * \file     os-impl-interrupts.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include <intLib.h>
#include <iv.h>

#include "os-vxworks.h"
#include "os-shared-interrupts.h"

/****************************************************************************************
                                 INT API (deprecated)
****************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_IntAttachHandler_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntAttachHandler_Impl  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter)
{
    /* The Xenomai-VxWorks emulation layer does not support interrupt control */
    if(intConnect(INUM_TO_IVEC(InterruptNumber),
      (VOIDFUNCPTR)InterruptHandler, parameter) != OK)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
} /* end OS_IntAttachHandler_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_IntUnlock_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntUnlock_Impl (int32 IntLevel)
{
    intUnlock(IntLevel);
    return(OS_SUCCESS);
} /* end OS_IntUnlock_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_IntLock_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntLock_Impl (void)
{
    return (int32)intLock();
} /* end OS_IntLock_Impl */



/*----------------------------------------------------------------
 *
 * Function: OS_IntEnable_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntEnable_Impl (int32 Level)
{
    int32 RetCode;
    int Status;

    Status = intEnable(Level);

    if(Status == OK)
    {
        RetCode = OS_SUCCESS;
    }
    else
    {
        RetCode = OS_ERROR;
    }

    return RetCode;
} /* end OS_IntEnable_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_IntDisable_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntDisable_Impl (int32 Level)
{
    int32 RetCode;
    int Status;

    Status = intDisable(Level);

    if(Status == OK)
    {
        RetCode = OS_SUCCESS;
    }
    else
    {
        RetCode = OS_ERROR;
    }

    return RetCode;
} /* end OS_IntDisable_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_IntSetMask_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntSetMask_Impl ( uint32 MaskSetting )
{
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_IntSetMask_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_IntGetMask_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntGetMask_Impl ( uint32 * MaskSettingPtr )
{
    *MaskSettingPtr = 0;
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_IntGetMask_Impl */

