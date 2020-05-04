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

