/* OSAL coverage stub replacement for functions in sys/ioctl.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/sys/ioctl.h>

int OCS_ioctl (int fd, unsigned long req, ...)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_ioctl);

    return Status;
}

