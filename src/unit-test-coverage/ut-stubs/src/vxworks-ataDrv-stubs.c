/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for ataDrv.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_drv_hdisk_ataDrv.h>

static OCS_BLK_DEV LOCAL_ATADEV = { 0 };

OCS_BLK_DEV *OCS_ataDevCreate(int ctrl, int drive, unsigned int nBlocks, unsigned int blkOffset)
{
    int32 Status;
    OCS_BLK_DEV *retval = NULL;

    Status = UT_DEFAULT_IMPL(OCS_ataDevCreate);
    if (Status == 0)
    {
        retval = &LOCAL_ATADEV;
    }

    return retval;
}



