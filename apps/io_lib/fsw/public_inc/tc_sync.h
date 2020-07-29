/******************************************************************************/
/** \file  tc_sync.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Provides the TC Channel Synchronization service.
*
*   \par References:
*     - "TC Synchronization and Channel Coding", CCSDS 231.0-B-2,
*       Issue 2, September 2010
*
*   \par Limitations, Assumptions, External Events, and Notes:
*     - Only receiving end services are provided
*     - This service provides the synchronization and pseudo-randomization 
*       services for TC Transfer Frames (TCTF) 
*     - BCH Decoding of the transfer frame is not covered in this librarly.
*       The associated error-correction/detection should be performed
*       in a seperate decoding library.
*     - the pseudo-randomization uses a Fibonacci LFSR with seed of 0xff and 
*       polynomial of x^8 + x^6 + x^4 + x^3 + x^2 + x + 1. Period of 255 bits.
*     - User may use either GetTransferFrame function if full CLTU is 
*       available, or build transfer frame incrementally by calling 
*       piecewise functions.
*
*   \par Modification History:
*     - 2015-12-08 | Guy de Carufel | OSR | Code Started 
*******************************************************************************/

#ifndef _IO_TC_SYNC_
#define _IO_TC_SYNC_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
** Includes
*******************************************************************************/
#include "io_lib.h"


/*******************************************************************************
** Macro Definitions
*******************************************************************************/
#define TC_SYNC_FOUND_TAIL_SEQ    1
#define TC_SYNC_SUCCESS           0
#define TC_SYNC_INVALID_POINTER  -1
#define TC_SYNC_INVALID_LENGTH   -2
#define TC_SYNC_INVALID_CLTU     -3

#define TC_SYNC_START_SEQ_SIZE          2
#define TC_SYNC_CODE_BLOCK_SIZE         8
#define TC_SYNC_CODE_BLOCK_DATA_SIZE    7
#define TC_SYNC_TAIL_SEQ_SIZE           8

/*******************************************************************************
** Function Declarations
*******************************************************************************/
/******************************************************************************/
/** \brief Initialize the static Pseudo-Random sequence
*
*   \par Assumptions, External Events, and Notes:
*       - Generates a pseudo-random sequence with poly:0x5f and seed:0xff
*       - Called by IO_LibInit

*   \return TC_SYNC_SUCCESS             Always successful.
*
*   \see 
*       #IO_LibInit
*       #IO_LIB_UTIL_GenPseudoRandomSeq
*******************************************************************************/
int32 TC_SYNC_LibInit(void);


/******************************************************************************/
/** \brief Perform TC Synchronization
*
*   \par Description/Algorithm
*     Extract the Transfer frame (including fill bytes) from a Communication
*     Linke Transfer Unit (CLTU). 
*
*   \par Assumptions, External Events, and Notes:
*     - Use this function if you have full CLTU available, otherwise build
*       the transfer frame piecewise functions (see below).
*     - The input is a full Communication Link Transfer Unit (CLTU)
*     - Any error check / detection should be done prior to calling this fnct.
*     - The transfer frame within the CLTU may have been randomized. Use
*       derandomize = TRUE for output transfer frame to be de-randomized.
*     - The size of the transfer frame buffer must be able to hold full TF.
*     - The CLTU must hold both a CLTU start and tail sequence.
*     - The parity byte within the CLTU code blocks are ignored.
*     - The full TF, including any fill bytes, are copied into the TF Buffer.
*
*   \param[out] pTfBuff      Pointer to the buffer to store transfer frame.     
*   \param[in]  pCltu        Pointer to the CLTU.
*   \param[in]  tfBuffSize   The size in octets of TF buffer
*   \param[in]  cltuLength   The size in octets of CLTU.
*   \param[in]  derandomize  Whether the frame should be de-randomized         
*
*   \return transfer frame size        Number of octets copied to TF buffer
*   \return TC_SYNC_INVALID_POINTER    If any input pointer is NULL
*   \return TC_SYNC_INVALID_LENGTH     If the transfer frame buffer is 
*                                      too small to hold full TF in CLTU.
*   \return TC_SYNC_INVALID_CLTU       If the CLTU has invalid start sequence,
*                                      or no tail sequence found.
*
*   \see 
*       #TC_SYNC_CheckStartSeq
*       #TC_SYNC_GetCodeBlockData
*       #TC_SYNC_DeRandomizeFrame
*******************************************************************************/
int32 TC_SYNC_GetTransferFrame(uint8 *pTfBuff, uint8 *pCltu, 
                               uint16 tfBuffSize, uint16 cltuLength, 
                               boolean derandomize);


/******************************************************************************/
/** \brief Check CLTU Start Sequence (piecewise function)
*
*   \par Description/Algorithm
*     Verify that start sequence is correct. Increments the cltuOffset past the
*     start sequence.
*
*   \par Assumptions, External Events, and Notes:
*     - The cltuOffset will be incremented by the start sequence size.
*       It should have a value of zero when function is called, although this 
*       is not enfforced.
*
*   \param[in]       pSeq        Pointer to the sequence to check.     
*   \param[in, out]  pCltuOffset Pointer to the cltu cursor offset.
*
*   \return TC_SYNC_SUCCESS          Start Sequence is correct.
*   \return TC_SYNC_INVALID_POINTER  If any input pointer is NULL
*   \return TC_SYNC_INVALID_CLTU     If the CLTU has invalid start sequence
*
*   \see 
*       #TC_SYNC_GetTransferFrame
*******************************************************************************/
int32 TC_SYNC_CheckStartSeq(uint8 *pSeq, uint16 *pCltuOffset);


/******************************************************************************/
/** \brief Get data from a CLTU code block (piecewise function)
*
*   \par Description/Algorithm
*     Get transfer frame data from a CLTU code block and increment TF and CLTU
*     cursor offsets. 
*
*   \par Assumptions, External Events, and Notes:
*     - The code block data may be encoded and / or randomized
*     - Any error check / detection should be done prior to calling this fnct.
*     - The codeBlock is assumed to be a full code block in size.
*     - The user should call this function for every code block received until
*       a tail sequence code block is found.
*     - The parity byte within the CLTU code blocks are ignored.
*     - Full code block data (including any fill bytes) are copied to TF.
*
*   \param[out]    pTfBuff       Pointer to the transfer frame buffer.
*   \param[in]     pCodeBlock    Pointer to the CLTU code block.
*   \param[in,out] pTfOffset     Pointer to the current TF cursor offset.
*   \param[in,out] pCltuOffset   Pointer to the current CLTU cursor offset.
*   \param[in]     tfBuffSize    Size of the TF Buffer.
*   \param[in]     cltuMaxLength Max length of CLTU.
*
*   \return TC_SYNC_FOUND_TAIL_SEQ   CLTU Tail sequence found.
*   \return TC_SYNC_SUCCESS          Non-tail Code Block data copied.
*   \return TC_SYNC_INVALID_POINTER  If any input pointer is NULL
*   \return TC_SYNC_INVALID_CLTU     End of cltu length reached w/o tail seq.
*   \return TC_SYNC_INVALID_LENGTH   TF Buffer is full.
*
*   \see 
*       #TC_SYNC_GetTransferFrame
*******************************************************************************/
int32 TC_SYNC_GetCodeBlockData(uint8 *pTfBuff, uint8 *pCodeBlock,
                               uint16 *pTfOffset, uint16 *pCltuOffset,
                               uint16 tfBuffSize, uint16 cltuMaxLength);


/******************************************************************************/
/** \brief Perform TC Frame De-Randomization (piecewise function) 
*
*   \par Description/Algorithm
*      Perform the standard CCSDS TC_Sync channel de-randomization.
*
*   \par Assumptions, External Events, and Notes:
*      - The presence or absence of pseudo-randomization is fixed for a
*        physical channel and is a managed property.
*
*   \param[in,out] pFrame     The input frame to de-randomize
*   \param[in]     frameSize  The size of the frame in bytes
*
*   \return TC_SYNC_SUCCESS   Always successful.
*
*   \see 
*       #TC_SYNC_GetTransferFrame
*       #IO_LIB_UTIL_PseudoRandomize
*******************************************************************************/
int32 TC_SYNC_DeRandomizeFrame(uint8 *pFrame, uint16 frameSize);

#endif /* _TC_SYNC_H_ */

/*==============================================================================
** End of file tc_sync.h
**============================================================================*/
