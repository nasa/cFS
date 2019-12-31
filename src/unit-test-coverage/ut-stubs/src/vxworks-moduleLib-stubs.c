/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for moduleLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_moduleLib.h>

OCS_STATUS OCS_moduleInfoGet(OCS_MODULE_ID moduleId, OCS_MODULE_INFO * pModuleInfo)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_moduleInfoGet);

    if (Status == 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OCS_moduleInfoGet), pModuleInfo, sizeof(*pModuleInfo)) < sizeof(*pModuleInfo))
    {
        memset(pModuleInfo, 0, sizeof(*pModuleInfo));
    }

    return Status;
}
