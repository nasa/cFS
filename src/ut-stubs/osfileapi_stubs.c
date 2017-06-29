/*
**
** File: osfileapi_stubs.c
**
** Copyright 2012-2013 United States Government as represented by the 
** Administrator of the National Aeronautics and Space Administration. 
** All Other Rights Reserved.  
**
** This software was created at NASA's Goddard Space Flight Center.
** This software is governed by the NASA Open Source Agreement and may be 
** used, distributed and modified only pursuant to the terms of that 
** agreement.
**
** Purpose: Unit test stubs for the OSAPI File Services routines
**
*/

#include <string.h>

#include "osapi.h"
#include "utstubs.h"
#include "utassert.h"
#include "uttools.h"

/*
** Standard File system API
*/
int32 OS_mkfs(char *address, char *devname, char * volname, uint32 blocksize,
              uint32 numblocks)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_mkfs);

    return status;
}

int32 OS_rmfs(char *devname)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_rmfs);

    return status;
}

int32 OS_mount(const char *devname, char* mountpoint)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_mount);

    return status;
}

int32 OS_unmount(const char *mountpoint)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_unmount);

    return status;
}

int32 OS_initfs(char *address, char *devname, char *volname,
                uint32 blocksize, uint32 numblocks)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_initfs);

    return status;
}


int32 OS_chmod  (const char *path, uint32 access)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_chmod);

    return Status;
}

int32 OS_stat   (const char *path, os_fstat_t *filestats)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_stat);

    return Status;
}

int32 OS_lseek(int32 filedes, int32 offset, uint32 whence)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_lseek);

    if (status == OS_SUCCESS)
    {
        status = offset;
    }

    return status;
}

int32 OS_remove (const char *path)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_remove);

    return Status;
} 

int32 OS_rename (const char *old, const char *new)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_rename);

    return Status;
}

int32 OS_cp (const char *src, const char *dest)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_cp);

    return Status;
}

int32 OS_mv (const char *src, const char *dest)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_mv);

    return Status;
}

/*
** Directory API
*/

os_dirp_t OS_opendir (const char *path)
{
    int32 Status;
    os_dirp_t Dirp;

    Status = UT_DEFAULT_IMPL(OS_opendir);

    if (Status == OS_SUCCESS)
    {
        /* Create a nonzero value.  Note that if the test code actually dereferences
         * this, it'll crash, but it should ONLY call other stubs which do not dereference it.
         *
         * This API will be replaced by an alternate API that can return status codes
         * just like the rest of OSAL.
         */
        Dirp = (os_dirp_t)(0xDEADBEEFU);
    }
    else
    {
        /* The OS_opendir API does not have a method to return error codes, just NULL */
        Dirp = NULL;
    }

    return Dirp;
}

int32 OS_closedir (os_dirp_t directory)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_closedir);

    return Status;
}

os_dirent_t *  OS_readdir (os_dirp_t directory)
{
    static os_dirent_t DefaultEntry;
    os_dirent_t *DirentPtr;
    int32 Status;
    uint32 CopySize;

    Status = UT_DEFAULT_IMPL(OS_readdir);

    if (Status == OS_SUCCESS)
    {
        /* The test code may register pointers to return as os_dirent_t* values */
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_readdir), (uint8 *)&DirentPtr, sizeof(DirentPtr));
        if (CopySize < sizeof(DirentPtr))
        {
            memset(&DefaultEntry, 0, sizeof(DefaultEntry));
            DirentPtr = &DefaultEntry;
        }
    }
    else
    {
        /* The OS_readdir API does not have a method to return error codes, just NULL */
        DirentPtr = NULL;
    }


    return DirentPtr;
}

void OS_rewinddir(os_dirp_t directory)
{
    /* Call the default impl so hooks will work */
    UT_DEFAULT_IMPL(OS_rewinddir);
}

int32 OS_mkdir (const char *path, uint32 access)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_mkdir);

    return Status;
}

int32  OS_rmdir (const char *path)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_rmdir);

    return Status;
}

int32 OS_ShellOutputToFile(char* Cmd, int32 OS_fd)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_ShellOutputToFile);

    return status;
}

int32 OS_fsBlocksFree(const char *name)
{
    int32 status;

    status = UT_DEFAULT_IMPL_RC(OS_fsBlocksFree, 100);

    return status;
}

/*
** Check to see if a file is open
*/
int32 OS_FileOpenCheck(char *Filename)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_FileOpenCheck);

    return status;
}

/*
** Close all open files
*/
int32 OS_CloseAllFiles(void)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CloseAllFiles);

    return status;
}

/*
** Close a file by filename
*/
int32 OS_CloseFileByName(char *Filename)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CloseFileByName);

    return status;
}

/*
** Returns the number of free bytes in a file system
** Note the 64 bit data type to support filesystems that
** are greater than 4 Gigabytes
*/
int32 OS_fsBytesFree(const char *name, uint64 *bytes_free)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_fsBytesFree);
    *bytes_free = 10000;

    return status;
}

/*
 * Checks the health of a file system and repairs it if neccesary
*/
os_fshealth_t OS_chkfs(const char *name, boolean repair)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_chkfs);

    return status;
}

/*
 * Returns in the parameter the physical drive underneith the mount point
*/
int32 OS_FS_GetPhysDriveName(char * PhysDriveName, char * MountPoint)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_FS_GetPhysDriveName);
    strncpy(PhysDriveName, MountPoint, OS_FS_PHYS_NAME_LEN);

    return status;
}

/*
** Translates a OSAL Virtual file system path to a host Local path
*/
int32 OS_TranslatePath( const char *VirtualPath, char *LocalPath)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TranslatePath);
    strncpy(LocalPath, VirtualPath, OS_MAX_PATH_LEN);

    return status;
}

/*
**  Returns information about the file system in an os_fsinfo_t
*/
int32 OS_GetFsInfo(os_fsinfo_t  *filesys_info)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_GetFsInfo);
    memset(filesys_info, 0, sizeof (*filesys_info));

    return status;
}



