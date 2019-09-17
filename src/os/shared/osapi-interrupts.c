/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   osapi-interrupts.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *         Contains the code related to interrupt handling.
 *         Implementation of these are mostly in the lower layer; however
 *         a wrapper must exist at this level which allows for unit testing.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * User defined include files
 */
#include "common_types.h"
#include "os-impl.h"

                        
/*----------------------------------------------------------------
 *
 * Function: OS_IntAttachHandler
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntAttachHandler  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter)
{
    if (InterruptHandler == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return OS_IntAttachHandler_Impl(InterruptNumber, InterruptHandler, parameter);
} /* end OS_IntAttachHandler */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_IntUnlock
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntUnlock (int32 IntFlags)
{
    return OS_IntUnlock_Impl(IntFlags);
} /* end OS_IntUnlock */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_IntLock
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntLock ( void )
{
    return OS_IntLock_Impl();
} /* end OS_IntLock */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_IntEnable
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntEnable(int32 Level)
{
    return OS_IntEnable_Impl(Level);
} /* end OS_IntEnable */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_IntDisable
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntDisable(int32 Level)
{
    return OS_IntDisable_Impl(Level);
} /* end OS_IntDisable */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_IntSetMask
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntSetMask ( uint32 MaskSetting )
{
    return OS_IntSetMask_Impl(MaskSetting);
} /* end OS_IntSetMask */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_IntGetMask
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_IntGetMask ( uint32 * MaskSettingPtr )
{
    return OS_IntGetMask_Impl(MaskSettingPtr);
} /* end OS_IntGetMask */

