/*
**
** File: ut_cfe_sb_hooks.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** $Id: ut_cfe_sb_hooks.c 1.1 2011/05/04 11:20:55EDT rmcgraw Exp  $
**
** Purpose: Unit test hooks for cFE Software Bus routines
**
** $Log: ut_cfe_sb_hooks.c  $
** Revision 1.1 2011/05/04 11:20:55EDT rmcgraw
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.1 2011/04/08 16:26:40EDT rmcgraw
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/cf/fsw/unit_test/ut-assert/src/project.pj
** Revision 1.3 2011/03/08 15:49:26EST sslegel
** Changed the SB Create Pipe Hook so that if the pipe already exists then it just returns the existing pipe id.
** Revision 1.2 2011/03/04 14:56:54EST sslegel
** Added a define for the maximum number of pipes
** Revision 1.1 2011/02/15 11:13:03EST sslegel
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/FSW-TOOLS-REPOSITORY/ut-assert/src/project.pj
**
*/

#include "cfe.h"
#include "utlist.h"
#include "uttools.h"
#include <string.h>

#define UT_CFE_SB_MAX_PIPES    32

typedef struct {
    char                PipeName[OS_MAX_API_NAME];
    UtListHead_t        MsgQueue;
    boolean             InUse;
} Ut_CFE_SB_PipeTableEntry_t;

UtListHead_t                MsgQueue;
Ut_CFE_SB_PipeTableEntry_t  PipeTable[UT_CFE_SB_MAX_PIPES];

void Ut_CFE_SB_ClearMsgQueue(void)
{
    UtList_Reset(&MsgQueue);
}

uint32 Ut_CFE_SB_GetMsgQueueDepth(void)
{
    return(UtList_Depth(&MsgQueue));
}

uint32 Ut_CFE_SB_GetMsgCount(uint16 MessageID)
{
    UtListNode_t        *CurrentNode;
    CFE_SB_Msg_t        *MessagePtr;
    uint32               MessageCount = 0;

    CurrentNode = UtList_First(&MsgQueue);
    while (CurrentNode) {
        MessagePtr = CurrentNode->Data;
        if (MessageID == CFE_SB_GetMsgId(MessagePtr)) {
            MessageCount++;
        }
        CurrentNode = CurrentNode->Next;
    }
    return(MessageCount);
}

int32 Ut_CFE_SB_SendMsgHook(CFE_SB_Msg_t *MsgPtr)
{
    UtList_Add(&MsgQueue, MsgPtr, CFE_SB_GetTotalMsgLength(MsgPtr), 0);

    UtPrintf("PKT: ");
    UtPrintx(MsgPtr, (uint16)(CFE_SB_GetTotalMsgLength(MsgPtr)));
    return CFE_SUCCESS;
}

boolean Ut_CFE_SB_PacketSent(uint16 MessageID)
{
    UtListNode_t        *CurrentNode;
    CFE_SB_Msg_t        *MessagePtr;

    CurrentNode = UtList_First(&MsgQueue);
    while (CurrentNode) {
        MessagePtr = CurrentNode->Data;
        if (MessageID == CFE_SB_GetMsgId(MessagePtr)) {
            return(TRUE);
        }
        CurrentNode = CurrentNode->Next;
    }
    return(FALSE);
}

void *Ut_CFE_SB_FindPacket(uint16 MessageID, uint32 MessageNumber)
{
    UtListNode_t        *CurrentNode;
    CFE_SB_Msg_t        *MessagePtr;
    uint32               MessageCount = 0;

    CurrentNode = UtList_First(&MsgQueue);
    while (CurrentNode) {
        MessagePtr = CurrentNode->Data;
        if (MessageID == CFE_SB_GetMsgId(MessagePtr)) {
            MessageCount++;
            if (MessageCount == MessageNumber) {
                return(CurrentNode->Data);
            }
        }
        CurrentNode = CurrentNode->Next;
    }
    return(NULL);
}

void Ut_CFE_SB_ClearPipes(void)
{
    uint32          i;

    for (i=0; i < UT_CFE_SB_MAX_PIPES; i++) {
        if (PipeTable[i].InUse == TRUE) {
            UtList_Reset(&PipeTable[i].MsgQueue);
        }
    }
    memset(&PipeTable, 0, sizeof(PipeTable));
}

int32 Ut_CFE_SB_CreatePipe(char *PipeName)
{
    uint32          i;

    for (i=0; i < UT_CFE_SB_MAX_PIPES; i++) {
        if (PipeTable[i].InUse == FALSE) {
            strncpy(PipeTable[i].PipeName, PipeName, OS_MAX_API_NAME);
            PipeTable[i].InUse = TRUE;
            return(i);
        }
    }
    return(-1);
}

int32 Ut_CFE_SB_GetPipeDepth(CFE_SB_PipeId_t PipeId)
{
    return(UtList_Depth(&PipeTable[PipeId].MsgQueue));
}

int32 Ut_CFE_SB_FindPipe(char *PipeName)
{
    uint32          i;

    for (i=0; i < UT_CFE_SB_MAX_PIPES; i++) {
        if ((PipeTable[i].InUse == TRUE) &&
            (strncmp(PipeTable[i].PipeName, PipeName, strlen(PipeName)) == 0)) {
            return(i);
        }
    }
    return(-1);
}

void Ut_CFE_SB_AddMsgToPipe(void *MsgPtr, CFE_SB_PipeId_t PipeId)
{
    if (PipeTable[PipeId].InUse == TRUE) {
        UtList_Add(&PipeTable[PipeId].MsgQueue, MsgPtr, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)MsgPtr), 0);
    }
    else {
        printf("Error - Invalid PipeId\n");
    }
}

int32 Ut_CFE_SB_CreatePipeHook(CFE_SB_PipeId_t *PipeIdPtr, uint16  Depth, char *PipeName)
{
    if (Ut_CFE_SB_FindPipe(PipeName) == -1) {
        *PipeIdPtr = Ut_CFE_SB_CreatePipe(PipeName);
    }
    else {
        *PipeIdPtr = Ut_CFE_SB_FindPipe(PipeName);
    }
    return(CFE_SUCCESS);
}

int32 Ut_CFE_SB_RcvMsgHook(CFE_SB_MsgPtr_t *BufPtr, CFE_SB_PipeId_t PipeId, int32 TimeOut)
{
    UtListNode_t        *CurrentNode;

    if (PipeTable[PipeId].InUse == TRUE) {

        if (UtList_IsEmpty(&PipeTable[PipeId].MsgQueue) == FALSE) {

            CurrentNode = UtList_First(&PipeTable[PipeId].MsgQueue);
            if (CurrentNode->Tag == TRUE) {         /* Indicates buffer is in use */
                UtList_DeleteFirst(&PipeTable[PipeId].MsgQueue);
            }

            if (UtList_IsEmpty(&PipeTable[PipeId].MsgQueue) == FALSE) {

                CurrentNode = UtList_First(&PipeTable[PipeId].MsgQueue);
                CurrentNode->Tag = TRUE;            /* Indicates buffer is in use */
               *BufPtr = CurrentNode->Data;
                return(CFE_SUCCESS);
            }
        }

        if (TimeOut == CFE_SB_POLL) {
            return(CFE_SB_NO_MESSAGE);
        }
        else {
            return(CFE_SB_TIME_OUT);
        }
    }
    else {
        printf("Error - Invalid PipeId\n");
        return(CFE_SB_NO_MESSAGE);
    }
}

void Ut_CFE_SB_InitMsgHook(void *MsgPtr, CFE_SB_MsgId_t MsgId, uint16 Length, boolean Clear)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    CCSDS_InitPkt ((CCSDS_PriHdr_t *)MsgPtr,(uint16)MsgId,Length,Clear);

#endif
} /* end Ut_CFE_SB_InitMsgHook */

uint16 Ut_CFE_SB_MsgHdrSizeHook(CFE_SB_MsgId_t MsgId)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    uint16 size;
    CCSDS_PriHdr_t  CCSDSPriHdr;
    CCSDS_WR_SID(CCSDSPriHdr,MsgId);

	  /* if secondary hdr is not present... */
	  if(CCSDS_RD_SHDR(CCSDSPriHdr) == 0){

	    size = sizeof(CCSDS_PriHdr_t);

	  }else if(CCSDS_RD_TYPE(CCSDSPriHdr) == CCSDS_CMD){

        size = CFE_SB_CMD_HDR_SIZE;

	  }else{

        size = CFE_SB_TLM_HDR_SIZE;
	  }

    return size;

#endif
}/* end Ut_CFE_SB_MsgHdrSizeHook */

void *Ut_CFE_SB_GetUserDataHook(CFE_SB_MsgPtr_t MsgPtr)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS
    uint8           *BytePtr;
    CFE_SB_MsgId_t  MsgId;
    uint16          HdrSize;

    BytePtr = (uint8 *)MsgPtr;
    MsgId   = CFE_SB_GetMsgId(MsgPtr);
    HdrSize = CFE_SB_MsgHdrSize(MsgId);

    return (BytePtr + HdrSize);
#endif
}/* end Ut_CFE_SB_GetUserDataHook */

CFE_SB_MsgId_t Ut_CFE_SB_GetMsgIdHook(CFE_SB_MsgPtr_t MsgPtr)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    return CCSDS_RD_SID(MsgPtr->Hdr);

#endif
}/* end Ut_CFE_SB_GetMsgIdHook */

void Ut_CFE_SB_SetMsgIdHook(CFE_SB_MsgPtr_t MsgPtr,
                     CFE_SB_MsgId_t MsgId)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    CCSDS_WR_SID(MsgPtr->Hdr,MsgId);

#endif
}/* end Ut_CFE_SB_SetMsgIdHook */

uint16 Ut_CFE_SB_GetUserDataLengthHook(CFE_SB_MsgPtr_t MsgPtr)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS
    uint16 TotalMsgSize;
    uint16 HdrSize;

    CFE_SB_MsgId_t MsgId;
    MsgId = CFE_SB_GetMsgId(MsgPtr);

    TotalMsgSize = CFE_SB_GetTotalMsgLength(MsgPtr);
    HdrSize = CFE_SB_MsgHdrSize(MsgId);

    return (TotalMsgSize - HdrSize);
#endif
}/* end Ut_CFE_SB_GetUserDataLengthHook */

void Ut_CFE_SB_SetUserDataLengthHook(CFE_SB_MsgPtr_t MsgPtr,uint16 DataLength)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    uint32 TotalMsgSize, HdrSize;
    CFE_SB_MsgId_t MsgId;
    MsgId = CFE_SB_GetMsgId(MsgPtr);

    TotalMsgSize = CFE_SB_GetTotalMsgLength(MsgPtr);
    HdrSize = CFE_SB_MsgHdrSize(MsgId);

    TotalMsgSize = HdrSize + DataLength;

    CCSDS_WR_LEN(MsgPtr->Hdr,TotalMsgSize);

#endif
}/* end Ut_CFE_SB_SetUserDataLengthHook */

uint16 Ut_CFE_SB_GetTotalMsgLengthHook(CFE_SB_MsgPtr_t MsgPtr)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    return CCSDS_RD_LEN(MsgPtr->Hdr);

#endif
}/* end Ut_CFE_SB_GetTotalMsgLengthHook */

void Ut_CFE_SB_SetTotalMsgLengthHook(CFE_SB_MsgPtr_t MsgPtr,uint16 TotalLength)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    CCSDS_WR_LEN(MsgPtr->Hdr,TotalLength);

#endif
}/* end Ut_CFE_SB_SetTotalMsgLengthHook */

CFE_TIME_SysTime_t Ut_CFE_SB_GetMsgTimeHook(CFE_SB_MsgPtr_t MsgPtr)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    CFE_TIME_SysTime_t  TimeFromMsg;
    CFE_SB_TlmHdr_t     *TlmHdrPtr;

    /* if msg type is a command or msg has no secondary hdr, return 0 */
    if((CCSDS_RD_TYPE(MsgPtr->Hdr) == CCSDS_CMD)||(CCSDS_RD_SHDR(MsgPtr->Hdr) == 0)){
        TimeFromMsg.Seconds    =  0;
        TimeFromMsg.Subseconds =  0;
    }else{
        TlmHdrPtr = (CFE_SB_TlmHdr_t *)MsgPtr;

        TimeFromMsg.Seconds    =  *((uint32 *)&TlmHdrPtr->Sec.Time[0]);

        /* Get the 16 bit subsecond field from the header and place it in the    */
        /* upper 16 bits of the 32 bit subsecond field of the CFE_TIME_SysTime_t */
        TimeFromMsg.Subseconds =  *((uint16 *)&TlmHdrPtr->Sec.Time[4]) << 16;

    }/* end if */

    return TimeFromMsg;

#endif
}/* end Ut_CFE_SB_GetMsgTimeHook */

int32 Ut_CFE_SB_SetMsgTimeHook(CFE_SB_MsgPtr_t MsgPtr,
                       CFE_TIME_SysTime_t Time)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    CFE_SB_TlmHdr_t *TlmHdrPtr;

   /* if msg type is a command or secondary header is not present... */
    if((CCSDS_RD_TYPE(MsgPtr->Hdr) == CCSDS_CMD)||(CCSDS_RD_SHDR(MsgPtr->Hdr) == 0)){
        return CFE_SB_WRONG_MSG_TYPE;
    }/* end if */

    TlmHdrPtr = (CFE_SB_TlmHdr_t *)MsgPtr;

    *((uint32 *)&TlmHdrPtr->Sec.Time[0]) = Time.Seconds;
    *((uint16 *)&TlmHdrPtr->Sec.Time[4]) = Time.Subseconds >> 16;

    return CFE_SUCCESS;

#endif
}/* end Ut_CFE_SB_SetMsgTimeHook */

//FIXME - not sure what to do about this yet, want to avoid any dependencies on other api functions if possible.
//void Ut_CFE_SB_TimeStampMsgHook(CFE_SB_MsgPtr_t MsgPtr)
//{
//    CFE_SB_SetMsgTime(MsgPtr,CFE_TIME_GetTime());
//
//}/* end Ut_CFE_SB_TimeStampMsgHook */

uint16 Ut_CFE_SB_GetCmdCodeHook(CFE_SB_MsgPtr_t MsgPtr)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    CFE_SB_CmdHdr_t     *CmdHdrPtr;

    /* if msg type is telemetry or there is no secondary hdr, return 0 */
    if((CCSDS_RD_TYPE(MsgPtr->Hdr) == CCSDS_TLM)||(CCSDS_RD_SHDR(MsgPtr->Hdr) == 0)){
        return 0;
    }/* end if */

    /* Cast the input pointer to a Cmd Msg pointer */
    CmdHdrPtr = (CFE_SB_CmdHdr_t *)MsgPtr;

    return CCSDS_RD_FC(CmdHdrPtr->Sec);

#endif
}/* end Ut_CFE_SB_GetCmdCodeHook */

int32 Ut_CFE_SB_SetCmdCodeHook(CFE_SB_MsgPtr_t MsgPtr,
                      uint16 CmdCode)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    CFE_SB_CmdHdr_t     *CmdHdrPtr;

    /* if msg type is telemetry or there is no secondary hdr... */
    if((CCSDS_RD_TYPE(MsgPtr->Hdr) == CCSDS_TLM)||(CCSDS_RD_SHDR(MsgPtr->Hdr) == 0)){
        return CFE_SB_WRONG_MSG_TYPE;
    }/* end if */

    /* Cast the input pointer to a Cmd Msg pointer */
    CmdHdrPtr = (CFE_SB_CmdHdr_t *)MsgPtr;

    CCSDS_WR_FC(CmdHdrPtr->Sec,CmdCode);

    return CFE_SUCCESS;

#endif

}/* end Ut_CFE_SB_SetCmdCodeHook */

uint16 Ut_CFE_SB_GetChecksumHook(CFE_SB_MsgPtr_t MsgPtr)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    CFE_SB_CmdHdr_t     *CmdHdrPtr;

    /* if msg type is telemetry or there is no secondary hdr... */
    if((CCSDS_RD_TYPE(MsgPtr->Hdr) == CCSDS_TLM)||(CCSDS_RD_SHDR(MsgPtr->Hdr) == 0)){
        return 0;
    }/* end if */

    /* cast the input pointer to a Cmd Msg pointer */
    CmdHdrPtr = (CFE_SB_CmdHdr_t *)MsgPtr;

    return CCSDS_RD_CHECKSUM(CmdHdrPtr->Sec);

#endif
}/* end Ut_CFE_SB_GetChecksumHook */

void Ut_CFE_SB_GenerateChecksumHook(CFE_SB_MsgPtr_t MsgPtr)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    CCSDS_CmdPkt_t    *CmdPktPtr;

    /* if msg type is telemetry or there is no secondary hdr... */
    if((CCSDS_RD_TYPE(MsgPtr->Hdr) == CCSDS_TLM)||(CCSDS_RD_SHDR(MsgPtr->Hdr) == 0)){
        return;
    }/* end if */

    CmdPktPtr = (CCSDS_CmdPkt_t *)MsgPtr;

    CCSDS_LoadCheckSum(CmdPktPtr);

#endif
}/* end Ut_CFE_SB_GenerateChecksumHook */

boolean Ut_CFE_SB_ValidateChecksumHook(CFE_SB_MsgPtr_t MsgPtr)
{
#ifdef MESSAGE_FORMAT_IS_CCSDS

    CCSDS_CmdPkt_t    *CmdPktPtr;

    /* if msg type is telemetry or there is no secondary hdr... */
    if((CCSDS_RD_TYPE(MsgPtr->Hdr) == CCSDS_TLM)||(CCSDS_RD_SHDR(MsgPtr->Hdr) == 0)){
        return FALSE;
    }/* end if */

    CmdPktPtr = (CCSDS_CmdPkt_t *)MsgPtr;

    return CCSDS_ValidCheckSum (CmdPktPtr);

#endif
}/* end Ut_CFE_SB_ValidateChecksumHook */

void CCSDS_InitPkt (CCSDS_PriHdr_t  *PktPtr,
                    uint16           StreamId,
                    uint16           Length,
                    boolean          Clear )
{
   uint16     SeqCount;

   /* Save the sequence count in case it must be preserved. */
   SeqCount = CCSDS_RD_SEQ(*PktPtr);

   /* Zero the entire packet if needed. */
   if (Clear)  memset((void *)PktPtr, 0, Length);

   /* Clear the primary header. */
   CCSDS_CLR_PRI_HDR(*PktPtr);

   /* Set the stream ID and length fields in the primary header. */
   CCSDS_WR_SID(*PktPtr, StreamId);
   CCSDS_WR_LEN(*PktPtr, Length);

   /* Restore the sequence count if needed. */
   if (!Clear)  CCSDS_WR_SEQ(*PktPtr, SeqCount);

} /* END CCSDS_InitPkt() */

void CCSDS_LoadCheckSum (CCSDS_CmdPkt_t *PktPtr)
{
   uint8    CheckSum;

   /* Clear the checksum field so the new checksum is correct. */
   CCSDS_WR_CHECKSUM(PktPtr->SecHdr, 0);

   /* Compute and load new checksum. */
   CheckSum = CCSDS_ComputeCheckSum(PktPtr);
   CCSDS_WR_CHECKSUM(PktPtr->SecHdr, CheckSum);

} /* END CCSDS_LoadCheckSum() */

boolean CCSDS_ValidCheckSum (CCSDS_CmdPkt_t *PktPtr)
{

   return (CCSDS_ComputeCheckSum(PktPtr) == 0);

} /* END CCSDS_ValidCheckSum() */

uint8 CCSDS_ComputeCheckSum (CCSDS_CmdPkt_t *PktPtr)
{
   uint16   PktLen   = CCSDS_RD_LEN(PktPtr->PriHdr);
   uint8   *BytePtr  = (uint8 *)PktPtr;
   uint8    CheckSum;

   CheckSum = 0xFF;
   while (PktLen--)  CheckSum ^= *(BytePtr++);

   return CheckSum;

} /* END CCSDS_ComputeCheckSum() */
