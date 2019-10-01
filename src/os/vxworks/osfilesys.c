/*
** File   : osfilesys.c
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
** Purpose: This file has the apis for all of the making
**          and mounting type of calls for file systems
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"

#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stat.h>
#include <ioLib.h>
#include <errnoLib.h>
#include <ramDrv.h>
#include <xbdBlkDev.h>
#include <xbdRamDisk.h>
#include <dosFsLib.h>

#ifdef USE_VXWORKS_ATA_DRIVER
#include "drv/hdisk/ataDrv.h"
#endif

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/****************************************************************************************
                                   Data Types
****************************************************************************************/

typedef struct
{
    BLK_DEV     *blkDev;
    device_t    xbd;
    uint32      xbdMaxPartitions;
} OS_impl_filesys_internal_record_t;

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

OS_impl_filesys_internal_record_t OS_impl_filesys_table[OS_MAX_FILE_SYSTEMS];

/****************************************************************************************
                                    Filesys API
****************************************************************************************/

                        
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
    int32 return_code;

    memset(impl, 0, sizeof (*impl));
    return_code = OS_ERR_NOT_IMPLEMENTED;
    switch(local->fstype)
    {
    case OS_FILESYS_TYPE_VOLATILE_DISK:
    {
        OS_DEBUG("OSAL: Starting a RAM disk at: 0x%08lX\n",(unsigned long)local->address );

        /*
        ** Create the ram disk device
        ** The 32 is the number of blocks per track.
        **  Other values dont seem to work here
        */
        impl->blkDev = ramDevCreate (local->address, local->blocksize , 32 , local->numblocks,  0);
        impl->xbdMaxPartitions = 1;
        break;
    }

#ifdef USE_VXWORKS_ATA_DRIVER
    case OS_FILESYS_TYPE_NORMAL_DISK:
    {
        /*
        ** Create the Flash disk device
        ** This code requires an ATA driver in the BSP, so it must be
        ** left out of the compilation BSPs without.
        */
        OS_DEBUG("OSAL: Starting an ATA DISK: %s\n", local->volume_name);
        impl->xbdMaxPartitions = 4;
        impl->blkDev = ataDevCreate(0, 0, 0, 0);
        break;
    }
#endif

    default:
        break;
    }

    if (impl->xbdMaxPartitions > 0)
    {
        /*
         * This code is common to RAM disks and ATA disks
         * (and anything else that relies on the xbd layer)
         */

        if (impl->blkDev == NULL)
        {
            /* there was an error calling the "DevCreate" function */
            OS_DEBUG("OSAL: Error creating low level block device\n");
            return_code = OS_FS_ERR_DRIVE_NOT_CREATED;
        }
        else
        {
            /*
             * Connect the low level block device to the xbd device
             */
            impl->xbd = xbdBlkDevCreateSync(impl->blkDev, local->volume_name);
            if (impl->xbd == NULLDEV)
            {
                return_code = OS_FS_ERR_DRIVE_NOT_CREATED;
            }
            else
            {
                /*
                 * Always using partition ":0"
                 *
                 * For ATA disks, this is different than the previous implementation
                 * which would try to open() all possible partitions in order until
                 * one was successful.
                 *
                 * From the original OS_GetPhysDeviceName() implementation comments:
                 *
                 *  The disk XBD code will add ":X" to the volume name you give to a disk, where
                 *  X is the partition number. While RAM disks are always 0 ( "RAM:0" ),
                 *  a physical disk such as a compact flash disk can be ":0", or ":1" etc,
                 *  depending on how the disk was partitioned.
                 *
                 * But there are two issues with trying all possible devices like that:
                 *  - Trying with open() actually mounts the filesystem, meaning it must
                 *    have a pre-existing dosFs on it
                 *  - It might not be consistent between devices/cards or even run-to-run.
                 *    (i.e. if a partition was formatted manually and then the software
                 *    restarted, a different block device might get mounted the second time)
                 */
                snprintf(local->system_mountpt, sizeof(local->system_mountpt),
                        "%s:0", local->volume_name);

                return_code = OS_SUCCESS;
            }
        }
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
    OS_impl_filesys_internal_record_t *impl = &OS_impl_filesys_table[filesys_id];

    if (impl->xbdMaxPartitions > 0 && impl->xbd != NULLDEV)
    {
        xbdBlkDevDelete(impl->xbd, NULL);
        impl->xbd = NULLDEV;
        impl->xbdMaxPartitions = 0;
    }

    /*
     * TBD: The VxWorks documentation does not seem to indicate any
     * "DevDelete" operation as the complement to ramDevCreate/ataDevCreate.
     */

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
    int status;

    /*
    ** Call the dos format routine
    */
    status = dosFsVolFormat(local->system_mountpt, DOS_OPT_BLANK, NULL);
    if ( status == -1 )
    {
        OS_DEBUG("OSAL: dosFsVolFormat failed. Errno = %d\n",errnoGet());
        return OS_FS_ERR_DRIVE_NOT_CREATED;
    }

    return OS_SUCCESS;

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
    int32 status;
    int fd;

    /*
     * Calling open() on the physical device path
     * mounts the device.
     */
    fd = open ( local->system_mountpt, O_RDONLY, 0644 );
    if ( fd < 0 )
    {
        status = OS_FS_ERROR;
    }
    else
    {
        status = OS_SUCCESS;
        close(fd);
    }

    return status;

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
    int32 status;
    int fd;

    /*
    ** vxWorks uses an ioctl to unmount
    */
    fd = open ( local->system_mountpt, O_RDONLY, 0644 );
    if ( fd < 0 )
    {
        status = OS_FS_ERROR;
    }
    else
    {
        if ( ioctl( fd, FIOUNMOUNT,0) < 0 )
        {
            status = OS_FS_ERROR;
        }
        else
        {
            status = OS_SUCCESS;
        }

        close(fd);
    }

    return status;

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
   struct statfs stat_buf;
   int return_code;

   if (statfs(local->system_mountpt, &stat_buf) != 0)
   {
       return_code = OS_FS_ERROR;
       memset(result, 0, sizeof(*result));
   }
   else
   {
       result->block_size = stat_buf.f_bsize;
       result->blocks_free = stat_buf.f_bfree;
       result->total_blocks = stat_buf.f_blocks;
       return_code = OS_FS_SUCCESS;
   }

   return return_code;

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
    OS_filesys_internal_record_t  *local = &OS_filesys_table[filesys_id];
    STATUS chk_status;
    int    flags;
    int    fd;

    fd = open (local->system_mountpt, O_RDONLY, 0);
    if (fd < 0)
    {
        return OS_FS_ERROR;
    }

    /* Fix the disk if there are errors */
    if (repair)
    {
        flags = DOS_CHK_REPAIR;
    }
    else
    {
        flags = DOS_CHK_ONLY;
    }

    flags |= DOS_CHK_VERB_SILENT;

    chk_status = ioctl(fd, FIOCHKDSK, flags);

    close(fd);

    if (chk_status != OK)
    {
        return OS_FS_ERROR;
    }

    return OS_FS_SUCCESS;

} /* end OS_FileSysCheckVolume_Impl */




