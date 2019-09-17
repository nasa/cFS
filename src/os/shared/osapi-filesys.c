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
 * \file   osapi-filesys.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/*
 * User defined include files
 */
#include "common_types.h"
#include "os-impl.h"

enum
{
   LOCAL_NUM_OBJECTS = OS_MAX_FILE_SYSTEMS,
   LOCAL_OBJID_TYPE = OS_OBJECT_TYPE_OS_FILESYS
};


/*
 * Internal filesystem state table entries
 */
OS_filesys_internal_record_t OS_filesys_table[LOCAL_NUM_OBJECTS];


#ifndef OS_DISABLE_VOLUME_TABLE
/*
 * This is the volume table reference. It is defined in the BSP/startup code for the board
 * In this implementation it is treated as a "const" -- any dynamic updates such as runtime
 * mount points are handled with an internal table.
 */
extern const OS_VolumeInfo_t OS_VolumeTable[];
#define OS_COMPAT_VOLTAB            OS_VolumeTable
#define OS_COMPAT_VOLTAB_SIZE       NUM_TABLE_ENTRIES
#else
/*
 * Alternatively, this module can work without an OS_VolumeTable at all.
 * In this mode, the PSP/BSP can explicitly instantiate any filesystem mappings
 * using the runtime API call(s) prior to starting the app.
 */
#define OS_COMPAT_VOLTAB            NULL
#define OS_COMPAT_VOLTAB_SIZE       0
#endif

                        
/*----------------------------------------------------------------
 *
 * Function: OS_FileSys_FindVirtMountPoint
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Checks if the filesys table index matches the "virtual_mountpt" field.
 *           Function is Compatible with the Search object lookup routine
 *
 *  Returns: true if the entry matches, false if it does not match
 *
 *-----------------------------------------------------------------*/
static bool OS_FileSys_FindVirtMountPoint(void *ref, uint32 local_id, const OS_common_record_t *obj)
{
    OS_filesys_internal_record_t *rec = &OS_filesys_table[local_id];
    const char *target = (const char*)ref;
    size_t mplen;

    if ((rec->flags & OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL) == 0)
    {
        return false;
    }

    mplen = strlen(rec->virtual_mountpt);
    return (mplen > 0 && strncmp(target, rec->virtual_mountpt, mplen) == 0 &&
            (target[mplen] == '/' || target[mplen] == 0));
} /* end OS_FileSys_FindVirtMountPoint */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_FileSys_InitLocalFromVolTable
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Pre-populates a local filesys table entry from the classic OS_VolumeTable
 *           This provides backward compatibility with existing PSP/BSP implementations.
 *
 *  Returns: OS_SUCCESS on success or appropriate error code.
 *
 *-----------------------------------------------------------------*/
static int32 OS_FileSys_InitLocalFromVolTable(OS_filesys_internal_record_t *local, const OS_VolumeInfo_t *Vol)
{
    int32 return_code = OS_SUCCESS;

    if (isgraph((int)Vol->VolumeName[0]) &&
            strcmp(Vol->DeviceName,"unused") != 0)
    {
        strncpy(local->volume_name, Vol->VolumeName, sizeof(local->volume_name)-1);
    }

    if (isgraph((int)Vol->PhysDevName[0]) &&
            strcmp(Vol->PhysDevName,"unused") != 0)
    {
        strncpy(local->system_mountpt, Vol->PhysDevName, sizeof(local->system_mountpt)-1);
    }

    if (isgraph((int)Vol->MountPoint[0]) &&
            strcmp(Vol->MountPoint,"unused") != 0)
    {
        strncpy(local->virtual_mountpt, Vol->MountPoint, sizeof(local->virtual_mountpt)-1);
    }

    /*
     * For filesystems marked as "FS_BASED" in the classic volume table, these
     * must be mounted and accessible prior to starting OSAL.  OSAL does
     * not directly manage these. The "FIXED" flag is used to indicate this.
     *
     * For other filesystem types, set the "fstype" field within the local
     * record which serves as a hint or guidance to the implementation
     * as to the proper filesystem to use when mounting or initializing the device.
     *
     * Note that the implementation may choose to ignore this guidance entirely,
     * and instead use a system-specific method (such as an /etc/fstab file) which
     * is fine.
     */
    if (Vol->VolumeType == FS_BASED)
    {
        local->flags |= OS_FILESYS_FLAG_IS_FIXED;
    }
    else if (Vol->VolumeType == RAM_DISK || Vol->VolatileFlag)
    {
        local->fstype = OS_FILESYS_TYPE_VOLATILE_DISK;
    }
    else if (Vol->VolumeType == ATA_DISK)
    {
        local->fstype = OS_FILESYS_TYPE_NORMAL_DISK;
    }
    else if (Vol->VolumeType == EEPROM_DISK)
    {
        local->fstype = OS_FILESYS_TYPE_MTD;
    }

    if (!Vol->FreeFlag)
    {
        local->flags |= OS_FILESYS_FLAG_IS_READY;
    }

    if (Vol->IsMounted)
    {
        local->flags |= OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL | OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM;
    }

    /* sanity check on mount points.
     * Issue a warning and drop the entry if it does not check out. */
    if ((local->flags & OS_FILESYS_FLAG_IS_READY) != 0 &&
            local->volume_name[0] == 0)
    {
        OS_DEBUG("WARNING: Filesystem %s has invalid volume name\n", local->device_name);
        return_code = OS_ERROR;
    }

    if ((local->flags & OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM) != 0 &&
            local->system_mountpt[0] == 0)
    {
        OS_DEBUG("WARNING: Filesystem %s has invalid system mount point\n", local->device_name);
        return_code = OS_ERROR;
    }

    if ((local->flags & OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL) != 0 &&
            local->virtual_mountpt[0] == 0)
    {
        OS_DEBUG("WARNING: Filesystem %s has invalid virtual mount point\n", local->device_name);
        return_code = OS_ERROR;
    }

    return return_code;
} /* end OS_FileSys_InitLocalFromVolTable */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_FileSys_SetupInitialParamsForDevice
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Pre-populates a local filesys table entry from the classic OS_VolumeTable
 *           This provides backward compatibility with existing PSP/BSP implementations.
 *
 *  Returns: OS_SUCCESS on success or appropriate error code.
 *
 *-----------------------------------------------------------------*/
static int32 OS_FileSys_SetupInitialParamsForDevice(const char *devname, OS_filesys_internal_record_t *local)
{
    const OS_VolumeInfo_t *Vol;
    int32 return_code;
    uint32 i;

    return_code = OS_ERR_NAME_NOT_FOUND;
    Vol = OS_COMPAT_VOLTAB;
    for (i = 0; i < OS_COMPAT_VOLTAB_SIZE; i++)
    {
        if (strcmp(Vol->DeviceName, devname) == 0)
        {
            return_code = OS_FileSys_InitLocalFromVolTable(local, Vol);
            break;
        }

        ++Vol;
    }

    return return_code;
} /* end OS_FileSys_SetupInitialParamsForDevice */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_FileSys_Initialize
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Implements Common code between the mkfs and initfs calls -
 *           mkfs passes the "should_format" as true and initfs passes as false.
 *
 *  Returns: OS_FS_SUCCESS on creating the disk, or appropriate error code.
 *
 *-----------------------------------------------------------------*/
static int32 OS_FileSys_Initialize(char *address, const char *fsdevname, const char * fsvolname, uint32 blocksize,
               uint32 numblocks, bool should_format)
{
    OS_common_record_t *global;
    OS_filesys_internal_record_t *local;
    int32 return_code;
    uint32 local_id;

    /*
    ** Check parameters
    */
    if ( fsdevname == NULL || fsvolname == NULL )
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    /* check names are not empty strings */
    if( fsdevname[0] == 0 || fsvolname[0] == 0)
    {
        return OS_FS_ERR_PATH_INVALID;
    }

    /* check names are not excessively long strings */
    if( strlen(fsdevname) >= sizeof(local->device_name) ||
            strlen(fsvolname) >= sizeof(local->volume_name))
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, fsdevname, &local_id, &global);
    if (return_code == OS_SUCCESS)
    {
        local = &OS_filesys_table[local_id];

        memset(local, 0, sizeof(*local));
        global->name_entry = local->device_name;
        strcpy(local->device_name, fsdevname);

        /* Get the initial settings from the classic volume table.
         * If this fails, that is OK - because passed-in values get preference anyway */
        if (OS_FileSys_SetupInitialParamsForDevice(fsdevname, local) != OS_SUCCESS)
        {
            /*
             * No known parameters for the device.
             *
             * if address was supplied, assume it is a RAM disk, otherwise
             * assume it is a regular disk.
             */
            if (address == NULL)
            {
                local->fstype = OS_FILESYS_TYPE_NORMAL_DISK;
            }
            else
            {
                local->fstype = OS_FILESYS_TYPE_VOLATILE_DISK;
            }

        }

        /* populate the VolumeName and BlockSize ahead of the Impl call,
         * so the implementation can reference this info if necessary */
        local->blocksize = blocksize;
        local->numblocks = numblocks;
        local->address = address;
        strcpy(local->volume_name, fsvolname);

        return_code = OS_FileSysStartVolume_Impl(local_id);

        if (return_code == OS_SUCCESS)
        {
            /*
             * The "mkfs" call also formats the device.
             * this is the primary difference between mkfs and initfs.
             */
            if (should_format)
            {
                return_code = OS_FileSysFormatVolume_Impl(local_id);
            }

            if (return_code == OS_SUCCESS)
            {
                local->flags |= OS_FILESYS_FLAG_IS_READY;
            }
            else
            {
                /* to avoid leaving in an intermediate state,
                 * this also stops the volume if formatting failed. */
                OS_FileSysStopVolume_Impl(local_id);
            }
        }

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, global, NULL);

    }

    return return_code;

} /* end OS_FileSys_Initialize */


/****************************************************************************************
                                  INITIALIZATION
 ***************************************************************************************/

                        
/*----------------------------------------------------------------
 *
 * Function: OS_FileSysAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysAPI_Init(void)
{
    uint32 i;
    uint32 local_id;
    int32 return_code = OS_SUCCESS;
    OS_common_record_t *global;
    OS_filesys_internal_record_t *local;
    const OS_VolumeInfo_t *Vol;

    memset(OS_filesys_table, 0, sizeof(OS_filesys_table));

    /*
     * For compatibility, migrate active entries of the BSP-provided OS_VolumeTable
     * into the local filesystem table.  In this implementation, the OS_VolumeTable
     * is not actually used at runtime, but it is referenced during this init
     * to initially populate the tables.  This allows existing PSP/BSP packages
     * to continue to work with their existing OS_VolumeTable definition.
     *
     * HOWEVER --- this table definition is problematic in regards to
     * identify a valid/used entry vs. an unused entry.
     *
     * a) Checking "Free" flag is insufficient.  Old implementations would
     *    consider (at least) the "DeviceName" as always valid and use this value,
     *    regardless of the state of the "Free" flag.
     * b) If an entry is all zero (i.e. from a memset() or an array index that was
     *    not explicitly initialized), the "Free" flag becomes false and the
     *    VolumeType field becomes "FS_BASED", so it aliases a valid entry.
     * c) Checking for an empty string in various fields will not work either, as
     *    many existing BSPs initialize the strings in unused fields to a string
     *    such as "unused" or " " (a space).
     *
     * Most existing PSP packages seem to set the device name in unused entries
     * to the special string "unused", whereas a valid entry starts with a slash (/).
     */
    Vol = OS_COMPAT_VOLTAB;
    for (i = 0; i < OS_COMPAT_VOLTAB_SIZE && return_code == OS_SUCCESS; i++)
    {
        if (Vol->DeviceName[0] == '/' && Vol->FreeFlag == false)
        {
            OS_DEBUG("OSAL: Registering VolumeTable map for %s on %s\n", Vol->PhysDevName, Vol->MountPoint);
            return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, Vol->DeviceName, &local_id, &global);
            if (return_code == OS_SUCCESS)
            {
                local = &OS_filesys_table[local_id];

                strncpy(local->device_name, Vol->DeviceName, sizeof(local->device_name)-1);
                global->name_entry = OS_filesys_table[local_id].device_name;

                return_code = OS_FileSys_InitLocalFromVolTable(&OS_filesys_table[local_id], Vol);

                return_code = OS_ObjectIdFinalizeNew(return_code, global, NULL);
            }
        }
        ++Vol;
    }

    return return_code;
} /* end OS_FileSysAPI_Init */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_FileSysAddFixedMap
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileSysAddFixedMap(uint32 *filesys_id, const char *phys_path, const char *virt_path)
{
    OS_common_record_t *global;
    OS_filesys_internal_record_t *local;
    int32 return_code;
    uint32 local_id;
    const char *dev_name;


    /*
     * Validate inputs
     */
    if (phys_path == NULL || virt_path == NULL)
    {
        return OS_INVALID_POINTER;
    }

    if (strlen(phys_path) >= OS_MAX_PATH_LEN ||
            strlen(virt_path) >= OS_MAX_PATH_LEN)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    /*
     * Generate a dev name by taking the basename of the phys_path.
     */
    dev_name = strrchr(phys_path, '/');
    if (dev_name == NULL)
    {
        dev_name = phys_path;
    }
    else
    {
        ++dev_name;
    }

    if (strlen(dev_name) >= OS_MAX_API_NAME)
    {
        return OS_ERR_NAME_TOO_LONG;
    }

    return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, dev_name, &local_id, &global);
    if (return_code == OS_SUCCESS)
    {
        local = &OS_filesys_table[local_id];

        memset(local, 0, sizeof(*local));
        global->name_entry = local->device_name;
        strcpy(local->device_name, dev_name);
        strcpy(local->volume_name, dev_name);
        strcpy(local->system_mountpt, phys_path);
        strcpy(local->virtual_mountpt, virt_path);

        /*
         * mark the entry that it is a fixed disk
         */
        local->flags =
                OS_FILESYS_FLAG_IS_FIXED            |
                OS_FILESYS_FLAG_IS_READY            |
                OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM   |
                OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, global, filesys_id);

    }

    return return_code;
} /* end OS_FileSysAddFixedMap */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_mkfs
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_mkfs (char *address, const char *devname, const char * volname, uint32 blocksize,
               uint32 numblocks)
{
    int32 return_code;

    return_code = OS_FileSys_Initialize(address, devname, volname, blocksize, numblocks, true);

    if (return_code == OS_ERR_INCORRECT_OBJ_STATE ||
            return_code == OS_ERR_NO_FREE_IDS)
    {
        /*
         * This is the historic filesystem-specific error code generated when
         * attempting to mkfs()/initfs() on a filesystem that was
         * already initialized, or of there were no free slots in the table.
         *
         * This code preserved just in case application code was checking for it.
         */
        return_code = OS_FS_ERR_DEVICE_NOT_FREE;
    }

    return return_code;

} /* end OS_mkfs */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_rmfs
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_rmfs (const char *devname)
{
    int32 return_code;
    uint32 local_id;
    OS_common_record_t *global;

    if ( devname == NULL )
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    if ( strlen(devname) >= OS_MAX_API_NAME )
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    return_code = OS_ObjectIdGetByName (OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, devname, &global);
    if (return_code == OS_SUCCESS)
    {
        OS_ObjectIdToArrayIndex(LOCAL_OBJID_TYPE, global->active_id, &local_id);

        /*
         * NOTE: It is likely that if the file system is mounted,
         * this call to stop the volume will fail.
         *
         * It would be prudent to first check the flags to ensure that
         * the filesystem is unmounted first, but this would break
         * compatibility with the existing unit tests.
         */
        return_code = OS_FileSysStopVolume_Impl(local_id);

        /* Free the entry in the master table now while still locked */
        if (return_code == OS_SUCCESS)
        {
           /* Only need to clear the ID as zero is the "unused" flag */
           global->active_id = 0;
        }

        OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
    }
    else
    {
        /*
         * Although OS_ERR_NAME_NOT_FOUND is more descriptive and a
         * better indicator of the real failure, this preserves
         * backward compatibility with unit tests that
         * are checking specifically for the "OS_FS_ERROR" code.
         */
        return_code = OS_FS_ERROR;
    }

    return return_code;
} /* end OS_rmfs */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_initfs
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_initfs (char *address,const char *devname, const char *volname,
                uint32 blocksize, uint32 numblocks)
{
    int32 return_code;

    return_code = OS_FileSys_Initialize(address, devname, volname, blocksize, numblocks, false);

    if (return_code == OS_ERR_INCORRECT_OBJ_STATE ||
            return_code == OS_ERR_NO_FREE_IDS)
    {
        /*
         * This is the historic filesystem-specific error code generated when
         * attempting to mkfs()/initfs() on a filesystem that was
         * already initialized, or of there were no free slots in the table.
         *
         * This code preserved just in case application code was checking for it.
         */
        return_code = OS_FS_ERR_DEVICE_NOT_FREE;
    }

    return return_code;

} /* end OS_initfs */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_mount
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_mount (const char *devname, const char* mountpoint)
{
    int32 return_code;
    uint32 local_id;
    OS_common_record_t *global;
    OS_filesys_internal_record_t *local;


    /* Check parameters */
    if ( devname == NULL || mountpoint == NULL )
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    if( strlen(devname) >= sizeof(local->device_name) ||
            strlen(mountpoint) >= sizeof(local->virtual_mountpt) )
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    return_code = OS_ObjectIdGetByName (OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, devname, &global);
    if (return_code == OS_SUCCESS)
    {
        OS_ObjectIdToArrayIndex(LOCAL_OBJID_TYPE, global->active_id, &local_id);
        local = &OS_filesys_table[local_id];

        /*
         * READY flag should be set (mkfs/initfs must have been called on this FS)
         * MOUNTED SYSTEM/VIRTUAL should always be unset.
         *
         * FIXED flag _should_ always be unset (these don't support mount/unmount)
         * but to support abstraction this is not enforced.
         */
        if ((local->flags & ~OS_FILESYS_FLAG_IS_FIXED) != OS_FILESYS_FLAG_IS_READY)
        {
            /* mount() cannot be used on this file system at this time */
            return_code = OS_ERR_INCORRECT_OBJ_STATE;
        }
        else if ((local->flags & OS_FILESYS_FLAG_IS_FIXED) != 0)
        {
            /* Won't mount if FIXED, but return SUCCESS for compatibility
             * with existing apps/PSP that attempt this operation. */
            return_code = OS_SUCCESS;
        }
        else
        {
            return_code = OS_FileSysMountVolume_Impl(local_id);
        }

        if (return_code == OS_SUCCESS)
        {
            /* mark as mounted in the local table.
             * For now this does both sides (system and virtual) */
            local->flags |= OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL;
            strcpy(local->virtual_mountpt, mountpoint);
        }

        OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
    }


    if (return_code != OS_SUCCESS)
    {
        /*
         * Although OS_ERR_NAME_NOT_FOUND is more descriptive and a
         * better indicator of the real failure, this preserves
         * backward compatibility with unit tests that
         * are checking specifically for the "OS_FS_ERROR" code.
         */
        return_code = OS_FS_ERROR;
    }

    return return_code;

} /* end OS_mount */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_unmount
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_unmount (const char *mountpoint)
{
    int32 return_code;
    uint32 local_id;
    OS_common_record_t *global;
    OS_filesys_internal_record_t *local;

    /* Check parameters */
    if ( mountpoint == NULL )
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    if( strlen(mountpoint) >= sizeof(local->virtual_mountpt) )
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    return_code = OS_ObjectIdGetBySearch(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE,
            OS_FileSys_FindVirtMountPoint, (void*)mountpoint, &global);

    if (return_code == OS_SUCCESS)
    {
        OS_ObjectIdToArrayIndex(LOCAL_OBJID_TYPE, global->active_id, &local_id);
        local = &OS_filesys_table[local_id];

        /*
         * FIXED flag should always be unset (these don't support mount/unmount at all)
         * READY flag should be set (mkfs/initfs must have been called on this FS)
         * MOUNTED SYSTEM/VIRTUAL should always be unset.
         *
         * The FIXED flag is not enforced to support abstraction.
         */
        if ((local->flags & ~OS_FILESYS_FLAG_IS_FIXED) !=
                (OS_FILESYS_FLAG_IS_READY | OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL))
        {
            /* unmount() cannot be used on this file system at this time */
            return_code = OS_ERR_INCORRECT_OBJ_STATE;
        }
        else if ((local->flags & OS_FILESYS_FLAG_IS_FIXED) != 0)
        {
            /* Won't unmount if FIXED, but return SUCCESS for compatibility
             * with existing apps/PSP that attempt this operation. */
            return_code = OS_SUCCESS;
        }
        else
        {
            return_code = OS_FileSysUnmountVolume_Impl(local_id);
        }

        if (return_code == OS_SUCCESS)
        {
            /* mark as mounted in the local table.
             * For now this does both sides (system and virtual) */
            local->flags &= ~(OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM | OS_FILESYS_FLAG_IS_MOUNTED_VIRTUAL);
        }

        OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
    }


    if (return_code != OS_SUCCESS)
    {
        /*
         * Although OS_ERR_NAME_NOT_FOUND is more descriptive and a
         * better indicator of the real failure, this preserves
         * backward compatibility with unit tests that
         * are checking specifically for the "OS_FS_ERROR" code.
         */
        return_code = OS_FS_ERROR;
    }

    return return_code;
} /* end OS_unmount */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_fsBlocksFree
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_fsBlocksFree (const char *name)
{
    int32 return_code;
    OS_statvfs_t statfs;
    uint32 local_id;
    OS_common_record_t *global;

    if ( name == NULL )
    {
        return(OS_FS_ERR_INVALID_POINTER);
    }

    if( strlen(name) >= OS_MAX_PATH_LEN )
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    return_code = OS_ObjectIdGetBySearch(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE,
            OS_FileSys_FindVirtMountPoint, (void*)name, &global);

    if (return_code == OS_SUCCESS)
    {
        OS_ObjectIdToArrayIndex(LOCAL_OBJID_TYPE, global->active_id, &local_id);

        return_code = OS_FileSysStatVolume_Impl(local_id, &statfs);

        OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);

        if (return_code == OS_SUCCESS)
        {
            return_code = statfs.blocks_free;
        }
    }
    else
    {
        /* preserves historical error code */
        return_code = OS_FS_ERR_PATH_INVALID;
    }

    return return_code;

} /* end OS_fsBlocksFree */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_fsBytesFree
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_fsBytesFree (const char *name, uint64 *bytes_free)
{
    int32 return_code;
    OS_statvfs_t statfs;
    uint32 local_id;
    OS_common_record_t *global;

    if ( name == NULL || bytes_free == NULL )
    {
        return(OS_FS_ERR_INVALID_POINTER);
    }

    if( strlen(name) >= OS_MAX_PATH_LEN )
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    return_code = OS_ObjectIdGetBySearch(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE,
            OS_FileSys_FindVirtMountPoint, (void*)name, &global);

    if (return_code == OS_SUCCESS)
    {
        OS_ObjectIdToArrayIndex(LOCAL_OBJID_TYPE, global->active_id, &local_id);

        return_code = OS_FileSysStatVolume_Impl(local_id, &statfs);

        OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);

        if (return_code == OS_SUCCESS)
        {
            *bytes_free = (uint64)statfs.blocks_free * (uint64)statfs.block_size;
        }
    }
    else
    {
        /* preserves historical error code */
        return_code = OS_FS_ERR_PATH_INVALID;
    }

    return return_code;

} /* end OS_fsBytesFree */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_chkfs
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_chkfs (const char *name, bool repair)
{
    uint32 local_id;
    int32 return_code;
    OS_common_record_t *global;

    /*
    ** Check for a null pointer
    */
    if (name == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    /*
    ** Check the length of the volume name
    */
    if ( strlen(name) >= OS_MAX_PATH_LEN )
    {
       return(OS_FS_ERR_PATH_TOO_LONG);
    }

    /* Get a reference lock, as a filesystem check could take some time. */
    return_code = OS_ObjectIdGetBySearch(OS_LOCK_MODE_REFCOUNT, LOCAL_OBJID_TYPE,
            OS_FileSys_FindVirtMountPoint, (void*)name, &global);

    if (return_code == OS_SUCCESS)
    {
        OS_ObjectIdToArrayIndex(LOCAL_OBJID_TYPE, global->active_id, &local_id);

        return_code = OS_FileSysCheckVolume_Impl(local_id, repair);

        OS_ObjectIdRefcountDecr(global);
    }

    return return_code;

} /* end OS_chkfs */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_FS_GetPhysDriveName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FS_GetPhysDriveName(char * PhysDriveName, const char * MountPoint)
{
    uint32 local_id;
    int32 return_code;
    OS_common_record_t *global;
    OS_filesys_internal_record_t *local;

    if (MountPoint == NULL || PhysDriveName == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    if( strlen(MountPoint) >= OS_MAX_PATH_LEN )
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    /* Get a reference lock, as a filesystem check could take some time. */
    return_code = OS_ObjectIdGetBySearch(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE,
            OS_FileSys_FindVirtMountPoint, (void*)MountPoint, &global);

    if (return_code == OS_SUCCESS)
    {
        OS_ObjectIdToArrayIndex(LOCAL_OBJID_TYPE, global->active_id, &local_id);
        local = &OS_filesys_table[local_id];

        if ((local->flags & OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM) != 0)
        {
            strncpy(PhysDriveName, local->system_mountpt, OS_FS_PHYS_NAME_LEN-1);
            PhysDriveName[OS_FS_PHYS_NAME_LEN-1] = 0;
        }
        else
        {
            return_code = OS_ERR_INCORRECT_OBJ_STATE;
        }

        OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
    }
    else
    {
        /* preserves historical (vague) error code */
        return_code = OS_FS_ERROR;
    }


    return return_code;
} /* end OS_FS_GetPhysDriveName */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_GetFsInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
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

   memset(filesys_info, 0, sizeof(*filesys_info));

   filesys_info->MaxFds = OS_MAX_NUM_OPEN_FILES;
   filesys_info->MaxVolumes = OS_MAX_FILE_SYSTEMS;

   OS_Lock_Global_Impl(OS_OBJECT_TYPE_OS_STREAM);

   for ( i = 0; i < OS_MAX_NUM_OPEN_FILES; i++ )
   {
      if ( OS_global_stream_table[i].active_id == 0)
      {
         filesys_info->FreeFds++;
      }
   }

   OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_STREAM);

   OS_Lock_Global_Impl(OS_OBJECT_TYPE_OS_FILESYS);

   for ( i = 0; i < NUM_TABLE_ENTRIES; i++ )
   {
       if ( OS_global_filesys_table[i].active_id == 0)
       {
           filesys_info->FreeVolumes++;
       }
   }

   OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_FILESYS);

   return(OS_FS_SUCCESS);
} /* end OS_GetFsInfo */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TranslatePath
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TranslatePath(const char *VirtualPath, char *LocalPath)
{
    uint32 local_id;
    int32 return_code;
    OS_common_record_t *global;
    OS_filesys_internal_record_t *local;
    size_t SysMountPointLen;
    size_t VirtPathLen;
    size_t VirtPathBegin;

    /*
    ** Check to see if the path pointers are NULL
    */
    if (VirtualPath == NULL || LocalPath == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    SysMountPointLen = 0;
    VirtPathLen = strlen(VirtualPath);
    VirtPathBegin = VirtPathLen;

    /*
    ** Check to see if the path is too long
    */
    if (VirtPathLen >= OS_MAX_PATH_LEN)
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

    /* Get a reference lock, as a filesystem check could take some time. */
    return_code = OS_ObjectIdGetBySearch(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE,
            OS_FileSys_FindVirtMountPoint, (void*)VirtualPath, &global);

    if (return_code != OS_SUCCESS)
    {
        return_code = OS_FS_ERR_PATH_INVALID;
    }
    else
    {
        OS_ObjectIdToArrayIndex(LOCAL_OBJID_TYPE, global->active_id, &local_id);
        local = &OS_filesys_table[local_id];

        if ((local->flags & OS_FILESYS_FLAG_IS_MOUNTED_SYSTEM) != 0)
        {
            SysMountPointLen = strlen(local->system_mountpt);
            VirtPathBegin = strlen(local->virtual_mountpt);
            if (SysMountPointLen < OS_MAX_LOCAL_PATH_LEN)
            {
                memcpy(LocalPath, local->system_mountpt, SysMountPointLen);
            }
        }
        else
        {
            return_code = OS_ERR_INCORRECT_OBJ_STATE;
        }

        OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
    }

    if (return_code == OS_SUCCESS)
    {
        if (VirtPathLen < VirtPathBegin)
        {
            return_code = OS_FS_ERR_PATH_INVALID;
        }
        else
        {
            VirtPathLen -= VirtPathBegin;
            if ((SysMountPointLen + VirtPathLen) < OS_MAX_LOCAL_PATH_LEN)
            {
                memcpy(&LocalPath[SysMountPointLen],&VirtualPath[VirtPathBegin], VirtPathLen);
                LocalPath[SysMountPointLen + VirtPathLen] = 0;
            }
            else
            {
                return_code = OS_FS_ERR_PATH_TOO_LONG;
            }
        }
    }

    return return_code;

} /* end OS_TranslatePath */

