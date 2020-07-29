/*
**
** File:  ut_cfe_evs_stubs.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_evs_stubs.c 1.1 2011/05/04 11:20:53EDT rmcgraw Exp  $
**
** Purpose: Unit test stubs for cFE Event Services routines
**
** $Log: ut_cfe_evs_stubs.c  $
** Revision 1.1 2011/05/04 11:20:53EDT rmcgraw
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/04/08 16:26:38EDT rmcgraw
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/02/15 11:13:02EST sslegel
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/src/project.pj
**
*/

/*
** Include section
*/

#include "cfe.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include <string.h>

Ut_CFE_EVS_HookTable_t          Ut_CFE_EVS_HookTable;
Ut_CFE_EVS_ReturnCodeTable_t    Ut_CFE_EVS_ReturnCodeTable[UT_CFE_EVS_MAX_INDEX];

void Ut_CFE_EVS_Reset(void)
{
    memset(&Ut_CFE_EVS_HookTable, 0, sizeof(Ut_CFE_EVS_HookTable));
    memset(&Ut_CFE_EVS_ReturnCodeTable, 0, sizeof(Ut_CFE_EVS_ReturnCodeTable));

    Ut_CFE_EVS_SetFunctionHook(UT_CFE_EVS_SENDEVENT_INDEX, &Ut_CFE_EVS_SendEventHook);
    Ut_CFE_EVS_ClearEventQueue();
}

void Ut_CFE_EVS_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_CFE_EVS_REGISTER_INDEX)    { Ut_CFE_EVS_HookTable.CFE_EVS_Register = FunPtr; }
    else if (Index == UT_CFE_EVS_SENDEVENT_INDEX)   { Ut_CFE_EVS_HookTable.CFE_EVS_SendEvent = FunPtr; }
    else                                            { printf("Unsupported EVS Index In SetFunctionHook Call %u", Index); }
}

void Ut_CFE_EVS_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_CFE_EVS_MAX_INDEX) {
        Ut_CFE_EVS_ReturnCodeTable[Index].Value = RtnVal;
        Ut_CFE_EVS_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported EVS Index In SetReturnCode Call %u\n", Index);
    }
}

boolean Ut_CFE_EVS_UseReturnCode(uint32 Index)
{
    if (Ut_CFE_EVS_ReturnCodeTable[Index].Count > 0) {
        Ut_CFE_EVS_ReturnCodeTable[Index].Count--;
        if (Ut_CFE_EVS_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}

int32 CFE_EVS_Register (void *Filters, uint16 NumEventFilters, uint16 FilterScheme)
{
    if (Ut_CFE_EVS_UseReturnCode(UT_CFE_EVS_REGISTER_INDEX))
        return Ut_CFE_EVS_ReturnCodeTable[UT_CFE_EVS_REGISTER_INDEX].Value;

    if (Ut_CFE_EVS_HookTable.CFE_EVS_Register)
        return(Ut_CFE_EVS_HookTable.CFE_EVS_Register(Filters, NumEventFilters, FilterScheme));

    return CFE_SUCCESS;
}

int32 CFE_EVS_SendEvent (uint16 EventID, uint16 EventType, const char *Spec, ... )
{
    char     BigBuf[CFE_EVS_MAX_MESSAGE_LENGTH];
    va_list  Ptr;

    va_start(Ptr, Spec);
    vsprintf(BigBuf, Spec, Ptr);
    va_end(Ptr);

    if (Ut_CFE_EVS_UseReturnCode(UT_CFE_EVS_SENDEVENT_INDEX))
        return Ut_CFE_EVS_ReturnCodeTable[UT_CFE_EVS_SENDEVENT_INDEX].Value;

    if (Ut_CFE_EVS_HookTable.CFE_EVS_SendEvent)
         return(Ut_CFE_EVS_HookTable.CFE_EVS_SendEvent(EventID, EventType, BigBuf));

    return CFE_SUCCESS;
}

int32 CFE_EVS_SendTimedEvent (CFE_TIME_SysTime_t Time, uint16 EventID, uint16 EventType, const char *Spec, ... )
{
    if (Ut_CFE_EVS_UseReturnCode(UT_CFE_EVS_SENDTIMEDEVENT_INDEX))
        return Ut_CFE_EVS_ReturnCodeTable[UT_CFE_EVS_SENDTIMEDEVENT_INDEX].Value;

    return CFE_SUCCESS;
}

int32 CFE_EVS_SendEventWithAppID (uint16 EventID, uint16 EventType,uint32 AppID, const char *Spec, ... )
{
    if (Ut_CFE_EVS_UseReturnCode(UT_CFE_EVS_SENDEVENTWITHAPPID_INDEX))
        return Ut_CFE_EVS_ReturnCodeTable[UT_CFE_EVS_SENDEVENTWITHAPPID_INDEX].Value;

    return CFE_SUCCESS;
}

int32 CFE_EVS_CleanUpApp (uint32 AppId)
{
    return CFE_SUCCESS;
}
