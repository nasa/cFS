/*
**
** File: ut_cfe_time_stubs.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_time_stubs.c 1.1 2011/05/04 11:20:58EDT rmcgraw Exp  $
**
** Purpose: Unit test stubs for cFE Time Services routines
**
** $Log: ut_cfe_time_stubs.c  $
** Revision 1.1 2011/05/04 11:20:58EDT rmcgraw
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/04/08 16:26:43EDT rmcgraw
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/02/15 11:13:05EST sslegel
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/src/project.pj
**
*/

#include "cfe.h"
#include "ut_cfe_time_stubs.h"
#include <string.h>

Ut_CFE_TIME_HookTable_t         Ut_CFE_TIME_HookTable;
Ut_CFE_TIME_ReturnCodeTable_t   Ut_CFE_TIME_ReturnCodeTable[UT_CFE_TIME_MAX_INDEX];

void Ut_CFE_TIME_Reset(void)
{
    memset(&Ut_CFE_TIME_HookTable, 0, sizeof(Ut_CFE_TIME_HookTable));
    memset(&Ut_CFE_TIME_ReturnCodeTable, 0, sizeof(Ut_CFE_TIME_ReturnCodeTable));
}

void Ut_CFE_TIME_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_CFE_TIME_GETTIME_INDEX)      { Ut_CFE_TIME_HookTable.CFE_TIME_GetTime = FunPtr; }
    else                                              { printf("Unsupported TIME Index In SetFunctionHook Call %u\n", Index); }
}

void Ut_CFE_TIME_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_CFE_TIME_MAX_INDEX) {
        Ut_CFE_TIME_ReturnCodeTable[Index].Value = RtnVal;
        Ut_CFE_TIME_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported TIME Index In SetReturnCode Call %u\n", Index);
    }
}

boolean Ut_CFE_TIME_UseReturnCode(uint32 Index)
{
    if (Ut_CFE_TIME_ReturnCodeTable[Index].Count > 0) {
        Ut_CFE_TIME_ReturnCodeTable[Index].Count--;
        if (Ut_CFE_TIME_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}

CFE_TIME_SysTime_t  CFE_TIME_GetTime(void)
{
    CFE_TIME_SysTime_t  Time;

    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_GetTime)
        return Ut_CFE_TIME_HookTable.CFE_TIME_GetTime();

    Time.Seconds = 0;
    Time.Subseconds = 0;

    return Time;
}
