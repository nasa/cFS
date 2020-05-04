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
 * \file     ut-adaptor-filetable-stub.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-filetable-stub.h"

#include <os-vxworks.h>
#include <os-impl-files.h>




OS_VxWorks_filehandle_entry_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];


void*  const UT_FileTableTest_OS_impl_filehandle_table = OS_impl_filehandle_table;
size_t const UT_FileTableTest_OS_impl_filehandle_table_SIZE = sizeof(OS_impl_filehandle_table);
