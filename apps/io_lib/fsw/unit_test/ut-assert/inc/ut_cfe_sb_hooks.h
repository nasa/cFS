/*
**
** File: ut_cfe_sb_hooks.h
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_sb_hooks.h 1.1 2011/05/04 11:20:20EDT rmcgraw Exp  $
**
** Purpose: Unit test header file for cFE Software Bus hooks.
**
** $Log: ut_cfe_sb_hooks.h  $
** Revision 1.1 2011/05/04 11:20:20EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.1 2011/04/08 16:25:54EDT rmcgraw 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/inc/project.pj
** Revision 1.2 2011/03/10 11:16:45EST sslegel 
** Added EventNotSent and PacketNotSent asserts
** Revision 1.1 2011/02/15 11:12:33EST sslegel 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/inc/project.pj
**
*/

#ifndef UT_CFE_SB_HOOKS_H_
#define UT_CFE_SB_HOOKS_H_

#include "cfe.h"
#include "utassert.h"
#include "utlist.h"

extern UtListHead_t        MsgQueue;

#define     UtAssert_PacketSent(MessageID, Description) \
                UtAssert(Ut_CFE_SB_PacketSent(MessageID), Description, __FILE__, __LINE__)

#define     UtAssert_PacketNotSent(MessageID, Description) \
                UtAssert(Ut_CFE_SB_PacketSent(MessageID) == FALSE, Description, __FILE__, __LINE__)

#define     UtAssert_NoPacketSent(Description) \
                UtAssert(UtList_IsEmpty(&MsgQueue), Description, __FILE__, __LINE__)

void                Ut_CFE_SB_ClearMsgQueue(void);
uint32              Ut_CFE_SB_GetMsgQueueDepth(void);
uint32              Ut_CFE_SB_GetMsgCount(uint16 MessageID);
int32               Ut_CFE_SB_SendMsgHook(CFE_SB_Msg_t *MsgPtr);
boolean             Ut_CFE_SB_PacketSent(uint16 MessageID);
void               *Ut_CFE_SB_FindPacket(uint16 MessageID, uint32 MessageNumber);

void                Ut_CFE_SB_ClearPipes(void);
int32               Ut_CFE_SB_CreatePipe(char *PipeName);
int32               Ut_CFE_SB_GetPipeDepth(CFE_SB_PipeId_t PipeId);
int32               Ut_CFE_SB_FindPipe(char *PipeName);
void                Ut_CFE_SB_AddMsgToPipe(void *MsgPtr, CFE_SB_PipeId_t PipeId);
int32               Ut_CFE_SB_CreatePipeHook(CFE_SB_PipeId_t *PipeIdPtr, uint16  Depth, char *PipeName);
int32               Ut_CFE_SB_RcvMsgHook(CFE_SB_MsgPtr_t *BufPtr, CFE_SB_PipeId_t PipeId, int32 TimeOut);

void                Ut_CFE_SB_InitMsgHook(void *MsgPtr,CFE_SB_MsgId_t MsgId, uint16 Length, boolean Clear);
uint16              Ut_CFE_SB_MsgHdrSizeHook(CFE_SB_MsgId_t MsgId);
void               *Ut_CFE_SB_GetUserDataHook(CFE_SB_MsgPtr_t MsgPtr);
CFE_SB_MsgId_t      Ut_CFE_SB_GetMsgIdHook(CFE_SB_MsgPtr_t MsgPtr);
void                Ut_CFE_SB_SetMsgIdHook(CFE_SB_MsgPtr_t MsgPtr,CFE_SB_MsgId_t MsgId);
uint16              Ut_CFE_SB_GetUserDataLengthHook(CFE_SB_MsgPtr_t MsgPtr);
void                Ut_CFE_SB_SetUserDataLengthHook(CFE_SB_MsgPtr_t MsgPtr,uint16 DataLength);
uint16              Ut_CFE_SB_GetTotalMsgLengthHook(CFE_SB_MsgPtr_t MsgPtr);
void                Ut_CFE_SB_SetTotalMsgLengthHook(CFE_SB_MsgPtr_t MsgPtr,uint16 TotalLength);
CFE_TIME_SysTime_t  Ut_CFE_SB_GetMsgTimeHook(CFE_SB_MsgPtr_t MsgPtr);
int32               Ut_CFE_SB_SetMsgTimeHook(CFE_SB_MsgPtr_t MsgPtr,CFE_TIME_SysTime_t Time);
void                Ut_CFE_SB_TimeStampMsgHook(CFE_SB_MsgPtr_t MsgPtr);
uint16              Ut_CFE_SB_GetCmdCodeHook(CFE_SB_MsgPtr_t MsgPtr);
int32               Ut_CFE_SB_SetCmdCodeHook(CFE_SB_MsgPtr_t MsgPtr,uint16 CmdCode);
uint16              Ut_CFE_SB_GetChecksumHook(CFE_SB_MsgPtr_t MsgPtr);
void                Ut_CFE_SB_GenerateChecksumHook(CFE_SB_MsgPtr_t MsgPtr);
boolean             Ut_CFE_SB_ValidateChecksumHook(CFE_SB_MsgPtr_t MsgPtr);

void                CCSDS_LoadCheckSum (CCSDS_CmdPkt_t *PktPtr);
void                CCSDS_InitPkt (CCSDS_PriHdr_t *PktPtr, uint16 StreamId, uint16 Length, boolean Clear);
boolean             CCSDS_ValidCheckSum (CCSDS_CmdPkt_t *PktPtr);
uint8               CCSDS_ComputeCheckSum (CCSDS_CmdPkt_t *PktPtr);

#endif
