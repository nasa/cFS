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

