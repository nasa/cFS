/*
**
** File: ut_cfe_es_stubs.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_es_stubs.c 1.2 2011/05/04 11:28:00EDT rmcgraw Exp  $
**
** Purpose: Unit test stubs for cFE Executive Services routines
**
** $Log: ut_cfe_es_stubs.c  $
** Revision 1.2 2011/05/04 11:28:00EDT rmcgraw
** Changed PoolCreateEx to have new parameter USE_MUTEX
** Revision 1.1 2011/05/04 11:20:51EDT rmcgraw
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/04/08 16:26:37EDT rmcgraw
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.5 2011/03/31 14:53:04EDT rmcgraw
** Added functionality and supressed compiler warnings
** Revision 1.4 2011/03/23 17:08:18EDT rmcgraw
** OS_FS_ERROR to OS_FS_SUCCESS for some OS file sys apis
** Revision 1.3 2011/03/09 10:26:12EST rmcgraw
** Added SetRtnCode logic to PoolCreateEx
** Revision 1.2 2011/03/07 17:53:39EST sslegel
** Added a default hook for ES_CFE_RunLoop
** Added additional return code support
** Revision 1.1 2011/02/15 11:13:01EST sslegel
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/src/project.pj
**
*/

/*
** Include section
*/

#include "cfe.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include <string.h>

Ut_CFE_ES_HookTable_t           Ut_CFE_ES_HookTable;
Ut_CFE_ES_ReturnCodeTable_t     Ut_CFE_ES_ReturnCodeTable[UT_CFE_ES_MAX_INDEX];

void Ut_CFE_ES_Reset(void)
{
    memset(&Ut_CFE_ES_HookTable, 0, sizeof(Ut_CFE_ES_HookTable));
    memset(&Ut_CFE_ES_ReturnCodeTable, 0, sizeof(Ut_CFE_ES_ReturnCodeTable));

    Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_RUNLOOP_INDEX, &Ut_CFE_ES_RunLoopHook);
}

void Ut_CFE_ES_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == UT_CFE_ES_RESETCFE_INDEX)             { Ut_CFE_ES_HookTable.CFE_ES_ResetCFE = FunPtr; }
    else if (Index == UT_CFE_ES_RESTARTAPP_INDEX)           { Ut_CFE_ES_HookTable.CFE_ES_RestartApp = FunPtr; }
    else if (Index == UT_CFE_ES_RELOADAPP_INDEX)            { Ut_CFE_ES_HookTable.CFE_ES_ReloadApp = FunPtr; }
    else if (Index == UT_CFE_ES_DELETEAPP_INDEX)            { Ut_CFE_ES_HookTable.CFE_ES_DeleteApp = FunPtr; }
    else if (Index == UT_CFE_ES_EXITAPP_INDEX)              { Ut_CFE_ES_HookTable.CFE_ES_ExitApp = FunPtr; }
    else if (Index == UT_CFE_ES_RUNLOOP_INDEX)              { Ut_CFE_ES_HookTable.CFE_ES_RunLoop = FunPtr; }
    else if (Index == UT_CFE_ES_WAITFORSTARTUPSYNC_INDEX)   { Ut_CFE_ES_HookTable.CFE_ES_WaitForStartupSync = FunPtr; }
    else if (Index == UT_CFE_ES_REGISTERAPP_INDEX)          { Ut_CFE_ES_HookTable.CFE_ES_RegisterApp = FunPtr; }
    else if (Index == UT_CFE_ES_GETAPPID_INDEX)             { Ut_CFE_ES_HookTable.CFE_ES_GetAppID = FunPtr; }
    else if (Index == UT_CFE_ES_GETAPPIDBYNAME_INDEX)       { Ut_CFE_ES_HookTable.CFE_ES_GetAppIDByName = FunPtr; }
    else if (Index == UT_CFE_ES_GETAPPNAME_INDEX)           { Ut_CFE_ES_HookTable.CFE_ES_GetAppName = FunPtr; }
    else if (Index == UT_CFE_ES_GETAPPINFO_INDEX)           { Ut_CFE_ES_HookTable.CFE_ES_GetAppInfo = FunPtr; }
    else if (Index == UT_CFE_ES_GETTASKINFO_INDEX)          { Ut_CFE_ES_HookTable.CFE_ES_GetTaskInfo = FunPtr; }
    else if (Index == UT_CFE_ES_REGISTERCHILDTASK_INDEX)    { Ut_CFE_ES_HookTable.CFE_ES_RegisterChildTask = FunPtr; }
    else if (Index == UT_CFE_ES_CREATECHILDTASK_INDEX)      { Ut_CFE_ES_HookTable.CFE_ES_CreateChildTask = FunPtr; }
    else if (Index == UT_CFE_ES_DELETECHILDTASK_INDEX)      { Ut_CFE_ES_HookTable.CFE_ES_DeleteChildTask = FunPtr; }
    else if (Index == UT_CFE_ES_EXITCHILDTASK_INDEX)        { Ut_CFE_ES_HookTable.CFE_ES_ExitChildTask = FunPtr; }
    else if (Index == UT_CFE_ES_INCREMENTTASKCOUNTER_INDEX) { Ut_CFE_ES_HookTable.CFE_ES_IncrementTaskCounter = FunPtr; }
    else if (Index == UT_CFE_ES_WRITETOSYSLOG_INDEX)        { Ut_CFE_ES_HookTable.CFE_ES_WriteToSysLog = FunPtr; }
    else if (Index == UT_CFE_ES_REGISTERDRIVER_INDEX)       { Ut_CFE_ES_HookTable.CFE_ES_RegisterDriver = FunPtr; }
    else if (Index == UT_CFE_ES_UNLOADDRIVER_INDEX)         { Ut_CFE_ES_HookTable.CFE_ES_UnloadDriver = FunPtr; }
    else if (Index == UT_CFE_ES_CALCULATECRC_INDEX)         { Ut_CFE_ES_HookTable.CFE_ES_CalculateCRC = FunPtr; }
    else if (Index == UT_CFE_ES_REGISTERCDS_INDEX)          { Ut_CFE_ES_HookTable.CFE_ES_RegisterCDS = FunPtr; }
    else if (Index == UT_CFE_ES_COPYTOCDS_INDEX)            { Ut_CFE_ES_HookTable.CFE_ES_CopyToCDS = FunPtr; }
    else if (Index == UT_CFE_ES_RESTOREFROMCDS_INDEX)       { Ut_CFE_ES_HookTable.CFE_ES_RestoreFromCDS = FunPtr; }
    else if (Index == UT_CFE_ES_POOLCREATE_INDEX)           { Ut_CFE_ES_HookTable.CFE_ES_PoolCreate = FunPtr; }
    else if (Index == UT_CFE_ES_POOLCREATEEX_INDEX)         { Ut_CFE_ES_HookTable.CFE_ES_PoolCreateEx = FunPtr; }
    else if (Index == UT_CFE_ES_GETPOOLBUF_INDEX)           { Ut_CFE_ES_HookTable.CFE_ES_GetPoolBuf = FunPtr; }
    else if (Index == UT_CFE_ES_GETPOOLBUFINFO_INDEX)       { Ut_CFE_ES_HookTable.CFE_ES_GetPoolBufInfo = FunPtr; }
    else if (Index == UT_CFE_ES_PUTPOOLBUF_INDEX)           { Ut_CFE_ES_HookTable.CFE_ES_PutPoolBuf = FunPtr; }
    else if (Index == UT_CFE_ES_GETMEMPOOLSTATS_INDEX)      { Ut_CFE_ES_HookTable.CFE_ES_GetMemPoolStats = FunPtr; }
    else if (Index == UT_CFE_ES_PERFLOGADD_INDEX)           { Ut_CFE_ES_HookTable.CFE_ES_PerfLogAdd = FunPtr; }
    else                                                    { printf("Unsupported ES Index In SetFunctionHook Call %u\n", Index); }
}

void Ut_CFE_ES_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_CFE_ES_MAX_INDEX) {
        Ut_CFE_ES_ReturnCodeTable[Index].Value = RtnVal;
        Ut_CFE_ES_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported ES Index In SetReturnCode Call %u\n", Index);
    }
}

boolean Ut_CFE_ES_UseReturnCode(uint32 Index)
{
    if (Ut_CFE_ES_ReturnCodeTable[Index].Count > 0) {
        Ut_CFE_ES_ReturnCodeTable[Index].Count--;
        if (Ut_CFE_ES_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}

int32 CFE_ES_GetResetType(uint32 *ResetSubtypePtr)
{
    return CFE_SUCCESS;
}

int32  CFE_ES_ResetCFE(uint32 ResetType)
{
        /* Check for specified return */
    if (Ut_CFE_ES_UseReturnCode(UT_CFE_ES_RESETCFE_INDEX))
        return Ut_CFE_ES_ReturnCodeTable[UT_CFE_ES_RESETCFE_INDEX].Value;

    return CFE_SUCCESS;
}

int32 CFE_ES_RestartApp(uint32 AppID)
{
    if (Ut_CFE_ES_UseReturnCode(UT_CFE_ES_RESTARTAPP_INDEX))
        return Ut_CFE_ES_ReturnCodeTable[UT_CFE_ES_RESTARTAPP_INDEX].Value;

    return CFE_SUCCESS;
}

int32 CFE_ES_ReloadApp(uint32 AppID, const char *AppFileName)
{
    if (Ut_CFE_ES_UseReturnCode(UT_CFE_ES_RELOADAPP_INDEX))
        return Ut_CFE_ES_ReturnCodeTable[UT_CFE_ES_RELOADAPP_INDEX].Value;

    return CFE_SUCCESS;
}

int32 CFE_ES_DeleteApp(uint32 AppID)
{
    if (Ut_CFE_ES_UseReturnCode(UT_CFE_ES_DELETEAPP_INDEX))
        return Ut_CFE_ES_ReturnCodeTable[UT_CFE_ES_DELETEAPP_INDEX].Value;

    return CFE_SUCCESS;
}

void CFE_ES_ExitApp(uint32 ExitStatus)
{
    return;
}

int32 CFE_ES_RunLoop(uint32 *ExitStatus)
{
    if (Ut_CFE_ES_UseReturnCode(UT_CFE_ES_RUNLOOP_INDEX))
        return Ut_CFE_ES_ReturnCodeTable[UT_CFE_ES_RUNLOOP_INDEX].Value;

    if (Ut_CFE_ES_HookTable.CFE_ES_RunLoop)
        return Ut_CFE_ES_HookTable.CFE_ES_RunLoop(ExitStatus);

    return TRUE;
}

void CFE_ES_WaitForStartupSync(uint32 TimeOutMilliseconds)
{
    return;
}

int32 CFE_ES_RegisterApp(void)
{
    if (Ut_CFE_ES_UseReturnCode(UT_CFE_ES_REGISTERAPP_INDEX))
        return Ut_CFE_ES_ReturnCodeTable[UT_CFE_ES_REGISTERAPP_INDEX].Value;

    return CFE_SUCCESS;
}

int32 CFE_ES_GetAppID(uint32 *AppIdPtr)
{
    return CFE_SUCCESS;
}

int32 CFE_ES_GetAppIDByName(uint32 *AppIdPtr, const char *AppName)
{
    return CFE_SUCCESS;
}

int32 CFE_ES_GetAppName(char *AppName, uint32 AppId, uint32 BufferLength)
{
    return CFE_SUCCESS;
}

int32 CFE_ES_GetAppInfo(CFE_ES_AppInfo_t *AppInfo, uint32 AppId)
{
    return CFE_SUCCESS;
}

int32 CFE_ES_GetTaskInfo(CFE_ES_TaskInfo_t *TaskInfo, uint32 TaskId)
{
    return CFE_SUCCESS;
}

int32  CFE_ES_RegisterChildTask(void)
{
    return CFE_SUCCESS;
}

int32  CFE_ES_CreateChildTask(uint32                          *TaskIdPtr,
                              const char                      *TaskName,
                              CFE_ES_ChildTaskMainFuncPtr_t    FunctionPtr,
                              uint32                          *StackPtr,
                              uint32                           StackSize,
                              uint32                           Priority,
                              uint32                           Flags)
{
    /* Check for specified return */
    if (Ut_CFE_ES_UseReturnCode(UT_CFE_ES_CREATECHILDTASK_INDEX))
        return Ut_CFE_ES_ReturnCodeTable[UT_CFE_ES_CREATECHILDTASK_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_ES_HookTable.CFE_ES_CreateChildTask)
        return Ut_CFE_ES_HookTable.CFE_ES_CreateChildTask(TaskIdPtr, TaskName, FunctionPtr, StackPtr, StackSize, Priority, Flags);

    return CFE_SUCCESS;
}

int32 CFE_ES_DeleteChildTask(uint32 TaskId)
{
    return CFE_SUCCESS;
}

void CFE_ES_ExitChildTask(void)
{
    return;
}

void  CFE_ES_IncrementTaskCounter(void)
{
    return;
}

int32 CFE_ES_WriteToSysLog(const char *SpecStringPtr, ...)
{
    return CFE_SUCCESS;
}

int32 CFE_ES_RegisterDriver(uint32 *DriverIdPtr, uint32 *DriverDescPtr)
{
    return CFE_SUCCESS;
}

int32 CFE_ES_UnloadDriver(uint32 DriverId)
{
    return CFE_SUCCESS;
}

uint32 CFE_ES_CalculateCRC(const void *DataPtr, uint32 DataLength, uint32 InputCRC, uint32 TypeCRC)
{
    return CFE_SUCCESS;
}

int32 CFE_ES_RegisterCDS(CFE_ES_CDSHandle_t *HandlePtr, int32 BlockSize, const char *Name)
{
    return CFE_SUCCESS;
}

int32 CFE_ES_CopyToCDS(CFE_ES_CDSHandle_t Handle, void *DataToCopy)
{
    return CFE_SUCCESS;
}

int32 CFE_ES_RestoreFromCDS(void *RestoreToMemory, CFE_ES_CDSHandle_t Handle)
{
    return CFE_SUCCESS;
}

int32 CFE_ES_PoolCreate(CFE_ES_MemHandle_t *HandlePtr, uint8 *MemPtr, uint32 Size)
{
    return CFE_SUCCESS;
}

int32 CFE_ES_PoolCreateEx(CFE_ES_MemHandle_t *HandlePtr, uint8 *MemPtr, uint32 Size, uint32 NumBlockSizes, uint32 *BlockSizes, uint16 UseMutex)
{
    /* Check for specified return */
    if (Ut_CFE_ES_UseReturnCode(UT_CFE_ES_POOLCREATEEX_INDEX))
        return Ut_CFE_ES_ReturnCodeTable[UT_CFE_ES_POOLCREATEEX_INDEX].Value;

    return CFE_SUCCESS;
}

int32 CFE_ES_GetPoolBuf(uint32 **BufPtr, CFE_ES_MemHandle_t HandlePtr, uint32 Size)
{
    /* Check for specified return */
    if (Ut_CFE_ES_UseReturnCode(UT_CFE_ES_GETPOOLBUF_INDEX))
        return Ut_CFE_ES_ReturnCodeTable[UT_CFE_ES_GETPOOLBUF_INDEX].Value;

    /* Check for Function Hook */
    if (Ut_CFE_ES_HookTable.CFE_ES_GetPoolBuf)
        return Ut_CFE_ES_HookTable.CFE_ES_GetPoolBuf(BufPtr,HandlePtr,Size);

    return CFE_SUCCESS;
}

int32 CFE_ES_GetPoolBufInfo(CFE_ES_MemHandle_t HandlePtr, uint32 *BufPtr)
{
    return CFE_SUCCESS;
}

int32 CFE_ES_PutPoolBuf(CFE_ES_MemHandle_t HandlePtr, uint32 *BufPtr)
{
    /* Check for specified return */
    if (Ut_CFE_ES_UseReturnCode(UT_CFE_ES_PUTPOOLBUF_INDEX))
        return Ut_CFE_ES_ReturnCodeTable[UT_CFE_ES_PUTPOOLBUF_INDEX].Value;

    return CFE_SUCCESS;
}

int32 CFE_ES_GetMemPoolStats(CFE_ES_MemPoolStats_t *BufPtr, CFE_ES_MemHandle_t  Handle)
{
    return CFE_SUCCESS;
}

void CFE_ES_PerfLogAdd(uint32 Marker, uint32 EntryExit)
{
    return;
}
