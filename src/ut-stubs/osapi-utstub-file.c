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


UT_DEFAULT_STUB(OS_FileAPI_Init,(void))

/*****************************************************************************
 *
 * Local Stub helper function for reading
 *
 *****************************************************************************/
static int32 UT_GenericReadStub(const char *fname, UT_EntryKey_t fkey, void *buffer, uint32 bsize)
{
    int32 status;
    uint32 CopySize;

    status = UT_DefaultStubImpl(fname, fkey, 0x7FFFFFFF);

    if (status == 0x7FFFFFFF)
    {
        CopySize = UT_Stub_CopyToLocal(fkey, buffer, bsize);

        /* If CopyToLocal returns zero, this probably means no buffer was supplied,
         * in which case just generate fill data and pretend it was read.
         */
        if (CopySize > 0)
        {
            status = CopySize;
        }
        else
        {
            memset(buffer, 0, bsize);
            status = bsize;
        }
    }
    else if (status > 0)
    {
        /* generate fill data for requested size */
        memset(buffer, 0, status);
    }

    return status;
}

/*****************************************************************************
 *
 * Local Stub helper function for writing
 *
 *****************************************************************************/
static int32 UT_GenericWriteStub(const char *fname, UT_EntryKey_t fkey, const void *buffer, uint32 bsize)
{
    int32   status;
    uint32  CopySize;

    status = UT_DefaultStubImpl(fname, fkey, 0x7FFFFFFF);

    if (status == 0x7FFFFFFF)
    {
        CopySize = UT_Stub_CopyFromLocal(fkey, buffer, bsize);

        /* If CopyFromLocal returns zero, this probably means no buffer was supplied,
         * in which case just throw out the data and pretend it was written.
         */
        if (CopySize > 0)
        {
            status = CopySize;
        }
        else
        {
            status = bsize;
        }
    }

    return status;
}


/*****************************************************************************
 *
 * Stub function for OS_creat()
 *
 *****************************************************************************/
int32 OS_creat(const char *path, int32 access)
{
    int32 status = OS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_creat);

    if (status == OS_SUCCESS)
    {
        status = UT_AllocStubObjId(UT_OBJTYPE_FILESTREAM);
    }

   return status;
}

/*****************************************************************************
 *
 * Stub function for OS_open()
 *
 *****************************************************************************/
int32 OS_open(const char *path, int32 access, uint32 mode)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_open);

    if (status == OS_SUCCESS)
    {
        status = UT_AllocStubObjId(UT_OBJTYPE_FILESTREAM);
    }

    return status;
}


/*****************************************************************************
 *
 * Stub function for OS_close()
 *
 *****************************************************************************/
int32 OS_close(uint32 filedes)
{
    int32 status = OS_FS_SUCCESS;

    status = UT_DEFAULT_IMPL(OS_close);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(UT_OBJTYPE_FILESTREAM, filedes);
    }

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_StreamRead()
 *
 *****************************************************************************/
int32 OS_StreamRead(uint32  filedes, void *buffer, uint32 nbytes, int32 timeout)
{
    return UT_GenericReadStub(__func__,UT_KEY(OS_StreamRead),buffer,nbytes);
}

/*****************************************************************************
 *
 * Stub function for OS_StreamWrite()
 *
 *****************************************************************************/
int32 OS_StreamWrite(uint32  filedes, const void *buffer, uint32 nbytes, int32 timeout)
{
    return UT_GenericWriteStub(__func__,UT_KEY(OS_StreamWrite),buffer,nbytes);
}

/*****************************************************************************
 *
 * Stub function for OS_read()
 *
 *****************************************************************************/
int32 OS_read(uint32 filedes, void *buffer, uint32 nbytes)
{
    return UT_GenericReadStub(__func__,UT_KEY(OS_read),buffer,nbytes);
}

/*****************************************************************************
 *
 * Stub function for OS_write()
 *
 *****************************************************************************/
int32 OS_write(uint32 filedes, const void *buffer, uint32 nbytes)
{
    return UT_GenericWriteStub(__func__,UT_KEY(OS_write),buffer,nbytes);
}

/*****************************************************************************
 *
 * Stub function for OS_chmod()
 *
 *****************************************************************************/
int32 OS_chmod  (const char *path, uint32 access)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_chmod);

    return Status;
}


/*****************************************************************************
 *
 * Stub function for OS_stat()
 *
 *****************************************************************************/
int32 OS_stat   (const char *path, os_fstat_t *filestats)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_stat);

    if (Status == OS_SUCCESS)
    {
        UT_Stub_CopyToLocal(UT_KEY(OS_stat), filestats, sizeof(*filestats));
    }
    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_lseek()
 *
 *****************************************************************************/
int32 OS_lseek(uint32 filedes, int32 offset, uint32 whence)
{
    int32 status;

    status = UT_DEFAULT_IMPL_RC(OS_lseek, offset);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_remove()
 *
 *****************************************************************************/
int32 OS_remove (const char *path)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_remove);

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_rename()
 *
 *****************************************************************************/
int32 OS_rename (const char *old, const char *new)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_rename);

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_cp()
 *
 *****************************************************************************/
int32 OS_cp (const char *src, const char *dest)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_cp);

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_mv()
 *
 *****************************************************************************/
int32 OS_mv (const char *src, const char *dest)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_mv);

    return Status;
}


/*****************************************************************************
 *
 * Stub function for OS_FDGetInfo()
 *
 *****************************************************************************/
int32 OS_FDGetInfo (uint32 filedes, OS_file_prop_t *fd_prop)
{
    int32 status;
    uint32 CopySize;

    status = UT_DEFAULT_IMPL(OS_FDGetInfo);

    if (status == OS_SUCCESS)
    {
        /* The user may supply specific entries to return */
        CopySize = UT_Stub_CopyToLocal(UT_KEY(OS_FDGetInfo), fd_prop, sizeof(*fd_prop));
        if (CopySize < sizeof(*fd_prop))
        {
            memset(fd_prop, 0, sizeof(*fd_prop));
            fd_prop->IsValid = true;
            fd_prop->User = 1;
            UT_FIXUP_ID(fd_prop->User, UT_OBJTYPE_TASK);
        }
    }

    return status;
}


/*****************************************************************************
 *
 * Stub function for OS_FileOpenCheck()
 *
 *****************************************************************************/
int32 OS_FileOpenCheck(const char *Filename)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_FileOpenCheck);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_CloseFileByName()
 *
 *****************************************************************************/
int32 OS_CloseFileByName(const char *Filename)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CloseFileByName);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_CloseAllFiles()
 *
 *****************************************************************************/
int32 OS_CloseAllFiles(void)
{
    int32 status;

    status = UT_DEFAULT_IMPL(OS_CloseAllFiles);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_ShellOutputToFile()
 *
 *****************************************************************************/
int32 OS_ShellOutputToFile(const char* Cmd, uint32 filedes)
{
    int32 status;

    /*
     * This allows a hook function to do something with the "Cmd" parameter
     */
    UT_Stub_RegisterContext(UT_KEY(OS_ShellOutputToFile),Cmd);

    status = UT_DEFAULT_IMPL(OS_ShellOutputToFile);

    return status;
}


