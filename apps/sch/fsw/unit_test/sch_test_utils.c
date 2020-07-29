 /*************************************************************************
 ** File:
 **   $Id: sch_test_utils.c 1.2 2017/06/21 15:28:59EDT mdeschu Exp  $
 **
 ** Purpose: 
 **   This file contains unit test utilities for the sch application.
 **
 ** References:
 **   Flight Software Branch C Coding Standard Version 1.2
 **   CFS Development Standards Document
 ** Notes:
 **
 *************************************************************************/

#include "sch_test_utils.h"
#include "sch_tbldefs.h"

SCH_MessageEntry_t    MessageTable[SCH_MAX_MESSAGES];
SCH_ScheduleEntry_t   ScheduleTable[SCH_TABLE_ENTRIES];

/*
 * Function Definitions
 */

void SCH_Test_Setup(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemSet(&SCH_AppData, 0, sizeof(SCH_AppData_t));

    SCH_AppData.MessageTable  = &MessageTable[0];
    SCH_AppData.ScheduleTable = &ScheduleTable[0];

    CFE_PSP_MemSet(SCH_AppData.MessageTable, 0, sizeof(SCH_MessageEntry_t)*SCH_MAX_MESSAGES);
    CFE_PSP_MemSet(SCH_AppData.ScheduleTable, 0, sizeof(SCH_ScheduleEntry_t)*SCH_TABLE_ENTRIES);

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
    Ut_OSTIMER_Reset();
} /* end SCH_Test_Setup */

void SCH_Test_TearDown(void)
{
    /* cleanup test environment */
} /* end SCH_Test_TearDown */

/*
 * Additional UT-Assert Stub Functions and Required Data Structures
 *
 * Note: This code needs to be moved into the UT-Assert library.  We are including it here for now because the 
 * next release of the UT-Assert library is not expected to happen in the near future.
 */

/* ut_cfe_evs_stubs */

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
    else if (Index == UT_CFE_EVS_RESETALLFILTERS_INDEX)   { Ut_CFE_EVS_HookTable.CFE_EVS_ResetAllFilters = FunPtr; }
    else
    {
        printf("Unsupported EVS Index In SetFunctionHook Call %lu", Index);
        UtAssert_True(FALSE, "Unsupported EVS Index In SetFunctionHook Call");
    }
}

void Ut_CFE_EVS_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_CFE_EVS_MAX_INDEX)
    {
        Ut_CFE_EVS_ReturnCodeTable[Index].Value = RtnVal;
        Ut_CFE_EVS_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported EVS Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported EVS Index In SetReturnCode Call");
    }
}

boolean Ut_CFE_EVS_UseReturnCode(uint32 Index)
{
    if (Ut_CFE_EVS_ReturnCodeTable[Index].Count > 0)
    {
        Ut_CFE_EVS_ReturnCodeTable[Index].Count--;
        if (Ut_CFE_EVS_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_CFE_EVS_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }
    
    return(FALSE);
}

void Ut_CFE_EVS_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_CFE_EVS_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}

int32 CFE_EVS_Register (void *Filters, uint16 NumEventFilters, uint16 FilterScheme)
{
    /* Check for specified return */
    if (Ut_CFE_EVS_UseReturnCode(UT_CFE_EVS_REGISTER_INDEX))
        return Ut_CFE_EVS_ReturnCodeTable[UT_CFE_EVS_REGISTER_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_EVS_HookTable.CFE_EVS_Register)
        return(Ut_CFE_EVS_HookTable.CFE_EVS_Register(Filters, NumEventFilters, FilterScheme));

    return CFE_SUCCESS;
}

int32 CFE_EVS_SendEvent (uint16 EventID, uint16 EventType, const char *Spec, ... )
{
    char     BigBuf[CFE_EVS_MAX_MESSAGE_LENGTH];
    va_list  Ptr;
  
    va_start(Ptr, Spec);
    vsnprintf(BigBuf, CFE_EVS_MAX_MESSAGE_LENGTH, Spec, Ptr);
    va_end(Ptr);

    /* Check for specified return */
    if (Ut_CFE_EVS_UseReturnCode(UT_CFE_EVS_SENDEVENT_INDEX))
        return Ut_CFE_EVS_ReturnCodeTable[UT_CFE_EVS_SENDEVENT_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_EVS_HookTable.CFE_EVS_SendEvent)
         return(Ut_CFE_EVS_HookTable.CFE_EVS_SendEvent(EventID, EventType, BigBuf));
 
    return CFE_SUCCESS;
}

int32 CFE_EVS_SendTimedEvent (CFE_TIME_SysTime_t Time, uint16 EventID, uint16 EventType, const char *Spec, ... )
{
    /* Check for specified return */
    if (Ut_CFE_EVS_UseReturnCode(UT_CFE_EVS_SENDTIMEDEVENT_INDEX))
        return Ut_CFE_EVS_ReturnCodeTable[UT_CFE_EVS_SENDTIMEDEVENT_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_EVS_HookTable.CFE_EVS_SendTimedEvent)
         return(Ut_CFE_EVS_HookTable.CFE_EVS_SendTimedEvent(Time, EventID, EventType, Spec));

    return CFE_SUCCESS;
}

int32 CFE_EVS_SendEventWithAppID (uint16 EventID, uint16 EventType, uint32 AppID, const char *Spec, ... )
{
    /* Check for specified return */
    if (Ut_CFE_EVS_UseReturnCode(UT_CFE_EVS_SENDEVENTWITHAPPID_INDEX))
        return Ut_CFE_EVS_ReturnCodeTable[UT_CFE_EVS_SENDEVENTWITHAPPID_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_EVS_HookTable.CFE_EVS_SendEventWithAppID)
         return(Ut_CFE_EVS_HookTable.CFE_EVS_SendEventWithAppID(EventID, EventType, AppID, Spec));

    return CFE_SUCCESS;
}

int32 CFE_EVS_CleanUpApp (uint32 AppId)
{
    /* Check for specified return */
    if (Ut_CFE_EVS_UseReturnCode(UT_CFE_EVS_CLEANUPAPP_INDEX))
        return Ut_CFE_EVS_ReturnCodeTable[UT_CFE_EVS_CLEANUPAPP_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_EVS_HookTable.CFE_EVS_CleanUpApp)
        return(Ut_CFE_EVS_HookTable.CFE_EVS_CleanUpApp(AppId));

    return CFE_SUCCESS;
}

int32 CFE_EVS_ResetAllFilters(void)
{
    /* Check for specified return */
    if (Ut_CFE_EVS_UseReturnCode(UT_CFE_EVS_RESETALLFILTERS_INDEX))
        return Ut_CFE_EVS_ReturnCodeTable[UT_CFE_EVS_RESETALLFILTERS_INDEX].Value;
 
    /* Check for Function Hook */
    if (Ut_CFE_EVS_HookTable.CFE_EVS_ResetAllFilters)
        return Ut_CFE_EVS_HookTable.CFE_EVS_ResetAllFilters();

    return OS_FS_SUCCESS;
}

/* end ut_cfe_evs_stubs */

/* ut_ostimer_stubs */

Ut_OSTIMER_HookTable_t          Ut_OSTIMER_HookTable;
Ut_OSTIMER_ReturnCodeTable_t    Ut_OSTIMER_ReturnCodeTable[UT_OSTIMER_MAX_INDEX];

void Ut_OSTIMER_Reset(void)
{
    memset(&Ut_OSTIMER_HookTable, 0, sizeof(Ut_OSTIMER_HookTable));
    memset(&Ut_OSTIMER_ReturnCodeTable, 0, sizeof(Ut_OSTIMER_ReturnCodeTable));
}

void Ut_OSTIMER_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if          (Index == UT_OSTIMER_CREATE_INDEX)    { Ut_OSTIMER_HookTable.OS_TimerCreate = FunPtr; }
    else if     (Index == UT_OSTIMER_SET_INDEX)       { Ut_OSTIMER_HookTable.OS_TimerSet = FunPtr; }
    else
    {
        printf("Unsupported OSTIMER Index In SetFunctionHook Call %lu", Index);
        UtAssert_True(FALSE, "Unsupported OSTIMER Index In SetFunctionHook Call");
    }
}

void Ut_OSTIMER_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_OSTIMER_MAX_INDEX)
    {
        Ut_OSTIMER_ReturnCodeTable[Index].Value = RtnVal;
        Ut_OSTIMER_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported OSTIMER Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported OSTIMER Index In SetReturnCode Call");
    }
}

boolean Ut_OSTIMER_UseReturnCode(uint32 Index)
{
    if (Ut_OSTIMER_ReturnCodeTable[Index].Count > 0)
    {
        Ut_OSTIMER_ReturnCodeTable[Index].Count--;
        if (Ut_OSTIMER_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_OSTIMER_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }
    
    return(FALSE);
}

void Ut_OSTIMER_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_OSTIMER_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}

int32 OS_TimerCreate(uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy, OS_TimerCallback_t  callback_ptr)
{
    /* Check for specified return */
    if (Ut_OSTIMER_UseReturnCode(UT_OSTIMER_CREATE_INDEX))
        return Ut_OSTIMER_ReturnCodeTable[UT_OSTIMER_CREATE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSTIMER_HookTable.OS_TimerCreate)
        return(Ut_OSTIMER_HookTable.OS_TimerCreate(timer_id, timer_name, clock_accuracy, callback_ptr));

    return CFE_SUCCESS;
}

int32 OS_TimerSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
{
    /* Check for specified return */
    if (Ut_OSTIMER_UseReturnCode(UT_OSTIMER_SET_INDEX))
        return Ut_OSTIMER_ReturnCodeTable[UT_OSTIMER_SET_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_OSTIMER_HookTable.OS_TimerSet)
        return(Ut_OSTIMER_HookTable.OS_TimerSet(timer_id, start_time, interval_time));

    return CFE_SUCCESS;
}

/* end ut_ostimer_stubs */

/* ut_cfe_time_stubs */

Ut_CFE_TIME_HookTable_t         Ut_CFE_TIME_HookTable;
Ut_CFE_TIME_ReturnCodeTable_t   Ut_CFE_TIME_ReturnCodeTable[UT_CFE_TIME_MAX_INDEX];

void Ut_CFE_TIME_Reset(void)
{
    memset(&Ut_CFE_TIME_HookTable, 0, sizeof(Ut_CFE_TIME_HookTable));
    memset(&Ut_CFE_TIME_ReturnCodeTable, 0, sizeof(Ut_CFE_TIME_ReturnCodeTable));

    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_ADD_INDEX, &Ut_CFE_TIME_AddHook);
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_SUBTRACT_INDEX, &Ut_CFE_TIME_SubtractHook);
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_SUB2MICROSECS_INDEX, &Ut_CFE_TIME_Sub2MicroSecsHook);
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_MICRO2SUBSECS_INDEX, &Ut_CFE_TIME_Micro2SubSecsHook);
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_PRINT_INDEX, &Ut_CFE_TIME_PrintHook);
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_COMPARE_INDEX, &Ut_CFE_TIME_CompareHook);
}

void Ut_CFE_TIME_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_CFE_TIME_GETUTC_INDEX)                  { Ut_CFE_TIME_HookTable.CFE_TIME_GetUTC = FunPtr; }
    else if (Index == UT_CFE_TIME_GETTAI_INDEX)                  { Ut_CFE_TIME_HookTable.CFE_TIME_GetTAI = FunPtr; }
    else if (Index == UT_CFE_TIME_GETTIME_INDEX)                 { Ut_CFE_TIME_HookTable.CFE_TIME_GetTime = FunPtr; }
    else if (Index == UT_CFE_TIME_MICRO2SUBSECS_INDEX)           { Ut_CFE_TIME_HookTable.CFE_TIME_Micro2SubSecs = FunPtr; }
    else if (Index == UT_CFE_TIME_SUB2MICROSECS_INDEX)           { Ut_CFE_TIME_HookTable.CFE_TIME_Sub2MicroSecs = FunPtr; }
    else if (Index == UT_CFE_TIME_ADD_INDEX)                     { Ut_CFE_TIME_HookTable.CFE_TIME_Add = FunPtr; }
    else if (Index == UT_CFE_TIME_SUBTRACT_INDEX)                { Ut_CFE_TIME_HookTable.CFE_TIME_Subtract = FunPtr; }
    else if (Index == UT_CFE_TIME_PRINT_INDEX)                   { Ut_CFE_TIME_HookTable.CFE_TIME_Print = FunPtr; }
    else if (Index == UT_CFE_TIME_COMPARE_INDEX)                 { Ut_CFE_TIME_HookTable.CFE_TIME_Compare = FunPtr; }
    else if (Index == UT_CFE_TIME_FS2CFESECONDS_INDEX)           { Ut_CFE_TIME_HookTable.CFE_TIME_FS2CFESeconds = FunPtr; }
    else if (Index == UT_CFE_TIME_REGISTERSYNCHCALLBACK_INDEX)   { Ut_CFE_TIME_HookTable.CFE_TIME_RegisterSynchCallback = FunPtr; }
    else if (Index == UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX) { Ut_CFE_TIME_HookTable.CFE_TIME_UnregisterSynchCallback = FunPtr; }
    else
    {
        printf("Unsupported TIME Index In SetFunctionHook Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported TIME Index In SetFunctionHook Call");
    }
}

void Ut_CFE_TIME_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_CFE_TIME_MAX_INDEX)
    {
        Ut_CFE_TIME_ReturnCodeTable[Index].Value = RtnVal;
        Ut_CFE_TIME_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported TIME Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported TIME Index In SetReturnCode Call");
    }
}

boolean Ut_CFE_TIME_UseReturnCode(uint32 Index)
{
    if (Ut_CFE_TIME_ReturnCodeTable[Index].Count > 0)
    {
        Ut_CFE_TIME_ReturnCodeTable[Index].Count--;
        if (Ut_CFE_TIME_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_CFE_TIME_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }
    
    return(FALSE);
}

void Ut_CFE_TIME_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_CFE_TIME_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}

CFE_TIME_SysTime_t  CFE_TIME_GetUTC(void)
{
    CFE_TIME_SysTime_t  Time;
    
    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_GetUTC)
        return Ut_CFE_TIME_HookTable.CFE_TIME_GetUTC();
    
    Time.Seconds = 0;
    Time.Subseconds = 0;
    
    return Time;
}

CFE_TIME_SysTime_t  CFE_TIME_GetTAI(void)
{
    CFE_TIME_SysTime_t  Time;
    
    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_GetTAI)
        return Ut_CFE_TIME_HookTable.CFE_TIME_GetTAI();
    
    Time.Seconds = 0;
    Time.Subseconds = 0;
    
    return Time;
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

uint32  CFE_TIME_Micro2SubSecs(uint32 MicroSeconds)
{
    /* Check for specified return */
    if (Ut_CFE_TIME_UseReturnCode(UT_CFE_TIME_MICRO2SUBSECS_INDEX))
        return Ut_CFE_TIME_ReturnCodeTable[UT_CFE_TIME_MICRO2SUBSECS_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_Micro2SubSecs)
        return Ut_CFE_TIME_HookTable.CFE_TIME_Micro2SubSecs(MicroSeconds);
    
    return(0);
}

uint32  CFE_TIME_Sub2MicroSecs(uint32 SubSeconds)
{
    /* Check for specified return */
    if (Ut_CFE_TIME_UseReturnCode(UT_CFE_TIME_SUB2MICROSECS_INDEX))
        return Ut_CFE_TIME_ReturnCodeTable[UT_CFE_TIME_SUB2MICROSECS_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_Sub2MicroSecs)
        return Ut_CFE_TIME_HookTable.CFE_TIME_Sub2MicroSecs(SubSeconds);
    
    return(0);
}

CFE_TIME_SysTime_t  CFE_TIME_Add(CFE_TIME_SysTime_t Time1, CFE_TIME_SysTime_t Time2)
{
    CFE_TIME_SysTime_t  Time;

    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_Add)
        return Ut_CFE_TIME_HookTable.CFE_TIME_Add(Time1, Time2);
    
    Time.Seconds = 0;
    Time.Subseconds = 0;

    return Time;
}

CFE_TIME_SysTime_t  CFE_TIME_Subtract(CFE_TIME_SysTime_t Time1, CFE_TIME_SysTime_t Time2)
{
    CFE_TIME_SysTime_t  Time;

    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_Subtract)
        return Ut_CFE_TIME_HookTable.CFE_TIME_Subtract(Time1, Time2);
 
    Time.Seconds = 0;
    Time.Subseconds = 0;

    return Time;
}

void CFE_TIME_Print(char *PrintBuffer, CFE_TIME_SysTime_t TimeToPrint)
{
    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_Print)
        Ut_CFE_TIME_HookTable.CFE_TIME_Print(PrintBuffer, TimeToPrint);

    return;
}

CFE_TIME_Compare_t  CFE_TIME_Compare(CFE_TIME_SysTime_t TimeA, CFE_TIME_SysTime_t TimeB)
{
    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_Compare)
        return Ut_CFE_TIME_HookTable.CFE_TIME_Compare(TimeA, TimeB);

    return TRUE;
}

uint32  CFE_TIME_FS2CFESeconds(uint32 SecondsFS)
{
    /* Check for specified return */
    if (Ut_CFE_TIME_UseReturnCode(UT_CFE_TIME_FS2CFESECONDS_INDEX))
        return Ut_CFE_TIME_ReturnCodeTable[UT_CFE_TIME_FS2CFESECONDS_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_FS2CFESeconds)
        return Ut_CFE_TIME_HookTable.CFE_TIME_FS2CFESeconds(SecondsFS);
    
    return(0);
}

int32  CFE_TIME_RegisterSynchCallback(CFE_TIME_SynchCallbackPtr_t CallbackFuncPtr)
{
    /* Check for specified return */
    if (Ut_CFE_TIME_UseReturnCode(UT_CFE_TIME_REGISTERSYNCHCALLBACK_INDEX))
        return Ut_CFE_TIME_ReturnCodeTable[UT_CFE_TIME_REGISTERSYNCHCALLBACK_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_RegisterSynchCallback)
        return Ut_CFE_TIME_HookTable.CFE_TIME_RegisterSynchCallback(CallbackFuncPtr);
    
    return(0);
}

int32  CFE_TIME_UnregisterSynchCallback(CFE_TIME_SynchCallbackPtr_t CallbackFuncPtr) 
{
    /* Check for specified return */
    if (Ut_CFE_TIME_UseReturnCode(UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX))
        return Ut_CFE_TIME_ReturnCodeTable[UT_CFE_TIME_UNREGISTERSYNCHCALLBACK_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_UnregisterSynchCallback)
        return Ut_CFE_TIME_HookTable.CFE_TIME_UnregisterSynchCallback(CallbackFuncPtr);
    
    return(0);
}

uint32  CFE_TIME_GetMETsubsecs(void) 
{
    /* Check for specified return */
    if (Ut_CFE_TIME_UseReturnCode(UT_CFE_TIME_GETMETSUBSECS_INDEX))
        return Ut_CFE_TIME_ReturnCodeTable[UT_CFE_TIME_GETMETSUBSECS_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_GetMETsubsecs)
        return Ut_CFE_TIME_HookTable.CFE_TIME_GetMETsubsecs();
    
    return(0);
}

uint16  CFE_TIME_GetClockInfo(void) 
{
    /* Check for specified return */
    if (Ut_CFE_TIME_UseReturnCode(UT_CFE_TIME_GETCLOCKINFO_INDEX))
        return Ut_CFE_TIME_ReturnCodeTable[UT_CFE_TIME_GETCLOCKINFO_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_TIME_HookTable.CFE_TIME_GetClockInfo)
        return Ut_CFE_TIME_HookTable.CFE_TIME_GetClockInfo();
    
    return(0);
}

/* end ut_cfe_time_stubs */



/* ut_cfe_sb_stubs */

CFE_SB_Qos_t CFE_SB_Default_Qos;

Ut_CFE_SB_HookTable_t           Ut_CFE_SB_HookTable;
Ut_CFE_SB_ReturnCodeTable_t     Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_MAX_INDEX];

void Ut_CFE_SB_Reset(void)
{
    memset(&Ut_CFE_SB_HookTable, 0, sizeof(Ut_CFE_SB_HookTable));
    memset(&Ut_CFE_SB_ReturnCodeTable, 0, sizeof(Ut_CFE_SB_ReturnCodeTable));
    
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_CREATEPIPE_INDEX, &Ut_CFE_SB_CreatePipeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SENDMSG_INDEX, &Ut_CFE_SB_SendMsgHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_RCVMSG_INDEX, &Ut_CFE_SB_RcvMsgHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_INITMSG_INDEX, &Ut_CFE_SB_InitMsgHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_MSGHDRSIZE_INDEX, &Ut_CFE_SB_MsgHdrSizeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETUSERDATA_INDEX, &Ut_CFE_SB_GetUserDataHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETMSGID_INDEX, &Ut_CFE_SB_GetMsgIdHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SETMSGID_INDEX, &Ut_CFE_SB_SetMsgIdHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETUSERDATALENGTH_INDEX, &Ut_CFE_SB_GetUserDataLengthHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SETUSERDATALENGTH_INDEX, &Ut_CFE_SB_SetUserDataLengthHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX, &Ut_CFE_SB_GetTotalMsgLengthHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SETTOTALMSGLENGTH_INDEX, &Ut_CFE_SB_SetTotalMsgLengthHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETMSGTIME_INDEX, &Ut_CFE_SB_GetMsgTimeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SETMSGTIME_INDEX, &Ut_CFE_SB_SetMsgTimeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCMDCODE_INDEX, &Ut_CFE_SB_GetCmdCodeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_SETCMDCODE_INDEX, &Ut_CFE_SB_SetCmdCodeHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GETCHECKSUM_INDEX, &Ut_CFE_SB_GetChecksumHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_GENERATECHECKSUM_INDEX, &Ut_CFE_SB_GenerateChecksumHook);
    Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_VALIDATECHECKSUM_INDEX, &Ut_CFE_SB_ValidateChecksumHook);

    Ut_CFE_SB_ClearPipes();
    Ut_CFE_SB_ClearMsgQueue();
}

void Ut_CFE_SB_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_CFE_SB_CREATEPIPE_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_CreatePipe = FunPtr; }
    else if (Index == UT_CFE_SB_DELETEPIPE_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_DeletePipe = FunPtr; }
    else if (Index == UT_CFE_SB_SUBSCRIBEEX_INDEX)        { Ut_CFE_SB_HookTable.CFE_SB_SubscribeEx = FunPtr; }
    else if (Index == UT_CFE_SB_SUBSCRIBE_INDEX)          { Ut_CFE_SB_HookTable.CFE_SB_Subscribe = FunPtr; }
    else if (Index == UT_CFE_SB_SUBSCRIBELOCAL_INDEX)     { Ut_CFE_SB_HookTable.CFE_SB_SubscribeLocal = FunPtr; }
    else if (Index == UT_CFE_SB_UNSUBSCRIBE_INDEX)        { Ut_CFE_SB_HookTable.CFE_SB_Unsubscribe = FunPtr; }
    else if (Index == UT_CFE_SB_UNSUBSCRIBELOCAL_INDEX)   { Ut_CFE_SB_HookTable.CFE_SB_UnsubscribeLocal = FunPtr; }
    else if (Index == UT_CFE_SB_SENDMSG_INDEX)            { Ut_CFE_SB_HookTable.CFE_SB_SendMsg = FunPtr; }
    else if (Index == UT_CFE_SB_PASSMSG_INDEX)            { Ut_CFE_SB_HookTable.CFE_SB_PassMsg = FunPtr; }
    else if (Index == UT_CFE_SB_RCVMSG_INDEX)             { Ut_CFE_SB_HookTable.CFE_SB_RcvMsg = FunPtr; }
    else if (Index == UT_CFE_SB_GETLASTSENDERID_INDEX)    { Ut_CFE_SB_HookTable.CFE_SB_GetLastSenderId = FunPtr; }
    else if (Index == UT_CFE_SB_ZEROCOPYGETPTR_INDEX)     { Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyGetPtr = FunPtr; }
    else if (Index == UT_CFE_SB_ZEROCOPYRELEASEPTR_INDEX) { Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyReleasePtr = FunPtr; }
    else if (Index == UT_CFE_SB_ZEROCOPYSEND_INDEX)       { Ut_CFE_SB_HookTable.CFE_SB_ZeroCopySend = FunPtr; }
    else if (Index == UT_CFE_SB_ZEROCOPYPASS_INDEX)       { Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyPass = FunPtr; }
    else if (Index == UT_CFE_SB_INITMSG_INDEX)            { Ut_CFE_SB_HookTable.CFE_SB_InitMsg = FunPtr; }
    else if (Index == UT_CFE_SB_MSGHDRSIZE_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_MsgHdrSize = FunPtr; }
    else if (Index == UT_CFE_SB_GETUSERDATA_INDEX)        { Ut_CFE_SB_HookTable.CFE_SB_GetUserData = FunPtr; }
    else if (Index == UT_CFE_SB_GETMSGID_INDEX)           { Ut_CFE_SB_HookTable.CFE_SB_GetMsgId = FunPtr; }
    else if (Index == UT_CFE_SB_SETMSGID_INDEX)           { Ut_CFE_SB_HookTable.CFE_SB_SetMsgId = FunPtr; }
    else if (Index == UT_CFE_SB_GETUSERDATALENGTH_INDEX)  { Ut_CFE_SB_HookTable.CFE_SB_GetUserDataLength = FunPtr; }
    else if (Index == UT_CFE_SB_SETUSERDATALENGTH_INDEX)  { Ut_CFE_SB_HookTable.CFE_SB_SetUserDataLength = FunPtr; }
    else if (Index == UT_CFE_SB_GETTOTALMSGLENGTH_INDEX)  { Ut_CFE_SB_HookTable.CFE_SB_GetTotalMsgLength = FunPtr; }
    else if (Index == UT_CFE_SB_SETTOTALMSGLENGTH_INDEX)  { Ut_CFE_SB_HookTable.CFE_SB_SetTotalMsgLength = FunPtr; }
    else if (Index == UT_CFE_SB_GETMSGTIME_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_GetMsgTime = FunPtr; }
    else if (Index == UT_CFE_SB_SETMSGTIME_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_SetMsgTime = FunPtr; }
    else if (Index == UT_CFE_SB_TIMESTAMPMSG_INDEX)       { Ut_CFE_SB_HookTable.CFE_SB_TimeStampMsg = FunPtr; }
    else if (Index == UT_CFE_SB_GETCMDCODE_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_GetCmdCode = FunPtr; }
    else if (Index == UT_CFE_SB_SETCMDCODE_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_SetCmdCode = FunPtr; }
    else if (Index == UT_CFE_SB_GETCHECKSUM_INDEX)        { Ut_CFE_SB_HookTable.CFE_SB_GetChecksum = FunPtr; }
    else if (Index == UT_CFE_SB_GENERATECHECKSUM_INDEX)   { Ut_CFE_SB_HookTable.CFE_SB_GenerateChecksum = FunPtr; }
    else if (Index == UT_CFE_SB_VALIDATECHECKSUM_INDEX)   { Ut_CFE_SB_HookTable.CFE_SB_ValidateChecksum = FunPtr; }
    else if (Index == UT_CFE_SB_CLEANUPAPP_INDEX)         { Ut_CFE_SB_HookTable.CFE_SB_CleanUpApp = FunPtr; }
    else
    {
        printf("Unsupported SB Index In SetFunctionHook Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported SB Index In SetFunctionHook Call");
    }
}

void Ut_CFE_SB_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_CFE_SB_MAX_INDEX) {
        Ut_CFE_SB_ReturnCodeTable[Index].Value = RtnVal;
        Ut_CFE_SB_ReturnCodeTable[Index].Count = CallCnt;
    }
    else
    {
        printf("Unsupported SB Index In SetReturnCode Call %lu\n", Index);
        UtAssert_True(FALSE, "Unsupported SB Index In SetReturnCode Call");
    }
}

boolean Ut_CFE_SB_UseReturnCode(uint32 Index)
{
    if (Ut_CFE_SB_ReturnCodeTable[Index].Count > 0)
    {
        Ut_CFE_SB_ReturnCodeTable[Index].Count--;
        if (Ut_CFE_SB_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }
    else if (Ut_CFE_SB_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero == TRUE)
    {
        return(TRUE);
    }
    
    return(FALSE);
}

void Ut_CFE_SB_ContinueReturnCodeAfterCountZero(uint32 Index)
{
    Ut_CFE_SB_ReturnCodeTable[Index].ContinueReturnCodeAfterCountZero = TRUE;
}

int32 CFE_SB_CreatePipe (CFE_SB_PipeId_t *PipeIdPtr, uint16 Depth, const char *PipeName)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_CREATEPIPE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_CREATEPIPE_INDEX].Value;
 
    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_CreatePipe)
        return Ut_CFE_SB_HookTable.CFE_SB_CreatePipe(PipeIdPtr, Depth, PipeName);
        
    return CFE_SUCCESS;
}

int32 CFE_SB_DeletePipe (CFE_SB_PipeId_t PipeId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_DELETEPIPE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_DELETEPIPE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_DeletePipe)
        return Ut_CFE_SB_HookTable.CFE_SB_DeletePipe(PipeId);
        
    return CFE_SUCCESS;
}

int32 CFE_SB_SubscribeEx (CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId,
                          CFE_SB_Qos_t Quality, uint16 MsgLim)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_SUBSCRIBEEX_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_SUBSCRIBEEX_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SubscribeEx)
        return Ut_CFE_SB_HookTable.CFE_SB_SubscribeEx(MsgId, PipeId, Quality, MsgLim);

    return CFE_SUCCESS;
}

int32 CFE_SB_Subscribe(CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_SUBSCRIBE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_SUBSCRIBE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_Subscribe)
        return Ut_CFE_SB_HookTable.CFE_SB_Subscribe(MsgId, PipeId);
    
    return CFE_SUCCESS;
}

int32 CFE_SB_SubscribeLocal(CFE_SB_MsgId_t   MsgId, CFE_SB_PipeId_t  PipeId, uint16 MsgLim)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_SUBSCRIBELOCAL_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_SUBSCRIBELOCAL_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SubscribeLocal)
        return Ut_CFE_SB_HookTable.CFE_SB_SubscribeLocal(MsgId, PipeId, MsgLim);
    
    return CFE_SUCCESS;
}

int32 CFE_SB_Unsubscribe(CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_UNSUBSCRIBE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_UNSUBSCRIBE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_Unsubscribe)
        return Ut_CFE_SB_HookTable.CFE_SB_Unsubscribe(MsgId, PipeId);
    
    return CFE_SUCCESS;
}

int32 CFE_SB_UnsubscribeLocal(CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_UNSUBSCRIBELOCAL_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_UNSUBSCRIBELOCAL_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_UnsubscribeLocal)
        return Ut_CFE_SB_HookTable.CFE_SB_UnsubscribeLocal(MsgId, PipeId);
    
    return CFE_SUCCESS;
}

int32 CFE_SB_SendMsg (CFE_SB_Msg_t *MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_SENDMSG_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_SENDMSG_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SendMsg)
        return Ut_CFE_SB_HookTable.CFE_SB_SendMsg(MsgPtr);

    return CFE_SUCCESS;
}

int32 CFE_SB_PassMsg (CFE_SB_Msg_t *MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_PASSMSG_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_PASSMSG_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_PassMsg)
        return Ut_CFE_SB_HookTable.CFE_SB_PassMsg(MsgPtr);

    return CFE_SUCCESS;
}

int32 CFE_SB_RcvMsg (CFE_SB_MsgPtr_t *BufPtr, CFE_SB_PipeId_t PipeId,
                    int32 TimeOut)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_RCVMSG_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_RCVMSG_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_RcvMsg)
        return Ut_CFE_SB_HookTable.CFE_SB_RcvMsg(BufPtr,PipeId,TimeOut);
    
    return CFE_SUCCESS;
}

uint32  CFE_SB_GetLastSenderId(CFE_SB_SenderId_t **Ptr,CFE_SB_PipeId_t  PipeId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETLASTSENDERID_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETLASTSENDERID_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetLastSenderId)
        return Ut_CFE_SB_HookTable.CFE_SB_GetLastSenderId(Ptr, PipeId);

    return CFE_SUCCESS;
}

CFE_SB_Msg_t  *CFE_SB_ZeroCopyGetPtr(uint16  MsgSize,CFE_SB_ZeroCopyHandle_t *BufferHandle)
{
    /* Can't specify return value - Ut_CFE_SB_SetReturnCode can only return integers */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyGetPtr)
        Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyGetPtr(MsgSize, BufferHandle);

    return NULL;
}

int32 CFE_SB_ZeroCopyReleasePtr(CFE_SB_Msg_t  *Ptr2Release,CFE_SB_ZeroCopyHandle_t BufferHandle)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_ZEROCOPYRELEASEPTR_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_ZEROCOPYRELEASEPTR_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyReleasePtr)
        return Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyReleasePtr(Ptr2Release, BufferHandle);

    return CFE_SUCCESS;
}

int32 CFE_SB_ZeroCopySend(CFE_SB_Msg_t   *MsgPtr, CFE_SB_ZeroCopyHandle_t BufferHandle)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_ZEROCOPYSEND_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_ZEROCOPYSEND_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_ZeroCopySend)
        return Ut_CFE_SB_HookTable.CFE_SB_ZeroCopySend(MsgPtr, BufferHandle);

    return CFE_SUCCESS;
}

int32 CFE_SB_ZeroCopyPass(CFE_SB_Msg_t   *MsgPtr, CFE_SB_ZeroCopyHandle_t BufferHandle)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_ZEROCOPYPASS_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_ZEROCOPYPASS_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyPass)
        return Ut_CFE_SB_HookTable.CFE_SB_ZeroCopyPass(MsgPtr, BufferHandle);

    return CFE_SUCCESS;
}

void CFE_SB_InitMsg (void *MsgPtr, CFE_SB_MsgId_t MsgId, uint16 Length, boolean Clear)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_InitMsg)
        Ut_CFE_SB_HookTable.CFE_SB_InitMsg(MsgPtr,MsgId,Length,Clear);

    return;       
}

uint16 CFE_SB_MsgHdrSize (CFE_SB_MsgId_t MsgId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_MSGHDRSIZE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_MSGHDRSIZE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_MsgHdrSize)
        return Ut_CFE_SB_HookTable.CFE_SB_MsgHdrSize(MsgId);
 
    return CFE_SUCCESS;
}

void *CFE_SB_GetUserData(CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETUSERDATA_INDEX))
        return (void *)(Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETUSERDATA_INDEX].Value);

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetUserData)
        return Ut_CFE_SB_HookTable.CFE_SB_GetUserData(MsgPtr);

    return  NULL;    
}

CFE_SB_MsgId_t CFE_SB_GetMsgId (CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETMSGID_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETMSGID_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetMsgId)
        return Ut_CFE_SB_HookTable.CFE_SB_GetMsgId(MsgPtr);
    
    return CFE_SUCCESS;
}

void  CFE_SB_SetMsgId (CFE_SB_MsgPtr_t MsgPtr, CFE_SB_MsgId_t MsgId)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SetMsgId)
        Ut_CFE_SB_HookTable.CFE_SB_SetMsgId(MsgPtr,MsgId);
    
    return;
}

uint16 CFE_SB_GetUserDataLength(CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETUSERDATALENGTH_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETUSERDATALENGTH_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetUserDataLength)
        return Ut_CFE_SB_HookTable.CFE_SB_GetUserDataLength(MsgPtr);
    
    return CFE_SUCCESS;
}/* end CFE_SB_GetUserDataLength */

void CFE_SB_SetUserDataLength(CFE_SB_MsgPtr_t MsgPtr,uint16 DataLength)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SetUserDataLength)
        Ut_CFE_SB_HookTable.CFE_SB_SetUserDataLength(MsgPtr,DataLength);
    
    return;
}

uint16 CFE_SB_GetTotalMsgLength(CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETTOTALMSGLENGTH_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETTOTALMSGLENGTH_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetTotalMsgLength)
        return Ut_CFE_SB_HookTable.CFE_SB_GetTotalMsgLength(MsgPtr);
    
    return CFE_SUCCESS;
}

void CFE_SB_SetTotalMsgLength(CFE_SB_MsgPtr_t MsgPtr,uint16 TotalLength)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SetTotalMsgLength)
        Ut_CFE_SB_HookTable.CFE_SB_SetTotalMsgLength(MsgPtr,TotalLength);
    
    return;
}

CFE_TIME_SysTime_t CFE_SB_GetMsgTime (CFE_SB_MsgPtr_t MsgPtr)
{
    CFE_TIME_SysTime_t  Time;

    /* Can't specify return value - ReturnCodeTable can only store integer values */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetMsgTime)
        return Ut_CFE_SB_HookTable.CFE_SB_GetMsgTime(MsgPtr);
        
    Time.Seconds    =  0;
    Time.Subseconds =  0;    
    
    return Time;
}

int32 CFE_SB_SetMsgTime (CFE_SB_MsgPtr_t MsgPtr, CFE_TIME_SysTime_t time)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_SETMSGTIME_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_SETMSGTIME_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SetMsgTime)
        return Ut_CFE_SB_HookTable.CFE_SB_SetMsgTime(MsgPtr, time);

    return CFE_SUCCESS;
}

void CFE_SB_TimeStampMsg (CFE_SB_MsgPtr_t MsgPtr)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_TimeStampMsg)
        Ut_CFE_SB_HookTable.CFE_SB_TimeStampMsg(MsgPtr);

    return;
}

uint16 CFE_SB_GetCmdCode (CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETCMDCODE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETCMDCODE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetCmdCode)
        return Ut_CFE_SB_HookTable.CFE_SB_GetCmdCode(MsgPtr);

    return CFE_SUCCESS;
}

int32 CFE_SB_SetCmdCode (CFE_SB_MsgPtr_t MsgPtr, uint16 CmdCode)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_SETCMDCODE_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_SETCMDCODE_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_SetCmdCode)
        return Ut_CFE_SB_HookTable.CFE_SB_SetCmdCode(MsgPtr,CmdCode);

    return CFE_SUCCESS;
}

uint16 CFE_SB_GetChecksum(CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_GETCHECKSUM_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_GETCHECKSUM_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GetChecksum)
        return Ut_CFE_SB_HookTable.CFE_SB_GetChecksum(MsgPtr);

    return CFE_SUCCESS;
}

void CFE_SB_GenerateChecksum(CFE_SB_MsgPtr_t MsgPtr)
{
    /* Can't specify return value - this is a void function */

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_GenerateChecksum)
        Ut_CFE_SB_HookTable.CFE_SB_GenerateChecksum(MsgPtr);

    return;
}

boolean CFE_SB_ValidateChecksum(CFE_SB_MsgPtr_t MsgPtr)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_VALIDATECHECKSUM_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_VALIDATECHECKSUM_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_ValidateChecksum)
        return Ut_CFE_SB_HookTable.CFE_SB_ValidateChecksum(MsgPtr);

    return(TRUE);
}

int32 CFE_SB_CleanUpApp (uint32 AppId)
{
    /* Check for specified return */
    if (Ut_CFE_SB_UseReturnCode(UT_CFE_SB_CLEANUPAPP_INDEX))
        return Ut_CFE_SB_ReturnCodeTable[UT_CFE_SB_CLEANUPAPP_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_SB_HookTable.CFE_SB_CleanUpApp)
        return Ut_CFE_SB_HookTable.CFE_SB_CleanUpApp(AppId);

    return CFE_SUCCESS;
}

/* end ut_cfe_sb_stubs */

/************************/
/*  End of File Comment */
/************************/
