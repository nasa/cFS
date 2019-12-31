/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for loadLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_loadLib.h>
#include <OCS_unldLib.h>

static OCS_MODULE LOCAL_MODULE = { 0 };

OCS_MODULE_ID OCS_loadModule(int fd, unsigned int symFlag)
{
    int32 Status;
    OCS_MODULE_ID retval;

    Status = UT_DEFAULT_IMPL(OCS_loadModule);
    if (Status == 0)
    {
        retval = &LOCAL_MODULE;
    }
    else
    {
        retval = NULL;
    }

    return retval;
}

OCS_STATUS OCS_unldByModuleId(OCS_MODULE_ID moduleId, int options)
{
    return (UT_DEFAULT_IMPL(OCS_unldByModuleId));
}

