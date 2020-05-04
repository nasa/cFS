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
 * \file     os-impl-filesys.h
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_FILESYS_H_
#define INCLUDE_OS_IMPL_FILESYS_H_

#include <osconfig.h>
#include <common_types.h>
#include <blkIo.h>
#include <xbdBlkDev.h>

typedef struct
{
    BLK_DEV     *blkDev;
    device_t    xbd;
    uint32      xbdMaxPartitions;
} OS_impl_filesys_internal_record_t;


extern OS_impl_filesys_internal_record_t OS_impl_filesys_table[OS_MAX_FILE_SYSTEMS];


#endif  /* INCLUDE_OS_IMPL_FILESYS_H_ */

