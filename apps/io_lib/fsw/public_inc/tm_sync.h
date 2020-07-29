/******************************************************************************/
/** \file  tm_sync.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Provides the TM Channel Synchronization service.
*
*   \par References:
*     - "TM Synchronization and Channel Coding", CCSDS 131.0-B-2,
*       Issue 2, August 2011
*
*   \par Limitations, Assumptions, External Events, and Notes:
*     - Only sending end services are provided
*     - This service provides the synchronization and pseudo-randomization 
*       services for frames, such as Transfer Frames or AOS TF.
*     - The fixed synchronization header is the Attached Sync Marker (ASM)
*     - Coding of the transfer frame (eg. Read-solomon coding) is not covered in
*       this librarly. 
*     - Emplementing certain coding algorithms (eg. Turbo, LDPC) may require a
*       licence
*     - Define a code specific ASM header in the coding library.
*     - the pseudo-randomization uses a Fibonacci LFSR with seed of 0xff and 
*       polynomial of x^8 + x^7 + x^5 + x^3 + 1. Period of 255 bits.
*
*   \par Modification History:
*     - 2015-10-29 | Guy de Carufel | OSR | Code Started 
*******************************************************************************/

#ifndef _IO_TM_SYNC_
#define _IO_TM_SYNC_

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
/* The following AMS is used for uncoded data, convolutional, Reed-Solomon,
 * concatenated and rate-7/8 LDPC coded data. */
#define TM_SYNC_ASM_STR "1ACFFC1D" 
#define TM_SYNC_ASM_SIZE 4

#define TM_SYNC_SUCCESS           0
#define TM_SYNC_INVALID_POINTER  -1
#define TM_SYNC_INVALID_ASM_SIZE -2
#define TM_SYNC_RANDOMIZE_ERROR  -3


/*******************************************************************************
** Function Declarations
*******************************************************************************/
/******************************************************************************/
/** \brief Initialize the static Pseudo-Random sequence
*
*   \par Assumptions, External Events, and Notes:
*       - Generates a pseudo-random sequence with poly:0xa9 and seed:0xff 

*   \return TM_SYNC_SUCCESS             Always successful.
*
*   \see 
*       #IO_LibInit
*       #IO_LIB_UTIL_GenPseudoRandomSeq
*******************************************************************************/
int32 TM_SYNC_LibInit(void);


/******************************************************************************/
/** \brief Perform TM Synchronization
*
*   \par Description/Algorithm
*     Build the Channel Access Data UNIT (CADU) by appending the AMS to the 
*     frame.
*
*   \par Assumptions, External Events, and Notes:
*     - The frame being passed in may have already been coded (eg. Reed-solomon)
*       Or the coding may be performed after sync. (eg. convolutional coding)
*     - In the case of uncoded frame, pass the TM_SYNC_ASM_UNCODED as the asmHdr
*       and TM_SYNC_ASM_UNCODED_SIZE as the asmSize parameters.
*     - The frame must starts at pBuff + asmSize.
*     - The size of the buffer must be asmSize + frameSize
*
*   \param[out] pBuff     Pointer to the buffer where CADU is stored.           
*   \param[in]  asmStr    The string representing the ASM to append             
*   \param[in]  asmSize   The size in octets of the final ASM (ASM Str size/2) 
*   \param[in]  frameSize The size of the frame to synchronize                  
*   \param[in]  randomize Whether the frame should be pseudo-randomized         
*
*   \return Size of CADU               Size of the CADU (asmSize + frameSize)
*   \return TM_SYNC_INVALID_ASM_LENGTH If the ASM length is not an even number 
*                                      or is < 4, as all ASM header are at least
*                                      4 bytes.
*   \return TM_SYNC_INVALID_POINTER    If any input pointer is NULL
*
*   \see 
*       #TM_SYNC_PseudoRandomize
*******************************************************************************/
int32 TM_SYNC_Synchronize(uint8 *pBuff, char *asmStr, uint8 asmSize, 
                          uint16 frameSize, boolean randomize);



/******************************************************************************/
/** \brief Perform TM Frame PseudoRandomization 
*
*   \par Description/Algorithm
*      Perform the standard CCSDS TM_Sync channel pseudo randomization.
*
*   \par Assumptions, External Events, and Notes:
*      - The presence or absence of pseudo-randomization is fixed for a
*        physical channel and is a managed property, known by receiver.
*
*   \param[in,out] pFrame     The input frame to randomize
*   \param[in]     frameSize  The size of the frame in bytes
*
*   \return TM_SYNC_SUCCESS   Always successful.
*
*   \see 
*       #TM_SYNC_Synchronize
*******************************************************************************/
int32 TM_SYNC_PseudoRandomize(uint8 *pFrame, uint16 frameSize);

#endif /* _TM_SYNC_H_ */

/*==============================================================================
** End of file tm_sync.h
**============================================================================*/
