/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   osfileapi.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file Contains all of the api calls for manipulating
 *            files in a file system for RTEMS
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"

#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

#define OS_REDIRECTSTRSIZE           15


/*
 * RTEMS does not have UID/GID so these are defined as 0.
 */
#define OS_IMPL_SELF_EUID       0
#define OS_IMPL_SELF_EGID       0


/****************************************************************************************
                                     GLOBALS
 ***************************************************************************************/

/*
 * Leverage the POSIX-style File I/O as this will mostly work on RTEMS,
 * with the exception that regular files cannot be passed to select(),
 * which means that we should NOT set the O_NONBLOCK flag on filehandles
 * like the standard POSIX OSAL does.
 */
const int OS_IMPL_REGULAR_FILE_FLAGS = 0;

/*
 * The global file handle table.
 *
 * This is shared by all OSAL entities that perform low-level I/O.
 */
/* The file/stream table is referenced by multiple entities, i.e. sockets, select, etc */
OS_Rtems_filehandle_entry_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];

/*
 * The directory handle table.
 */
DIR *OS_impl_dir_table[OS_MAX_NUM_OPEN_DIRS];



/****************************************************************************************
                                     COMMON ROUTINES
 After including the OS/C-library specific include files, the basic UNIX file API is used
 ****************************************************************************************/

/*
 * The "I/O" portable block includes the generic
 * posix-style read/write/seek/close operations
 */
#include "../portable/os-impl-posix-io.c"

/*
 * The "Files" portable block includes impl
 * calls for named files i.e. FileOpen and FileStat
 * This is anything that operates on a pathname.
 */
#include "../portable/os-impl-posix-files.c"

/*
 * The "Dirs" portable block includes impl
 * calls for reading directory contents using
 * posix-style opendir/readdir/closedir etc.
 */
#include "../portable/os-impl-posix-dirs.c"


/****************************************************************************************
                         IMPLEMENTATION-SPECIFIC ROUTINES
             These are specific to this particular operating system
 ****************************************************************************************/

/* --------------------------------------------------------------------------------------
    Name: OS_Rtems_StreamAPI_Impl_Init

    Purpose: File/Stream subsystem global initialization

    Returns: OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
int32 OS_Rtems_StreamAPI_Impl_Init(void)
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

    return OS_SUCCESS;
} /* end OS_Rtems_StreamAPI_Impl_Init */

/* --------------------------------------------------------------------------------------
    Name: OS_Rtems_DirAPI_Impl_Init

    Purpose: Directory table initialization

    Returns: OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
int32 OS_Rtems_DirAPI_Impl_Init(void)
{
   memset(OS_impl_dir_table, 0, sizeof(OS_impl_dir_table));
   return OS_SUCCESS;
} /* end OS_Rtems_DirAPI_Impl_Init */


/* FIXME - need to do something better here */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ShellOutputToFile_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ShellOutputToFile_Impl(uint32 file_id, const char *Cmd)
{
   /*
   ** this is a #define to avoid a 'variable length array' warning
   ** 15 is for the size of the redirection string that is added
   ** to the command
   */
   char LocalCmd [OS_MAX_CMD_LEN + OS_REDIRECTSTRSIZE];
   int32 Result;

   strncpy(LocalCmd,Cmd,OS_MAX_CMD_LEN +OS_REDIRECTSTRSIZE);

   /* Make sure that we are able to access this file */
   fchmod(OS_impl_filehandle_table[file_id].fd, 0666);

   /*
   ** add in the extra chars necessary to perform the redirection
   ** 1 for stdout and 2 for stderr. they are redirected to the
   ** file descriptor passed in
   */
   snprintf(LocalCmd, sizeof(LocalCmd), "%s 1>&%d 2>&%d",
         Cmd,
         OS_impl_filehandle_table[file_id].fd,
         OS_impl_filehandle_table[file_id].fd);

   Result = system(LocalCmd);

   if (Result != 0)
   {
      return OS_FS_ERROR;
   }
   return OS_SUCCESS;
} /* end OS_ShellOutputToFile_Impl */

