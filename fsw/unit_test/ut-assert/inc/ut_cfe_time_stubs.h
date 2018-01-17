/*
**
** File: ut_cfe_time_stubs.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_time_stubs.h 1.1 2011/05/04 11:20:23EDT rmcgraw Exp  $
**
** Purpose: cFE Time Services Header file for unit test stubs
**
** $Log: ut_cfe_time_stubs.h  $
** Revision 1.1 2011/05/04 11:20:23EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.1 2011/04/08 16:25:57EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.1 2011/02/15 11:12:35EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/inc/project.pj
**
*/

#ifndef UT_CFE_TIME_STUBS_H_
#define UT_CFE_TIME_STUBS_H_

typedef enum 
{
    UT_CFE_TIME_GETTIME_INDEX,
    UT_CFE_TIME_MAX_INDEX
} Ut_CFE_TIME_INDEX_t;

typedef struct
{
    CFE_TIME_SysTime_t (*CFE_TIME_GetTime)(void);
} Ut_CFE_TIME_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} Ut_CFE_TIME_ReturnCodeTable_t;

void Ut_CFE_TIME_Reset(void);
void Ut_CFE_TIME_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_CFE_TIME_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);

#endif
