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
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"

#include <fcntl.h>
#include <dirent.h>
#include <sys/statvfs.h>

#include <rtems/blkdev.h>
#include <rtems/diskdevs.h>
#include <rtems/error.h>
#include <rtems/fsmount.h>
#include <rtems/ramdisk.h>
#include <rtems/rtems-rfs.h>
#include <rtems/rtems-rfs-format.h>

#include "os-shared-filesys.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                   Data Types
****************************************************************************************/

typedef struct
{
    char blockdev_name[OS_MAX_PATH_LEN];

    struct ramdisk *allocated_disk;

    /* other data to pass to "mount" when mounting this disk */
    const char                 *mount_fstype;
    rtems_filesystem_options_t mount_options;
    const void                 *mount_data;

} OS_impl_filesys_internal_record_t;

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/*
 * The prefix used for "real" device nodes on this platform
 */
const char OS_RTEMS_DEVICEFILE_PREFIX[] = "/dev/";

/*
 * The implementation-specific file system state table.
 * This keeps record of the RTEMS driver and mount options for each filesystem
 */
OS_impl_filesys_internal_record_t OS_impl_filesys_table[OS_MAX_FILE_SYSTEMS];

/****************************************************************************************
                                    Filesys API
 ***************************************************************************************/

/* --------------------------------------------------------------------------------------
    Name: OS_Posix_FileSysAPI_Impl_Init

    Purpose: Filesystem API global initialization

    Returns: OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
int32 OS_Rtems_FileSysAPI_Impl_Init(void)
{
    /* clear the local filesys table */
    memset(OS_impl_filesys_table, 0, sizeof(OS_impl_filesys_table));
    return OS_SUCCESS;
} /* end OS_Rtems_FileSysAPI_Impl_Init */



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
    OS_impl_filesys_internal_record_t *impl = &OS_impl_filesys_table[filesys_id];
    rtems_status_code sc;
    int32  return_code;

    return_code = OS_ERR_NOT_IMPLEMENTED;
    memset(impl,0,sizeof(*impl));

    /*
     * Determine basic type of filesystem, if not already known
     */
    if (local->fstype == OS_FILESYS_TYPE_UNKNOWN &&
            strncmp(local->device_name, OS_RTEMS_DEVICEFILE_PREFIX, sizeof(OS_RTEMS_DEVICEFILE_PREFIX)-1) == 0)
    {
        /*
         * If referring to a real device in the /dev filesystem,
         * then assume it is a normal disk.
         */
        local->fstype = OS_FILESYS_TYPE_NORMAL_DISK;
    }

    /*
     * Take action based on the type of volume
     */
    switch (local->fstype)
    {
    case OS_FILESYS_TYPE_FS_BASED:
    {
        /*
         * This "mount" type is basically not a mount at all,
         * No new filesystem is created, just put the files in a
         * directory under the root FS.
         *
         * This is basically a pass-thru/no-op mode for compatibility
         * with FS_BASED entries in existing volume tables.
         */
        return_code = OS_SUCCESS;
        break;
    }
    case OS_FILESYS_TYPE_VOLATILE_DISK:
    {
        OS_DEBUG("No RAMDISK available at address %p\n", local->address);

        impl->allocated_disk = ramdisk_allocate(
                local->address,
                local->blocksize,
                local->numblocks,
                false
        );

        if (impl->allocated_disk == NULL)
        {
            OS_DEBUG("ramdisk_allocate() failed\n");
            return_code = OS_INVALID_POINTER;
            break;
        }

        impl->mount_fstype = RTEMS_FILESYSTEM_TYPE_RFS;
        impl->mount_options = RTEMS_FILESYSTEM_READ_WRITE;
        snprintf(impl->blockdev_name, sizeof(impl->blockdev_name), "%s%c", RAMDISK_DEVICE_BASE_NAME, (int)filesys_id + 'a');

        sc = rtems_blkdev_create(
                impl->blockdev_name,
                local->blocksize,
                local->numblocks,
                ramdisk_ioctl,
                impl->allocated_disk
        );
        if (sc != RTEMS_SUCCESSFUL)
        {
            OS_DEBUG("rtems_blkdev_create() failed: %s.\n", rtems_status_text(sc));
            return_code = OS_ERROR;
        }


        OS_DEBUG("RAM disk initialized: volume=%s device=%s address=0x%08lX\n",
                local->volume_name, impl->blockdev_name, (unsigned long)local->address);

        return_code = OS_SUCCESS;
        break;
    }
    default:
        break;
    }


    /*
     * If the operation was generally successful but a (real) FS
     * mount point was not supplied, then generate one now.
     *
     * The path will be simply /<VOLNAME>
     */
    if (return_code == OS_SUCCESS && local->system_mountpt[0] == 0)
    {
        snprintf(local->system_mountpt, sizeof(local->system_mountpt), "/%s", local->volume_name);
        OS_DEBUG("OSAL: using mount point %s for %s\n",
                local->system_mountpt, local->volume_name);
    }


    return return_code;

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
    OS_impl_filesys_internal_record_t *impl = &OS_impl_filesys_table[filesys_id];

    /*
     * If this was a dynamically allocated disk, then unlink it.
     */
    if (impl->allocated_disk != NULL)
    {
        unlink(impl->blockdev_name);
    }

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
    OS_filesys_internal_record_t  *local = &OS_filesys_table[filesys_id];
    OS_impl_filesys_internal_record_t *impl = &OS_impl_filesys_table[filesys_id];
    rtems_rfs_format_config config;
    int32  return_code;
    int         sc;

    return_code = OS_ERR_NOT_IMPLEMENTED;

    switch(local->fstype)
    {
    case OS_FILESYS_TYPE_FS_BASED:
    {
        /*
         * In this mode a format is a no-op, as it is simply a directory
         * within an already existing and mounted filesystem.
         *
         * This needs to return success for backward compatibility even
         * though it is inappropriate to "format" this type of FS.
         *
         * It could clear the directory, but that might risk deleting
         * something unintentional, so it is better to leave it alone.
         */
        return_code = OS_SUCCESS;
        break;
    }
    case OS_FILESYS_TYPE_VOLATILE_DISK:
    {
        /*
        ** Format the RAM disk with the RFS file system
        */
        memset (&config, 0, sizeof(config));
        config.inode_overhead = 30;
        sc = rtems_rfs_format(impl->blockdev_name, &config);
        if ( sc < 0 )
        {
           OS_DEBUG("OSAL: Error: RFS format of %s failed: %s\n",
                   impl->blockdev_name, strerror(errno));
           return_code = OS_FS_ERR_DRIVE_NOT_CREATED;
        }
        else
        {
            return_code = OS_SUCCESS;
        }
        break;
    }
    default:
        break;
    }

    return return_code;

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
    OS_impl_filesys_internal_record_t *impl = &OS_impl_filesys_table[filesys_id];
    struct stat stat_buf;

    /*
     * This will do a mkdir() for the mount point if it does
     * not already exist.
     */
    if (stat(local->system_mountpt, &stat_buf) != 0)
    {
        if (mkdir(local->system_mountpt, S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO) < 0)
        {
            OS_DEBUG("ERROR: Cannot create mount point %s: %s\n",
                    local->system_mountpt, strerror(errno));
            return OS_FS_ERR_DRIVE_NOT_CREATED;
        }
    }
    else if (!S_ISDIR(stat_buf.st_mode) )
    {
        OS_DEBUG("ERROR: Volume %s exists and is not a directory\n",
                local->system_mountpt);
        return OS_FS_ERR_DRIVE_NOT_CREATED;
    }

    /*
     * Only do the mount() syscall for real devices.
     * For other types of filesystem mounts (e.g. FS_BASED), this is a no-op
     */
    if (local->fstype == OS_FILESYS_TYPE_VOLATILE_DISK ||
            local->fstype == OS_FILESYS_TYPE_NORMAL_DISK)
    {
        /*
        ** Mount the Disk
        */
        if ( mount(impl->blockdev_name, local->system_mountpt,
                impl->mount_fstype, impl->mount_options, impl->mount_data) != 0 )
        {
            OS_DEBUG("OSAL: Error: mount of %s to %s failed: %s\n",
                    impl->blockdev_name, local->system_mountpt, strerror(errno));
            return OS_ERROR;
        }
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
    OS_filesys_internal_record_t  *local = &OS_filesys_table[filesys_id];

    if (local->fstype == OS_FILESYS_TYPE_VOLATILE_DISK ||
            local->fstype == OS_FILESYS_TYPE_NORMAL_DISK)
    {
        /*
        ** Try to unmount the disk
        */
        if ( unmount(local->system_mountpt) < 0)
        {
           OS_DEBUG("OSAL: RTEMS unmount of %s failed :%s\n",local->system_mountpt, strerror(errno));
           return OS_ERROR;
        }
    }

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
   int32 return_code;

   if ( statvfs(local->system_mountpt, &stat_buf) != 0 )
   {
       /*
        * The ENOSYS error means it is not implemented at the system level.
        * This should translate to the OS_ERR_NOT_IMPLEMENTED OSAL code.
        */
       if (errno == ENOSYS)
       {
           return_code = OS_ERR_NOT_IMPLEMENTED;
       }
       else
       {
           OS_DEBUG("%s: %s\n", local->system_mountpt, strerror(errno));
           return_code = OS_ERROR;
       }
   }
   else
   {
       result->block_size = stat_buf.f_bsize;
       result->blocks_free = stat_buf.f_bfree;
       result->total_blocks = stat_buf.f_blocks;

       return_code = OS_SUCCESS;
   }

   return (return_code);
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





