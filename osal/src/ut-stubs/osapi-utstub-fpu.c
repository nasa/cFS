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
 * \file osapi_stubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"

#ifndef OSAL_OMIT_DEPRECATED

/*****************************************************************************
 *
 * Stub function for OS_FPUExcAttachHandler()
 *
 *****************************************************************************/
int32 OS_FPUExcAttachHandler   (uint32 ExceptionNumber, osal_task_entry ExceptionHandler ,
                                 int32 parameter)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_FPUExcAttachHandler), ExceptionNumber);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_FPUExcAttachHandler), ExceptionHandler);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_FPUExcAttachHandler), parameter);

    int32 status;

    status = UT_DEFAULT_IMPL_ARGS(OS_FPUExcAttachHandler, ExceptionNumber, ExceptionHandler, parameter);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_FPUExcEnable()
 *
 *****************************************************************************/
int32 OS_FPUExcEnable          (int32 ExceptionNumber)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_FPUExcEnable), ExceptionNumber);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_FPUExcEnable);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_FPUExcDisable()
 *
 *****************************************************************************/
int32 OS_FPUExcDisable         (int32 ExceptionNumber)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_FPUExcDisable), ExceptionNumber);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_FPUExcDisable);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_FPUExcSetMask()
 *
 *****************************************************************************/
int32 OS_FPUExcSetMask         (uint32 mask)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_FPUExcSetMask), mask);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_FPUExcSetMask);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_FPUExcGetMask()
 *
 *****************************************************************************/
int32 OS_FPUExcGetMask         (uint32 *mask)
{
    UT_Stub_RegisterContext(UT_KEY(OS_FPUExcGetMask), mask);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_FPUExcGetMask);
    *mask = 0x55aa55aa;

    return status;
}

#endif /* OSAL_OMIT_DEPRECATED */
