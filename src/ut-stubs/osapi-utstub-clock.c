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
 * Stub function for OS_GetLocalTime()
 *
 *****************************************************************************/
int32 OS_GetLocalTime(OS_time_t *time_struct)
{
    int32 status;
    uint32 count;

    UT_Stub_RegisterContext(UT_KEY(OS_GetLocalTime), time_struct);
    status = UT_DEFAULT_IMPL(OS_GetLocalTime);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_GetLocalTime), time_struct, sizeof(*time_struct)) < sizeof(*time_struct))
    {
        count = UT_GetStubCount(UT_KEY(OS_GetLocalTime));
        time_struct->microsecs = 10000 * (count % 100);
        time_struct->seconds = 1 + (count / 100);
    }

    return status;

}/* end OS_GetLocalTime */

/*****************************************************************************
 *
 * Stub function for OS_SetLocalTime()
 *
 *****************************************************************************/
int32 OS_SetLocalTime(OS_time_t *time_struct)
{
    int32 status;

    UT_Stub_RegisterContext(UT_KEY(OS_SetLocalTime), time_struct);
    status = UT_DEFAULT_IMPL(OS_SetLocalTime);

    return status;

} /*end OS_SetLocalTime */

