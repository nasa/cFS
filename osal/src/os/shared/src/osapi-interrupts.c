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
 * \file     osapi-interrupts.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
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
#include "os-shared-interrupts.h"

#ifndef OSAL_OMIT_DEPRECATED /* Remove file and remove from build when deleted */
                             /* Optionally remove from sources once source selection is in cmake options */

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

#endif /* OSAL_OMIT_DEPRECATED */
