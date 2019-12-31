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
#ifndef _OSAL_OVERRIDE_XBDBLKDEV_H_
#define _OSAL_OVERRIDE_XBDBLKDEV_H_

#include <OCS_xbdBlkDev.h>
#include <vxWorks.h>
#include <blkIo.h>

/* ----------------------------------------- */
/* mappings for declarations in xbdBlkDev.h */
/* ----------------------------------------- */

#define NULLDEV                   OCS_NULLDEV
#define device_t                  OCS_device_t
#define xbdBlkDevCreateSync       OCS_xbdBlkDevCreateSync
#define xbdBlkDevDelete           OCS_xbdBlkDevDelete



#endif /* _OSAL_OVERRIDE_XBDBLKDEV_H_ */

