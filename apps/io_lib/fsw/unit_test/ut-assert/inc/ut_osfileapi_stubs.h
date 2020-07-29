/*
**
** File:  ut_osfileapi_stubs.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_osfileapi_stubs.h 1.1 2011/05/04 11:20:25EDT rmcgraw Exp  $
**
** Purpose: OSAPI File Services Header file for unit test stubs.
**
** $Log: ut_osfileapi_stubs.h  $
** Revision 1.1 2011/05/04 11:20:25EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.1 2011/04/08 16:25:59EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.3 2011/03/30 09:58:56EDT rmcgraw 
** Added Hook and Return enhancements to Directory APIs
** Revision 1.2 2011/03/24 13:14:54EDT rmcgraw 
** Added Hook and RtnCode functionality to OS_stat
** Revision 1.1 2011/02/15 11:12:36EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/inc/project.pj
**
*/

#ifndef UT_OSFILEAPI_STUBS_H_
#define UT_OSFILEAPI_STUBS_H_

typedef enum 
{
    UT_OSFILEAPI_CREAT_INDEX,
    UT_OSFILEAPI_WRITE_INDEX,
    UT_OSFILEAPI_READ_INDEX,
    UT_OSFILEAPI_OPENDIR_INDEX,
    UT_OSFILEAPI_READDIR_INDEX,
    UT_OSFILEAPI_CLOSE_INDEX,
    UT_OSFILEAPI_OPEN_INDEX,
    UT_OSFILEAPI_CLOSEDIR_INDEX,
    UT_OSFILEAPI_STAT_INDEX,
    UT_OSFILEAPI_FDGETINFO_INDEX,
    UT_OSFILEAPI_MAX_INDEX
} Ut_OSFILEAPI_INDEX_t;

typedef struct
{
    int32 (*OS_creat)(const char *,int32);
    int32 (*OS_write)(int32, void *, uint32);
    int32 (*OS_read)(int32, void *, uint32);
    os_dirp_t (*OS_opendir)(const char *path);
    os_dirent_t* (*OS_readdir)(os_dirp_t directory);
    int32 (*OS_close)(int32  filedes);
    int32 (*OS_open)(const char *path,  int32 access,  uint32  mode);
    int32 (*OS_closedir)(os_dirp_t directory);
    int32 (*OS_stat)(const char *path, os_fstat_t *filestats);    
    int32 (*OS_FDGetInfo) (int32 filedes, OS_FDTableEntry *fd_prop);

} Ut_OSFILEAPI_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count; 
} Ut_OSFILEAPI_ReturnCodeTable_t;

void Ut_OSFILEAPI_Reset(void);
void Ut_OSFILEAPI_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_OSFILEAPI_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);

#endif 
