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


UT_DEFAULT_STUB(OS_DirAPI_Init,(void))

/*****************************************************************************
 *
 * Stub for OS_mkdir() function
 *
 *****************************************************************************/
int32 OS_mkdir (const char *path, uint32 access)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_mkdir);

    return Status;
}

/*****************************************************************************
 *
 * Stub for OS_rmdir() function
 *
 *****************************************************************************/
int32  OS_rmdir (const char *path)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_rmdir);

    return Status;
}



/*****************************************************************************
 *
 * Stub for OS_DirectoryOpen() function
 *
 *****************************************************************************/
int32 OS_DirectoryOpen(uint32 *dir_id, const char *path)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_DirectoryOpen);

    if (Status == OS_SUCCESS)
    {
        *dir_id = UT_AllocStubObjId(UT_OBJTYPE_DIR);
    }
    else
    {
        *dir_id = 0xDEADBEEFU;
    }


    return Status;
}

/*****************************************************************************
 *
 * Stub for OS_DirectoryClose() function
 *
 *****************************************************************************/
int32 OS_DirectoryClose(uint32 dir_id)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_DirectoryClose);

    if (Status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_DIR, dir_id);
    }

    return Status;
}

/*****************************************************************************
 *
 * Stub for OS_DirectoryRewind() function
 *
 *****************************************************************************/
int32 OS_DirectoryRewind(uint32 dir_id)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_DirectoryRewind);

    return Status;
}

/*****************************************************************************
 *
 * Stub for OS_DirectoryRead() function
 *
 *****************************************************************************/
int32 OS_DirectoryRead(uint32 dir_id, os_dirent_t *dirent)
{
    int32 Status;
    uint32 CopySize;

    Status = UT_DEFAULT_IMPL(OS_DirectoryRead);

    if (Status == OS_SUCCESS)
    {
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_DirectoryRead), dirent, sizeof(*dirent));
        if (CopySize < sizeof(*dirent))
        {
            memset(dirent, 0, sizeof(*dirent));
        }
    }

    return Status;
}


#ifndef OSAL_OMIT_DEPRECATED

/*****************************************************************************
 *
 * Stub for OS_opendir() function
 * Should be removed in the next version of OSAL
 *
 *****************************************************************************/
os_dirp_t OS_opendir (const char *path)
{
    int32 Status;
    os_dirp_t Dirp;

    Status = UT_DEFAULT_IMPL(OS_opendir);

    if (Status == OS_SUCCESS)
    {
        /* Create a non-null value.  Note that if the test code actually dereferences
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

/*****************************************************************************
 *
 * Stub for OS_closedir() function
 * Should be removed in the next version of OSAL
 *
 *****************************************************************************/
int32 OS_closedir (os_dirp_t directory)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_closedir);

    return Status;
}

/*****************************************************************************
 *
 * Stub for OS_readdir() function
 * Should be removed in the next version of OSAL
 *
 *****************************************************************************/
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
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_readdir), &DirentPtr, sizeof(DirentPtr));
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

/*****************************************************************************
 *
 * Stub for OS_rewinddir() function
 * Should be removed in the next version of OSAL
 *
 *****************************************************************************/
void OS_rewinddir(os_dirp_t directory)
{
    /* Call the default impl so hooks will work */
    UT_DEFAULT_IMPL(OS_rewinddir);
}
#endif

