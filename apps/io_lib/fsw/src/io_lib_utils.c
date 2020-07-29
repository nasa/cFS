/******************************************************************************/
/** \file  io_lib_utils.c
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*  
*   \brief Function Definitions of IO Library utilities
*
*   \par
*     Provides common utilities used by IO libraries.
*
*   \par Limitations, Assumptions, External Events, and Notes:
*
*   \par Modification History:
*     - 2015-12-08 | Guy de Carufel | OSR | Code Started 
*******************************************************************************/

#include "io_lib_utils.h"

const static uint8 revBitsTbl[16] = {0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
                                     0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf};

static uint8 IO_LIB_UTIL_ReverseBits(uint8 byte);

/*****************************************************************************/
/** \brief IO_LIB_UTIL_PseudoRandomize
******************************************************************************/
int32 IO_LIB_UTIL_PseudoRandomize(uint8 *pBuff, uint16 buffSize, uint8 *pPrSeq)
{
    uint32 bit = 0;
    uint16 bitOffset = 0;
    uint16 byte = 0;
    uint8 prSeqIdx = 0;

    for (byte = 0; byte < buffSize; ++byte)
    {
        bit = (byte * 8) % 255;
        prSeqIdx = bit / 8;
        bitOffset = bit % 8;
        
        pBuff[byte] ^= (pPrSeq[prSeqIdx] << bitOffset) |
                        (pPrSeq[(prSeqIdx + 1)%32] >> (8-bitOffset));
    }                

    return IO_LIB_SUCCESS;
}


/*****************************************************************************/
/** \brief IO_LIB_UTIL_GenPseudoRandomSeq
******************************************************************************/
int32 IO_LIB_UTIL_GenPseudoRandomSeq(uint8 *pSeqBuff, uint8 poly, uint8 seed)
{                               
    /*
     * Notes:
     *   - Note the last stored shift registry contains last 7-bits completing 
     *     255 bit cycle + the first bit of the next cycle.
     *   - Implemented as a 8-bit Fibonacci Linear feedback shift register 
     */
    
    /* Initialize as seed  */
    uint8 lfsr = seed;
    uint8 bit;
    uint8 period = 0;
    uint8 ii = 0;

    for (period = 0; period < 249; ++period)
    {
        /* Store the new lfsr value in reverse bit order 
           to the pseudo-random sequence */
        if (period % 8 == 0)
        {
            pSeqBuff[period/8] = IO_LIB_UTIL_ReverseBits(lfsr);
        }
        
        bit = 0;
        for (ii = 0; ii < 8; ++ii)
        {
            bit ^= (((lfsr >> ii) & (poly >> ii)) & 1);
        }
        
        lfsr = (lfsr >> 1) | (bit << 7);
    }

    return IO_LIB_SUCCESS;
}


/*****************************************************************************/
/** \brief IO_LIB_UTIL_ReverseBits
*
*   \par Description:
*       Reverse bit order in a byte 
*
******************************************************************************/
static uint8 IO_LIB_UTIL_ReverseBits(uint8 byte) 
{
    return (revBitsTbl[byte&0xf]<<4) | (revBitsTbl[byte>>4]);
}
