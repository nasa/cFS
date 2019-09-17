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
 *          files in a file system for posix
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"

#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


/****************************************************************************************
                                     GLOBALS
 ***************************************************************************************/

/*
 * The global file handle table.
 *
 * This is shared by all OSAL entities that perform low-level I/O.
 */
OS_Posix_filehandle_entry_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];

/*
 * The directory handle table.
 */
DIR *OS_impl_dir_table[OS_MAX_NUM_OPEN_DIRS];


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

/* --------------------------------------------------------------------------------------
    Name: OS_Posix_DirAPI_Impl_Init

    Purpose: Directory table initialization

    Returns: OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
int32 OS_Posix_DirAPI_Impl_Init(void)
{
   memset(OS_impl_dir_table, 0, sizeof(OS_impl_dir_table));
   return OS_SUCCESS;
} /* end OS_Posix_DirAPI_Impl_Init */



                        
/*----------------------------------------------------------------
 *
 * Function: OS_ShellOutputToFile_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ShellOutputToFile_Impl(uint32 file_id, const char* Cmd)
{
   pid_t cpid;
   uint32 local_id;
   int wstat;
   const char *shell = getenv("SHELL");

   if (shell == NULL)
   {
       shell = "/bin/sh";
   }

   cpid = fork();
   if (cpid < 0)
   {
       OS_DEBUG("%s(): Error during fork(): %s\n", __func__, strerror(errno));
       return OS_ERROR;
   }

   if (cpid == 0)
   {
       /* child process */
       dup2(OS_impl_filehandle_table[file_id].fd, STDOUT_FILENO);
       dup2(OS_impl_filehandle_table[file_id].fd, STDERR_FILENO);

       /* close all _other_ filehandles */
       for (local_id = 0; local_id < OS_MAX_NUM_OPEN_FILES; ++local_id)
       {
           if (OS_global_stream_table[local_id].active_id != 0)
           {
               close(OS_impl_filehandle_table[local_id].fd);
           }
       }

       execl(shell, "sh", "-c", Cmd, NULL); /* does not return if successful */
       exit(EXIT_FAILURE);
   }

   if (waitpid(cpid, &wstat, 0) != cpid)
   {
       OS_DEBUG("%s(): Error during waitpid(): %s\n", __func__, strerror(errno));
       return OS_ERROR;
   }

   if (!WIFEXITED(wstat) || WEXITSTATUS(wstat) != 0)
   {
       OS_DEBUG("%s(): Error from child process: %d\n", __func__, wstat);
       return OS_ERROR;
   }

   return OS_SUCCESS;
} /* end OS_ShellOutputToFile_Impl */

