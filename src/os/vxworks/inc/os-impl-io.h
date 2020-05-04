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
 * \file     os-impl-io.h
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_IO_H_
#define INCLUDE_OS_IMPL_IO_H_

#include <osconfig.h>
#include <common_types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

typedef struct
{
    int fd;
    bool selectable;
} OS_VxWorks_filehandle_entry_t;

/*
 * The global file handle table.
 *
 * This table is shared across multiple units (files, sockets, etc) and they will share
 * the same file handle table from the basic file I/O.
 */
extern OS_VxWorks_filehandle_entry_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];

/*
 * VxWorks needs to cast the argument to "write()" to avoid a warning.
 * This can be turned off in a future version if the vendor fixes the
 * prototype to be standards-compliant
 */
#define GENERIC_IO_CONST_DATA_CAST  (void*)


#endif  /* INCLUDE_OS_IMPL_IO_H_ */

