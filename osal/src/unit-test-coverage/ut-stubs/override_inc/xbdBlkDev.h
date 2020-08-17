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

