/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for xbdBlkDev.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_xbdBlkDev.h>

OCS_device_t OCS_xbdBlkDevCreateSync  (OCS_BLK_DEV *bd, const char *name)
{
    int32 status = UT_DEFAULT_IMPL(OCS_xbdBlkDevCreateSync);
    OCS_device_t res;

    if (status != 0)
    {
        res = OCS_NULLDEV;  /* NULLDEV (0) means error */
    }
    else
    {
        res = (OCS_device_t)0x123456; /* something non-zero means success */
    }

    return res;
}

OCS_STATUS OCS_xbdBlkDevDelete  (OCS_device_t dev, OCS_BLK_DEV **ppbd)
{
    return (UT_DEFAULT_IMPL(OCS_xbdBlkDevDelete));
}

