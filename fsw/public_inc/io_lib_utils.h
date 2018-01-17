/******************************************************************************/
/** \file  io_lib_utils.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*  
*   \brief Header file of IO Library utilities
*
*   \par Description:
*     Provides common utilities used by IO libraries.
*
*   \par Limitations, Assumptions, External Events, and Notes:
*     - Utilties are intended for use by other IO_LIB functions but may also
*       be called directly by applications if required.
*
*   \par Modification History:
*     - 2015-12-08 | Guy de Carufel | OSR | Code Started 
*******************************************************************************/

#ifndef _IO_LIB_UTILS_
#define _IO_LIB_UTILS_

#include "io_lib.h"

/******************************************************************************/
/** \brief PseudoRandomize Buffer 
*
*   \par Description/Algorithm
*       Perform a XOR on buffer with supplied pseudo-random sequence (255 bits).
*
*   \par Assumptions, External Events, and Notes:
*       - Sequence must be 255 bits long (stored in 32 byte array)
*       - Sequence can be generated using IO_LIB_UTIL_GenPseudoRandomSeq 
*       - This function is derived from CCSDS TC_SYNC and TM_SYNC specs. 
*
*   \param[in,out] pBuff      Pointer to the Buffer to randomize
*   \param[in]     buffSize   Size of buffer in bytes
*   \param[in]     pPrSeq     Pointer to 32-byte pseudo-random seq. array
*
*   \return IO_LIB_SUCCESS             
*
*   \see 
*       #IO_LIB_UTIL_GenPseudoRandomSeq
*******************************************************************************/
int32 IO_LIB_UTIL_PseudoRandomize(uint8 *pBuff, uint16 buffSize, uint8 *pPrSeq);


/******************************************************************************/
/** \brief Generate a 255-bit pseudo-random sequence
*
*   \par Description/Algorithm
*       Generates a pseudo-random sequence, implemented as a 8-bit Fibonacci 
*       Linear feedback shift register based on supplied polynomial and seed.
*
*   \par Assumptions, External Events, and Notes:
*       - Implemented as a 8-bit Fibonacci Linear feedback shift register 
*       - Length of the pSeqBuff is 32 bytes (to hold 255 bits + 1st bit of next
*         cycle)
*       - The supplied polynomial must be an 8-bit polynomial Hex value.
*         eg: poly = 0xa9 for (x^8 + x^7 + x^5 + x^3 + 1) w/ x^8 ommitted
*       - Seed is the initial shift registry value (eg. 0xff)
*       - Function to be called by other libraries requiring an 
*         8-bit pseudo-random sequence.
*
*   \param[in,out] pSeqBuff   Pointer to buffer holding pseudo-random sequence. 
*   \param[in]     poly       8-bit polynomial (eg. 0xa9: x^8+x^7+x^5+x^3+1)
*   \param[in]     seed       Initial value of shift register (eg.0xff)
*
*   \return IO_LIB_SUCCESS             
*
*   \see 
*       #IO_LIB_UTIL_PseudoRandomize
*******************************************************************************/
int32 IO_LIB_UTIL_GenPseudoRandomSeq(uint8 *pSeqBuff, uint8 poly, uint8 seed);

#endif /* _IO_LIB_UTILS_ */

/*==============================================================================
** End of file io_lib_utils.h
**============================================================================*/
