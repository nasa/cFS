/*
** File   : osfilesys.c
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author : Nicholas Yanchik / NASA GSFC Code 582.0
**
** Purpose: This file has the apis for all of the making
**          and mounting type of calls for file systems
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/statvfs.h>

#include <rtems.h>
#include <rtems/blkdev.h> 
#include <rtems/diskdevs.h>
#include <rtems/error.h>
#include <rtems/fsmount.h>
#include <rtems/ramdisk.h>
#include <rtems/rtems-rfs.h>
#include <rtems/rtems-rfs-format.h>

#include "common_types.h"
#include "osapi.h"
#include "osconfig.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/
/*
 * Let the IO system allocation the next available major number.
*/
#define RTEMS_DRIVER_AUTO_MAJOR (0)

#undef  OS_DEBUG_PRINTF
/****************************************************************************************
                                   Prototypes
****************************************************************************************/
int32 OS_check_name_length(const char *path);

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/
/* 
** This is the volume table reference. It is defined in the BSP/startup code for the board
*/
extern OS_VolumeInfo_t OS_VolumeTable [NUM_TABLE_ENTRIES]; 

/*
** Fd Table
*/
extern OS_FDTableEntry OS_FDTable[OS_MAX_NUM_OPEN_FILES];

/*
** A semaphore to guard the RTEMS file system calls.
** The RTEMS file system calls are not guarded as of RTEMS 4.10
*/
extern rtems_id        OS_VolumeTableSem;

/*
** These external references are for the RTEMS RAM disk device descriptor table
** This is necessary for the RAM disk. These tables can either be here, or 
** in a RTEMS kernel startup file. In this case, the tables are in the 
** application startup
**
** Currently, it does not appear possible to create multiple arbitrary disks
** The RAM disk driver appears to require these specific variables.
*/
extern rtems_ramdisk_config                    rtems_ramdisk_configuration[];
extern rtems_driver_address_table              rtems_ramdisk_io_ops;
extern const rtems_filesystem_operations_table rtems_rfs_ops;
/****************************************************************************************
                                    Filesys API
****************************************************************************************/
/*
** System Level API 
*/

/*
** Create the RAM disk.
** This currently supports one RAM disk.
*/
int32 rtems_setup_ramdisk (char *phys_dev_name, uint32 *address, uint32 block_size, uint32 num_blocks)
{
  rtems_device_major_number major;
  rtems_status_code         sc;
  uint32                    local_address;
 
  /*
  ** check parameters
  */
  if ( num_blocks == 0 )
  {
     #ifdef OS_DEBUG_PRINTF
        printf("OSAL: Error: Cannot setup RAM disk, No size given.\n");
     #endif
     return(OS_FS_ERROR);
  }
  if ( block_size != 512 )
  {
     #ifdef OS_DEBUG_PRINTF
        printf("OSAL: Error: RAM Disk currently needs a block size of 512.\n");
     #endif
     return(OS_FS_ERROR);
  }
  if ( address == 0 )
  {
       #ifdef OS_DEBUG_PRINTF
          printf("OSAL: RAM disk address is zero: allocating %d bytes from heap\n",
                  (int)(block_size * num_blocks));
       #endif
       local_address = (uint32) malloc (block_size * num_blocks);
       if (!local_address)
       {
          #ifdef OS_DEBUG_PRINTF
             printf ("OSAL: Error: no memory for RAM disk 0\n");
          #endif
          return(OS_FS_ERROR); 
       }
      
       /*
       ** Clear the memory for the disk 
       */
       memset ( (void *)local_address, 0, (block_size * num_blocks));

       /*
       ** Assign the address
       */
       rtems_ramdisk_configuration[0].location = (int *) local_address;
  }
  else
  {
       /*
       ** Assign the address 
       */
       rtems_ramdisk_configuration[0].location = (int *) address;
  } 

  /*
  ** Assign the size
  */
  rtems_ramdisk_configuration[0].block_size =  block_size;
  rtems_ramdisk_configuration[0].block_num =  num_blocks;

  /*
  ** Register the RAM Disk driver.
  */
  sc = rtems_io_register_driver (RTEMS_DRIVER_AUTO_MAJOR,
                                 &rtems_ramdisk_io_ops,
                                 &major);
  if (sc != RTEMS_SUCCESSFUL)
  {
    #ifdef OS_DEBUG_PRINTF
       printf ("OSAL: Error: RAM driver not initialized: %s\n",
               rtems_status_text (sc));
    #endif
    return (OS_FS_ERROR);
  }
  #ifdef OS_DEBUG_PRINTF
     printf ("OSAL: RAM disk initialized: Address = 0x%08X\n",
              (unsigned int)rtems_ramdisk_configuration[0].location);
  #endif

  return(OS_FS_SUCCESS);
}
/*---------------------------------------------------------------------------------------
    Name: OS_mkfs

    Purpose: Makes a RAM disk on the target with the RTEMS RFS file system
    
    Returns: OS_FS_ERR_INVALID_POINTER if devname is NULL
             OS_FS_ERR_DRIVE_NOT_CREATED if the OS calls to create the the drive failed
             OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full
             OS_FS_SUCCESS on creating the disk

    Note: if address == 0, then a malloc will be called to create the disk
---------------------------------------------------------------------------------------*/
int32 OS_mkfs (char *address, char *devname,char * volname, uint32 blocksize, 
               uint32 numblocks)
{
    int                     i;
    uint32                  ReturnCode;
    rtems_rfs_format_config config;
    rtems_status_code       rtems_sc;

    /*
    ** Check parameters
    */
    if ( devname == NULL || volname == NULL )
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    if( strlen(devname) >= OS_FS_DEV_NAME_LEN || strlen(volname) >= OS_FS_VOL_NAME_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }
 
    /*
    ** Lock 
    */
    rtems_sc = rtems_semaphore_obtain (OS_VolumeTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    /* find an open entry in the Volume Table */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (OS_VolumeTable[i].FreeFlag == TRUE && OS_VolumeTable[i].IsMounted == FALSE
            && strcmp(OS_VolumeTable[i].DeviceName, devname) == 0)
            break;
    }

    if (i >= NUM_TABLE_ENTRIES)
    {
        rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);
        return OS_FS_ERR_DEVICE_NOT_FREE;
    }

    /*
    ** Create the RAM disk and format it with the RFS file system.
    ** This requires RTEMS 4.10
    */    
    if (OS_VolumeTable[i].VolumeType == RAM_DISK)
    {
        /*
        ** Create the RAM disk device 
        */
        ReturnCode = rtems_setup_ramdisk (OS_VolumeTable[i].PhysDevName, (uint32 *)address, 
                                          blocksize, numblocks);
        if ( ReturnCode == OS_FS_ERROR )
        {
           ReturnCode = OS_FS_ERR_DRIVE_NOT_CREATED;
        }
        else
        {
           /*
           ** Format the RAM disk with the RFS file system
           */
           memset (&config, 0, sizeof(rtems_rfs_format_config));
           if ( rtems_rfs_format ( OS_VolumeTable[i].PhysDevName, &config ) < 0 )
           {
              #ifdef OS_DEBUG_PRINTF
                 printf("OSAL: Error: RFS format of %s failed: %s\n",
                        OS_VolumeTable[i].PhysDevName, strerror(errno));
              #endif
              ReturnCode = OS_FS_ERR_DRIVE_NOT_CREATED;
           }
           else
           {
              /*
              ** Success
              */
              OS_VolumeTable[i].FreeFlag = FALSE;
              strcpy(OS_VolumeTable[i].VolumeName, volname);
              OS_VolumeTable[i].BlockSize = blocksize;
              ReturnCode = OS_FS_SUCCESS;
           }
        }
    }
    else if (OS_VolumeTable[i].VolumeType == FS_BASED)
    {
       /*
       ** FS_BASED will map the OSAL Volume to an already mounted host filesystem
       */
       
       /* 
       ** Enter the info in the table 
       */
       OS_VolumeTable[i].FreeFlag = FALSE;
       strcpy(OS_VolumeTable[i].VolumeName, volname);
       OS_VolumeTable[i].BlockSize = blocksize;

       ReturnCode = OS_FS_SUCCESS;
    }
    else
    {
        /* 
        ** The VolumeType is something else that is not supported right now 
        */
        ReturnCode = OS_FS_ERROR;
    }

    /*
    ** Unlock
    */
    rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);

    return ReturnCode; 
       
} /* end OS_mkfs */

/*---------------------------------------------------------------------------------------
    Name: OS_rmfs

    Purpose: Removes a file system from the volume table. 
    
    Returns: OS_FS_ERR_INVALID_POINTER if devname is NULL
             OS_FS_ERROR is the drive specified cannot be located
             OS_FS_SUCCESS on removing  the disk
---------------------------------------------------------------------------------------*/
int32 OS_rmfs (char *devname)
{
    int               i;
    int32             ReturnCode;
    rtems_status_code rtems_sc;

    if (devname == NULL)
    {
        ReturnCode =  OS_FS_ERR_INVALID_POINTER;
    }
    else if ( strlen(devname) >= OS_FS_DEV_NAME_LEN )
    {
        ReturnCode = OS_FS_ERR_PATH_TOO_LONG;
    }
    else
    {
        /*
        ** Lock 
        */
        rtems_sc = rtems_semaphore_obtain (OS_VolumeTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

        /* find this entry in the Volume Table */
        for (i = 0; i < NUM_TABLE_ENTRIES; i++)
        {
            if (OS_VolumeTable[i].FreeFlag == FALSE && OS_VolumeTable[i].IsMounted == FALSE
                && strcmp(OS_VolumeTable[i].DeviceName, devname) == 0)
            {
                break;
            }
        }

        /* We can't find that entry in the table */
        if (i >= NUM_TABLE_ENTRIES)
        {
            ReturnCode =  OS_FS_ERROR;
        }
        else
        {
            /* Free this entry in the table */
            OS_VolumeTable[i].FreeFlag = TRUE;
            
            /* desconstruction of the filesystem to come later */
            ReturnCode = OS_FS_SUCCESS;
        }
        /*
        ** Unlock
        */
        rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);
    }
    return ReturnCode;

}/* end OS_rmfs */

/*---------------------------------------------------------------------------------------
    Name: OS_initfs

    Purpose: Inititalizes a file system on the target

    Returns: OS_FS_ERR_INVALID_POINTER if devname is NULL
             OS_FS_DRIVE_NOT_CREATED if the OS calls to create the the drive failed
             OS_FS_SUCCESS on creating the disk
             OS_FS_ERR_PATH_TOO_LONG if the name is too long
             OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full
    
---------------------------------------------------------------------------------------*/
int32 OS_initfs (char *address,char *devname, char *volname, 
                uint32 blocksize, uint32 numblocks)
{
    int               i;
    int32             ReturnCode;
    rtems_status_code rtems_sc;

    if ( devname == NULL || volname == NULL )
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    if( strlen(devname) >= OS_FS_DEV_NAME_LEN || strlen(volname) >= OS_FS_VOL_NAME_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }
 
    /*
    ** Lock 
    */
    rtems_sc = rtems_semaphore_obtain (OS_VolumeTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    /* find an open entry in the Volume Table */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (OS_VolumeTable[i].FreeFlag == TRUE && OS_VolumeTable[i].IsMounted == FALSE
            && strcmp(OS_VolumeTable[i].DeviceName, devname) == 0)
            break;
    }

    if (i >= NUM_TABLE_ENTRIES)
    {
        rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);
        return OS_FS_ERR_DEVICE_NOT_FREE;
    }

    /*
    ** Initialize the RAM disk.
    */    
    if (OS_VolumeTable[i].VolumeType == RAM_DISK)
    {
        #ifdef OS_DEBUG_PRINTF
           printf("OSAL: Re-Initializing a RAM disk at: 0x%08X\n",(unsigned int)address );
        #endif
        /*
        ** Create the RAM disk device. Do not erase the disk! 
        */
        ReturnCode = rtems_setup_ramdisk (OS_VolumeTable[i].PhysDevName, (uint32 *) address, 
                                          blocksize, numblocks);
        if ( ReturnCode != OS_FS_SUCCESS )
        {
           ReturnCode = OS_FS_ERR_DRIVE_NOT_CREATED;
        }
        else
        {
            /*
            ** Success
            */
            OS_VolumeTable[i].FreeFlag = FALSE;
            strcpy(OS_VolumeTable[i].VolumeName, volname);
            OS_VolumeTable[i].BlockSize = blocksize;
            ReturnCode = OS_FS_SUCCESS;
        }
    }
    else if (OS_VolumeTable[i].VolumeType == FS_BASED)
    {
       /* now enter the info in the table */
       OS_VolumeTable[i].FreeFlag = FALSE;
       strcpy(OS_VolumeTable[i].VolumeName, volname);
       OS_VolumeTable[i].BlockSize = blocksize;
    
       /* note we don't know the mount point yet */
           
       ReturnCode = OS_FS_SUCCESS;
    }   
    else
    {
        /* 
        ** VolumeType is something else that is not supported right now 
        */
        ReturnCode = OS_FS_ERROR;
    }

    /*
    ** Unlock
    */
    rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);

    return(ReturnCode);
 
}/* end OS_initfs */

/*--------------------------------------------------------------------------------------
    Name: OS_mount
    
    Purpose: mounts a drive.

---------------------------------------------------------------------------------------*/

int32 OS_mount (const char *devname, char* mountpoint)
{
    int               i;
    rtems_status_code rtems_sc;

    /* Check parameters */
    if ( devname == NULL || mountpoint == NULL )
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    if( strlen(devname) >= OS_FS_DEV_NAME_LEN || strlen(mountpoint) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    /*
    ** Lock 
    */
    rtems_sc = rtems_semaphore_obtain (OS_VolumeTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    /* find the device in the table */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if ((OS_VolumeTable[i].FreeFlag == FALSE )  &&  
            (OS_VolumeTable[i].IsMounted == FALSE ) &&
            (strcmp(OS_VolumeTable[i].DeviceName, devname) == 0))
        {
            break;
        }
    }

    /* Return an error if an un-mounted device was not found */
    if (i >= NUM_TABLE_ENTRIES)
    {
       rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);
       return OS_FS_ERROR;
    }

    if (OS_VolumeTable[i].VolumeType == RAM_DISK)
    {
       /*
       ** Mount the RFS Disk
       ** Note: This code only works with RTEMS 4.10 and up. The mount API has changed with
       **       RTEMS 4.10
       */
       if ( mount(OS_VolumeTable[i].PhysDevName, mountpoint, RTEMS_FILESYSTEM_TYPE_RFS, 0, NULL) != 0 )
       {
           #ifdef OS_DEBUG_PRINTF
              printf("OSAL: Error: mount of %s to %s failed: %s\n",
                          OS_VolumeTable[i].PhysDevName,
                          mountpoint, strerror(errno));
           #endif
           rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);
           return OS_FS_ERROR;
       }

       /* attach the mountpoint */
       strcpy(OS_VolumeTable[i].MountPoint, mountpoint);
       OS_VolumeTable[i].IsMounted = TRUE;
   }
   else if ( OS_VolumeTable[i].VolumeType == FS_BASED )
   {
       /* attach the mountpoint */
       strcpy(OS_VolumeTable[i].MountPoint, mountpoint);
       OS_VolumeTable[i].IsMounted = TRUE;
   }
   else
   {
       /* 
       ** VolumeType is not supported right now 
       */
       rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);
       return OS_FS_ERROR;
   }

   /*
   ** Unlock
   */
   rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);

   return OS_FS_SUCCESS;
    
}/* end OS_mount */
/*--------------------------------------------------------------------------------------
    Name: OS_unmount
    
    Purpose: unmounts a drive. and therefore makes all file descriptors pointing into
             the drive obsolete.

    Returns: OS_FS_ERR_INVALID_POINTER if name is NULL
             OS_FS_ERR_PATH_TOO_LONG if the absolute path given is too long
             OS_FS_ERROR if the OS calls failed
             OS_FS_SUCCESS if success
---------------------------------------------------------------------------------------*/
int32 OS_unmount (const char *mountpoint)
{
   char              local_path [OS_MAX_LOCAL_PATH_LEN];
   int32             status;
   rtems_status_code rtems_sc;
   int               i;
    
   if (mountpoint == NULL)
   {
       return OS_FS_ERR_INVALID_POINTER;
   }

   if (strlen(mountpoint) >= OS_MAX_PATH_LEN)
   {
       return OS_FS_ERR_PATH_TOO_LONG;
   }

   status = OS_TranslatePath(mountpoint, (char *)local_path);
    
   /*
   ** Lock 
   */
   rtems_sc = rtems_semaphore_obtain (OS_VolumeTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

   for (i = 0; i < NUM_TABLE_ENTRIES; i++)
   {
       if (OS_VolumeTable[i].FreeFlag == FALSE && OS_VolumeTable[i].IsMounted == TRUE
            && strcmp(OS_VolumeTable[i].MountPoint, mountpoint) == 0)
           break;
   }

   /* make sure we found the device */
   if (i >= NUM_TABLE_ENTRIES)
   {
       #ifdef OS_DEBUG_PRINTF
          printf("OSAL: Error: unmount of %s failed: invalid volume table entry.\n",
                     local_path);
       #endif
       rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);
       return OS_FS_ERROR;
   }

   if (OS_VolumeTable[i].VolumeType == RAM_DISK)
   {
      /*
      ** Try to unmount the disk
      */
      if ( unmount(local_path) < 0) 
      {
         #ifdef OS_DEBUG_PRINTF
            printf("OSAL: RTEMS unmount of %s failed :%s\n",local_path, strerror(errno));
         #endif
         rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);
         return OS_FS_ERROR;
      } 

      /* release the information from the table */
      OS_VolumeTable[i].IsMounted = FALSE;
      strcpy(OS_VolumeTable[i].MountPoint, "");
  
   } 
   else if ( OS_VolumeTable[i].VolumeType == FS_BASED )
   {

      /* release the information from the table */
      OS_VolumeTable[i].IsMounted = FALSE;
      strcpy(OS_VolumeTable[i].MountPoint, "");

   } 
   else
   {
       /* 
       ** VolumeType is not supported right now 
       */
       rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);
       return OS_FS_ERROR;
   }

   /*
   ** Unlock
   */
   rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);
   return OS_FS_SUCCESS;
    
}/* end OS_umount */

/*--------------------------------------------------------------------------------------
    Name: OS_fsBlocksFree

    Purpose: Returns the number of free blocks in a volume
 
    Returns: OS_FS_ERR_INVALID_POINTER if name is NULL
             OS_FS_ERR_PATH_TOO_LONG if the name is too long
             OS_FS_ERROR if the OS call failed
             The number of blocks free in a volume if success
---------------------------------------------------------------------------------------*/
int32 OS_fsBlocksFree (const char *name)
{

   int               status;
   int32             NameStatus;
   struct statvfs    stat_buf;
   rtems_status_code rtems_sc;
   char              tmpFileName[OS_MAX_LOCAL_PATH_LEN +1];
   
   if ( name == NULL )
   {
      return(OS_FS_ERR_INVALID_POINTER);
   }
  
   /*
   ** Check the length of the volume name
   */
   if ( strlen(name) >= OS_MAX_PATH_LEN )
   {
      return(OS_FS_ERR_PATH_TOO_LONG);
   }

 
   /*
   ** Translate the path
   */
   NameStatus = OS_TranslatePath(name, tmpFileName);
   
   /*
   ** Lock 
   */
   rtems_sc = rtems_semaphore_obtain (OS_VolumeTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

   status = statvfs(tmpFileName, &stat_buf);
   
   /*
   ** Unlock
   */
   rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);

   if ( status == 0 )
   {
      return(stat_buf.f_bfree);
   }
   else 
   {
      return OS_FS_ERROR;
   }

}/* end OS_fsBlocksFree */

/*--------------------------------------------------------------------------------------
    Name: OS_fsBytesFree

    Purpose: Returns the number of free bytes in a volume
 
    Returns: OS_FS_ERR_INVALID_POINTER if name is NULL
             OS_FS_ERR_PATH_TOO_LONG if the name is too long
             OS_FS_ERROR if the OS call failed
             OS_FS_SUCCESS if the call succeeds 
---------------------------------------------------------------------------------------*/
int32 OS_fsBytesFree (const char *name, uint64 *bytes_free)
{
   int               status;
   rtems_status_code rtems_sc;
   int32             NameStatus;
   struct statvfs    stat_buf;
   uint64            bytes_free_local;
   char              tmpFileName[OS_MAX_LOCAL_PATH_LEN +1];

   if ( name == NULL || bytes_free == NULL )
   {
      return(OS_FS_ERR_INVALID_POINTER);
   }

   /*
   ** Check the length of the volume name
   */
   if ( strlen(name) >= OS_MAX_PATH_LEN )
   {
      return(OS_FS_ERR_PATH_TOO_LONG);
   }

   /*
   ** Translate the path
   */
   NameStatus = OS_TranslatePath(name, tmpFileName);

   /*
   ** Lock 
   */
   rtems_sc = rtems_semaphore_obtain (OS_VolumeTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

   status = statvfs(tmpFileName, &stat_buf);

   /*
   ** Unlock
   */
   rtems_sc = rtems_semaphore_release (OS_VolumeTableSem);

   if ( status == 0 )
   {
      bytes_free_local = stat_buf.f_bfree * stat_buf.f_bsize;
      *bytes_free = bytes_free_local;
      return(OS_FS_SUCCESS);
   }
   else
   {
      return(OS_FS_ERROR);
   }

}/* end OS_fsBytesFree */

/*--------------------------------------------------------------------------------------
    Name: OS_chkfs
    
    Purpose: Checks the drives for inconsisenties and either repairs it or not

    Returns: OS_FS_ERR_INVALID_POINTER if name is NULL
             OS_FS_SUCCESS if success
             OS_FS_ERROR if the OS calls fail

---------------------------------------------------------------------------------------*/
os_fshealth_t OS_chkfs (const char *name, boolean repair)
{ 
    return OS_FS_UNIMPLEMENTED;

}/* end OS_chkfs */
/*--------------------------------------------------------------------------------------
    Name: OS_FS_GetPhysDriveName
    
    Purpose: Returns the name of the physical volume associated with the drive,
             when given the mount point of the drive

    Returns: OS_FS_ERR_INVALID_POINTER if either  parameter is NULL
             OS_SUCCESS if success
             OS_FS_ERROR if the mountpoint could not be found
---------------------------------------------------------------------------------------*/
int32 OS_FS_GetPhysDriveName(char * PhysDriveName, char * MountPoint)
{
    int32 ReturnCode;
    int   i;
    
    if (MountPoint == NULL || PhysDriveName == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }
  
    if( strlen(PhysDriveName) >= OS_FS_DEV_NAME_LEN || strlen(MountPoint) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }
 
    /*
    ** look for the CFS Mount Point in the VolumeTable 
    */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (OS_VolumeTable[i].FreeFlag == FALSE &&
            strncmp(OS_VolumeTable[i].MountPoint, MountPoint, OS_MAX_PATH_LEN) == 0)
        {
            break;
        }
    }

    /* 
    ** Make sure we found a valid volume table entry 
    */
    if (i >= NUM_TABLE_ENTRIES)
    {
        ReturnCode = OS_FS_ERROR;
    }
    else
    {
       /*
       ** Yes, so copy the physical drive name  
       */
       strncpy(PhysDriveName, OS_VolumeTable[i].PhysDevName,OS_FS_PHYS_NAME_LEN);
       ReturnCode = OS_SUCCESS;
    }

    return ReturnCode;
}/* end OS_FS_GetPhysDriveName */

/*-------------------------------------------------------------------------------------
 * Name: OS_TranslatePath
 * 
 * Purpose: Because of the abstraction of the filesystem across OSes, we have to change
 *          the name of the {file, directory, drive} to be what the OS can actually 
 *          accept
---------------------------------------------------------------------------------------*/
int32 OS_TranslatePath(const char *VirtualPath, char *LocalPath)
{
    /*
    ** Check to see if the path pointers are NULL
    */
    if (VirtualPath == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    if (LocalPath == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    /*
    ** Check to see if the path is too long
    */
    if (strlen(VirtualPath) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    /*
    ** All valid Virtual paths must start with a '/' character
    */
    if ( VirtualPath[0] != '/' )
    {
       return OS_FS_ERR_PATH_INVALID;
    }
 
    /*
    ** In the RTEMS version of the OSAL, the virtual paths are the same
    ** as the physical paths. So translating a path is simply copying it over.
    */
    strncpy(LocalPath, VirtualPath, strlen(VirtualPath));
    LocalPath[strlen(VirtualPath)] = '\0'; /* Truncate it with a NULL. */

    #ifdef OS_DEBUG_PRINTF
       printf("VirtualPath: %s, Length: %d\n",VirtualPath, (int)strlen(VirtualPath));
       printf("LocalPath: %s, Length: %d\n",LocalPath, (int)strlen(LocalPath));
    #endif

    return OS_FS_SUCCESS;
    
} /* end OS_TranslatePath */

/*---------------------------------------------------------------------------------------
    Name: OS_FS_GetErrorName()

    Purpose: a handy debugging tool that will copy the name of the error code to a buffer

    Returns: OS_FS_ERROR if given error number is unknown
              OS_FS_SUCCESS if given error is found and copied to the buffer
--------------------------------------------------------------------------------------- */
int32 OS_FS_GetErrorName(int32 error_num, os_fs_err_name_t * err_name)
{
    os_fs_err_name_t local_name;
    int32            return_code;

    return_code = OS_FS_SUCCESS;
    
    switch (error_num)
    {
        case OS_FS_SUCCESS: 
            strcpy(local_name,"OS_FS_SUCCESS"); break;
        case OS_FS_ERROR: 
            strcpy(local_name,"OS_FS_ERROR"); break;
        case OS_FS_ERR_INVALID_POINTER: 
            strcpy(local_name,"OS_FS_ERR_INVALID_POINTER"); break;
        case OS_FS_ERR_PATH_TOO_LONG: 
            strcpy(local_name,"OS_FS_ERR_PATH_TOO_LONG"); break;
        case OS_FS_ERR_NAME_TOO_LONG: 
            strcpy(local_name,"OS_FS_ERR_NAME_TOO_LONG"); break;
        case OS_FS_UNIMPLEMENTED: 
            strcpy(local_name,"OS_FS_UNIMPLEMENTED"); break;
        case OS_FS_ERR_PATH_INVALID:
            strcpy(local_name,"OS_FS_ERR_PATH_INVALID"); break;
        case OS_FS_ERR_DRIVE_NOT_CREATED: 
            strcpy(local_name,"OS_FS_ERR_DRIVE_NOT_CREATED"); break;
        case OS_FS_ERR_NO_FREE_FDS: 
            strcpy(local_name,"OS_FS_ERR_NO_FREE_FDS"); break;
        case OS_FS_ERR_INVALID_FD: 
            strcpy(local_name,"OS_FS_ERR_INVALID_FD"); break;
        case OS_FS_ERR_DEVICE_NOT_FREE: 
            strcpy(local_name,"OS_FS_DEVICE_NOT_FREE"); break;
        default: strcpy(local_name,"ERROR_UNKNOWN");
            return_code = OS_FS_ERROR;
    }
    strcpy((char*) err_name, local_name);
    return return_code;
}

/*--------------------------------------------------------------------------------------
     Name: OS_GetFsInfo
       
     Purpose: returns information about the file system in an os_fsinfo_t
         
     Returns: OS_FS_ERR_INVALID_POINTER if filesys_info is NULL
              OS_FS_SUCCESS if success

              Note: The information returned is in the structure pointed to by filesys_info         
---------------------------------------------------------------------------------------*/
int32 OS_GetFsInfo(os_fsinfo_t  *filesys_info)
{
   int i;
 
   /*
   ** Check to see if the file pointers are NULL
   */
   if (filesys_info == NULL)
   {
       return OS_FS_ERR_INVALID_POINTER;
   }

   filesys_info->MaxFds = OS_MAX_NUM_OPEN_FILES;
   filesys_info->MaxVolumes = NUM_TABLE_ENTRIES;
 
   filesys_info->FreeFds = 0;
   for ( i = 0; i < OS_MAX_NUM_OPEN_FILES; i++ )
   {
      if ( OS_FDTable[i].IsValid == FALSE)
      {
         filesys_info->FreeFds++;
      }       

   }

   filesys_info->FreeVolumes = 0;
   for ( i = 0; i < NUM_TABLE_ENTRIES; i++ )
   {
      if (OS_VolumeTable[i].FreeFlag == TRUE )
      {
         filesys_info->FreeVolumes++;
      }
   }

   return(OS_FS_SUCCESS);
}
