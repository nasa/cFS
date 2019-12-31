/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* pull in the OSAL configuration */
#include "osconfig.h"


#include "ut-osfilesys.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osfilesys.c"

/* For proper linkage this variable must be instantiated here */
OS_filesys_internal_record_t        OS_filesys_table[OS_MAX_FILE_SYSTEMS];


OS_SharedGlobalVars_t OS_SharedGlobalVars =
      {
            .Initialized = false
      };


void Osapi_Internal_SetupFileSysEntry(uint32 id, OCS_BLK_DEV *blkdev, OCS_device_t xbddev, uint32 MaxParts)
{
    OS_impl_filesys_table[id].blkDev = blkdev;
    OS_impl_filesys_table[id].xbd = xbddev;
    OS_impl_filesys_table[id].xbdMaxPartitions = MaxParts;
}

