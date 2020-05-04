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
 * \file     ut-adaptor-filesys.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_FILESYS_H_
#define INCLUDE_UT_ADAPTOR_FILESYS_H_

#include <common_types.h>
#include <OCS_xbdBlkDev.h>

extern void*  const UT_Ref_OS_impl_filesys_table;
extern size_t const UT_Ref_OS_impl_filesys_table_SIZE;


void UT_FileSysTest_SetupFileSysEntry(uint32 id, OCS_BLK_DEV *blkdev, OCS_device_t xbddev, uint32 MaxParts);


#endif  /* INCLUDE_UT_ADAPTOR_FILESYS_H_ */

