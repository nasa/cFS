/*
**
** File: ut_cfe_sb_stubs.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_sb_stubs.h 1.1 2011/05/04 11:20:21EDT rmcgraw Exp  $
**
** Purpose: cFE Software Bus Header file for unit test stubs
**
** $Log: ut_cfe_sb_stubs.h  $
** Revision 1.1 2011/05/04 11:20:21EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.1 2011/04/08 16:25:55EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.1 2011/02/15 11:12:34EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/inc/project.pj
**
*/

#ifndef UT_CFE_SB_STUBS_H_
#define UT_CFE_SB_STUBS_H_

typedef enum 
{
    UT_CFE_SB_CREATEPIPE_INDEX,
    UT_CFE_SB_DELETEPIPE_INDEX,
    UT_CFE_SB_SUBSCRIBEEX_INDEX,
    UT_CFE_SB_SUBSCRIBE_INDEX,
    UT_CFE_SB_SUBSCRIBELOCAL_INDEX,
    UT_CFE_SB_UNSUBSCRIBE_INDEX,
    UT_CFE_SB_UNSUBSCRIBELOCAL_INDEX,
    UT_CFE_SB_SENDMSG_INDEX,
    UT_CFE_SB_PASSMSG_INDEX,
    UT_CFE_SB_RCVMSG_INDEX,
    UT_CFE_SB_GETLASTSENDERID_INDEX,
    UT_CFE_SB_ZEROCOPYGETPTR_INDEX,
    UT_CFE_SB_ZEROCOPYRELEASEPTR_INDEX,
    UT_CFE_SB_ZEROCOPYSEND_INDEX,
    UT_CFE_SB_ZEROCOPYPASS_INDEX,
    UT_CFE_SB_INITMSG_INDEX,
    UT_CFE_SB_MSGHDRSIZE_INDEX,
    UT_CFE_SB_GETUSERDATA_INDEX,
    UT_CFE_SB_GETMSGID_INDEX,
    UT_CFE_SB_SETMSGID_INDEX,
    UT_CFE_SB_GETUSERDATALENGTH_INDEX,
    UT_CFE_SB_SETUSERDATALENGTH_INDEX,
    UT_CFE_SB_GETTOTALMSGLENGTH_INDEX,
    UT_CFE_SB_SETTOTALMSGLENGTH_INDEX,
    UT_CFE_SB_GETMSGTIME_INDEX,
    UT_CFE_SB_SETMSGTIME_INDEX,
    UT_CFE_SB_TIMESTAMPMSG_INDEX,
    UT_CFE_SB_GETCMDCODE_INDEX,
    UT_CFE_SB_SETCMDCODE_INDEX,
    UT_CFE_SB_GETCHECKSUM_INDEX,
    UT_CFE_SB_GENERATECHECKSUM_INDEX,
    UT_CFE_SB_VALIDATECHECKSUM_INDEX,
    UT_CFE_SB_CLEANUPAPP_INDEX,
    UT_CFE_SB_MAX_INDEX
} Ut_CFE_SB_INDEX_t;

typedef struct
{
    int32 (*CFE_SB_CreatePipe)(CFE_SB_PipeId_t *PipeIdPtr,uint16  Depth, char *PipeName);
    int32 (*CFE_SB_DeletePipe)(CFE_SB_PipeId_t PipeId);
    int32 (*CFE_SB_SubscribeEx)(CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId,CFE_SB_Qos_t Quality, uint16 MsgLim);
    int32 (*CFE_SB_Subscribe)(CFE_SB_MsgId_t  MsgId, CFE_SB_PipeId_t PipeId);
    int32 (*CFE_SB_SubscribeLocal)(CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId, uint16 MsgLim);
    int32 (*CFE_SB_Unsubscribe)(CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId);
    int32 (*CFE_SB_UnsubscribeLocal)(CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId);   
    int32 (*CFE_SB_SendMsg)(CFE_SB_Msg_t   *MsgPtr);
    int32 (*CFE_SB_PassMsg)(CFE_SB_Msg_t   *MsgPtr);
    int32 (*CFE_SB_RcvMsg)(CFE_SB_MsgPtr_t  *BufPtr, CFE_SB_PipeId_t  PipeId, int32 TimeOut);    
    int32 (*CFE_SB_GetLastSenderId)(CFE_SB_SenderId_t **Ptr,CFE_SB_PipeId_t  PipeId); 
    int32 (*CFE_SB_ZeroCopyGetPtr)(uint16  MsgSize,CFE_SB_ZeroCopyHandle_t *BufferHandle); 
    int32 (*CFE_SB_ZeroCopyReleasePtr)(CFE_SB_Msg_t  *Ptr2Release,CFE_SB_ZeroCopyHandle_t BufferHandle); 
    int32 (*CFE_SB_ZeroCopySend)(CFE_SB_Msg_t   *MsgPtr,CFE_SB_ZeroCopyHandle_t BufferHandle);
    int32 (*CFE_SB_ZeroCopyPass)(CFE_SB_Msg_t   *MsgPtr,CFE_SB_ZeroCopyHandle_t BufferHandle); 
    int32 (*CFE_SB_InitMsg)(void *MsgPtr,CFE_SB_MsgId_t MsgId, uint16 Length, boolean Clear); 
    int32 (*CFE_SB_MsgHdrSize)(CFE_SB_MsgId_t MsgId);
    void *(*CFE_SB_GetUserData)(CFE_SB_MsgPtr_t MsgPtr);
    int32 (*CFE_SB_GetMsgId)(CFE_SB_MsgPtr_t MsgPtr);
    int32 (*CFE_SB_SetMsgId)(CFE_SB_MsgPtr_t MsgPtr,CFE_SB_MsgId_t MsgId);
    int32 (*CFE_SB_GetUserDataLength)(CFE_SB_MsgPtr_t MsgPtr);
    int32 (*CFE_SB_SetUserDataLength)(CFE_SB_MsgPtr_t MsgPtr,uint16 DataLength);
    int32 (*CFE_SB_GetTotalMsgLength)(CFE_SB_MsgPtr_t MsgPtr);
    int32 (*CFE_SB_SetTotalMsgLength)(CFE_SB_MsgPtr_t MsgPtr,uint16 TotalLength);
    CFE_TIME_SysTime_t (*CFE_SB_GetMsgTime)(CFE_SB_MsgPtr_t MsgPtr);
    int32 (*CFE_SB_SetMsgTime)(CFE_SB_MsgPtr_t MsgPtr,CFE_TIME_SysTime_t Time);
    int32 (*CFE_SB_TimeStampMsg)(CFE_SB_MsgPtr_t MsgPtr);
    int32 (*CFE_SB_GetCmdCode)(CFE_SB_MsgPtr_t MsgPtr);
    int32 (*CFE_SB_SetCmdCode)(CFE_SB_MsgPtr_t MsgPtr,uint16 CmdCode);
    int32 (*CFE_SB_GetChecksum)(CFE_SB_MsgPtr_t MsgPtr);
    int32 (*CFE_SB_GenerateChecksum)(CFE_SB_MsgPtr_t MsgPtr);
    int32 (*CFE_SB_ValidateChecksum)(CFE_SB_MsgPtr_t MsgPtr);
    int32 (*CFE_SB_CleanUpApp)(uint32 AppId);
} Ut_CFE_SB_HookTable_t;

typedef struct
{
    int32   Value;
    uint32  Count; 
} Ut_CFE_SB_ReturnCodeTable_t;

void Ut_CFE_SB_Reset(void);
void Ut_CFE_SB_SetFunctionHook(uint32 Index, void *FunPtr);
void Ut_CFE_SB_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);

#endif
