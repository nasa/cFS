/*
**
** File:  ut_cfe_fs_stubs.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_fs_stubs.c 1.1 2011/05/04 11:20:53EDT rmcgraw Exp  $
**
** Purpose: Unit test stubs for cFE File system routines
**
** $Log: ut_cfe_fs_stubs.c  $
** Revision 1.1 2011/05/04 11:20:53EDT rmcgraw
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/04/08 16:26:39EDT rmcgraw
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.2 2011/03/22 14:19:02EDT rmcgraw
** Use Rtn Code Added to CFE_FS_WriteHeader
** Revision 1.1 2011/02/15 11:13:02EST sslegel
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/src/project.pj
**
*/

#include "cfe.h"
#include "ut_cfe_fs_stubs.h"
#include <string.h>

Ut_CFE_FS_HookTable_t           Ut_CFE_FS_HookTable;
Ut_CFE_FS_ReturnCodeTable_t     Ut_CFE_FS_ReturnCodeTable[UT_CFE_FS_MAX_INDEX];

void Ut_CFE_FS_Reset(void)
{
    memset(&Ut_CFE_FS_HookTable, 0, sizeof(Ut_CFE_FS_HookTable));
    memset(&Ut_CFE_FS_ReturnCodeTable, 0, sizeof(Ut_CFE_FS_ReturnCodeTable));
}

void Ut_CFE_FS_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_CFE_FS_READHDR_INDEX)                  { Ut_CFE_FS_HookTable.CFE_FS_ReadHeader = FunPtr; }
    else if (Index == UT_CFE_FS_WRITEHDR_INDEX)                 { Ut_CFE_FS_HookTable.CFE_FS_WriteHeader = FunPtr; }
    else if (Index == UT_CFE_FS_SETTIMESTAMP_INDEX)             { Ut_CFE_FS_HookTable.CFE_FS_SetTimestamp = FunPtr; }
    else if (Index == UT_CFE_FS_ISGZFILE_INDEX)                 { Ut_CFE_FS_HookTable.CFE_FS_IsGzFile = FunPtr; }
    else if (Index == UT_CFE_FS_EXTRACTFILENAMEFROMPATH_INDEX)  { Ut_CFE_FS_HookTable.CFE_FS_ExtractFilenameFromPath = FunPtr; }
    else if (Index == UT_CFE_FS_DECOMPRESS_INDEX)               { Ut_CFE_FS_HookTable.CFE_FS_Decompress = FunPtr; }
    else                                                        { printf("Invalid FS Index In SetFunctionHook Call %u\n", Index); }
}

void Ut_CFE_FS_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_CFE_FS_MAX_INDEX) {
        Ut_CFE_FS_ReturnCodeTable[Index].Value = RtnVal;
        Ut_CFE_FS_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported FS Index In SetReturnCode Call %u\n", Index);
    }
}

boolean Ut_CFE_FS_UseReturnCode(uint32 Index)
{
    if (Ut_CFE_FS_ReturnCodeTable[Index].Count > 0) {
        Ut_CFE_FS_ReturnCodeTable[Index].Count--;
        if (Ut_CFE_FS_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}

int32 CFE_FS_ReadHeader(CFE_FS_Header_t *Hdr, int32 FileDes)
{
    return CFE_SUCCESS;
}

int32 CFE_FS_WriteHeader(int32 FileDes, CFE_FS_Header_t *Hdr)
{
    /* Check for specified return */
    if (Ut_CFE_FS_UseReturnCode(UT_CFE_FS_WRITEHDR_INDEX))
        return Ut_CFE_FS_ReturnCodeTable[UT_CFE_FS_WRITEHDR_INDEX].Value;

    return CFE_SUCCESS;
}

int32 CFE_FS_SetTimestamp(int32 FileDes, CFE_TIME_SysTime_t NewTimestamp)
{
    return CFE_SUCCESS;
}

boolean CFE_FS_IsGzFile(const char *FileName)
{
    return TRUE;
}

int32 CFE_FS_ExtractFilenameFromPath(const char *OriginalPath, char *FileNameOnly)
{
    return CFE_SUCCESS;
}

int32 CFE_FS_Decompress( const char * SourceFile, const char * DestinationFile )
{
    return CFE_SUCCESS;
}
