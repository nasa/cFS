/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for dirent.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_dirent.h>

struct OCS_DIR
{
    int d;
};

static struct OCS_DIR OCS_LOCAL_DIR;
struct OCS_dirent OCS_LOCAL_DIRENT;

int OCS_closedir (OCS_DIR * dirp)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_closedir);

    return Status;
}

OCS_DIR *OCS_opendir (const char * name)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_opendir);

    if (Status != 0)
    {
        return (OCS_DIR*)0;
    }

    return &OCS_LOCAL_DIR;
}

struct OCS_dirent *OCS_readdir (OCS_DIR * dirp)
{
    int32 Status;
    struct OCS_dirent *retval = (struct OCS_dirent *)0;

    Status = UT_DEFAULT_IMPL(OCS_readdir);

    if (Status == 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OCS_readdir), &retval, sizeof(retval)) < sizeof(retval) &&
            UT_GetStubCount(UT_KEY(OCS_readdir)) < 2)
    {
        memset(&OCS_LOCAL_DIRENT, 0, sizeof(OCS_LOCAL_DIRENT));
        retval = &OCS_LOCAL_DIRENT;
    }

    return retval;
}

void OCS_rewinddir (OCS_DIR * dirp)
{
    UT_DEFAULT_IMPL(OCS_rewinddir);
}


