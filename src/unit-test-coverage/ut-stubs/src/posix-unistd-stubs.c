/* OSAL coverage stub replacement for unistd.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/unistd.h>


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


