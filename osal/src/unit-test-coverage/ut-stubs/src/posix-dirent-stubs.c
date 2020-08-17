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


