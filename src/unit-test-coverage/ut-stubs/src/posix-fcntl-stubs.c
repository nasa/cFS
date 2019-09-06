/* OSAL coverage stub replacement for functions in fcntl.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/fcntl.h>

int OCS_fcntl (int fd, int cmd, ...)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_fcntl);

    return Status;
}

int OCS_open (const char * file, int oflag, ...)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_open);

    return Status;
}


