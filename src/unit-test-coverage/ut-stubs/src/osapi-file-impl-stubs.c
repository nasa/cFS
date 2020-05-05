/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     osapi-file-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-file.h"
#include "os-shared-dir.h"


/*
 * File API abstraction layer
 */

UT_DEFAULT_STUB(OS_FileOpen_Impl,(uint32 file_id, const char *local_path, int32 flags, int32 access))
UT_DEFAULT_STUB(OS_FileStat_Impl,(const char *local_path, os_fstat_t *filestat))
UT_DEFAULT_STUB(OS_FileRemove_Impl,(const char *local_path))
UT_DEFAULT_STUB(OS_FileRename_Impl,(const char *old_path, const char *new_path))
UT_DEFAULT_STUB(OS_FileChmod_Impl, (const char *local_path, uint32 access))
UT_DEFAULT_STUB(OS_ShellOutputToFile_Impl,(uint32 file_id, const char* Cmd))

/*
 * Directory API abstraction layer
 */
UT_DEFAULT_STUB(OS_DirCreate_Impl,(const char *local_path, uint32 access))
UT_DEFAULT_STUB(OS_DirOpen_Impl,(uint32 dir_id, const char *local_path))
UT_DEFAULT_STUB(OS_DirClose_Impl,(uint32 dir_id))
UT_DEFAULT_STUB(OS_DirRead_Impl,(uint32 dir_id, os_dirent_t *dirent))
UT_DEFAULT_STUB(OS_DirRewind_Impl,(uint32 dir_id))
UT_DEFAULT_STUB(OS_DirRemove_Impl,(const char *local_path))


/*
 * Stream abstraction layer (applies to sockets and files)
 */
int32 OS_GenericRead_Impl(uint32 stream_id, void *buffer, uint32 nbytes, int32 timeout)
{
    int32 Status = UT_DEFAULT_IMPL(OS_GenericRead_Impl);

    if (Status == OS_SUCCESS)
    {
        Status = UT_Stub_CopyToLocal(UT_KEY(OS_GenericRead_Impl), buffer, nbytes);
    }

    return Status;
}

int32 OS_GenericWrite_Impl(uint32 stream_id, const void *buffer, uint32 nbytes, int32 timeout)
{
    int32 Status = UT_DEFAULT_IMPL(OS_GenericWrite_Impl);

    if (Status == OS_SUCCESS)
    {
        Status = UT_Stub_CopyFromLocal(UT_KEY(OS_GenericWrite_Impl), (const uint8*)buffer, nbytes);
    }

    return Status;
}

UT_DEFAULT_STUB(OS_GenericSeek_Impl,(uint32 file_id, int32 offset, uint32 whence))
UT_DEFAULT_STUB(OS_GenericClose_Impl,(uint32 file_id))


