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
    UT_Stub_RegisterContext(UT_KEY(OS_creat), path);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_creat), access);

    int32 status;

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
    UT_Stub_RegisterContext(UT_KEY(OS_open), path);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_open), access);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_open), mode);

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
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_close), filedes);

    int32 status;

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
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_read), filedes);
    UT_Stub_RegisterContext(UT_KEY(OS_read), buffer);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_read), nbytes);

    return UT_GenericReadStub(__func__,UT_KEY(OS_read),buffer,nbytes);
}

/*****************************************************************************
 *
 * Stub function for OS_write()
 *
 *****************************************************************************/
int32 OS_write(uint32 filedes, const void *buffer, uint32 nbytes)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_write), filedes);
    UT_Stub_RegisterContext(UT_KEY(OS_write), buffer);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_write), nbytes);

    return UT_GenericWriteStub(__func__,UT_KEY(OS_write),buffer,nbytes);
}

/*****************************************************************************
 *
 * Stub function for OS_TimedRead()
 *
 *****************************************************************************/
int32 OS_TimedRead(uint32  filedes, void *buffer, uint32 nbytes, int32 timeout)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimedRead), filedes);
    UT_Stub_RegisterContext(UT_KEY(OS_TimedRead), buffer);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimedRead), nbytes);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimedRead), timeout);

    return UT_GenericReadStub(__func__,UT_KEY(OS_TimedRead),buffer,nbytes);
}

/*****************************************************************************
 *
 * Stub function for OS_TimedWrite()
 *
 *****************************************************************************/
int32 OS_TimedWrite(uint32  filedes, const void *buffer, uint32 nbytes, int32 timeout)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimedWrite), filedes);
    UT_Stub_RegisterContext(UT_KEY(OS_TimedWrite), buffer);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimedWrite), nbytes);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_TimedWrite), timeout);

    return UT_GenericWriteStub(__func__,UT_KEY(OS_TimedWrite),buffer,nbytes);
}


/*****************************************************************************
 *
 * Stub function for OS_chmod()
 *
 *****************************************************************************/
int32 OS_chmod  (const char *path, uint32 access)
{
    UT_Stub_RegisterContext(UT_KEY(OS_chmod), path);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_chmod), access);

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
    UT_Stub_RegisterContext(UT_KEY(OS_stat), path);
    UT_Stub_RegisterContext(UT_KEY(OS_stat), filestats);

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
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_lseek), filedes);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_lseek), offset);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_lseek), whence);

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
    UT_Stub_RegisterContext(UT_KEY(OS_remove), path);

    int32 Status;

    Status = UT_DEFAULT_IMPL(OS_remove);

    return Status;
}

/*****************************************************************************
 *
 * Stub function for OS_rename()
 *
 *****************************************************************************/
int32 OS_rename (const char *old_filename, const char *new_filename)
{
    UT_Stub_RegisterContext(UT_KEY(OS_rename), old_filename);
    UT_Stub_RegisterContext(UT_KEY(OS_rename), new_filename);

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
    UT_Stub_RegisterContext(UT_KEY(OS_cp), src);
    UT_Stub_RegisterContext(UT_KEY(OS_cp), dest);

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
    UT_Stub_RegisterContext(UT_KEY(OS_mv), src);
    UT_Stub_RegisterContext(UT_KEY(OS_mv), dest);

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
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_FDGetInfo), filedes);
    UT_Stub_RegisterContext(UT_KEY(OS_FDGetInfo), fd_prop);

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
    UT_Stub_RegisterContext(UT_KEY(OS_FileOpenCheck), Filename);

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
    UT_Stub_RegisterContext(UT_KEY(OS_CloseFileByName), Filename);

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
    UT_Stub_RegisterContext(UT_KEY(OS_ShellOutputToFile), Cmd);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_ShellOutputToFile), filedes);

    int32 status;

    /*
     * This allows a hook function to do something with the "Cmd" parameter
     */

    status = UT_DEFAULT_IMPL(OS_ShellOutputToFile);

    return status;
}


