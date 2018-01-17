/*
**
** File: ut_osfileapi_stubs.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_osfileapi_stubs.c 1.1 2011/05/04 11:21:00EDT rmcgraw Exp  $
**
** Purpose: Unit test stubs for the OSAPI File Services routines
**
** $Log: ut_osfileapi_stubs.c  $
** Revision 1.1 2011/05/04 11:21:00EDT rmcgraw
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/04/08 16:26:45EDT rmcgraw
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.6 2011/03/31 14:53:05EDT rmcgraw
** Added functionality and supressed compiler warnings
** Revision 1.5 2011/03/30 09:58:57EDT rmcgraw
** Added Hook and Return enhancements to Directory APIs
** Revision 1.4 2011/03/24 13:14:55EDT rmcgraw
** Added Hook and RtnCode functionality to OS_stat
** Revision 1.3 2011/03/23 17:18:08EDT rmcgraw
** correct OS_read return value
** Revision 1.2 2011/03/23 17:08:19EDT rmcgraw
** OS_FS_ERROR to OS_FS_SUCCESS for some OS file sys apis
** Revision 1.1 2011/02/15 11:13:05EST sslegel
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/src/project.pj
**
*/

#include "cfe.h"
#include "ut_osfileapi_stubs.h"
#include <string.h>

Ut_OSFILEAPI_HookTable_t           Ut_OSFILEAPI_HookTable;
Ut_OSFILEAPI_ReturnCodeTable_t     Ut_OSFILEAPI_ReturnCodeTable[UT_OSFILEAPI_MAX_INDEX];

void Ut_OSFILEAPI_Reset(void)
{
    memset(&Ut_OSFILEAPI_HookTable, 0, sizeof(Ut_OSFILEAPI_HookTable));
    memset(&Ut_OSFILEAPI_ReturnCodeTable, 0, sizeof(Ut_OSFILEAPI_ReturnCodeTable));
}

void Ut_OSFILEAPI_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_OSFILEAPI_CREAT_INDEX)    { Ut_OSFILEAPI_HookTable.OS_creat = FunPtr; }
    else if (Index == UT_OSFILEAPI_WRITE_INDEX)    { Ut_OSFILEAPI_HookTable.OS_write = FunPtr; }
    else if (Index == UT_OSFILEAPI_READ_INDEX)     { Ut_OSFILEAPI_HookTable.OS_read = FunPtr; }
    else if (Index == UT_OSFILEAPI_OPENDIR_INDEX)  { Ut_OSFILEAPI_HookTable.OS_opendir = FunPtr; }
    else if (Index == UT_OSFILEAPI_READDIR_INDEX)  { Ut_OSFILEAPI_HookTable.OS_readdir = FunPtr; }
    else if (Index == UT_OSFILEAPI_CLOSE_INDEX)    { Ut_OSFILEAPI_HookTable.OS_close = FunPtr; }
    else if (Index == UT_OSFILEAPI_OPEN_INDEX)     { Ut_OSFILEAPI_HookTable.OS_open = FunPtr; }
    else if (Index == UT_OSFILEAPI_CLOSEDIR_INDEX) { Ut_OSFILEAPI_HookTable.OS_closedir = FunPtr; }
    else if (Index == UT_OSFILEAPI_STAT_INDEX)     { Ut_OSFILEAPI_HookTable.OS_stat = FunPtr; }
    else if (Index == UT_OSFILEAPI_FDGETINFO_INDEX){ Ut_OSFILEAPI_HookTable.OS_FDGetInfo = FunPtr; }
    else                                           { printf("Unsupported OSFILEAPI Index In SetFunctionHook Call %u\n", Index); }
}

void Ut_OSFILEAPI_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_OSFILEAPI_MAX_INDEX) {
        Ut_OSFILEAPI_ReturnCodeTable[Index].Value = RtnVal;
        Ut_OSFILEAPI_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported OSFILEAPI Index In SetReturnCode Call %u\n", Index);
    }
}

boolean Ut_OSFILEAPI_UseReturnCode(uint32 Index)
{
    if (Ut_OSFILEAPI_ReturnCodeTable[Index].Count > 0) {
        Ut_OSFILEAPI_ReturnCodeTable[Index].Count--;
        if (Ut_OSFILEAPI_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}

/*
** Standard File system API
*/

int32 OS_creat  (const char *path, int32  access)
{
    /* Check for specified return */
    if (Ut_OSFILEAPI_UseReturnCode(UT_OSFILEAPI_CREAT_INDEX))
        return Ut_OSFILEAPI_ReturnCodeTable[UT_OSFILEAPI_CREAT_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSFILEAPI_HookTable.OS_creat)
        return Ut_OSFILEAPI_HookTable.OS_creat(path, access);

    return OS_FS_ERROR;
}

int32 OS_open   (const char *path,  int32 access,  uint32  mode)
{
    /* Check for specified return */
    if (Ut_OSFILEAPI_UseReturnCode(UT_OSFILEAPI_OPEN_INDEX))
        return Ut_OSFILEAPI_ReturnCodeTable[UT_OSFILEAPI_OPEN_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSFILEAPI_HookTable.OS_open)
        return Ut_OSFILEAPI_HookTable.OS_open(path, access, mode);

    return OS_FS_ERROR;
}

int32 OS_close (int32  filedes)
{
    /* Check for specified return */
    if (Ut_OSFILEAPI_UseReturnCode(UT_OSFILEAPI_CLOSE_INDEX))
        return Ut_OSFILEAPI_ReturnCodeTable[UT_OSFILEAPI_CLOSE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSFILEAPI_HookTable.OS_close)
        return Ut_OSFILEAPI_HookTable.OS_close(filedes);

    return OS_FS_SUCCESS;
}

int32 OS_read  (int32  filedes, void *buffer, uint32 nbytes)
{
    /* Check for specified return */
    if (Ut_OSFILEAPI_UseReturnCode(UT_OSFILEAPI_READ_INDEX))
        return Ut_OSFILEAPI_ReturnCodeTable[UT_OSFILEAPI_READ_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSFILEAPI_HookTable.OS_read)
        return Ut_OSFILEAPI_HookTable.OS_read(filedes, buffer, nbytes);

    return OS_FS_ERROR;
}

int32 OS_write (int32  filedes, void *buffer, uint32 nbytes)
{
    /* Check for specified return */
    if (Ut_OSFILEAPI_UseReturnCode(UT_OSFILEAPI_WRITE_INDEX))
        return Ut_OSFILEAPI_ReturnCodeTable[UT_OSFILEAPI_WRITE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSFILEAPI_HookTable.OS_write)
        return Ut_OSFILEAPI_HookTable.OS_write(filedes, buffer, nbytes);

    return nbytes;
}

int32 OS_chmod  (const char *path, uint32 access)
{
    return OS_FS_SUCCESS;
}

int32 OS_stat   (const char *path, os_fstat_t *filestats)
{
    /* Check for specified return */
    if (Ut_OSFILEAPI_UseReturnCode(UT_OSFILEAPI_STAT_INDEX))
        return Ut_OSFILEAPI_ReturnCodeTable[UT_OSFILEAPI_STAT_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSFILEAPI_HookTable.OS_stat)
        return Ut_OSFILEAPI_HookTable.OS_stat(path, filestats);

    return OS_FS_ERROR;
}

int32 OS_lseek  (int32  filedes, int32 offset, uint32 whence)
{
    return OS_FS_SUCCESS;
}

int32 OS_remove (const char *path)
{
    return OS_FS_SUCCESS;
}

int32 OS_rename (const char *old, const char *new)
{
    return OS_FS_SUCCESS;
}

int32 OS_cp (const char *src, const char *dest)
{
    return OS_FS_SUCCESS;
}

int32 OS_mv (const char *src, const char *dest)
{
    return OS_FS_SUCCESS;
}

/*
** Directory API
*/

int32 OS_mkdir (const char *path, uint32 access)
{
    return OS_FS_SUCCESS;
}

os_dirp_t OS_opendir (const char *path)
{
    /* Check for specified return */
    if (Ut_OSFILEAPI_UseReturnCode(UT_OSFILEAPI_OPENDIR_INDEX))
        return (os_dirp_t)Ut_OSFILEAPI_ReturnCodeTable[UT_OSFILEAPI_OPENDIR_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSFILEAPI_HookTable.OS_opendir)
        return Ut_OSFILEAPI_HookTable.OS_opendir(path);

    return NULL;
}

int32 OS_closedir (os_dirp_t directory)
{
    /* Check for specified return */
    if (Ut_OSFILEAPI_UseReturnCode(UT_OSFILEAPI_CLOSEDIR_INDEX))
        return Ut_OSFILEAPI_ReturnCodeTable[UT_OSFILEAPI_CLOSEDIR_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSFILEAPI_HookTable.OS_closedir)
        return Ut_OSFILEAPI_HookTable.OS_closedir(directory);

    return OS_FS_SUCCESS;
}

os_dirent_t *  OS_readdir (os_dirp_t directory)
{
    /* Check for specified return */
    if (Ut_OSFILEAPI_UseReturnCode(UT_OSFILEAPI_READDIR_INDEX))
        return (os_dirent_t *)Ut_OSFILEAPI_ReturnCodeTable[UT_OSFILEAPI_READDIR_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSFILEAPI_HookTable.OS_readdir)
        return Ut_OSFILEAPI_HookTable.OS_readdir(directory);

    return NULL;
}

int32  OS_rmdir (const char *path)
{
    return OS_FS_ERROR;
}

int32 OS_check_name_length(const char *path)
{
    return OS_FS_SUCCESS;
}

int32 OS_ShellOutputToFile(char* Cmd, int32 OS_fd)
{
    return OS_FS_SUCCESS;
}

int32 OS_FDGetInfo (int32 filedes, OS_FDTableEntry *fd_prop)
{
    /* Check for specified return */
    if (Ut_OSFILEAPI_UseReturnCode(UT_OSFILEAPI_FDGETINFO_INDEX))
        return Ut_OSFILEAPI_ReturnCodeTable[UT_OSFILEAPI_FDGETINFO_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSFILEAPI_HookTable.OS_FDGetInfo)
        return Ut_OSFILEAPI_HookTable.OS_FDGetInfo(filedes,fd_prop);

    return OS_FS_ERR_INVALID_FD;
}
