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


UT_DEFAULT_STUB(OS_NetworkAPI_Init,(void))


int32 OS_NetworkGetHostName(char *host_name, uint32 name_len)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_NetworkGetHostName);

    if (status == OS_SUCCESS && name_len > 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OS_NetworkGetHostName), host_name, name_len) == 0)
    {
        strncpy(host_name, "ut", name_len-1);
        host_name[name_len-1] = 0;
    }

    return status;
}

int32 OS_NetworkGetID(void)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_NetworkGetID);

    return status;

}/* end OS_NetworkGetID */



