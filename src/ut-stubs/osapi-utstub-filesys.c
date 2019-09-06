/*
 *  Copyright (c) 2004-2018, United States government as represented by the
 *  administrator of the National Aeronautics Space Administration.
 *  All rights reserved. This software was created at NASA Glenn
 *  Research Center pursuant to government contracts.
 *
 *  This is governed by the NASA Open Source Agreement and may be used,
 *  distributed and modified only according to the terms of that agreement.
 */

/**
 * \file osapi_stubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"


UT_DEFAULT_STUB(OS_FileSysAPI_Init,(void))


/*****************************************************************************
 *
 * Stub function for OS_mkfs()
 *
 *****************************************************************************/
int32 OS_mkfs(char *address, const char *devname, const char * volname, uint32 blocksize,
              uint32 numblocks)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_mkfs);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_rmfs()
 *
 *****************************************************************************/
int32 OS_rmfs(const char *devname)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_rmfs);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_initfs()
 *
 *****************************************************************************/
int32 OS_initfs(char *address, const char *devname, const char *volname,
                uint32 blocksize, uint32 numblocks)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_initfs);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_mount()
 *
 *****************************************************************************/
int32 OS_mount(const char *devname, const char* mountpoint)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_mount);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_unmount()
 *
 *****************************************************************************/
int32 OS_unmount(const char *mountpoint)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_unmount);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_fsBlocksFree()
 *
 *****************************************************************************/
int32 OS_fsBlocksFree(const char *name)
{
    int32 status;

    status = UT_DEFAULT_IMPL_RC(OS_fsBlocksFree, 100);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_fsBytesFree()
 *
 *****************************************************************************/
int32 OS_fsBytesFree(const char *name, uint64 *bytes_free)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_fsBytesFree);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_fsBytesFree), bytes_free, sizeof(*bytes_free)) < sizeof(*bytes_free))
    {
        *bytes_free = 10000;
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_chkfs()
 *
 *****************************************************************************/
int32 OS_chkfs(const char *name, bool repair)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_chkfs);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_FS_GetPhysDriveName()
 *
 *****************************************************************************/
int32 OS_FS_GetPhysDriveName(char * PhysDriveName, const char * MountPoint)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_FS_GetPhysDriveName);
    strncpy(PhysDriveName, MountPoint, OS_FS_PHYS_NAME_LEN);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_GetFsInfo()
 *
 *****************************************************************************/
int32 OS_GetFsInfo(os_fsinfo_t  *FilesysInfo)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_GetFsInfo);
    memset(FilesysInfo, 0, sizeof (*FilesysInfo));

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_TranslatePath()
 *
 *****************************************************************************/
int32 OS_TranslatePath( const char *VirtualPath, char *LocalPath)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_TranslatePath);

    if (status == OS_SUCCESS &&
            UT_Stub_CopyToLocal(UT_KEY(OS_TranslatePath), LocalPath, OS_MAX_LOCAL_PATH_LEN) == 0)
    {
        strncpy(LocalPath, VirtualPath, OS_MAX_LOCAL_PATH_LEN);
    }

    return status;
}


