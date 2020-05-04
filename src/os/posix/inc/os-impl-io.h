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
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_IO_H_
#define INCLUDE_OS_IMPL_IO_H_

#include <osconfig.h>
#include <common_types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct
{
    int fd;
    bool selectable;
} OS_Posix_file_internal_record_t;

/*
 * The global file handle table.
 *
 * This is shared by all OSAL entities that perform low-level I/O.
 */
extern OS_Posix_file_internal_record_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];



#endif  /* INCLUDE_OS_IMPL_IO_H_ */

