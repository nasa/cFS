/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file   os-impl-posix-files.c
 * \author joseph.p.hickey@nasa.gov
 *
 * This file Contains all of the api calls for manipulating files
 * in a file system / C library that implements the POSIX-style file API
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * Inclusions Defined by OSAL layer.
 *
 * This must include whatever is required to get the prototypes of these functions:
 *
 *   open()
 *   stat()
 *   chmod()
 *   remove()
 *   rename()
 */
#include <string.h>
#include <errno.h>

#include "os-impl-files.h"
#include "os-shared-file.h"


/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/


/****************************************************************************************
                                 Named File API
 ***************************************************************************************/




/*----------------------------------------------------------------
 *
 * Function: OS_FileOpen_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileOpen_Impl(uint32 local_id, const char *local_path, int32 flags, int32 access)
{
   int os_perm;
   int os_mode;

   /*
   ** Check for a valid access mode
   ** For creating a file, OS_READ_ONLY does not make sense
   */
   switch(access)
   {
       case OS_WRITE_ONLY:
          os_perm = O_WRONLY;
           break;
       case OS_READ_ONLY:
          os_perm = O_RDONLY;
           break;
       case OS_READ_WRITE:
          os_perm = O_RDWR;
           break;
       default:
           return OS_ERROR;
   }

   if (flags & OS_FILE_FLAG_CREATE)
   {
      os_perm |= O_CREAT;
   }
   if (flags & OS_FILE_FLAG_TRUNCATE)
   {
      os_perm |= O_TRUNC;
   }

   os_perm |= OS_IMPL_REGULAR_FILE_FLAGS;

   os_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

   OS_impl_filehandle_table[local_id].fd = open(local_path, os_perm, os_mode);

   if (OS_impl_filehandle_table[local_id].fd < 0)
   {
       OS_DEBUG("open(%s): %s\n", local_path, strerror(errno));
       return OS_ERROR;
   }

   /*
    * If the flags included O_NONBLOCK, then
    * enable the "select" call on this handle.
    */
   OS_impl_filehandle_table[local_id].selectable =
           ((os_perm & O_NONBLOCK) != 0);

   return OS_SUCCESS;
} /* end OS_FileOpen_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FileStat_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileStat_Impl(const char *local_path, os_fstat_t *FileStats)
{
   struct stat st;
   mode_t readbits;
   mode_t writebits;
   mode_t execbits;

   if ( stat(local_path, &st) < 0 )
   {
      return OS_ERROR;
   }

   FileStats->FileSize = st.st_size;
   FileStats->FileTime = st.st_mtime;

   /* note that the "fst_mode" member is already zeroed by the caller */
   if (S_ISDIR(st.st_mode))
   {
      FileStats->FileModeBits |= OS_FILESTAT_MODE_DIR;
   }

   /* always check world bits */
   readbits = S_IROTH;
   writebits = S_IWOTH;
   execbits = S_IXOTH;

   if (OS_IMPL_SELF_EUID == st.st_uid)
   {
      /* we own the file so use user bits for simplified perms */
      readbits |= S_IRUSR;
      writebits |= S_IWUSR;
      execbits |= S_IXUSR;
   }

   if (OS_IMPL_SELF_EGID == st.st_gid)
   {
      /* our group owns the file so use group bits for simplified perms */
      readbits |= S_IRGRP;
      writebits |= S_IWGRP;
      execbits |= S_IXGRP;
   }

   if (st.st_mode & readbits)
   {
      FileStats->FileModeBits |= OS_FILESTAT_MODE_READ;
   }
   if (st.st_mode & writebits)
   {
      FileStats->FileModeBits |= OS_FILESTAT_MODE_WRITE;
   }
   if (st.st_mode & execbits)
   {
      FileStats->FileModeBits |= OS_FILESTAT_MODE_EXEC;
   }

   return OS_SUCCESS;
} /* end OS_FileStat_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_FileChmod_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileChmod_Impl(const char *local_path, uint32 access)
{
    mode_t readbits;
    mode_t writebits;
    struct stat st;

    /*
     * In order to preserve any OTHER mode bits,
     * first stat() the file and then modify the st_mode
     * to match the desired access level.
     *
     * In particular, this should preserve the execute bit,
     * which is generally not part of the OSAL API, but
     * is important for the underlying OS.
     */
    if ( stat(local_path, &st) < 0 )
    {
       return OS_ERROR;
    }

    /* always check world bits */
    readbits = S_IROTH;
    writebits = S_IWOTH;

    if (OS_IMPL_SELF_EUID == st.st_uid)
    {
       /* we own the file so use user bits */
       readbits |= S_IRUSR;
       writebits |= S_IWUSR;
    }

    if (OS_IMPL_SELF_EGID == st.st_gid)
    {
       /* our group owns the file so use group bits */
       readbits |= S_IRGRP;
       writebits |= S_IWGRP;
    }

    if (access == OS_WRITE_ONLY || access == OS_READ_WRITE)
    {
        /* set all "write" mode bits */
        st.st_mode |= writebits;
    }
    else
    {
        /* clear all "write" mode bits */
        st.st_mode &= ~writebits;
    }

    if (access == OS_READ_ONLY || access == OS_READ_WRITE)
    {
        /* set all "read" mode bits */
        st.st_mode |= readbits;
    }
    else
    {
        /* clear all "read" mode bits */
        st.st_mode &= ~readbits;
    }

    /* finally, write the modified mode back to the file */
    if ( chmod(local_path, st.st_mode) < 0 )
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;

} /* end OS_FileChmod_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FileRemove_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileRemove_Impl(const char *local_path)
{
   if ( remove (local_path) < 0 )
   {
      return OS_ERROR;
   }

   return OS_SUCCESS;
} /* end OS_FileRemove_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FileRename_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileRename_Impl(const char *old_path, const char *new_path)
{
   if ( rename (old_path, new_path) < 0 )
   {
      return OS_ERROR;
   }

   return OS_SUCCESS;
} /* end OS_FileRename_Impl */

