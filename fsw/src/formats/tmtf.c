/*******************************************************************************
** File: tmtf.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** Purpose:
**   Provides functionality to handling telemetry space data link protocol
**   (TM SDLP) transfer frames (TF).
**
** History:
**   04/26/15, A. Asp, Odyssey Space Research, LLC
**    * Created
 *   10/22/2015, G. de Carufel, Odyssey Space Research, LLC
 *    -AddIdlePacket and CRC computation. Revised overflow buffer as queue.
**
*******************************************************************************/

#include "cfe.h"

#include "../../public_inc/tmtf.h"

static uint16 crcTable[256];    /* CRC Table generated through GenCrcTable */


/*------------------------------------------------------------------------------
 *
 * Macros for reading and writing the fields in a Telemetry Space Data Link
 * Protocol Transfer Frame.  All of the macros are used in a similar way:
 *
 *   TMTF_RD_xxx(hdr)        -- Read field xxx from TF header.
 *   TMTF_WR_xxx(hdr,value)  -- Write value to field xxx of TF header.
 *
 * Note that hdr is a reference to the actual TF structure,
 * not to a pointer to the structure.  If using a pointer, one must
 * refer to the structure as *pointer.
 *
 * The TMTF_WR macros may refer to the 'hdr' more than once; thus
 * the expression for 'hdr' must NOT contain any side effects.
 *
 *----------------------------------------------------------------------------*/
#define TMTF_WR_TF_VERSION(hdr,val)    ((hdr).Id[0] = ((hdr).Id[0] & 0x3F)    | \
                                                      ((val) << 6))

#define TMTF_WR_SCID(hdr,val)          ((hdr).Id[0] = (((hdr).Id[0] & 0xC0)   | \
                                                       (((val) >> 4) & 0x3F))), \
                                       ((hdr).Id[1] = (((hdr).Id[1] & 0x0F)   | \
                                                       (((val) & 0x0F) << 4)))

#define TMTF_WR_VCID(hdr,val)          ((hdr).Id[1] = (((hdr).Id[1] & 0xF1)   | \
                                                       (((val) & 0x07) << 1)))

#define TMTF_RD_OCFFLAG(hdr)           ((hdr).Id[1] & 0x01)
#define TMTF_WR_OCFFLAG(hdr,val)       ((hdr).Id[1] = (((hdr).Id[1] & 0xFE)   | \
                                                       ((val) & 0x01)))

#define TMTF_RD_MCID(hdr)              (((hdr).Id[0] << 8 | ((hdr).Id[1])) >> 4)
#define TMTF_RD_GVCID(hdr)             ((hdr).Id[0] << 8 | ((hdr).Id[1] & 0xFE))

#define TMTF_WR_MCFRMCNT(hdr,val)      ((hdr).McFrameCount = (val & 0xFF))

#define TMTF_WR_VCFRMCNT(hdr,val)      ((hdr).VcFrameCount = (val & 0xFF))

#define TMTF_RD_SECHDRFLAG(hdr)        (((hdr).DataFieldStatus[0] & 0x80) >> 7)
#define TMTF_WR_SECHDRFLAG(hdr,val)    ((hdr).DataFieldStatus[0] = (((val) << 7) | \
                                             ((hdr).DataFieldStatus[0] & 0x7F)))

#define TMTF_WR_SYNCFLAG(hdr,val)      ((hdr).DataFieldStatus[0] = ((((val) << 6) & 0x40) | \
                                             ((hdr).DataFieldStatus[0] & 0xBF)))

#define TMTF_WR_PKTORDERFLAG(hdr,val)  ((hdr).DataFieldStatus[0] = ((((val) << 5) & 0x20) | \
                                            ((hdr).DataFieldStatus[0] & 0xDF)))

#define TMTF_WR_SEGLENGTHID(hdr,val)   ((hdr).DataFieldStatus[0] = ((((val) << 3) & 0x18) | \
                                            ((hdr).DataFieldStatus[0] & 0xE7)))

#define TMTF_WR_FIRSTHDRPTR(hdr,val)   ((hdr).DataFieldStatus[0] = ((((val) >> 8) & 0x07) | \
                                            ((hdr).DataFieldStatus[0] & 0xF8))),            \
                                       ((hdr).DataFieldStatus[1] = (val) & 0xFF)

#define TMTF_RD_SECHDRLENGTH(shdr)     ((shdr) & 0x3F)
#define TMTF_WR_SECHDRLENGTH(shdr,val) ((shdr) = ((shdr) & 0xC0) | ((val) & 0x3F))

#define TMTF_WR_CRC(ptr,val)           (((ptr)[0] = (val >> 8) & 0xFF),\
                                        ((ptr)[1] = (val) & 0xFF))

/* Prototypes for internal functions */
static void   TMTF_GenCrcTable(uint32 polynomial);


/*
 * Function: TMTF_LibInit
 *
 * Notes:
 *  -Called by IO_LibInit()
 *
 */
int32 TMTF_LibInit(void)
{
    TMTF_GenCrcTable(TMTF_CRC_POLYNOMIAL);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetVersion
 *
 * Notes:
 *
 */
int32 TMTF_SetVersion(TMTF_PriHdr_t *tfPtr, uint16 val)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    TMTF_WR_TF_VERSION(*tfPtr, val);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetScId
 *
 * Notes:
 *
 */
int32 TMTF_SetScId(TMTF_PriHdr_t *tfPtr, uint16 val)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    TMTF_WR_SCID(*tfPtr, val);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetVcId
 *
 * Notes:
 *
 */
int32 TMTF_SetVcId(TMTF_PriHdr_t *tfPtr, uint16 val)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    TMTF_WR_VCID(*tfPtr, val);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetOcfFlag
 *
 * Notes:
 *
 */
int32 TMTF_SetOcfFlag(TMTF_PriHdr_t *tfPtr, boolean val)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    TMTF_WR_OCFFLAG(*tfPtr, val);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_GetMcId
 *
 * Notes:
 *
 */
int32 TMTF_GetMcId(TMTF_PriHdr_t *tfPtr)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    return TMTF_RD_MCID(*tfPtr);
}


/*
 * Function: TMTF_GetGlobalVcId
 *
 * Notes:
 *
 */
int32 TMTF_GetGlobalVcId(TMTF_PriHdr_t *tfPtr)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    return TMTF_RD_GVCID(*tfPtr);
}



/*
 * Function: TMTF_SetMcFrameCount
 *
 * Notes:
 *
 */
int32 TMTF_SetMcFrameCount(TMTF_PriHdr_t *tfPtr, uint16 val)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    TMTF_WR_MCFRMCNT(*tfPtr, val);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetVcFrameCount
 *
 * Notes:
 *
 */
int32 TMTF_SetVcFrameCount(TMTF_PriHdr_t *tfPtr, uint16 val)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    TMTF_WR_VCFRMCNT(*tfPtr, val);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_IncrVcFrameCount
 *
 * Notes:
 *
 */
int32 TMTF_IncrVcFrameCount(TMTF_PriHdr_t *tfPtr)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    /* Increment the Virtual channel frame count */
    tfPtr->VcFrameCount++;

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetSecHdrFlag
 *
 * Notes:
 *
 */
int32 TMTF_SetSecHdrFlag(TMTF_PriHdr_t *tfPtr, boolean val)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    TMTF_WR_SECHDRFLAG(*tfPtr, val);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetSyncFlag
 *
 * Notes:
 *
 */
int32 TMTF_SetSyncFlag(TMTF_PriHdr_t *tfPtr, boolean val)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    TMTF_WR_SYNCFLAG(*tfPtr, val);

    if (val == FALSE)
    {
        TMTF_WR_PKTORDERFLAG(*tfPtr, 0);
        TMTF_WR_SEGLENGTHID(*tfPtr, 3);   /* binary '11' = not used */
    }

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetPacketOrderFlag
 *
 * Notes:
 *
 */
int32 TMTF_SetPacketOrderFlag(TMTF_PriHdr_t *tfPtr, boolean val)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    TMTF_WR_PKTORDERFLAG(*tfPtr, val);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetSegLengthId
 *
 * Notes:
 *
 */
int32 TMTF_SetSegLengthId(TMTF_PriHdr_t *tfPtr, uint16 val)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    TMTF_WR_SEGLENGTHID(*tfPtr, val);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetFirstHdrPtr
 *
 * Notes:
 *
 */
int32 TMTF_SetFirstHdrPtr(TMTF_PriHdr_t *tfPtr, uint16 val)
{
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    TMTF_WR_FIRSTHDRPTR(*tfPtr, val);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetSecHdrLength
 *
 * Notes:
 *  -The actual length set in the secondary header is one less than the number
 *   of octets in the secondary header.
 *
 */
int32 TMTF_SetSecHdrLength(TMTF_PriHdr_t *tfPtr, uint8 val)
{
    uint8 *secHdrIdPtr = (uint8 *)(tfPtr) + TMTF_PRIHDR_LENGTH;

    if ((tfPtr == NULL))
    {
        return TMTF_INVALID_POINTER;
    }

    if (TMTF_RD_SECHDRFLAG(*tfPtr) == FALSE)
    {
        return TMTF_INVALID_SECHDR;
    }

    if ((val < 1) || (val > TMTF_SECHDR_MAX_LENGTH))
    {
        return TMTF_INVALID_LENGTH;
    }

    TMTF_WR_SECHDRLENGTH(*secHdrIdPtr, val);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetSecHdrData
 *
 * Notes:
 *  -The secondary header length in the secondary header is one less than the
 *   total length of the secondary header, which equals the actual length of
 *   the secondary header data field.
 *
 */
int32 TMTF_SetSecHdrData(TMTF_PriHdr_t *tfPtr, uint8 *data, uint8 length)
{
    uint8 secHdrDataLength = 0;
    uint8 *secHdrIdPtr     = (uint8 *)(tfPtr) + TMTF_PRIHDR_LENGTH;

    if ((tfPtr == NULL) || (data == NULL))
    {
        return TMTF_INVALID_POINTER;
    }

    if (TMTF_RD_SECHDRFLAG(*tfPtr) == FALSE)
    {
        return TMTF_INVALID_SECHDR;
    }

    secHdrDataLength = TMTF_RD_SECHDRLENGTH(*(uint8 *)secHdrIdPtr);
    if (length > secHdrDataLength)
    {
        return TMTF_INVALID_LENGTH;
    }

    CFE_PSP_MemCpy(secHdrIdPtr + 1, data, length);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_SetOcf
 *
 * Notes:
 *
 *
 */
int32 TMTF_SetOcf(TMTF_PriHdr_t *tfPtr, uint8 *data, uint16 offset)
{
    if ((tfPtr == NULL) || (data == NULL))
    {
        return TMTF_INVALID_POINTER;
    }

    if (TMTF_RD_OCFFLAG(*tfPtr) == FALSE)
    {
        return TMTF_ERROR;
    }

    CFE_PSP_MemCpy((uint8 *)tfPtr + offset, data, TMTF_OCF_LENGTH);

    return TMTF_SUCCESS;
}


/*
 * Function: TMTF_UpdateErrCtrlField
 *
 */
int32 TMTF_UpdateErrCtrlField(TMTF_PriHdr_t *tfPtr, uint16 offset)
{
    uint16 reg = 0xffffU;
    uint8 *octPtr = NULL;
    uint8 byte;
    uint32 len = 0;
    
    if (tfPtr == NULL)
    {
        return TMTF_INVALID_POINTER;
    }

    if (offset < sizeof(TMTF_PriHdr_t))
    {
        return TMTF_INVALID_LENGTH;
    }

    /* First clear the CRC bytes */
    CFE_PSP_MemSet((void *)((uint8 *)tfPtr + offset), 0x00, 2);
    
    octPtr = (uint8 *) tfPtr;

    len = offset + 2;
    while (len--)
    {
        byte = (reg >> 8) & 0xff;
        reg = (reg << 8) | *octPtr;
        reg ^= crcTable[byte];
        octPtr++;
    }

    TMTF_WR_CRC((uint8 *)tfPtr + offset, reg);

    return TMTF_SUCCESS;
}



/* ---------------------------  Helper Functions  ----------------------------- */


/*
 * Function: TMTF_GenCrcTable
 *
 * Purpose:
 *   Generate the CRC Table for CRC computation
 
 * Arguments:
 *   polynomial  : The polynomial coefficients (eg. 0x1021: x^16 + x^12 + x^5 + 1)
 *
 * Note:
 *   - The highest order coefficient is not required in the polynomial.
 *
 */
void TMTF_GenCrcTable(uint32 polynomial)
{
    uint32 remainder = 0;
    uint16 topbit = 1 << 15;
    uint16 val = 0;
    uint8 bit = 0;

    for (val = 0; val < 256; ++val)
    {
        /* The first remainder (16-bit) is the divident. */
        remainder = val << 8;
        
        /* Perform modulo-2 division, one bit at a time */
        for (bit = 0; bit < 8; ++bit)
        {
            /* If the remainder has topbit, divide by polynomial */
            if (remainder & topbit)
            {
                remainder = (uint16)(remainder << 1) ^ (uint16)(polynomial);
            }
            else
            {
                remainder = remainder << 1;
            }
        }

        crcTable[val] = (uint16)remainder;       
    }
}
