/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* OSAL coverage stub replacement for xbdBlkDev.h */
#ifndef _OSAL_STUB_XBDBLKDEV_H_
#define _OSAL_STUB_XBDBLKDEV_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>
#include <OCS_blkIo.h>

/* ----------------------------------------- */
/* constants normally defined in xbdBlkDev.h */
/* ----------------------------------------- */
#define OCS_NULLDEV     ((OCS_device_t)0)

/* ----------------------------------------- */
/* types normally defined in xbdBlkDev.h */
/* ----------------------------------------- */
typedef int OCS_device_t;

/* ----------------------------------------- */
/* prototypes normally declared in xbdBlkDev.h */
/* ----------------------------------------- */
extern OCS_device_t OCS_xbdBlkDevCreateSync  (OCS_BLK_DEV *bd, const char *name);
extern OCS_STATUS OCS_xbdBlkDevDelete  (OCS_device_t dev, OCS_BLK_DEV **ppbd);



#endif /* _OSAL_STUB_XBDBLKDEV_H_ */

