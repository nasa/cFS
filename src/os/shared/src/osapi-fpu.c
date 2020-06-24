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
 * \file     osapi-fpu.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         Contains the code related to floating point mode setting.
 *         Implementation of these are mostly in the lower layer; however
 *         a wrapper must exist at this level which allows for unit testing.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef OSAL_OMIT_DEPRECATED

/*
 * User defined include files
 */
#include "os-shared-fpu.h"

/*----------------------------------------------------------------
 *
 * Function: OS_FPUExcAttachHandler
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FPUExcAttachHandler   (uint32 ExceptionNumber, osal_task_entry ExceptionHandler ,
                                 int32 parameter)
{
    if (ExceptionHandler == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return OS_FPUExcAttachHandler_Impl(ExceptionNumber, ExceptionHandler, parameter);
} /* end OS_FPUExcAttachHandler */


/*----------------------------------------------------------------
 *
 * Function: OS_FPUExcSetMask
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FPUExcSetMask(uint32 mask)
{
    return OS_FPUExcSetMask_Impl(mask);
} /* end OS_FPUExcSetMask */


/*----------------------------------------------------------------
 *
 * Function: OS_FPUExcGetMask
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FPUExcGetMask(uint32 *mask)
{
    if (mask == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return OS_FPUExcGetMask_Impl(mask);
} /* end OS_FPUExcGetMask */

/*----------------------------------------------------------------
 *
 * Function: OS_FPUExcEnable
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FPUExcEnable          (int32 ExceptionNumber)
{
    return OS_FPUExcEnable_Impl(ExceptionNumber);
} /* end OS_FPUExcEnable */

/*----------------------------------------------------------------
 *
 * Function: OS_FPUExcDisable
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FPUExcDisable         (int32 ExceptionNumber)
{
    return OS_FPUExcDisable_Impl(ExceptionNumber);
} /* end OS_FPUExcDisable */

#endif /* OSAL_OMIT_DEPRECATED */
