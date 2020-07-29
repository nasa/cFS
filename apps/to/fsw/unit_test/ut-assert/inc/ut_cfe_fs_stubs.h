/*
**
** File:  ut_cfe_fs_stubs.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_fs_stubs.h 1.1 2011/05/04 11:20:20EDT rmcgraw Exp  $
**
** Purpose: cFE File System Header file for unit test stubs
**
** $Log: ut_cfe_fs_stubs.h  $
** Revision 1.1 2011/05/04 11:20:20EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.1 2011/04/08 16:25:54EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.1 2011/02/15 11:12:33EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/inc/project.pj
**
*/

#ifndef UT_CFE_FS_STUBS_H_
#define UT_CFE_FS_STUBS_H_

typedef enum 
{
    UT_CFE_FS_READHDR_INDEX,
    UT_CFE_FS_WRITEHDR_INDEX,
    UT_CFE_FS_SETTIMESTAMP_INDEX,
    UT_CFE_FS_ISGZFILE_INDEX,
    UT_CFE_FS_EXTRACTFILENAMEFROMPATH_INDEX,
    UT_CFE_FS_DECOMPRESS_INDEX,
    UT_CFE_FS_MAX_INDEX
} Ut_CFE_FS_INDEX_t;

typedef struct
{
    int32 (*CFE_FS_ReadHeader)(CFE_FS_Header_t *Hdr, int32 FileDes);
    int32 (*CFE_FS_WriteHeader)(int32 FileDes, CFE_FS_Header_t *Hdr);
    int32 (*CFE_FS_SetTimestamp)(int32 FileDes, CFE_TIME_SysTime_t NewTimestamp);
    int32 (*CFE_FS_IsGzFile)(char *FileName);
    int32 (*CFE_FS_ExtractFilenameFromPath)(char *OriginalPath, char *FileNameOnly);
    int32 (*CFE_FS_Decompress)( char * SourceFile, char * DestinationFile );
} Ut_CFE_FS_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} Ut_CFE_FS_ReturnCodeTable_t;

void Ut_CFE_FS_Reset(void);
void Ut_CFE_FS_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_CFE_FS_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);

#endif 
