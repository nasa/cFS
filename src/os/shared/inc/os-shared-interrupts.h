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

