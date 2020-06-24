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
 * \file     os-shared-fpu.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_FPU_H_
#define INCLUDE_OS_SHARED_FPU_H_

#include <os-shared-globaldefs.h>

#ifndef OSAL_OMIT_DEPRECATED


/****************************************************************************************
            FLOATING POINT CONFIG/EXCEPTION API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*
 * FPU API low-level handlers
 * These may also not be implementable on some platforms
 */
int32 OS_FPUExcAttachHandler_Impl(uint32 ExceptionNumber, osal_task_entry ExceptionHandler,
                                 int32 parameter);
int32 OS_FPUExcEnable_Impl(int32 ExceptionNumber);
int32 OS_FPUExcDisable_Impl(int32 ExceptionNumber);

/*----------------------------------------------------------------
   Function: OS_FPUExcSetMask_Impl

    Purpose: This function sets the FPU exception mask

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FPUExcSetMask_Impl(uint32 mask);

/*----------------------------------------------------------------
   Function: OS_FPUExcGetMask_Impl

    Purpose: This function gets the FPU exception mask

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FPUExcGetMask_Impl(uint32 *mask);

#endif

#endif  /* INCLUDE_OS_SHARED_FPU_H_ */

