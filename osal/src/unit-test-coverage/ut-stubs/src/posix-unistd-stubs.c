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

/* OSAL coverage stub replacement for unistd.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_unistd.h>


#define OCS_MAX_RDWR_SIZE       0x01000000  /* 16MB */


int OCS_close (int fd)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_close);

    return Status;
}


OCS_gid_t OCS_getegid (void)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_getegid);

    return Status;
}


OCS_uid_t OCS_geteuid (void)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_geteuid);

    return Status;
}


long int OCS_gethostid (void)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_gethostid);

    return Status;
}


int OCS_gethostname (char * name, size_t len)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_gethostname);

    if (Status == 0 && len > 0)
    {
        strncpy(name, "ut", len-1);
        name[len-1] = 0;
    }

    return Status;
}


OCS_pid_t OCS_getpid (void)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_getpid);

    return Status;
}


OCS_off_t OCS_lseek (int fd, OCS_off_t offset, int whence)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_lseek);

    return Status;
}


OCS_ssize_t OCS_read (int fd, void * buf, size_t n)
{
    int32 Status;
    uint32 CopySize;

    Status = UT_DEFAULT_IMPL_RC(OCS_read, OCS_MAX_RDWR_SIZE);

    if (Status > 0)
    {
        if (Status > n)
        {
            CopySize = n;
        }
        else
        {
            CopySize = Status;
        }

        CopySize = UT_Stub_CopyToLocal(UT_KEY(OCS_read), buf, CopySize);

        if (CopySize != 0)
        {
            Status = CopySize;
        }
        else if (Status <= n)
        {
            memset(buf, 'r', Status);
        }
        else if (UT_GetStubCount(UT_KEY(OCS_read) < 4))
        {
            memset(buf, 'r', n);
            Status = n;
        }
        else
        {
            Status = 0;
        }
    }

    return Status;
}


int OCS_rmdir (const char * path)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_rmdir);

    return Status;
}


long int OCS_sysconf (int name)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sysconf);

    return Status;
}


OCS_ssize_t OCS_write (int fd, const void * buf, size_t n)
{
    int32 Status;
    uint32 CopySize;

    Status = UT_DEFAULT_IMPL_RC(OCS_write, OCS_MAX_RDWR_SIZE);

    if (Status > 0)
    {
        if (Status > n)
        {
            CopySize = n;
        }
        else
        {
            CopySize = Status;
        }

        CopySize = UT_Stub_CopyFromLocal(UT_KEY(OCS_write), buf, CopySize);

        if (CopySize != 0)
        {
            Status = CopySize;
        }
        else if (Status > n)
        {
            Status = n;
        }
    }

    return Status;
}


