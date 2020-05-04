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
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include "os-shared-interrupts.h"

/****************************************************************************************
                                    INT API (deprecated)
 ***************************************************************************************/


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
    return(OS_ERR_NOT_IMPLEMENTED);
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
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_IntUnlock_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_IntLock_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntLock_Impl ( void )
{
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_IntLock_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_IntEnable_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntEnable_Impl(int32 Level)
{
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_IntEnable_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_IntDisable_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntDisable_Impl(int32 Level)
{
    return(OS_ERR_NOT_IMPLEMENTED);
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

