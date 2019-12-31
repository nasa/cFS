/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for file mode bits
 * this file is shared by several UT replacement headers */
#ifndef _OSAL_UT_OSFILESYS_H_
#define _OSAL_UT_OSFILESYS_H_

#include <common_types.h>
#include <OCS_xbdBlkDev.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

void Osapi_Internal_SetupFileSysEntry(uint32 id, OCS_BLK_DEV *blkdev, OCS_device_t xbddev, uint32 MaxParts);




#endif /* _OSAL_UT_OSFILESYS_H_ */

