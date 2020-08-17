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

/* OSAL coverage stub replacement for memPartLib.h */
#ifndef _OSAL_STUB_MEMPARTLIB_H_
#define _OSAL_STUB_MEMPARTLIB_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>

/* ----------------------------------------- */
/* constants normally defined in memPartLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in memPartLib.h */
/* ----------------------------------------- */
typedef struct OCS_PART * OCS_PART_ID;

typedef struct
{
    unsigned long numBytesFree;    /* Number of Free Bytes in Partition       */
    unsigned long numBlocksFree;   /* Number of Free Blocks in Partition      */
    unsigned long maxBlockSizeFree;/* Maximum block size that is free.        */
    unsigned long numBytesAlloc;   /* Number of Allocated Bytes in Partition  */
    unsigned long numBlocksAlloc;  /* Number of Allocated Blocks in Partition */
}  OCS_MEM_PART_STATS;



/* ----------------------------------------- */
/* prototypes normally declared in memPartLib.h */
/* ----------------------------------------- */
extern OCS_STATUS OCS_memPartShow(OCS_PART_ID partId,            /* partition ID */
                   int     type               /* 0 = statistics, 1 = statistics & list */);

extern OCS_STATUS OCS_memPartInfoGet(OCS_PART_ID          partId,    /* partition ID */
        OCS_MEM_PART_STATS * ppartStats /* partition stats structure */);



extern OCS_PART_ID OCS_memSysPartId;


#endif /* _OSAL_STUB_MEMPARTLIB_H_ */

