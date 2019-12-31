/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub basic data types */
#ifndef _OSAL_STUB_BLKIO_H_
#define _OSAL_STUB_BLKIO_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>

/* The module and blk_dev types are used in several headers */
typedef struct OCS_BLK_DEV { int bd; }  OCS_BLK_DEV;
typedef OCS_BLK_DEV*                    OCS_BLK_DEV_ID;


#endif /* _OSAL_STUB_BLKIO_H_ */

