/* OSAL coverage stub replacement for functions in sys/stat.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/sys/stat.h>
#include <overrides/sys/statvfs.h>


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

int OCS_mkdir (const char * path, OCS_mode_t mode)
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

