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
 * \file     os-shared-interrupts.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_INTERRUPTS_H_
#define INCLUDE_OS_SHARED_INTERRUPTS_H_

#include <os-shared-globaldefs.h>


#ifndef OSAL_OMIT_DEPRECATED

/****************************************************************************************
                 INTERRUPT API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*
 * Interrupt API low-level handlers
 *
 * These are defined for completeness but may not be implementable on
 * multi-user platforms where interrupts are handled exclusively at the
 * kernel level.  They may work on single-user RTOS's like
 * VxWorks or RTEMS, but not Linux.  As such they should not be
 * relied upon.
 */

/*----------------------------------------------------------------
   Function: OS_IntAttachHandler_Impl

    Purpose: The call associates a specified C routine to a specified interrupt
            number.Upon occurring of the InterruptNumber the InerruptHandler
            routine will be called and passed the parameter.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_IntAttachHandler_Impl  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter);

/*----------------------------------------------------------------
   Function: OS_IntUnlock_Impl

    Purpose: Enable previous state of interrupts

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_IntUnlock_Impl (int32 IntLevel);

/*----------------------------------------------------------------
   Function: OS_IntLock_Impl

    Purpose: Disable interrupts

    Returns: A key value that can be used to restore interrupts
 ------------------------------------------------------------------*/
int32 OS_IntLock_Impl ( void );

/*----------------------------------------------------------------
   Function: OS_IntEnable_Impl

    Purpose: Enable previous state of interrupts

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_IntEnable_Impl(int32 Level);

/*----------------------------------------------------------------
   Function: OS_IntDisable_Impl

    Purpose: Disable the corresponding interrupt number.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_IntDisable_Impl(int32 Level);

/*----------------------------------------------------------------
   Function: OS_IntSetMask_Impl

    Purpose: Set the cpu mask register for interrupts

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_IntSetMask_Impl ( uint32 MaskSetting );

/*----------------------------------------------------------------
   Function: OS_IntGetMask_Impl

    Purpose: Read and output the setting of the cpu mask register

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_IntGetMask_Impl ( uint32 * MaskSettingPtr );

#endif /* OSAL_OMIT_DEPRECATED */


#endif  /* INCLUDE_OS_SHARED_INTERRUPTS_H_ */

