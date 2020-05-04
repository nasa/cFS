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
 * \file     osapi-utstub-bsp.c
 * \author   joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"

/*
 *********************************************************************************
 *          PUBLIC API (application-callable functions)
 *********************************************************************************
 */

/*----------------------------------------------------------------
   OS_BSP_GetArgC
   See full description in header
 ------------------------------------------------------------------*/
uint32 OS_BSP_GetArgC(void)
{
    int32 status = UT_DEFAULT_IMPL(OS_BSP_GetArgC);

    return status;
}

/*----------------------------------------------------------------
   OS_BSP_GetArgV
   See full description in header
 ------------------------------------------------------------------*/
char * const * OS_BSP_GetArgV(void)
{
    void *buffer;
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BSP_GetArgV);
    if (status == 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OS_BSP_GetArgV), &buffer, sizeof(buffer)) < sizeof(buffer))
    {
        buffer = NULL;
    }
    return buffer;
}

