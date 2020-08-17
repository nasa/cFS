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

/* OSAL coverage stub replacement for functions in sys/stat.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_stat.h>


int OCS_fchmod (int fd, OCS_mode_t mode)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_fchmod);

    return Status;
}

int OCS_chmod (const char *path, OCS_mode_t mode)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_chmod);

    return Status;
}

int OCS_mkdir (const char * path, ...)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_mkdir);

    return Status;
}

int OCS_stat (const char * file, struct OCS_stat * buf)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_stat);

    if (Status == 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OCS_stat), buf, sizeof(*buf)) < sizeof(*buf))
    {
        memset(buf, 0, sizeof(*buf));
    }

    return Status;
}

int OCS_statvfs (const char * file, struct OCS_statvfs * buf)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_statvfs);

    if (Status == 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OCS_statvfs), buf, sizeof(*buf)) < sizeof(*buf))
    {
        memset(buf, 0, sizeof(*buf));
    }

    return Status;
}

