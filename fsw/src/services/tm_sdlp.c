/******************************************************************************/
/** \file  tm_sdlp.c
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*  
*   \brief Function Definitions for TM_SDLP
*
*   \par
*     Provides Telemetry Space Data Link Protocol (TM_SDLP) services
*
*   \par Modification History:
*     - 2015-04-26 | Alan A. Asp | OSR | Code Started (originally in tmtf.c)
*     - 2015-10-22 | Guy de Carufel | OSR | Migrated from tmtf.c. 
*           Major revision: Comments, Structs, idle data, overflow, API.
*******************************************************************************/

#include "tm_sdlp.h"

static int32 TM_SDLP_AddData(TM_SDLP_FrameInfo_t *pFrameInfo, uint8 *pData, 
                             uint16 dataLength, boolean isPacket);
static int32 TM_SDLP_CopyToOverflow(TM_SDLP_OverflowInfo_t *pOverflow, 
                                    uint8 *data, uint16 length, 
                                    boolean isPartial);
static int32 TM_SDLP_CopyFromOverflow(TM_SDLP_FrameInfo_t *pFrameInfo);



/*****************************************************************************/
/** \brief TMTF_SDLP_InitIdlePacket
******************************************************************************/
int32 TM_SDLP_InitIdlePacket(CFE_SB_Msg_t *pIdlePacket, uint8 *pIdlePattern,
                             uint16 bufferLength, uint32 patternBitLength)
{
    uint8 *pIdleData = NULL;
    uint16 idleDataLength = 0;
    uint32 bit = 0;
    uint16 byte = 0;
    uint16 bitOffset = 0;
    uint16 byteIdx = 0;
    uint16 patternLength = 0;
    int32 iStatus = TM_SDLP_SUCCESS;
    
    if (pIdlePacket == NULL || pIdlePattern == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_InitIdlePacket Error: "
                          "Input Pointer is Null.");
        
        iStatus = TM_SDLP_INVALID_POINTER;
        goto end_of_function;
    }

    if (patternBitLength == 0)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_InitIdlePacket Error: "
                          "Input patternBitLength is 0.");
        
        iStatus = TM_SDLP_INVALID_LENGTH;
        goto end_of_function;
    }

    /* Idle packet, as specified in CCSDS 133.0-B-1 */
    CFE_SB_InitMsg((void *) pIdlePacket, 0x3ffU, bufferLength, FALSE);
    idleDataLength = CFE_SB_GetUserDataLength(pIdlePacket);
    pIdleData = CFE_SB_GetUserData(pIdlePacket);

    patternLength = patternBitLength / 8;
    if (patternBitLength % 8 != 0) 
    {
        patternLength++;
    }

    /* Build the idle data from a provided pattern */
    for (byte = 0; byte < idleDataLength; ++byte)
    {
        bit = (byte * 8) % patternBitLength;
        byteIdx = bit / 8;
        bitOffset = bit % 8;

        pIdleData[byte] = (pIdlePattern[byteIdx] << bitOffset) |
                          (pIdlePattern[(byteIdx + 1) % patternLength] >> 
                           (8-bitOffset));
    }

end_of_function:
    return iStatus;
}


/*****************************************************************************/
/** \brief TMTF_SDLP_InitChannel
******************************************************************************/
int32 TM_SDLP_InitChannel(TM_SDLP_FrameInfo_t *pFrameInfo, 
                          uint8 *pTfBuffer, uint8 *pOverflowBuffer,
                          TM_SDLP_GlobalConfig_t *pGlobalConfig, 
                          TM_SDLP_ChannelConfig_t *pChannelConfig)
{
    int32 iStatus = TM_SDLP_SUCCESS;
    int32 dataFieldLength;
    uint16 dataFieldOffset;
    uint16 secHdrLength;
    uint16 gvcid = 0;
    char mutName[OS_MAX_API_NAME];
    
    if (pGlobalConfig == NULL || pChannelConfig == NULL || pFrameInfo == NULL ||
        pOverflowBuffer == NULL || pTfBuffer == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_InitChannel Error: "
                          "Input Pointer is Null.");
        
        iStatus = TM_SDLP_INVALID_POINTER;
        goto end_of_function;
    }
    
    secHdrLength = pChannelConfig->secHdrLength;

    /* The secHdr Length must be between 1-63 bytes if present. 
     * (TM_SDLP 4.1.3.1.3) */
    if ((pChannelConfig->fshFlag == TRUE && 
         (secHdrLength > TMTF_SECHDR_MAX_LENGTH || secHdrLength < 1)) ||
        (pChannelConfig->fshFlag == FALSE && secHdrLength != 0))        
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_InitChannel Error: "
                          "Invalid SecHdrLength:%d", secHdrLength);
        
        iStatus = TM_SDLP_INVALID_LENGTH;
        goto end_of_function;
    }
    
    dataFieldLength = (int32) pGlobalConfig->frameLength;
    dataFieldOffset = TMTF_PRIHDR_LENGTH;

    if (secHdrLength > 0)
    {
        dataFieldOffset += secHdrLength + 1;
    }

    dataFieldLength -= dataFieldOffset;

    if (pChannelConfig->ocfFlag == TRUE)
    {
        dataFieldLength -= TMTF_OCF_LENGTH;
    }

    if (pGlobalConfig->hasErrCtrl == TRUE)
    {
        dataFieldLength -= TMTF_ERR_CTRL_FIELD_LENGTH;
    }

    if (dataFieldLength < 0)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_InitChannel Error: "
                          "Invalid Length Configuration.");
        
        iStatus = TM_SDLP_INVALID_LENGTH;
        goto end_of_function;
    }

    /* Update the Transfer Frame Info */
    pFrameInfo->dataFieldLength     = (uint16) dataFieldLength;
    pFrameInfo->dataFieldOffset     = dataFieldOffset;
    pFrameInfo->ocfOffset           = dataFieldOffset + (uint16) dataFieldLength;
    pFrameInfo->freeOctets          = pFrameInfo->dataFieldLength;
    pFrameInfo->currentDataOffset   = pFrameInfo->dataFieldOffset;
    pFrameInfo->globConfig          = pGlobalConfig;
    pFrameInfo->chnlConfig          = pChannelConfig;
    pFrameInfo->frame               = (TMTF_PriHdr_t *) pTfBuffer; 
    pFrameInfo->isFirstHdrPtrSet    = FALSE;
    pFrameInfo->isReady             = FALSE;

    if (pChannelConfig->ocfFlag == TRUE)
    {
        pFrameInfo->errCtrlOffset   = pFrameInfo->ocfOffset + TMTF_OCF_LENGTH;
    }
    else
    {
        pFrameInfo->errCtrlOffset   = pFrameInfo->ocfOffset;
    }
    
    /* Set the overflowInfo */
    pFrameInfo->overflowInfo.buffSize      = pChannelConfig->overflowSize;
    pFrameInfo->overflowInfo.freeOctets    = pChannelConfig->overflowSize;
    pFrameInfo->overflowInfo.partialOctets = 0;
    pFrameInfo->overflowInfo.dataStart     = pOverflowBuffer;
    pFrameInfo->overflowInfo.dataEnd       = pOverflowBuffer;
    pFrameInfo->overflowInfo.buffer        = pOverflowBuffer;

    /* Initialize the TF buffer */
    CFE_PSP_MemSet((void *)pTfBuffer, 0, pGlobalConfig->frameLength);
    TMTF_SetScId(pFrameInfo->frame, pGlobalConfig->scId);
    TMTF_SetVcId(pFrameInfo->frame, pChannelConfig->vcId);
    TMTF_SetOcfFlag(pFrameInfo->frame, pChannelConfig->ocfFlag);

    /* Initialize the Overflow buffer */
    CFE_PSP_MemSet((void *)pOverflowBuffer, 0, pChannelConfig->overflowSize);
    
    /* Set secondary header flag and sec hdr length */
    if (pChannelConfig->fshFlag == TRUE)
    {
        TMTF_SetSecHdrFlag(pFrameInfo->frame, 1);
        TMTF_SetSecHdrLength(pFrameInfo->frame, secHdrLength);
    }

    /* If we are using the VCP service (CCSDS packets) [TM_SDLP 4.1.2.7] 
     * - Sync Flag set to 0
     * - Packet Order flag set to 0
     * - Segmentation Length ID: must be binary '11' 
     *   */  
    if (pChannelConfig->dataType == 0)
    {
        TMTF_SetSyncFlag(pFrameInfo->frame, 0);
        TMTF_SetPacketOrderFlag(pFrameInfo->frame, 0);
        TMTF_SetSegLengthId(pFrameInfo->frame, 3); 
        TMTF_SetFirstHdrPtr(pFrameInfo->frame, TMTF_NO_FIRST_HDR_PTR);
    }
    /* If VCA service is used, set sync flag to 1. All other fields are
     * undefined [TM_SDLP 4.1.2.7] */
    else
    {
        TMTF_SetSyncFlag(pFrameInfo->frame, 1);
    }

    /* Create the Mutex */
    gvcid = TMTF_GetGlobalVcId(pFrameInfo->frame);
    sprintf(mutName, "TF Global VC ID %d", gvcid);
    OS_MutSemCreate(&pFrameInfo->mutexId, mutName, 0); 

    pFrameInfo->isInitialized = TRUE;

end_of_function:
    return iStatus;
}


/*****************************************************************************/
/** \brief TMTF_SDLP_FrameHasData
******************************************************************************/
int32 TM_SDLP_FrameHasData(TM_SDLP_FrameInfo_t *pFrameInfo)
{
    int32 hasData = 0;

    if (pFrameInfo == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_FrameHasData Error: "
                          "Input Pointer is Null.");
        
        hasData = TM_SDLP_INVALID_POINTER;
        goto end_of_function;
    }
    
    if (pFrameInfo->freeOctets < pFrameInfo->dataFieldLength)
    {
        hasData = 1;
    }

end_of_function:
    return hasData;
}


/******************************************************************************/
/** \brief TM_SDLP_AddPacket
*******************************************************************************/
int32 TM_SDLP_AddPacket(TM_SDLP_FrameInfo_t *pFrameInfo, CFE_SB_Msg_t *pPacket)
{
    uint16 length = 0;
    int32 iStatus = TM_SDLP_SUCCESS;

    if (pFrameInfo == NULL || pPacket == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_AddPacket Error: "
                          "Input Pointer is Null.");
        
        iStatus = TM_SDLP_INVALID_POINTER;
        goto end_of_function;
    }
    
    if (pFrameInfo->isInitialized == FALSE)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_AddPacket Error: "
                          "The channel is not initialized.");
        
        iStatus = TM_SDLP_FRAME_NOT_INIT;
        goto end_of_function;
    }
    
    length = CFE_SB_GetTotalMsgLength(pPacket);

    OS_MutSemTake(pFrameInfo->mutexId);
    iStatus = TM_SDLP_AddData(pFrameInfo, (uint8 *) pPacket, length, TRUE);
    OS_MutSemGive(pFrameInfo->mutexId);

end_of_function:
    return iStatus;
}

                           
/******************************************************************************/
/** \brief TM_SDLP_AddIdlePacket
*******************************************************************************/
int32 TM_SDLP_AddIdlePacket(TM_SDLP_FrameInfo_t *pFrameInfo,
                            CFE_SB_Msg_t *pIdlePacket)
{
    int32 iStatus = TM_SDLP_SUCCESS;
    uint16 lengthToCopy = 0;

    if (pFrameInfo == NULL || pIdlePacket == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_AddIdlePacket Error: "
                          "Input Pointer is Null.");
        
        iStatus = TM_SDLP_INVALID_POINTER;
        goto end_of_function;
    }

    if (pFrameInfo->isInitialized == FALSE)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_AddIdlePacket Error: "
                          "The channel is not initialized.");
        
        iStatus = TM_SDLP_FRAME_NOT_INIT;
        goto end_of_function;
    }

    OS_MutSemTake(pFrameInfo->mutexId);
    
    lengthToCopy = pFrameInfo->freeOctets;

    /* If no free octets, no idle data to add. Done. */
    if (lengthToCopy == 0)
    {
        OS_MutSemGive(pFrameInfo->mutexId);
        iStatus = TM_SDLP_SUCCESS;
        goto end_of_function;
    }
    /* Minimum length of idle packet is 7. */
    else if (lengthToCopy < 7)
    {
        lengthToCopy = 7;
    }

    /* The Message ID of the idle buffer should always be 0x3ff (Idle Packet). */
    if (CFE_SB_GetMsgId(pIdlePacket) != 0x3ffU)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_AddIdlePacket Error: "
                          "The IdlePacket has MsgId other than 0x3ff.");
        
        OS_MutSemGive(pFrameInfo->mutexId);
        iStatus = TM_SDLP_ERROR;
        goto end_of_function;
    }

    /* Set the idlePacket length in header to lenghtToCopy */
    CFE_SB_SetTotalMsgLength(pIdlePacket, lengthToCopy);
    
    /* Add the idle packet. May spill over to overflow buffer. */
    /* iStatus should always return 0 free-octet if successful. */
    iStatus = TM_SDLP_AddData(pFrameInfo, (uint8 *) pIdlePacket, lengthToCopy, 
                              TRUE);
    OS_MutSemGive(pFrameInfo->mutexId);

end_of_function:
    return iStatus;
}    


/******************************************************************************/
/** \brief TM_SDLP_AddVcaData
*******************************************************************************/
int32 TM_SDLP_AddVcaData(TM_SDLP_FrameInfo_t *pFrameInfo, uint8 *pData,
                         uint16 dataLength)
{
    int32 iStatus = TM_SDLP_SUCCESS;

    if (pFrameInfo == NULL || pData == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_AddVcaData Error: "
                          "Input Pointer is Null.");
        
        iStatus = TM_SDLP_INVALID_POINTER;
        goto end_of_function;
    }
    
    /* Check if the frame is initialized */
    if (pFrameInfo->isInitialized == FALSE)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_AddVcaData Error: "
                          "The channel is not initialized.");
        
        iStatus = TM_SDLP_FRAME_NOT_INIT;
        goto end_of_function;
    }
    
    OS_MutSemTake(pFrameInfo->mutexId);
    iStatus = TM_SDLP_AddData(pFrameInfo, pData, dataLength, FALSE);
    OS_MutSemGive(pFrameInfo->mutexId);

end_of_function:
    return iStatus;
}


/******************************************************************************/
/** \brief TM_SDLP_StartFrame
*******************************************************************************/
int32 TM_SDLP_StartFrame(TM_SDLP_FrameInfo_t *pFrameInfo) 
{
    uint16 lengthToCopy = 0;
    uint16 lengthCopied = 0;
    TM_SDLP_OverflowInfo_t *pOverflow;
    int32 iStatus = TM_SDLP_SUCCESS;
    
    if (pFrameInfo == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_StartFrame Error: "
                          "Input Pointer is Null.");
        
        iStatus = TM_SDLP_INVALID_POINTER;
        goto end_of_function;
    }
    
    /* Check if frame has been initialized */
    if (pFrameInfo->isInitialized == FALSE)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_StartFrame Error: "
                          "The channel is not initialized.");
        
        iStatus = TM_SDLP_FRAME_NOT_INIT;
        goto end_of_function;
    }

    OS_MutSemTake(pFrameInfo->mutexId);
    
    /* If the frame is already started, issue a warning. */
    if (pFrameInfo->isReady == TRUE)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_INFORMATION,
                          "TM_SDLP_StartFrame: "
                          "The frame was already started. Will continue.");
    }
    
    /* Set Frame as ready */
    pFrameInfo->isReady = TRUE;
    
    pOverflow = &pFrameInfo->overflowInfo;
    lengthToCopy = pOverflow->buffSize - pOverflow->freeOctets;

    if (lengthToCopy > 0)
    {
        /* Only copy as much as TF allows */
        if (lengthToCopy > pFrameInfo->dataFieldLength)
        {
            lengthToCopy = pFrameInfo->dataFieldLength;
        }
        
        while (lengthToCopy > 0)
        {
            lengthCopied = TM_SDLP_CopyFromOverflow(pFrameInfo);
            lengthToCopy -= lengthCopied;
        }
    }

    OS_MutSemGive(pFrameInfo->mutexId);

end_of_function:
    return iStatus;
}


/******************************************************************************/
/** \brief TM_SDLP_SetOidFrame
*******************************************************************************/
int32 TM_SDLP_SetOidFrame(TM_SDLP_FrameInfo_t *pFrameInfo, 
                          CFE_SB_Msg_t *pIdlePacket)
{
    int32 iStatus = TM_SDLP_SUCCESS;
    uint8 *pIdleData = NULL;
    
    if (pFrameInfo == NULL || pIdlePacket == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_SetOidFrame Error: "
                          "Input Pointer is Null.");
        
        iStatus = TM_SDLP_INVALID_POINTER;
        goto end_of_function;
    }
    
    /* Check if frame has been initialized */
    if (pFrameInfo->isInitialized == FALSE)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_SetOidFrame Error: "
                          "The channel is not initialized.");
        
        iStatus = TM_SDLP_FRAME_NOT_INIT;
        goto end_of_function;
    }

    OS_MutSemTake(pFrameInfo->mutexId);

    /* If the frame is not empty, This method should not be called. */
    if (pFrameInfo->freeOctets != pFrameInfo->dataFieldLength)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_SetOidFrame Error: "
                          "The frame is not empty. VC ID:%u",
                          pFrameInfo->chnlConfig->vcId);
        
        OS_MutSemGive(pFrameInfo->mutexId);
        iStatus = TM_SDLP_ERROR;
        goto end_of_function;
    }

    pIdleData = CFE_SB_GetUserData(pIdlePacket);    

    TMTF_SetFirstHdrPtr(pFrameInfo->frame, TMTF_OID_FIRST_HDR_PTR);
    pFrameInfo->isFirstHdrPtrSet = TRUE;
    iStatus = TM_SDLP_AddData(pFrameInfo, pIdleData, pFrameInfo->dataFieldLength, 
                              FALSE);
    
    OS_MutSemGive(pFrameInfo->mutexId);

end_of_function:
    return iStatus;
}


/******************************************************************************/
/** \brief TM_SDLP_CompleteFrame
*******************************************************************************/
int32 TM_SDLP_CompleteFrame(TM_SDLP_FrameInfo_t *pFrameInfo,
                            uint8 *pMcFrameCnt, uint8 *pOcf)
{
    uint8 vcFrameCnt = 0;
    int32 iStatus = TM_SDLP_SUCCESS;

    if (pFrameInfo == NULL || pMcFrameCnt == NULL) 
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_CompleteFrame Error: "
                          "Input Pointer is Null.");
        
        iStatus = TM_SDLP_INVALID_POINTER;
        goto end_of_function;
    }
    
    /* Check if frame has been initialized */
    if (pFrameInfo->isInitialized == FALSE)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_CompleteFrame Error: "
                          "The channel is not initialized.");
        
        iStatus = TM_SDLP_FRAME_NOT_INIT;
        goto end_of_function;
    }
    
    if (pFrameInfo->chnlConfig->ocfFlag == TRUE && pOcf == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP_CompleteFrame Error: "
                          "The Input OCF Pointer is NULL.");
        
        iStatus = TM_SDLP_INVALID_POINTER;
        goto end_of_function;
    }
    
    OS_MutSemTake(pFrameInfo->mutexId);

    /* Increment the master channel frame count */
    *pMcFrameCnt = *pMcFrameCnt + 1;
    TMTF_SetMcFrameCount(pFrameInfo->frame, *pMcFrameCnt);

    /* Increment VC frame count if it is a virtual channel */
    if (pFrameInfo->chnlConfig->isMaster == FALSE)
    {
        vcFrameCnt = TMTF_IncrVcFrameCount(pFrameInfo->frame);
    }

    /* If an OCF Field is present, set it. */
    if (pFrameInfo->chnlConfig->ocfFlag)
    {
        TMTF_SetOcf(pFrameInfo->frame, pOcf, pFrameInfo->ocfOffset);
    }
   
    /* If an ErrCtrl Field is present, set it. */
    if (pFrameInfo->globConfig->hasErrCtrl)
    {
        TMTF_UpdateErrCtrlField(pFrameInfo->frame, pFrameInfo->errCtrlOffset);
    }
    
    /* This may happen if the frame is filled by a partial Packet */
    if (pFrameInfo->isFirstHdrPtrSet == FALSE)
    {
        TMTF_SetFirstHdrPtr(pFrameInfo->frame, TMTF_NO_FIRST_HDR_PTR);
    }

    /* Reset frame metadata */
    pFrameInfo->freeOctets          = pFrameInfo->dataFieldLength;
    pFrameInfo->currentDataOffset   = pFrameInfo->dataFieldOffset;
    pFrameInfo->isFirstHdrPtrSet    = FALSE;
    pFrameInfo->isReady             = FALSE;

    OS_MutSemGive(pFrameInfo->mutexId);

end_of_function:
    return iStatus;
}


/*******************************************************************************
** Static Functions
*******************************************************************************/

/******************************************************************************/
/** \brief Add Generic Data to the Transfer Frame
*
*   \par Description/Algorithm
*       Copies a data buffer to the TF data field at the next free octet.
*
*   \par Assumptions, External Events, and Notes:
*       - Lower level function called by AddPacket, AddIdlePacket, AddVcaData
*       - Data unit will be segmented into overflow buffer if frame is full.
*       - Transfer frame will be populated with overflow data if frame is 
*         empty prior to adding data.
*       - isPacket flag is used to set the First Header Pointer to the start 
*         of the suplied packet.
*
*   \param[in,out] pFrameInfo  Pointer to the Frame info/working struct.
*   \param[in]     pData       Pointer to data buffer
*   \param[in]     dataLength  Length of data to copy
*   \param[in]     isPacket    Data is a packet (VCP PDU / Idle packet)
*
*   \return Frame FreeOctets
*   \return TM_SDLP_FRAME_NOT_READY    If frame has not been started
*   \return TM_SDLP_OVERFLOW_FULL      Data dropped. The overflow buffer is full
*
*   \see
*       #TM_SDLP_AddPacket
*       #TM_SDLP_AddIdlePacket
*******************************************************************************/
static int32 TM_SDLP_AddData(TM_SDLP_FrameInfo_t *pFrameInfo, uint8 *pData, 
                             uint16 dataLength, boolean isPacket)
{
    uint16 lengthToCopy = dataLength;
    int32 iStatus = TM_SDLP_SUCCESS;
    boolean isPartial = FALSE;

    /* Check if the frame is ready to add new data. */
    if (pFrameInfo->isReady == FALSE)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP Error: "
                          "The Frame is not ready. Call StartFrame().");
        
        iStatus = TM_SDLP_FRAME_NOT_READY;
        goto end_of_function;
    }

    /* If data needs to be segmented, copy extra octets to overflow buffer */
    if (pFrameInfo->freeOctets < lengthToCopy)
    {
        lengthToCopy = pFrameInfo->freeOctets;
        if (lengthToCopy > 0)
        {
            isPartial = TRUE;
        }

        iStatus = TM_SDLP_CopyToOverflow(&pFrameInfo->overflowInfo,
                                      pData + lengthToCopy, 
                                      dataLength - lengthToCopy, isPartial);
        if (iStatus < 0)
        {
            goto end_of_function;
        }
    }

    CFE_PSP_MemCpy((void *) pFrameInfo->frame + pFrameInfo->currentDataOffset, 
                   pData, lengthToCopy);
    pFrameInfo->freeOctets -= lengthToCopy;

    if ((isPacket == TRUE) && (pFrameInfo->isFirstHdrPtrSet == FALSE))
    {
        uint16 firstHdrPtr = pFrameInfo->currentDataOffset - 
                             pFrameInfo->dataFieldOffset;
        TMTF_SetFirstHdrPtr(pFrameInfo->frame, firstHdrPtr);
        pFrameInfo->isFirstHdrPtrSet = TRUE;
    }
    pFrameInfo->currentDataOffset += lengthToCopy;
    iStatus = (int32) pFrameInfo->freeOctets;

end_of_function:
    return iStatus;
}


/******************************************************************************/
/** \brief Copy data to the end of the overflow queue
*
*   \par Description/Algorithm
*       Copy data to overflow queue
*
*   \par Assumptions, External Events, and Notes:
 *      - If overflow buffer is full, message is dropped in AddData
 *      - The overflow queue is implemented as a sliding window buffer.
 *      - Function called by AddData()
 *      - Input pointers are checked by calling function. 
*
*   \param[in,out] pOverflow        Pointer to the Overflow info
*   \param[in]     data             Pointer to the data to copy
*   \param[in]     length           Length of the data to copy
*   \param[in]     isPartial        Is data partial 
*
*   \return TM_SDLP_SUCCESS             If successful.
*   \return TM_SDLP_INVALID_LENGTH      If an input length is invalid
*   \return TM_SDLP_OVERFLOW_FULL       If overflow buffer is full
*
*   \see 
*       #TM_SDLP_AddData
*******************************************************************************/
static int32 TM_SDLP_CopyToOverflow(TM_SDLP_OverflowInfo_t *pOverflow, uint8 *data, 
                                    uint16 length, boolean isPartial)
{
    int32 iStatus = TM_SDLP_SUCCESS;
    uint16 lengthToEnd;

    if (length > pOverflow->buffSize)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_ERROR,
                          "TM_SDLP ERROR: "
                          "Message Length too large for overflow Buffer.");
       
        iStatus = TM_SDLP_INVALID_LENGTH;
        goto end_of_function;
    }

    /* If the buffer is full, drop message. */
    if (length > pOverflow->freeOctets)
    {
        CFE_EVS_SendEvent(IO_LIB_TM_SDLP_EID, CFE_EVS_INFORMATION,
                          "TM_SDLP Warning: "
                          "The Frame's OverflowBuffer is Full. Message Dropped.");
       
       iStatus = TM_SDLP_OVERFLOW_FULL;
       goto end_of_function;
    }

    /* Get size left in buffer after dataEnd cursor */
    lengthToEnd = pOverflow->buffSize - (pOverflow->dataEnd - pOverflow->buffer);

    /* Copy the data at the dataEnd cursor */
    if (length < lengthToEnd)
    {
        CFE_PSP_MemCpy(pOverflow->dataEnd, data, length);
        pOverflow->dataEnd += length;
    }
    /* Wrap arround data in overflow buffer */
    else
    {
        CFE_PSP_MemCpy(pOverflow->dataEnd, data, lengthToEnd);
        CFE_PSP_MemCpy(pOverflow->buffer, data + lengthToEnd,
                       length - lengthToEnd);
        pOverflow->dataEnd = pOverflow->buffer + (length - lengthToEnd);
    }
    
    /* If we are passing only partial data, set the partialOctets */
    if (isPartial == TRUE)
    {
        pOverflow->partialOctets = length;
    }
    
    pOverflow->freeOctets -= length;

end_of_function:
    return iStatus;
}


/******************************************************************************/
/** \brief Copy one packet or partial octects from overflow queue.
*
*   \par Description/Algorithm
*       Copy one packet or partial octects from the start of the overflow queue
*       to the transfer frame.
*
*   \par Assumptions, External Events, and Notes:
*      - The overflow queue is implemented as a sliding window buffer.
*      - Function called by StartFrame()
*      - Input pointers are checked by calling function. 
*
*   \param[out] pFrameInfo     Pointer to the Frame info/working struct.
*
*   \return lengthCopied    Length of data copied
*
*   \see 
*       #TM_SDLP_StartFrame
*******************************************************************************/
static int32 TM_SDLP_CopyFromOverflow(TM_SDLP_FrameInfo_t *pFrameInfo)
{
    uint8 msgHdr[6];
    uint16 lengthToCopy;
    uint16 lengthToEnd;
    boolean setHeader;
    uint16 freeOctets;
    TM_SDLP_OverflowInfo_t *pOverflow = &pFrameInfo->overflowInfo;

    lengthToEnd = pOverflow->buffSize - 
                  (pOverflow->dataStart - pOverflow->buffer);

    freeOctets = pFrameInfo->freeOctets;

    /* First Determine how many octets to copy from overflow buffer. */
    
    /* If there are partial octets, copy those. */
    if (pFrameInfo->overflowInfo.partialOctets > 0)
    {
        lengthToCopy = pFrameInfo->overflowInfo.partialOctets;
        setHeader = FALSE;
    }
    /* Otherwise, copy the first packet */
    else 
    {
        /* If the the lengthToEnd is shorter than the Packet Primary Header,
         * Copy the header locally first. */
        if (lengthToEnd < 6)
        {
            CFE_PSP_MemCpy((void *)msgHdr, (void *) pOverflow->dataStart, 
                           lengthToEnd);
            CFE_PSP_MemCpy((void *)(msgHdr + lengthToEnd), 
                           (void *) pOverflow->buffer, 6 - lengthToEnd);
            lengthToCopy = CFE_SB_GetTotalMsgLength((CFE_SB_Msg_t *) msgHdr);
        }
        else
        {
            lengthToCopy = 
              CFE_SB_GetTotalMsgLength((CFE_SB_Msg_t *) pOverflow->dataStart);
        }
        
        setHeader = TRUE;
    }

    /* Only copy as many octets as TF has free octets available */
    if (freeOctets < lengthToCopy)
    {
        /* If we are passing a full packet, set the new value of partialOctets */
        if (setHeader == TRUE)
        {
            pFrameInfo->overflowInfo.partialOctets = lengthToCopy - freeOctets;
        }
        /* If we are passing partial octets, revise the partial octet value */
        else
        {
            pFrameInfo->overflowInfo.partialOctets -= freeOctets;
        }
        
        /* The length to copy is freeOctets */
        lengthToCopy = freeOctets;
    }
    /* If we are copying partial octets and there is enough freeOctets for all
     * partial octets, reset the partialOctets to 0. */
    else if (setHeader == FALSE)
    {
        pFrameInfo->overflowInfo.partialOctets = 0;
    }

    /* If the length to the end is greater than the length to copy, copy it. */
    if (lengthToEnd > lengthToCopy)
    {
        freeOctets = TM_SDLP_AddData(pFrameInfo, pOverflow->dataStart, 
                                     lengthToCopy, setHeader);
        pOverflow->dataStart += lengthToCopy;
    }
    /* Wrap arround overflow buffer if required */
    else
    {
        freeOctets = TM_SDLP_AddData(pFrameInfo, pOverflow->dataStart, 
                                  lengthToEnd, setHeader);
        freeOctets = TM_SDLP_AddData(pFrameInfo, pOverflow->buffer, 
                                  lengthToCopy - lengthToEnd, setHeader);
        pOverflow->dataStart = pOverflow->buffer + 
                                 (lengthToCopy - lengthToEnd);
    }
    
    /* Revise the number of free Octets in overflow buffer */
    pOverflow->freeOctets += lengthToCopy;
    pFrameInfo->freeOctets = freeOctets;

    return lengthToCopy;
}
