/******************************************************************************/
/** \file  tc_sync.c
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
*   \brief Provides the TC Channel Synchronization service.
*
*   \par Modification History:
*     - 2015-12-08 | Guy de Carufel | OSR | Code Started 
*******************************************************************************/
#include <stdlib.h>

#include "tc_sync.h"
#include "io_lib_utils.h"


static uint8 prSeq[32]; 
const static uint8 startSeq[2] = {0xeb, 0x90};
const static uint8 tailSeq[8] = {0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0x79};


/*****************************************************************************/
/** \brief TC_SYNC_LibInit
******************************************************************************/
int32 TC_SYNC_LibInit(void)
{
   IO_LIB_UTIL_GenPseudoRandomSeq(&prSeq[0], 0x5f, 0xff);

    return TC_SYNC_SUCCESS;
}


/*****************************************************************************/
/** \brief TC_SYNC_GetTransferFrame
******************************************************************************/
int32 TC_SYNC_GetTransferFrame(uint8 *pTfBuff, uint8 *pCltu, 
                               uint16 tfBuffSize, uint16 cltuLength, 
                               boolean derandomize)
{
    int32 iStatus = TC_SYNC_SUCCESS;
    uint16 tfOffset = 0;
    uint16 cltuOffset = 0;
    uint8 *pCltuCursor = &pCltu[2];

    if (pCltu == NULL || pTfBuff == NULL)
    {
        iStatus = TC_SYNC_INVALID_POINTER;
        goto end_of_function;
    }

    /* Get Start Sequence of CLTU and increment cltuOffset */
    if (TC_SYNC_CheckStartSeq(pCltu, &cltuOffset) < 0)
    {
        iStatus = TC_SYNC_INVALID_CLTU;
        goto end_of_function;
    }

    while(1)
    {
        iStatus = TC_SYNC_GetCodeBlockData(pTfBuff, pCltuCursor,
                                         &tfOffset, &cltuOffset,
                                         tfBuffSize, cltuLength);
        /* An error occured. return error code. */
        if (iStatus < 0)
        {
            break;
        }
        /* The tail sequence was found. Return length of TF. */
        else if (iStatus == TC_SYNC_FOUND_TAIL_SEQ)
        {
            iStatus = tfOffset;
            break;
        }
        pCltuCursor = &pCltu[cltuOffset];
    }
    
    /* Derandomize if applicable */
    if (iStatus > 0 && derandomize == TRUE)
    {
        TC_SYNC_DeRandomizeFrame(&pTfBuff[0], tfOffset);
    }

end_of_function:
    return iStatus;
}


/*****************************************************************************/
/** \brief TC_SYNC_CheckStartSeq
******************************************************************************/
int32 TC_SYNC_CheckStartSeq(uint8 *pSeq, uint16 *pCltuOffset)
{
    int32 iStatus = TC_SYNC_SUCCESS;
    
    if (pSeq == NULL || pCltuOffset == NULL)
    {
        iStatus = TC_SYNC_INVALID_POINTER;
        goto end_of_function;
    }

    if (pSeq[0] != startSeq[0] || pSeq[1] != startSeq[1])
    {
        iStatus = TC_SYNC_INVALID_CLTU;
    }
    else
    {
        *pCltuOffset += TC_SYNC_START_SEQ_SIZE;
    }

end_of_function:
    return iStatus;
}


/*****************************************************************************/
/** \brief TC_SYNC_GetCodeBlockData
******************************************************************************/
int32 TC_SYNC_GetCodeBlockData(uint8 *pTfBuff, uint8 *pCodeBlock,
                               uint16 *pTfOffset, uint16 *pCltuOffset,
                               uint16 tfBuffSize, uint16 cltuMaxLength)
{
    int32 iStatus = TC_SYNC_SUCCESS;
    uint8 byte = 0;
    uint16 lengthCopied;
    uint16 lengthRead; 

    if (pTfBuff == NULL || pCodeBlock == NULL || 
        pTfOffset == NULL || pCltuOffset == NULL)
    {
        iStatus = TC_SYNC_INVALID_POINTER;
        goto end_of_function;
    }

    lengthRead = *pCltuOffset + TC_SYNC_CODE_BLOCK_SIZE; 
    if (lengthRead > cltuMaxLength)
    {
        iStatus = TC_SYNC_INVALID_CLTU;
        goto end_of_function;
    }

    /* Check for tail sequence */
    for (byte = 0; byte < TC_SYNC_TAIL_SEQ_SIZE; ++byte)
    {
        if (pCodeBlock[byte] != tailSeq[byte])
        {
            break;
        }
    }
    /* We found the tail sequence. Done. */
    if (byte == TC_SYNC_TAIL_SEQ_SIZE)
    {
        iStatus = TC_SYNC_FOUND_TAIL_SEQ;
        goto end_of_function;
    }

    lengthCopied = *pTfOffset + TC_SYNC_CODE_BLOCK_DATA_SIZE;

    /* If we've reached the end of the TF Buffer, the 
     * CLTU message is too large for the buffer. */
    if (lengthCopied > tfBuffSize)
    {
        iStatus = TC_SYNC_INVALID_LENGTH;
        goto end_of_function;
    }

    /*Extract 7-bytes of information from code block */
    CFE_PSP_MemCpy((void *) &pTfBuff[*pTfOffset], (void *) pCodeBlock, 
                   TC_SYNC_CODE_BLOCK_DATA_SIZE);
    
    /* Update offsets of cursor location */
    *pTfOffset = lengthCopied;
    *pCltuOffset = lengthRead;

end_of_function:
    return iStatus;
}


/*****************************************************************************/
/** \brief TC_SYNC_DeRandomizeFrame
******************************************************************************/
int32 TC_SYNC_DeRandomizeFrame(uint8 *pFrame, uint16 frameSize)
{
    return IO_LIB_UTIL_PseudoRandomize(pFrame, frameSize, prSeq);
}
