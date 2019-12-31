/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for memPartLib.h */
#ifndef _OSAL_OVERRIDE_MEMPARTLIB_H_
#define _OSAL_OVERRIDE_MEMPARTLIB_H_

#include <OCS_memPartLib.h>
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in memPartLib.h */
/* ----------------------------------------- */
#define PART_ID         OCS_PART_ID
#define MEM_PART_STATS  OCS_MEM_PART_STATS

#define memPartShow    OCS_memPartShow
#define memPartInfoGet OCS_memPartInfoGet
#define memSysPartId   OCS_memSysPartId


#endif /* _OSAL_OVERRIDE_MEMPARTLIB_H_ */
