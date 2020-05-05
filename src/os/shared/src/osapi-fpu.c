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
