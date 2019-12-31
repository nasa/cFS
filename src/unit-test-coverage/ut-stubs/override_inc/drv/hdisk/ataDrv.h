/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for drv/hdisk/ataDrv.h */
#ifndef _OSAL_OVERRIDE_DRV_HDISK_ATADRV_H_
#define _OSAL_OVERRIDE_DRV_HDISK_ATADRV_H_

#include <OCS_drv_hdisk_ataDrv.h>
#include <vxWorks.h>
#include <blkIo.h>

/* ----------------------------------------- */
/* mappings for declarations in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */
#define ataDevCreate       OCS_ataDevCreate



#endif /* _OSAL_OVERRIDE_DRV_HDISK_ATADRV_H_ */
