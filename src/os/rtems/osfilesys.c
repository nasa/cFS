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
 * \file   osfilesys.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file has the apis for all of the making
 *          and mounting type of calls for file systems
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


/****************************************************************************************
                                   Data Types
****************************************************************************************/

typedef struct
{
    char blockdev_name[OS_MAX_PATH_LEN];
    rtems_device_minor_number minor;

    /* other data to pass to "mount" when mounting this disk */
    const char                 *mount_fstype;
    rtems_filesystem_options_t mount_options;
    const void                 *mount_data;

} OS_impl_filesys_internal_record_t;

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/*
 * The implementation-specific file system state table.
 * This keeps record of the RTEMS driver and mount options for each filesystem
 */
OS_impl_filesys_internal_record_t OS_impl_filesys_table[OS_MAX_FILE_SYSTEMS];


/*
 * These external references are for the RTEMS RAM disk device descriptor table
 * This is necessary for the RAM disk. These tables can either be here, or
 * in a RTEMS kernel startup file. In this case, the tables are in the
 * application startup
 *
 * Currently, it does not appear possible to create multiple arbitrary disks
 * The RAM disk driver appears to require these specific variables.
 */
extern rtems_ramdisk_config                    rtems_ramdisk_configuration[];
extern size_t                                  rtems_ramdisk_configuration_size;

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
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStartVolume_Impl (uint32 filesys_id)
{
    OS_filesys_internal_record_t  *local = &OS_filesys_table[filesys_id];
    OS_impl_filesys_internal_record_t *impl = &OS_impl_filesys_table[filesys_id];
    uint32 os_idx;
    int32  return_code;

    return_code = OS_ERR_NOT_IMPLEMENTED;
    memset(impl,0,sizeof(*impl));

    /*
     * Take action based on the type of volume
     */
    switch (local->fstype)
    {
    case OS_FILESYS_TYPE_DEFAULT:
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
        /*
         * This finds the correct driver "minor number"
         * to use for the RAM disk (i.e. /dev/rd<X>)
         */

        /* find a matching entry in the OS ramdisk table,
         * (identified by the location address) */
        for (os_idx = 0; os_idx < rtems_ramdisk_configuration_size; ++os_idx)
        {
            if (rtems_ramdisk_configuration[os_idx].location == local->address)
            {
                impl->minor = os_idx;
                break;
            }
        }

        if (os_idx >= rtems_ramdisk_configuration_size)
        {
            OS_DEBUG("No RAMDISK available at address %p\n", local->address);
            return_code = OS_INVALID_POINTER;
            break;
        }
        if ( local->numblocks > rtems_ramdisk_configuration[os_idx].block_num)
        {
           OS_DEBUG("OSAL: Error: RAM disk too large, %lu blocks requested, %lu available.\n",
                   (unsigned long)local->numblocks,
                   (unsigned long)rtems_ramdisk_configuration[os_idx].block_num);
           return_code = OS_FS_ERROR;
           break;
        }
        if ( local->blocksize != rtems_ramdisk_configuration[os_idx].block_size )
        {
           OS_DEBUG("OSAL: Error: RAM Disk needs a block size of %lu.\n",
                   (unsigned long)rtems_ramdisk_configuration[os_idx].block_size);
           return_code = OS_FS_ERROR;
           break;
        }

        snprintf(impl->blockdev_name, sizeof(impl->blockdev_name), "%s%c", RAMDISK_DEVICE_BASE_NAME, (int)impl->minor + 'a');
        impl->mount_fstype = RTEMS_FILESYSTEM_TYPE_RFS;

        OS_DEBUG("OSAL: RAM disk initialized: volume=%s device=%s address=0x%08lX\n",
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
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStopVolume_Impl (uint32 filesys_id)
{
    /* Currently nothing to do here */
    return OS_SUCCESS;

} /* end OS_FileSysStopVolume_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_FileSysFormatVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
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
    case OS_FILESYS_TYPE_DEFAULT:
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
 *           See prototype in os-impl.h for argument/return detail
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
    ** Mount the Disk
    */
    if ( mount(impl->blockdev_name, local->system_mountpt,
            impl->mount_fstype, impl->mount_options, impl->mount_data) != 0 )
    {
        OS_DEBUG("OSAL: Error: mount of %s to %s failed: %s\n",
                impl->blockdev_name, local->system_mountpt, strerror(errno));
        return OS_FS_ERROR;
    }

    return OS_SUCCESS;

} /* end OS_FileSysMountVolume_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_FileSysUnmountVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysUnmountVolume_Impl (uint32 filesys_id)
{
    OS_filesys_internal_record_t  *local = &OS_filesys_table[filesys_id];

    /*
    ** Try to unmount the disk
    */
    if ( unmount(local->system_mountpt) < 0)
    {
       OS_DEBUG("OSAL: RTEMS unmount of %s failed :%s\n",local->system_mountpt, strerror(errno));
       return OS_FS_ERROR;
    }

    return OS_SUCCESS;

} /* end OS_FileSysUnmountVolume_Impl */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_FileSysStatVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysStatVolume_Impl (uint32 filesys_id, OS_statvfs_t *result)
{
   OS_filesys_internal_record_t  *local = &OS_filesys_table[filesys_id];
   struct statvfs stat_buf;

   if ( statvfs(local->system_mountpt, &stat_buf) != 0 )
   {
       return OS_FS_ERROR;
   }

   result->block_size = stat_buf.f_bsize;
   result->blocks_free = stat_buf.f_bfree;
   result->total_blocks = stat_buf.f_blocks;

   return(OS_FS_SUCCESS);
} /* end OS_FileSysStatVolume_Impl */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_FileSysCheckVolume_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysCheckVolume_Impl (uint32 filesys_id, bool repair)
{
    return OS_ERR_NOT_IMPLEMENTED;
} /* end OS_FileSysCheckVolume_Impl */





