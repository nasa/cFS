/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for functions in sys/ioctl.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_sys_ioctl.h>

int OCS_ioctl (int fd, unsigned long req, ...)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_ioctl);

    return Status;
}

