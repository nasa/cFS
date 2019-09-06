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
 * \file   osapi-fpu.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
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


/*
 * User defined include files
 */
#include "common_types.h"
#include "os-impl.h"


int32 OS_FPUExcAttachHandler   (uint32 ExceptionNumber, void * ExceptionHandler ,
                                 int32 parameter)
{
    if (ExceptionHandler == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return OS_FPUExcAttachHandler_Impl(ExceptionNumber, ExceptionHandler, parameter);
}

/*
 *
 *   Name: OS_FPUExcSetMask
 *
 *   Purpose: This function sets the FPU exception mask
 *
 *   Notes: The exception environment is local to each task Therefore this must be
 *          called for each task that that wants to do floating point and catch exceptions.
 */
int32 OS_FPUExcSetMask(uint32 mask)
{
    return OS_FPUExcSetMask_Impl(mask);
}

/*
 *
 *   Name: OS_FPUExcGetMask
 *
 *   Purpose: This function gets the FPU exception mask
 *
 *   Notes: The exception environment is local to each task Therefore this must be
 *          called for each task that that wants to do floating point and catch exceptions.
 */
int32 OS_FPUExcGetMask(uint32 *mask)
{
    return OS_FPUExcGetMask_Impl(mask);
}

int32 OS_FPUExcEnable          (int32 ExceptionNumber)
{
    return OS_FPUExcEnable_Impl(ExceptionNumber);
}

int32 OS_FPUExcDisable         (int32 ExceptionNumber)
{
    return OS_FPUExcDisable_Impl(ExceptionNumber);
}


