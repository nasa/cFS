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
 * \file     os-impl-filesys.c
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/vfs.h>

#include "os-posix.h"
#include "os-shared-filesys.h"


/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/
const char OS_POSIX_DEVICEFILE_PREFIX[] = "/dev/";

/****************************************************************************************
                                Filesys API
 ***************************************************************************************/

/* --------------------------------------------------------------------------------------
    Name: OS_Posix_FileSysAPI_Impl_Init

    Purpose: Filesystem API global initialization

    Returns: OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
int32 OS_Posix_FileSysAPI_Impl_Init(void)
{
    return OS_SUCCESS;
} /* end OS_Posix_FileSysAPI_Impl_Init */


/*
 * System Level API
 */


/*----------------------------------------------------------------
 *
 * Function: OS_FileSysStartVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStartVolume_Impl (uint32 filesys_id)
{
    OS_filesys_internal_record_t  *local = &OS_filesys_table[filesys_id];
    struct stat stat_buf;
    const char *tmpdir;
    uint32 i;
    enum
    {
        VOLATILE_DISK_LOC_DEV_SHM,
        VOLATILE_DISK_LOC_ENV,
        VOLATILE_DISK_LOC_VARTMP,
        VOLATILE_DISK_LOC_TMP,
        VOLATILE_DISK_LOC_MAX
    };

    /*
     * Determine basic type of filesystem, if not already known
     */
    if (local->fstype == OS_FILESYS_TYPE_UNKNOWN &&
            strncmp(local->device_name, OS_POSIX_DEVICEFILE_PREFIX, sizeof(OS_POSIX_DEVICEFILE_PREFIX)-1) == 0)
    {
        /*
         * If referring to a real device in the /dev filesystem,
         * then assume it is a normal disk.
         */
        local->fstype = OS_FILESYS_TYPE_NORMAL_DISK;
    }

    /*
     * For VOLATILE volumes, there are two options:
     *  - The /dev/shm filesystem, if it exists
     *  - The /tmp filesystem
     *
     * The /dev/shm is preferable because it should actually be a ramdisk, but
     * it is system-specific - should exist on Linux if it is mounted.
     * The /tmp file system might be a regular persistent disk, but should always exist
     * on any POSIX-compliant OS.
     */

    tmpdir = NULL;
    if (local->fstype == OS_FILESYS_TYPE_VOLATILE_DISK)
    {
        /* find a suitable location to keep the volatile disk */
        for (i=0; i <= VOLATILE_DISK_LOC_MAX; ++i)
        {
            switch(i)
            {
            case VOLATILE_DISK_LOC_DEV_SHM:
                /* This is most preferable because it should actually be a ramdisk */
                tmpdir = "/dev/shm";
                break;
            case VOLATILE_DISK_LOC_ENV:
                /* try the TMPDIR environment variable, if set */
                tmpdir = getenv("TMPDIR");
                break;
            case VOLATILE_DISK_LOC_VARTMP:
                /* try /var/tmp directory */
                tmpdir = "/var/tmp";
                break;
            case VOLATILE_DISK_LOC_TMP:
                /* use /tmp directory as a last resort */
                tmpdir = "/tmp";
                break;
            default:
                tmpdir = NULL;
                break;
            }

            if (tmpdir != NULL && stat(tmpdir, &stat_buf) == 0)
            {
                /* check if the user has write permission to the directory */
                if ( (stat_buf.st_mode & S_IWOTH) != 0 ||
                        ((stat_buf.st_mode & S_IWGRP) != 0 && stat_buf.st_gid == getegid()) ||
                        ((stat_buf.st_mode & S_IWUSR) != 0 && stat_buf.st_uid == geteuid()))
                {
                    break;
                }
            }
        }

        if (tmpdir == NULL)
        {
            /* OS provides no place to put the volume */
            OS_DEBUG("No storage location for volatile volumes");
            return OS_FS_ERR_DRIVE_NOT_CREATED;
        }

        snprintf(local->system_mountpt, sizeof(local->system_mountpt),
                "%s/osal:%s", tmpdir, local->volume_name);
    }

    return OS_SUCCESS;

} /* end OS_FileSysStartVolume_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_FileSysStopVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStopVolume_Impl (uint32 filesys_id)
{
    /*
     * This is a no-op.
     *
     * Volatile volumes are just directories created in the temp dir,
     * and this will not remove the directories just in case something
     * went wrong.
     *
     * If the volume is started again, the directory will be re-used.
     */
    return OS_SUCCESS;

} /* end OS_FileSysStopVolume_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FileSysFormatVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysFormatVolume_Impl (uint32 filesys_id)
{
    /*
     * In theory, this should wipe any existing files in the ramdisk,
     * but since ramdisks here are implemented using a directory within a tmpfs,
     * removal of such files could be risky if something goes wrong,
     * because it might remove files that were important.
     *
     * So the safest option is just a no-op.
     * (this is also backward compatible since POSIX mkfs was always a no-op)
     */
    return OS_SUCCESS;

} /* end OS_FileSysFormatVolume_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_FileSysMountVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysMountVolume_Impl (uint32 filesys_id)
{
    OS_filesys_internal_record_t  *local = &OS_filesys_table[filesys_id];
    struct stat stat_buf;

    /*
     * This will do a mkdir() for the mount point if it does
     * not already exist.
     */
    if (stat(local->system_mountpt, &stat_buf) != 0)
    {
        if (mkdir(local->system_mountpt, 0700) < 0)
        {
            OS_DEBUG("ERROR: Cannot create mount point %s: %s",
                    local->system_mountpt, strerror(errno));
            return OS_FS_ERR_DRIVE_NOT_CREATED;
        }
    }
    else if (!S_ISDIR(stat_buf.st_mode) )
    {
        OS_DEBUG("ERROR: Volume %s exists and is not a directory",
                local->system_mountpt);
        return OS_FS_ERR_DRIVE_NOT_CREATED;
    }

    /*
     * NOTE: The mount() system call could be used here to actually
     * mount a disk, if warranted.  For all current POSIX-based PSPs,
     * this is not needed, because the volumes are all pre-mounted
     * through the system init before OSAL starts.
     *
     * For volatile filesystems (ramdisk) these were created within
     * a temp filesystem, so all that is needed is to ensure the
     * mount point exists.  For any other FS type, trigger an
     * error to indicate that it is not implemented in this OSAL.
     */
    if (local->fstype != OS_FILESYS_TYPE_VOLATILE_DISK &&
            local->fstype != OS_FILESYS_TYPE_FS_BASED)
    {
        /* the mount command is not implemented for this FS type */
        return OS_ERR_NOT_IMPLEMENTED;
    }


    return OS_SUCCESS;

} /* end OS_FileSysMountVolume_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FileSysUnmountVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysUnmountVolume_Impl (uint32 filesys_id)
{
    /*
     * NOTE: Mounting/Unmounting on POSIX is not implemented.
     * For backward compatibility this call must return success.
     *
     * This is a no-op.  The mount point that was created during
     * the mount process can stay for the next mount.
     */
    return OS_SUCCESS;

} /* end OS_FileSysUnmountVolume_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_FileSysStatVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStatVolume_Impl (uint32 filesys_id, OS_statvfs_t *result)
{
   OS_filesys_internal_record_t  *local = &OS_filesys_table[filesys_id];
   struct statvfs stat_buf;

   if ( statvfs(local->system_mountpt, &stat_buf) != 0 )
   {
       return OS_ERROR;
   }

   result->block_size = stat_buf.f_bsize;
   result->blocks_free = stat_buf.f_bfree;
   result->total_blocks = stat_buf.f_blocks;

   return(OS_SUCCESS);
} /* end OS_FileSysStatVolume_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_FileSysCheckVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysCheckVolume_Impl (uint32 filesys_id, bool repair)
{
    return OS_ERR_NOT_IMPLEMENTED;
} /* end OS_FileSysCheckVolume_Impl */

