/*
**
** File: ut_cfe_es_stubs.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_es_stubs.h 1.2 2011/05/04 11:28:00EDT rmcgraw Exp  $
**
** Purpose: cFE Executive Services Header file for unit test stubs
**
** $Log: ut_cfe_es_stubs.h  $
** Revision 1.2 2011/05/04 11:28:00EDT rmcgraw 
** Changed PoolCreateEx to have new parameter USE_MUTEX
** Revision 1.1 2011/05/04 11:20:18EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.1 2011/04/08 16:25:51EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.1 2011/02/15 11:12:32EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/inc/project.pj
**
*/

#ifndef UT_CFE_ES_STUBS_H_
#define UT_CFE_ES_STUBS_H_

typedef enum 
{
    UT_CFE_ES_RESETCFE_INDEX,
    UT_CFE_ES_RESTARTAPP_INDEX,
    UT_CFE_ES_RELOADAPP_INDEX,
    UT_CFE_ES_DELETEAPP_INDEX,
    UT_CFE_ES_EXITAPP_INDEX,
    UT_CFE_ES_RUNLOOP_INDEX,
    UT_CFE_ES_WAITFORSTARTUPSYNC_INDEX,
    UT_CFE_ES_REGISTERAPP_INDEX,
    UT_CFE_ES_GETAPPID_INDEX,
    UT_CFE_ES_GETAPPIDBYNAME_INDEX,
    UT_CFE_ES_GETAPPNAME_INDEX,
    UT_CFE_ES_GETAPPINFO_INDEX,
    UT_CFE_ES_GETTASKINFO_INDEX,
    UT_CFE_ES_REGISTERCHILDTASK_INDEX,
    UT_CFE_ES_CREATECHILDTASK_INDEX,
    UT_CFE_ES_DELETECHILDTASK_INDEX,
    UT_CFE_ES_EXITCHILDTASK_INDEX,
    UT_CFE_ES_INCREMENTTASKCOUNTER_INDEX,
    UT_CFE_ES_WRITETOSYSLOG_INDEX,
    UT_CFE_ES_REGISTERDRIVER_INDEX,
    UT_CFE_ES_UNLOADDRIVER_INDEX,
    UT_CFE_ES_CALCULATECRC_INDEX,
    UT_CFE_ES_REGISTERCDS_INDEX,
    UT_CFE_ES_COPYTOCDS_INDEX,
    UT_CFE_ES_RESTOREFROMCDS_INDEX,
    UT_CFE_ES_POOLCREATE_INDEX,
    UT_CFE_ES_POOLCREATEEX_INDEX,
    UT_CFE_ES_GETPOOLBUF_INDEX,
    UT_CFE_ES_GETPOOLBUFINFO_INDEX,
    UT_CFE_ES_PUTPOOLBUF_INDEX,
    UT_CFE_ES_GETMEMPOOLSTATS_INDEX,
    UT_CFE_ES_PERFLOGADD_INDEX,
    UT_CFE_ES_MAX_INDEX
} Ut_CFE_ES_INDEX_t;

typedef struct
{
    int32 (*CFE_ES_ResetCFE)(uint32 ResetType);
    int32 (*CFE_ES_RestartApp)(uint32 AppID);
    int32 (*CFE_ES_ReloadApp)(uint32 AppID_API, const char *AppFileName);
    int32 (*CFE_ES_DeleteApp)(uint32 AppID);
    int32 (*CFE_ES_ExitApp)(uint32 ExitStatus);
    int32 (*CFE_ES_RunLoop)(uint32 *ExitStatus);
    int32 (*CFE_ES_WaitForStartupSync)(uint32 TimeOutMilliseconds);
    int32 (*CFE_ES_RegisterApp)(void);
    int32 (*CFE_ES_GetAppID)(uint32 *AppIdPtr);
    int32 (*CFE_ES_GetAppIDByName)(uint32 *AppIdPtr, char *AppName);
    int32 (*CFE_ES_GetAppName)(char *AppName, uint32 AppId, uint32 BufferLength);
    int32 (*CFE_ES_GetAppInfo)(CFE_ES_AppInfo_t *AppInfo, uint32 AppId);
    int32 (*CFE_ES_GetTaskInfo)(CFE_ES_TaskInfo_t *TaskInfo, uint32 TaskId);
    int32 (*CFE_ES_RegisterChildTask)(void);
    int32 (*CFE_ES_CreateChildTask)(uint32 *TaskIdPtr, const char *TaskName, CFE_ES_ChildTaskMainFuncPtr_t  FunctionPtr,const uint32 *StackPtr, uint32 StackSize, uint32 Priority, uint32 Flags);
    int32 (*CFE_ES_DeleteChildTask)(uint32 TaskId);
    int32 (*CFE_ES_ExitChildTask)(void);
    int32 (*CFE_ES_IncrementTaskCounter)(void);
    int32 (*CFE_ES_WriteToSysLog)(const char *SpecStringPtr, ...);
    int32 (*CFE_ES_RegisterDriver)(uint32 *DriverIdPtr, uint32 *DriverDescPtr);
    int32 (*CFE_ES_UnloadDriver)(uint32 DriverId);
    int32 (*CFE_ES_CalculateCRC)(void *DataPtr, uint32 DataLength, uint32 InputCRC, uint32 TypeCRC);
    int32 (*CFE_ES_RegisterCDS)(CFE_ES_CDSHandle_t *HandlePtr, int32 BlockSize, const char *Name);
    int32 (*CFE_ES_CopyToCDS)(CFE_ES_CDSHandle_t Handle, void *DataToCopy);
    int32 (*CFE_ES_RestoreFromCDS)(void *RestoreToMemory, CFE_ES_CDSHandle_t Handle);
    int32 (*CFE_ES_PoolCreate)(uint32 *HandlePtr, uint8 *MemPtr, uint32 Size);
    int32 (*CFE_ES_PoolCreateEx)(uint32 *HandlePtr, uint8 *MemPtr, uint32 Size, uint32 NumBlockSizes, uint32 *BlockSizes, uint16 UseMutex);
    int32 (*CFE_ES_GetPoolBuf)(uint32 **BufPtr, CFE_ES_MemHandle_t HandlePtr, uint32 Size);
    int32 (*CFE_ES_GetPoolBufInfo)(CFE_ES_MemHandle_t HandlePtr, uint32 *BufPtr);
    int32 (*CFE_ES_PutPoolBuf)(CFE_ES_MemHandle_t HandlePtr, uint32 *BufPtr);
    int32 (*CFE_ES_GetMemPoolStats)(CFE_ES_MemPoolStats_t *BufPtr, CFE_ES_MemHandle_t  Handle);
    int32 (*CFE_ES_PerfLogAdd)(uint32 Marker, uint32 EntryExit);    
} Ut_CFE_ES_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} Ut_CFE_ES_ReturnCodeTable_t;

void Ut_CFE_ES_Reset(void);
void Ut_CFE_ES_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_CFE_ES_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);

#endif
