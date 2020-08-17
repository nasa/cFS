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

/* OSAL coverage stub replacement for drv/hdisk/ataDrv.h */
#ifndef _OSAL_STUB_DRV_HDISK_ATADRV_H_
#define _OSAL_STUB_DRV_HDISK_ATADRV_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>
#include <OCS_blkIo.h>

/* ----------------------------------------- */
/* constants normally defined in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */
extern OCS_BLK_DEV *OCS_ataDevCreate(int ctrl, int drive, unsigned int nBlocks, unsigned int blkOffset);



#endif /* _OSAL_STUB_DRV_HDISK_ATADRV_H_ */

