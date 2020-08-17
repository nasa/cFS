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
 * \file     os-impl-fpu.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-shared-fpu.h"

/****************************************************************************************
                                 FPU API (deprecated)
****************************************************************************************/


/*----------------------------------------------------------------
 *
 * Function: OS_FPUExcAttachHandler_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FPUExcAttachHandler_Impl(uint32 ExceptionNumber, osal_task_entry ExceptionHandler,
                                 int32 parameter)
{
    /*
    ** Not implemented in VxWorks.
    */
    return(OS_ERR_NOT_IMPLEMENTED);
} /* end OS_FPUExcAttachHandler_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FPUExcEnable_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FPUExcEnable_Impl(int32 ExceptionNumber)
{
    /*
    ** Not implemented in VxWorks.
    */
    return(OS_SUCCESS);
} /* end OS_FPUExcEnable_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FPUExcDisable_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FPUExcDisable_Impl(int32 ExceptionNumber)
{
    /*
    ** Not implemented in VxWorks.
    */
    return(OS_SUCCESS);
} /* end OS_FPUExcDisable_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_FPUExcSetMask_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FPUExcSetMask_Impl(uint32 mask)
{
    int32 Status;

    /*
     * only implemented on ppc except ppc440
     * carry-over from previous vxworks osal
     */
#if defined(_PPC_) && CPU != PPC440
    vxFpscrSet( mask);
    Status = OS_SUCCESS;
#else
    Status = OS_ERR_NOT_IMPLEMENTED;
#endif

    return Status;
} /* end OS_FPUExcSetMask_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_FPUExcGetMask_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FPUExcGetMask_Impl(uint32 *mask)
{
    int32 Status;

#if defined(_PPC_) && CPU != PPC440
    *mask = vxFpscrGet();
    Status = OS_SUCCESS;
#else
    Status = OS_ERR_NOT_IMPLEMENTED;
#endif
    return Status;
} /* end OS_FPUExcGetMask_Impl */

