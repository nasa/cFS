/*
** File   : osfileapi.c
**
**      Copyright (c) 2004-2015, United States government as represented by the
**      administrator of the National Aeronautics Space Administration.
**      All rights reserved. This software was created at NASA Glenn
**      Research Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used,
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author : Joe Hickey based on original RTEMS implementation by Nicholas Yanchik
**
** Purpose: This file Contains all of the api calls for manipulating
**            files in a file system for RTEMS
**
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"

#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <taskLib.h>
#include <sysLib.h>
#include <shellLib.h>
#include <errnoLib.h>

/* 
 * Leverage the POSIX-style File I/O as this will also work on vxWorks.
 */

/*
 * VxWorks does not have UID/GID so these are defined as 0.
 */
#define OS_IMPL_SELF_EUID 0
#define OS_IMPL_SELF_EGID 0

/*
 * VxWorks needs to cast the argument to "write()" to avoid a warning.
 * This can be turned off in a future version if the vendor fixes the
 * prototype to be standards-compliant
 */
#define GENERIC_IO_CONST_DATA_CAST	(void*)

/*
 * The global file handle table.
 *
 * This is shared by all OSAL entities that perform low-level I/O.
 */
OS_VxWorks_filehandle_entry_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];

/*
 * The directory handle table.
 */
DIR *OS_impl_dir_table[OS_MAX_NUM_OPEN_DIRS];



/*
 * Flag(s) to set on file handles for regular files
 * This sets all regular filehandles to be non-blocking by default.
 *
 * In turn, the implementation will utilize select() to determine
 * a filehandle readiness to read/write.
 */
const int OS_IMPL_REGULAR_FILE_FLAGS = 0;



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
 * VxWorks does not quite follow the POSIX definition
 * for the directory access, so the os-impl-posix-dirs block
 * cannot be used here.  This is similar code but VxWorks
 * defines the e.g. mkdir() system calls differently.
 */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_DirCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirCreate_Impl(const char *local_path, uint32 access)
{
   int32 return_code;

   if ( mkdir(local_path) != OK )
   {
      return_code = OS_FS_ERROR;
   }
   else
   {
      return_code = OS_FS_SUCCESS;
   }

   return return_code;
} /* end OS_DirCreate_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_DirOpen_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirOpen_Impl(uint32 local_id, const char *local_path)
{
   OS_impl_dir_table[local_id] = opendir(local_path);
   if (OS_impl_dir_table[local_id] == NULL)
   {
      return OS_FS_ERROR;
   }
   return OS_FS_SUCCESS;
} /* end OS_DirOpen_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_DirClose_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirClose_Impl(uint32 local_id)
{
   closedir(OS_impl_dir_table[local_id]);
   OS_impl_dir_table[local_id] = NULL;
   return OS_FS_SUCCESS;
} /* end OS_DirClose_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_DirRead_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirRead_Impl(uint32 local_id, os_dirent_t *dirent)
{
   struct dirent *de;

   /* NOTE - the readdir() call is non-reentrant ....
    * However, this is performed while the global dir table lock is taken.
    * Therefore this ensures that only one such call can occur at any given time.
    *
    * Static analysis tools may warn about this because they do not know
    * this function is externally serialized via the global lock.
    */
   /* cppcheck-suppress readdirCalled */
   /* cppcheck-suppress nonreentrantFunctionsreaddir */
   de = readdir(OS_impl_dir_table[local_id]);
   if (de == NULL)
   {
      return OS_FS_ERROR;
   }

   strncpy(dirent->FileName, de->d_name, OS_MAX_PATH_LEN - 1);
   dirent->FileName[OS_MAX_PATH_LEN - 1] = 0;

   return OS_FS_SUCCESS;
} /* end OS_DirRead_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_DirRewind_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirRewind_Impl(uint32 local_id)
{
   rewinddir(OS_impl_dir_table[local_id]);
   return OS_FS_SUCCESS;
} /* end OS_DirRewind_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_DirRemove_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirRemove_Impl(const char *local_path)
{
   if ( rmdir(local_path) < 0 )
   {
      return OS_FS_ERROR;
   }

   return OS_FS_SUCCESS;
} /* end OS_DirRemove_Impl */


/****************************************************************************************
                                     DEFINES
****************************************************************************************/

#define OS_REDIRECTSTRSIZE           15
#define OS_SHELL_TMP_FILE_EXT        ".out"
#define OS_SHELL_TMP_FILE_EXT_LEN    4
#define OS_SHELL_CMD_TASK_STACK_SIZE 16384
#define OS_SHELL_CMD_TASK_PRIORITY   250


                        
/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_StreamAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_StreamAPI_Impl_Init(void)
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
} /* end OS_VxWorks_StreamAPI_Impl_Init */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_DirAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_DirAPI_Impl_Init(void)
{
   memset(OS_impl_dir_table, 0, sizeof(OS_impl_dir_table));
   return OS_SUCCESS;
} /* end OS_VxWorks_DirAPI_Impl_Init */


                        
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
    int32 ReturnCode = OS_FS_ERROR;
    int32 Result = ERROR;
    int32 fdCmd;
    uint32 cmdidx;
    char * shellName;

    /* Create a file to write the command to (or write over the old one) */
    fdCmd = OS_creat(OS_SHELL_CMD_INPUT_FILE_NAME,OS_READ_WRITE);

    if (fdCmd < OS_FS_SUCCESS)
    {
        return OS_FS_ERROR;
    }

    if (OS_ConvertToArrayIndex(fdCmd, &cmdidx) == OS_SUCCESS)
    {
        /* copy the command to the file, and then seek back to the beginning of the file */
        OS_write(fdCmd, Cmd, strlen(Cmd));
        OS_lseek(fdCmd,0,OS_SEEK_SET);

        /* Create a shell task the will run the command in the file, push output to OS_fd */
        Result = shellGenericInit("INTERPRETER=Cmd", 0, NULL, &shellName, false, false,
                OS_impl_filehandle_table[cmdidx].fd,
                OS_impl_filehandle_table[file_id].fd,
                OS_impl_filehandle_table[file_id].fd);
    }

    if (Result == OK)
    {
        /* Wait for the command to terminate */
        do
        {
            taskDelay(sysClkRateGet());
        }
        while (taskNameToId(shellName) != ((TASK_ID)ERROR));

        ReturnCode =  OS_SUCCESS;
    }

    /* Close the file descriptor */
    OS_close(fdCmd);

    return ReturnCode;

} /* end OS_ShellOutputToFile_Impl */

