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
 * \file     os-impl-files.c
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "os-posix.h"
#include "os-impl-files.h"
#include "os-shared-file.h"

/****************************************************************************************
                                     GLOBALS
 ***************************************************************************************/

/*
 * The global file handle table.
 *
 * This is shared by all OSAL entities that perform low-level I/O.
 */
OS_Posix_file_internal_record_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];


/*
 * These two constants (EUID and EGID) are local cache of the
 * euid and egid of the user running the OSAL application.  They
 * assist the "stat" implementation in determination of permissions.
 *
 * For an OS that does not have multiple users, these could be
 * defined as 0.  Otherwise they should be populated via the system
 * geteuid/getegid calls.
 */
uid_t OS_IMPL_SELF_EUID = 0;
gid_t OS_IMPL_SELF_EGID = 0;

/*
 * Flag(s) to set on file handles for regular files
 * This sets all regular filehandles to be non-blocking by default.
 *
 * In turn, the implementation will utilize select() to determine
 * a filehandle readiness to read/write.
 */
const int OS_IMPL_REGULAR_FILE_FLAGS = O_NONBLOCK;


/****************************************************************************************
                         IMPLEMENTATION-SPECIFIC ROUTINES
             These are specific to this particular operating system
 ****************************************************************************************/

/* --------------------------------------------------------------------------------------
    Name: OS_Posix_StreamAPI_Impl_Init

    Purpose: File/Stream subsystem global initialization

    Returns: OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
int32 OS_Posix_StreamAPI_Impl_Init(void)
{
    uint32 local_id;

    /*
     * init all filehandles to -1, which is always invalid.
     * this isn't strictly necessary but helps when debugging.
     */
    for (local_id = 0; local_id <  OS_MAX_NUM_OPEN_FILES; ++local_id)
    {
        OS_impl_filehandle_table[local_id].fd = -1;
    }

    OS_IMPL_SELF_EUID = geteuid();
    OS_IMPL_SELF_EGID = getegid();

    return OS_SUCCESS;
} /* end OS_Posix_StreamAPI_Impl_Init */

