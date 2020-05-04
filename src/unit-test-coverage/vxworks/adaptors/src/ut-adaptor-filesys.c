/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     ut-adaptor-filesys.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-filesys.h"

#include <os-vxworks.h>
#include <os-impl-filesys.h>



void*  const UT_Ref_OS_impl_filesys_table = OS_impl_filesys_table;
size_t const UT_Ref_OS_impl_filesys_table_SIZE = sizeof(OS_impl_filesys_table);

void UT_FileSysTest_SetupFileSysEntry(uint32 id, OCS_BLK_DEV *blkdev, OCS_device_t xbddev, uint32 MaxParts)
{
    OS_impl_filesys_table[id].blkDev = blkdev;
    OS_impl_filesys_table[id].xbd = xbddev;
    OS_impl_filesys_table[id].xbdMaxPartitions = MaxParts;
}

