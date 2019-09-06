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

/*---------------------------------------------------------------------------------------
 * Name: OS_IntAttachHandler
 *
 * Purpose:
 *      The call associates a specified C routine to a specified interrupt
 *      number.Upon occurring of the InterruptNumber , the InerruptHandler
 *      routine will be called and passed the parameter.
 *
 * Assumptions and Notes:
 *
 * Parameters:
 *      InterruptNumber : The Interrupt Number that will cause the start of
 *                        the ISR
 *      InerruptHandler : The ISR associatd with this interrupt
 *      parameter :The parameter that is passed to the ISR
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *     OS_SUCCESS on success
 *     OS_INVALID_POINTER if the Interrupt handler pointer is NULL
---------------------------------------------------------------------------------------*/
int32 OS_IntAttachHandler  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter)
{
    if (InterruptHandler == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return OS_IntAttachHandler_Impl(InterruptNumber, InterruptHandler, parameter);
}

/*---------------------------------------------------------------------------------------
 * Name: OS_IntUnlock
 * Purpose:
 *      Enable the interrupts.
 *
 * Assumptions and Notes:
 *
 * Parameters:
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *      OS_SUCCESS
---------------------------------------------------------------------------------------*/
int32 OS_IntUnlock (int32 IntFlags)
{
    return OS_IntUnlock_Impl(IntFlags);
}

/*---------------------------------------------------------------------------------------
 * Name: OS_Intlock
 * Purpose:
 *      Disable the interrupts.
 *
 * Assumptions and Notes:
 *
 * Parameters:
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *      OS_SUCCESS
---------------------------------------------------------------------------------------*/
int32 OS_IntLock ( void )
{
    return OS_IntLock_Impl();
}

/*---------------------------------------------------------------------------------------
 * Name: OS_IntEnable
 * Purpose:
 *      Enables interrupts through Level
 *
 * Assumptions and Notes:
 *
 * Parameters:
 *              Level - the interrupts to enable
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *      OS_SUCCESS
---------------------------------------------------------------------------------------*/
int32 OS_IntEnable(int32 Level)
{
    return OS_IntEnable_Impl(Level);
}

/*---------------------------------------------------------------------------------------
 * Name: OS_IntDisable
 * Purpose:
 *      Disables interrupts through Level
 *
 * Assumptions and Notes:
 *
 * Parameters:
 *              Level - the interrupts to disable
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *      OS_SUCCESS
---------------------------------------------------------------------------------------*/

int32 OS_IntDisable(int32 Level)
{
    return OS_IntDisable_Impl(Level);
}

/*---------------------------------------------------------------------------------------
 * Name: OS_SetMask
 * Purpose:
 *      Set the masking register to mask and unmask interrupts
 *
 * Assumptions and Notes:
 *      HW interrupt control is not supported from a user task
 *
 * Parameters:
 *      MaskSetting :the value to be written into the mask register
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *
---------------------------------------------------------------------------------------*/
int32 OS_IntSetMask ( uint32 MaskSetting )
{
    return OS_IntSetMask_Impl(MaskSetting);
}

/*--------------------------------------------------------------------------------------
 * Name: OS_GetMask
 * Purpose:
 *      Read and report the setting of the cpu mask register.
 *
 * Assumptions and Notes:
 *      HW interrupt control is not supported from a user task
 *
 * Parameters:
 *      MaskSettingPtr : pointer to a location where the function store the
 *                               reading of the cpu mask register.
 *
 * Global Inputs: None
 *
 * Global Outputs: None
 *
 *
 * Return Values:
 *
---------------------------------------------------------------------------------------*/
int32 OS_IntGetMask ( uint32 * MaskSettingPtr )
{
    return OS_IntGetMask_Impl(MaskSettingPtr);
}

