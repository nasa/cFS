/*
 *  Copyright (c) 2004-2018, United States government as represented by the
 *  administrator of the National Aeronautics Space Administration.
 *  All rights reserved. This software was created at NASA Glenn
 *  Research Center pursuant to government contracts.
 *
 *  This is governed by the NASA Open Source Agreement and may be used,
 *  distributed and modified only according to the terms of that agreement.
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


/*****************************************************************************
 *
 * Stub function for OS_FPUExcAttachHandler()
 *
 *****************************************************************************/
int32 OS_FPUExcAttachHandler   (uint32 ExceptionNumber, void * ExceptionHandler ,
                                 int32 parameter)
{
    int32 status;

    UT_Stub_RegisterContext(UT_KEY(OS_FPUExcAttachHandler), ExceptionHandler);

    status = UT_DEFAULT_IMPL(OS_FPUExcAttachHandler);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_FPUExcEnable()
 *
 *****************************************************************************/
int32 OS_FPUExcEnable          (int32 ExceptionNumber)
{
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
    int32 status;

    status = UT_DEFAULT_IMPL(OS_FPUExcGetMask);
    *mask = 0x55aa55aa;

    return status;
}

