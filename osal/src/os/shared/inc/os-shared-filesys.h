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
 * \file     os-shared-filesys.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_FILESYS_H_
#define INCLUDE_OS_SHARED_FILESYS_H_

#include <os-shared-globaldefs.h>

/**
 * This flag will be set on the internal record to indicate
 * that the filesystem is "fixed" and therefore not mountable
 * or unmountable by OSAL on the system side.
 *
 * The filesystem should be configured and mounted at the right
 * spot prior to starting OSAL.
 */
#define OS_FILESYS_FLAG_IS_FIXED               0x01


/**
 * This flag will be set on the internal record to indicate
 * that the low level device driver has been started.
 *
 * On Linux, this might mean that the relevant block device
 * module has been loaded and an appropriate /dev entry exists.
 *
 * On VxWorks, this means that the low-level block device
 * is registered in the kernel and XBD layers.
 */
#define OS_FILESYS_FLAG_IS_READY               0x02

/**
 * This flag will be set on the internal record to indicate
 * that the file system is accessible within the underlying
 * operating system, i.e. that the system_mountpt is valid.
 */
#define OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM      0x10

/**
 * This flag will be set on the internal record to indicate
 * that the file system is mounted within the virtualized
 * file system exposed to applications.
 */
#define OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL     0x20

/**
 * These definitions apply to the "type" field within
 * the file system record.  This field may serve as a
 * hint or guidance for the implementation layer as to
 * what type of file system to use when initializing or
 * mounting the file system.
 */
enum
{
    OS_FILESYS_TYPE_UNKNOWN = 0,    /**< Unspecified or unknown file system type */
    OS_FILESYS_TYPE_FS_BASED,       /**< A emulated virtual file system that maps to another file system location */
    OS_FILESYS_TYPE_NORMAL_DISK,    /**< A traditional disk drive or something that emulates one */
    OS_FILESYS_TYPE_VOLATILE_DISK,  /**< A temporary/volatile file system or RAM disk */
    OS_FILESYS_TYPE_MTD,            /**< A "memory technology device" such as FLASH or EEPROM */
    OS_FILESYS_TYPE_MAX
};

/*
 * The data type filled in by the "statvfs" call.
 *
 * This is defined here since there is no public API to get this info,
 * only the total bytes free is accessible via the current OSAL API.
 *
 * However, returning the detailed info at this level means that the
 * more detailed information could be made available with a new API call.
 */
typedef struct
{
    uint32 block_size;
    uint64 total_blocks;
    uint64 blocks_free;
} OS_statvfs_t;


typedef struct
{
    char device_name[OS_FS_DEV_NAME_LEN];      /**< The name of the underlying block device, if applicable */
    char volume_name[OS_FS_VOL_NAME_LEN];
    char system_mountpt[OS_MAX_LOCAL_PATH_LEN]; /**< The name/prefix where the contents are accessible in the host operating system */
    char virtual_mountpt[OS_MAX_PATH_LEN];  /**< The name/prefix in the OSAL Virtual File system exposed to applications */
    char *address;
    uint32 blocksize;
    uint32 numblocks;
    uint8 flags;
    uint8 fstype;
} OS_filesys_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_filesys_internal_record_t        OS_filesys_table[OS_MAX_FILE_SYSTEMS];


/*
 * File system abstraction layer
 */

/*---------------------------------------------------------------------------------------
   Name: OS_FileSysAPI_Init

   Purpose: Initialize the OS-independent layer for file systems

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_FileSysAPI_Init             (void);



/*----------------------------------------------------------------
   Function: OS_FileSysStartVolume_Impl

    Purpose: Starts/Registers a file system on the target

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FileSysStartVolume_Impl (uint32 filesys_id);

/*----------------------------------------------------------------
   Function: OS_FileSysStopVolume_Impl

    Purpose: Stops/Unregisters a file system on the target

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FileSysStopVolume_Impl (uint32 filesys_id);

/*----------------------------------------------------------------
   Function: OS_FileSysFormatVolume_Impl

    Purpose: Formats a file system on the target to prepare it for use

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FileSysFormatVolume_Impl (uint32 filesys_id);

/*----------------------------------------------------------------
   Function: OS_FileSysCheckVolume_Impl

    Purpose: Checks the drives and optionally repairs inconsistencies

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FileSysCheckVolume_Impl (uint32 filesys_id, bool repair);

/*----------------------------------------------------------------
   Function: OS_FileSysStatVolume_Impl

    Purpose: Returns stats about a volume

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FileSysStatVolume_Impl (uint32 filesys_id, OS_statvfs_t *result);

/*----------------------------------------------------------------
   Function: OS_FileSysMountVolume_Impl

    Purpose: mounts a drive

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FileSysMountVolume_Impl (uint32 filesys_id);

/*----------------------------------------------------------------
   Function: OS_FileSysUnmountVolume_Impl

    Purpose: unmounts a drive.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_FileSysUnmountVolume_Impl (uint32 filesys_id);


/*
 * Internal helper functions
 *
 * Not normally invoked outside this unit, except for unit testing
 */

bool OS_FileSys_FindVirtMountPoint(void *ref, uint32 local_id, const OS_common_record_t *obj);
int32 OS_FileSys_SetupInitialParamsForDevice(const char *devname, OS_filesys_internal_record_t *local);
int32 OS_FileSys_Initialize(char *address, const char *fsdevname, const char * fsvolname, uint32 blocksize,
               uint32 numblocks, bool should_format);

#ifndef OSAL_OMIT_DEPRECATED
int32 OS_FileSys_InitLocalFromVolTable(OS_filesys_internal_record_t *local, const OS_VolumeInfo_t *Vol);
#endif

#endif  /* INCLUDE_OS_SHARED_FILESYS_H_ */

