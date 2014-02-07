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
** Purpose: This file has the api's for all of the making
**          and mounting type of calls for file systems
**
** $Date: 2013/12/16 12:59:48GMT-05:00 $
** $Revision: 1.14 $
** $Log: osfilesys.c  $
** Revision 1.14 2013/12/16 12:59:48GMT-05:00 acudmore 
** Use macros for volume name length and physical device name length.
** Remove use of TranslatePath call in GetPhysDriveName call
** Revision 1.13 2013/07/29 12:08:50GMT-05:00 acudmore 
** Updated paramter checks in all functions
**   removed hardcoded numbers
** Revision 1.12 2013/07/25 14:32:58GMT-05:00 acudmore 
** Added OS_GetFsInfo
** Revision 1.11 2013/07/22 15:53:59GMT-05:00 acudmore 
** conditionally compile debug printfs
** Revision 1.10 2012/12/11 14:04:35GMT-05:00 acudmore 
** updated return codes for consistency
** Revision 1.9 2012/11/28 16:56:16EST acudmore 
** Remove OS X and Cygwin Support
** Revision 1.8 2011/06/27 15:50:19EDT acudmore 
** Went over APIs and Documentation for return code consistency.
** Updated documentation, function comments, and return codes as needed.
** Revision 1.7 2011/05/24 14:11:58EDT acudmore 
** Removed code that makes directories on Linux for file system path mapping.
** File system mapping will now just map to existing directories. These directories should be 
** created before starting the OSAL based application. 
** For example: on OSAL "/cf" might map to "/media/cf0"
** Revision 1.6 2011/05/16 16:04:46EDT acudmore 
** Do not append device to directory name in OS_TranslatePath. 
** This is so the mapping is easier to setup on posix
** Revision 1.5 2010/11/12 12:41:37EST acudmore 
** updated error codes in comments.
** Revision 1.4 2010/11/12 12:00:44EST acudmore 
** replaced copyright character with (c) and added open source notice where needed.
** Revision 1.3 2010/02/23 11:05:00EST acudmore 
** Updated fsBlocksFree and fsBytesFree to use statvfs
** Revision 1.2 2010/02/22 15:50:03EST acudmore 
** Added OS_fsBytesFree to posix port
** Revision 1.1 2010/02/17 13:12:03EST acudmore 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-OSAL-REPOSITORY/src/os/posix/project.pj
** Revision 1.4 2009/08/03 14:14:29EDT acudmore 
** Turned off debug printfs
** Revision 1.3 2009/07/16 13:01:46EDT acudmore 
** Fixed warnings found by ARC.
** Revision 1.2 2009/07/14 14:26:35EDT acudmore 
** Fixed virtual vs. physical filename errors.
** Created a new config parameter and created OS_TranslatePath.
** OS_TranslatePath will replace OS_NameChange, but OS_NameChange is still included for now.
** Revision 1.1 2008/04/20 22:36:04EDT ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-OSAL-REPOSITORY/src/os/linux/project.pj
** Revision 1.1 2007/10/16 16:14:54EDT apcudmore 
** Initial revision
** Member added to project d:/mksdata/MKS-OSAL-REPOSITORY/src/os/linux/project.pj
** Revision 1.11 2007/05/25 09:17:55EDT njyanchik 
** I added the rmfs call to the OSAL and updated the unit test stubs to match
** Revision 1.10 2007/04/24 11:36:38EDT njyanchik 
** I Implemented the followiing fixes:
** Items 1,2,3 are for vxworks 5.5 , so we don't have to change that at all
** Item 4: fixed by adding a check for the length of the volume name (volname) on entry to the function
** Items 5,6, fixed by making the final strcpy a strncpy in OS_NameChange to make sure the string returned is less than or equal to the maximum number of bytes.
** Item 7: fixed by making the first strcpy in OS_NameChange a strncpy to prevent the input from being too long. This way the string length of LocalName won't be too long to use in line 704.
** Item 9: Fixed by making the error number parameter an int32 instead of a uint32
** Revision 1.9 2007/02/27 15:22:02EST njyanchik 
** This CP has the initial import of the new file descripor table mechanism

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

#include "common_types.h"
#include "osapi.h"

#include <sys/vfs.h>
/****************************************************************************************
                                     DEFINES
****************************************************************************************/

# define ERROR (-1)
#undef OS_DEBUG_PRINTF

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

/****************************************************************************************
                                Filesys API
****************************************************************************************/

/*
** System Level API 
*/

/*---------------------------------------------------------------------------------------
    Name: OS_mkfs

    Purpose: Makes a file system on the target

    Returns: OS_FS_ERR_INVALID_POINTER if devname is NULL
             OS_FS_ERR_DRIVE_NOT_CREATED if the OS calls to create the the drive failed
             OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full
             OS_FS_SUCCESS on creating the disk
    
---------------------------------------------------------------------------------------*/

int32 OS_mkfs (char *address, char *devname, char * volname, uint32 blocksize, 
               uint32 numblocks)
{
    int i;
    
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

    /* find an open entry in the Volume Table */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (OS_VolumeTable[i].FreeFlag == TRUE && OS_VolumeTable[i].IsMounted == FALSE
            && strcmp(OS_VolumeTable[i].DeviceName, devname) == 0)
            break;
    }

    if (i >= NUM_TABLE_ENTRIES)
    {
        return OS_FS_ERR_DEVICE_NOT_FREE;
    }

    /* make a disk if it is FS based */
    /*------------------------------- */
    if (OS_VolumeTable[i].VolumeType == FS_BASED)
    {
       /* now enter the info in the table */
       OS_VolumeTable[i].FreeFlag = FALSE;
       strcpy(OS_VolumeTable[i].VolumeName, volname);
       OS_VolumeTable[i].BlockSize = blocksize;
    
    }  
    else
    {
        return OS_FS_ERR_DRIVE_NOT_CREATED;
    }
    return OS_FS_SUCCESS; 
    
} /* end OS_mkfs */

/*---------------------------------------------------------------------------------------
    Name: OS_rmfs

    Purpose: Inititalizes a file system on the target
    
    Returns: OS_FS_ERR_INVALID_POINTER if devname is NULL
             OS_FS_ERROR is the drive specified cannot be located
             OS_FS_SUCCESS on removing  the disk
---------------------------------------------------------------------------------------*/

int32 OS_rmfs (char *devname)
{
    int i;
    int32 ReturnCode;

    if (devname ==NULL)
    {
        ReturnCode =  OS_FS_ERR_INVALID_POINTER;
    }
    else if ( strlen(devname) >= OS_FS_DEV_NAME_LEN )
    {
        ReturnCode = OS_FS_ERR_PATH_TOO_LONG;
    }
    else
    {
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

    }
    return ReturnCode;
}/* end OS_rmfs */

/*---------------------------------------------------------------------------------------
    Name: OS_initfs

    Purpose: Inititalizes a file system on the target

    Returns: OS_FS_ERR_INVALID_POINTER if devname or volname are  NULL
             OS_FS_ERR_PATH_TOO_LONG if the name is too long
             OS_FS_ERR_DEVICE_NOT_FREE if the volume table is full
             OS_FS_ERR_DRIVE_NOT_CREATED on error
             OS_FS_SUCCESS on creating the disk
    
---------------------------------------------------------------------------------------*/
int32 OS_initfs (char *address,char *devname, char *volname, 
                uint32 blocksize, uint32 numblocks)
{
    int i;
   
    if ( devname == NULL || volname == NULL )
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    if( strlen(devname) >= OS_FS_DEV_NAME_LEN || strlen(volname) >= OS_FS_VOL_NAME_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    /* find an open entry in the Volume Table */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (OS_VolumeTable[i].FreeFlag == TRUE && OS_VolumeTable[i].IsMounted == FALSE
            && strcmp(OS_VolumeTable[i].DeviceName, devname) == 0)
            break;
    }

    if (i >= NUM_TABLE_ENTRIES)
    {
        return OS_FS_ERR_DEVICE_NOT_FREE;
    }

    /* make a disk if it is FS based */
    /*------------------------------- */
    if (OS_VolumeTable[i].VolumeType == FS_BASED)
    {
       /* now enter the info in the table */
       OS_VolumeTable[i].FreeFlag = FALSE;
       strcpy(OS_VolumeTable[i].VolumeName, volname);
       OS_VolumeTable[i].BlockSize = blocksize;
    } 
    else
    {
        return OS_FS_ERR_DRIVE_NOT_CREATED;
    }

   return OS_FS_SUCCESS; 

}/* end OS_initfs */

/*--------------------------------------------------------------------------------------
    Name: OS_mount
    
    Purpose: mounts a drive.

---------------------------------------------------------------------------------------*/

int32 OS_mount (const char *devname, char* mountpoint)
{
    int i;

    /* Check parameters */
    if ( devname == NULL || mountpoint == NULL )
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    if( strlen(devname) >= OS_FS_DEV_NAME_LEN || strlen(mountpoint) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    /* find the device in the table */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (OS_VolumeTable[i].FreeFlag == FALSE && OS_VolumeTable[i].IsMounted == FALSE
            && strcmp(OS_VolumeTable[i].DeviceName, devname) == 0)
            break;
    }

    /* make sure we found the device */
    if (i >= NUM_TABLE_ENTRIES)
    {
        return OS_FS_ERROR;
    }

    /* attach the mountpoint */
    strcpy(OS_VolumeTable[i].MountPoint, mountpoint);
    OS_VolumeTable[i].IsMounted = TRUE;

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
    int i;
    
    if (mountpoint == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    if (strlen(mountpoint) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (OS_VolumeTable[i].FreeFlag == FALSE && OS_VolumeTable[i].IsMounted == TRUE
             && strcmp(OS_VolumeTable[i].MountPoint, mountpoint) == 0)
            break;
    }

    /* make sure we found the device */
    if (i >= NUM_TABLE_ENTRIES)
    {
        return OS_FS_ERROR;
    }

    /* release the informationm from the table */
    OS_VolumeTable[i].IsMounted = FALSE;
    strcpy(OS_VolumeTable[i].MountPoint, "");
    
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

   int            status;
   int32          NameStatus;
   struct statvfs stat_buf;
   char           tmpFileName[OS_MAX_LOCAL_PATH_LEN +1];
   
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
   
   status = statvfs(tmpFileName, &stat_buf);
   
   if ( status == 0 )
   {
      return(stat_buf.f_bfree);
   }
   return OS_FS_ERROR;

}/* end OS_fsBlocksFree */

/*--------------------------------------------------------------------------------------
    Name: OS_fsBytesFree

    Purpose: Returns the number of free bytes in a volume
 
    Returns: OS_FS_ERR_INVALID_POINTER if name is NULL
             OS_FS_ERR_PATH_TOO_LONG if the name is too long
             OS_FS_ERROR if the OS call failed
             OS_FS_SUCCESS if success
---------------------------------------------------------------------------------------*/
int32 OS_fsBytesFree (const char *name, uint64 *bytes_free)
{
   int             status;
   int32           NameStatus;
   struct statvfs  stat_buf;
   uint64          bytes_free_local;
   char            tmpFileName[OS_MAX_LOCAL_PATH_LEN +1];

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

   status = statvfs(tmpFileName, &stat_buf);
   if ( status == 0 )
   {
      bytes_free_local = stat_buf.f_bfree * stat_buf.f_bsize;
      *bytes_free = bytes_free_local;
      return(OS_FS_SUCCESS);
   }
   return(OS_FS_ERROR);

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
             when given the OSAL mount point of the drive

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
    char devname [OS_MAX_PATH_LEN];
    char filename[OS_MAX_PATH_LEN];
    int  NumChars;
    int  DeviceLen;
    int  FilenameLen;
    int  i=0;

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
    ** Fill the file and device name to be sure they do not have garbage
    */
    memset((void *)devname,0,OS_MAX_PATH_LEN);
    memset((void *)filename,0,OS_MAX_PATH_LEN);
   
    /* 
    ** We want to find the number of chars to where the second "/" is.
    ** Since we know the first one is in spot 0, we start looking at 1, and go until
    ** we find it.
    */    
    NumChars = 1;
    while ((NumChars <= strlen(VirtualPath)) && (VirtualPath[NumChars] != '/'))
    {
        NumChars++;
    }
    
    /* 
    ** Don't let it overflow to cause a segfault when trying to get the highest level
    ** directory 
    */
    if (NumChars > strlen(VirtualPath))
    {
        NumChars = strlen(VirtualPath);
    }
  
    /*
    ** copy over only the part that is the device name 
    */
    strncpy(devname, VirtualPath, NumChars);
    devname[NumChars] = '\0'; /* Truncate it with a NULL. */
    DeviceLen = strlen(devname);
    
    /*
    ** Copy everything after the devname as the path/filename
    */
    strncpy(filename, &(VirtualPath[NumChars]), OS_MAX_PATH_LEN);
    FilenameLen = strlen(filename);
    
#ifdef OS_DEBUG_PRINTF 
    printf("VirtualPath: %s, Length: %d\n",VirtualPath, (int)strlen(VirtualPath));
    printf("NumChars: %d\n",NumChars);
    printf("devname: %s\n",devname);
    printf("filename: %s\n",filename);
#endif

    /*
    ** look for the dev name we found in the VolumeTable 
    */
    for (i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (OS_VolumeTable[i].FreeFlag == FALSE && 
            strncmp(OS_VolumeTable[i].MountPoint, devname,NumChars) == 0)
        {
            break;
        }
    }

    /* 
    ** Make sure we found a valid drive 
    */
    if (i >= NUM_TABLE_ENTRIES)
    {
        return OS_FS_ERR_PATH_INVALID;
    }
    
    /* 
    ** copy over the physical first part of the drive 
    */
    strncpy(LocalPath,OS_VolumeTable[i].PhysDevName,OS_MAX_LOCAL_PATH_LEN);
    NumChars = strlen(LocalPath);

    /*
    ** Add the file name
    */
    strncat(LocalPath, filename, (OS_MAX_LOCAL_PATH_LEN - NumChars));

#ifdef OS_DEBUG_PRINTF
    printf("Result of TranslatePath = %s\n",LocalPath);
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
    int32 return_code;

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

