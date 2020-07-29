/*******************************************************************************
** File: tc_transfer_frame.c
**
** Copyright 2017 United States Government as represented by the Administrator
** of the National Aeronautics and Space Administration.  No copyright is
** claimed in the United States under Title 17, U.S. Code.
** All Other Rights Reserved.
**
** Purpose:
**   Provides functionality to handling telecommand space data link protocol
**   (TC SDLP) transfer frames (TF).
**
** History:
**   04/02/15, A. Asp, Odyssey Space Research, LLC
**    * Created
**
*******************************************************************************/

#include "../../public_inc/tctf.h"
#include "cfe.h"




/*------------------------------------------------------------------------------
 *
 * Macros for reading and writing the fields in a Telecommand Space Data Link
 * Protocol Transfer Frame.  All of the macros are used in a similar way:
 *
 *   TCTF_RD_xxx(hdr)        -- Read field xxx from TF header.
 *   TCTF_WR_xxx(hdr,value)  -- Write value to field xxx of TF header.
 *
 * Note that hdr is a reference to the actual TF structure,
 * not to a pointer to the structure.  If using a pointer, one must
 * refer to the structure as *pointer.
 *
 * The TCTF_WR macros may refer to the 'hdr' more than once; thus
 * the expression for 'hdr' must NOT contain any side effects.
 *
 *----------------------------------------------------------------------------*/
#define TCTF_RD_VERSION(hdr)                ((hdr).Octet[0] >> 6)
#define TCTF_WR_TF_VERSION(hdr,val)         ((hdr).Octet[0] = ((hdr).Octet[0] & 0x3F) | \
                                                              ((val) << 6))

#define TCTF_RD_BYPASS_FLG(hdr)             (((hdr).Octet[0] >> 5) & 0x01)
#define TCTF_WR_BYPASS_FLG(hdr,val)         ((hdr).Octet[0] = ((hdr).Octet[0] & 0xDF) | \
                                                              (((val) & 0x01) << 5))

#define TCTF_RD_CTLCMD_FLG(hdr)             (((hdr).Octet[0] >> 4) & 0x01)
#define TCTF_WR_CTLCMD_FLG(hdr,val)         ((hdr).Octet[0] = ((hdr).Octet[0] & 0xEF) | \
                                                              (((val) & 0x01) << 4))

#define TCTF_RD_SCID(hdr)                   ((((hdr).Octet[0] & 0x03) << 8) | ((hdr).Octet[1]))
#define TCTF_WR_SCID(hdr,val)               ((hdr).Octet[0] = ((hdr).Octet[0] & 0xFC) | \
                                                              (((val) >> 8) & 0x03),    \
                                             (hdr).Octet[1] = (val))

#define TCTF_RD_VCID(hdr)                   ((hdr).Octet[2] >> 2)
#define TCTF_WR_VCID(hdr,val)               ((hdr).Octet[2] = ((hdr).Octet[2] & 0x03) | \
                                                              ((val) << 2))

#define TCTF_RD_LENGTH(hdr)                 ((((hdr).Octet[2] & 0x03) << 8) | ((hdr).Octet[3]))
#define TCTF_WR_LENGTH(hdr)                 ((hdr).Octet[2] = ((hdr).Octet[2] & 0xFC) | \
                                                              (((val) >> 8) & 0x03),    \
                                             (hdr).Octet[3] = (val))

#define TCTF_RD_SEGHDR_SEQ_FLGS(hdr)        ((hdr).SegHdr >> 6)

#define TCTF_RD_SEGHDR_MAPID(hdr)           ((hdr).SegHdr & 0x3F)


/* Prototypes */
static uint16 TCTF_GetPayloadOffset(TCTF_ChannelService_t *channelService);


/*
 * Function: TCTF_GetVersion
 *
 * Purpose:
 *   Return the version number for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *
 * Return:
 *   The version number for the transfer frame
 *
 * Notes:
 *
 */
uint16 TCTF_GetVersion(TCTF_Hdr_t *tfPtr)
{
    if (tfPtr == NULL)
    {
        return 0;
    }

    return TCTF_RD_VERSION(*tfPtr);
}


/*
 * Function: TCTF_GetBypassFlag
 *
 * Purpose:
 *   Return the bypass flag for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *
 * Return:
 *   The bypass flag for the transfer frame
 *
 * Notes:
 *
 */
uint16 TCTF_GetBypassFlag(TCTF_Hdr_t *tfPtr)
{
    if (tfPtr == NULL)
    {
        return 0;
    }

    return TCTF_RD_BYPASS_FLG(*tfPtr);
}


/*
 * Function: TCTF_GetCtlCmdFlg
 *
 * Purpose:
 *   Return control command flag for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *
 * Return:
 *   The control command flag for the transfer frame
 *
 * Notes:
 *
 */
uint16 TCTF_GetCtlCmdFlag(TCTF_Hdr_t *tfPtr)
{
    if (tfPtr == NULL)
    {
        return 0;
    }

    return TCTF_RD_CTLCMD_FLG(*tfPtr);
}


/*
 * Function: TCTF_GetScId
 *
 * Purpose:
 *   Return spacecraft ID for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *
 * Return:
 *   The spacecraft ID for the transfer frame
 *
 * Notes:
 *
 */
uint16 TCTF_GetScId(TCTF_Hdr_t *tfPtr)
{
    if (tfPtr == NULL)
    {
        return 0;
    }

    return TCTF_RD_SCID(*tfPtr);
}


/*
 * Function: TCTF_GetVcId
 *
 * Purpose:
 *   Return the virtual channel ID for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *
 * Return:
 *   The virtual channel ID for the transfer frame
 *
 * Notes:
 *
 */
uint16 TCTF_GetVcId(TCTF_Hdr_t *tfPtr)
{
    if (tfPtr == NULL)
    {
        return 0;
    }

    return TCTF_RD_VCID(*tfPtr);
}


/*
 * Function: TCTF_GetLength
 *
 * Purpose:
 *   Return the number of octets for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *
 * Return:
 *   The number of octets for the transfer frame
 *
 * Notes:
 *  -The frame length in the TF is defined as one fewer than the total octets in the
 *   frame.  Need to add that one back in for the correct number.
 *
 */
uint16 TCTF_GetLength(TCTF_Hdr_t *tfPtr)
{
    uint16 length = 0;

    if (tfPtr == NULL)
    {
        return 0;
    }

    length = TCTF_RD_LENGTH(*tfPtr) + 1;

    return length;
}


/*
 * Function: TCTF_GetSeqNum
 *
 * Purpose:
 *   Return the sequence number for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *
 * Return:
 *   The sequence number for the transfer frame
 *
 * Notes:
 *
 */
uint8 TCTF_GetSeqNum(TCTF_Hdr_t *tfPtr)
{
    if (tfPtr == NULL)
    {
        return 0;
    }

    return tfPtr->Sequence;
}


/*
 * Function: TCTF_GetPayloadLength
 *
 * Purpose:
 *   Return the number of octets for the payload data in the transfer frame
 *
 * Arguments:
 *   tfPtr          : pointer to the transfer frame
 *   channelService : pointer to the identifiers and service type for the expected TF
 *
 * Return:
 *   The number of octets for the payload data
 *
 * Notes:
 *   Assumes the length specified in the header is correct
 *
 */
uint16 TCTF_GetPayloadLength(TCTF_Hdr_t *tfPtr, TCTF_ChannelService_t *channelService)
{
    uint16 length = 0;

    if (tfPtr == NULL || channelService == NULL)
    {
        return 0;
    }

    length = TCTF_GetLength(tfPtr) - TCTF_PRIHDR_SIZE;

    if (channelService->HasSegHdr == TRUE)
    {
        length -= TCTF_SEGHDR_SIZE;
    }

    if (channelService->HasFrameErrCtl == TRUE)
    {
        length -= TCTF_FRAME_ERROR_CONTROL_SIZE;
    }

    return length;
}


/*
 * Function: TCTF_CopyData
 *
 * Purpose:
 *   Copies the transfer frame data field to the input buffer
 *
 * Arguments:
 *   toBuffer      : pointer to the destination buffer
 *   tfPtr         : pointer to the transfer frame
 *   channelService: pointer to the identifiers and service type for the expected TF
 *
 * Returns:
 *   The copied size
 *
 * Notes:
 *   Assumes the destination buffer has enough space for the maximum transfer
 *   frame payload length
 *
 */
uint16 TCTF_CopyData(uint8 *toBuffer, TCTF_Hdr_t *tfPtr, TCTF_ChannelService_t *channelService)
{
    uint16 dataLength = 0;
    uint16 offset     = 0;

    if (toBuffer == NULL || tfPtr == NULL || channelService == NULL)
    {
        return 0;
    }

    dataLength = TCTF_GetPayloadLength(tfPtr, channelService);
    offset     = TCTF_GetPayloadOffset(channelService);

    CFE_PSP_MemCpy(toBuffer, (uint8 *)(tfPtr) + offset, dataLength);

    return dataLength;
}


/*
 * Function: TCTF_GetSegHdrSeqFlags
 *
 * Purpose:
 *   Return the segment header sequence flags for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *
 * Return:
 *   The sequence flags for the transfer frame from the segment header
 *
 * Notes:
 *  -Sequence flags are represented by 2 bits, but being returned in a larger type.
 *   Used the extra bits to return an illegal flag if the segment header doesn't exist.
 *
 */
uint16 TCTF_GetSegHdrSeqFlags(TCTF_Hdr_t *tfPtr)
{
    if ((tfPtr == NULL) || (TCTF_RD_CTLCMD_FLG(*tfPtr) == TCTF_CONTROL_FRAME))
    {
        return TCTF_NO_SEGHDR;
    }

    return TCTF_RD_SEGHDR_SEQ_FLGS(*tfPtr);
}


/*
 * Function: TCTF_GetSegHdrMapId
 *
 * Purpose:
 *   Return the segment header Multiplexer Access Point (MAP) ID for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *
 * Return:
 *   The MAP ID for the transfer frame from the segment header
 *
 * Notes:
 *  -MAP IDs are represented by 6 bits, but being returned in a larger type.
 *   Used the extra bits to return an illegal flag if the segment header doesn't exist.
 *
 */
uint16 TCTF_GetSegHdrMapId(TCTF_Hdr_t *tfPtr)
{
    if ((tfPtr == NULL) || (TCTF_RD_CTLCMD_FLG(*tfPtr) == TCTF_CONTROL_FRAME))
    {
        return TCTF_NO_SEGHDR;
    }

    return TCTF_RD_SEGHDR_MAPID(*tfPtr);
}


/*
 * Function: TCTF_GetPayloadOffset
 *
 * Purpose:
 *   Return the octet offset to the transfer frame payload
 *
 * Arguments:
 *   channelService : pointer to the identifiers and service type for the expected TF
 *
 * Return:
 *   The offset to the transfer frame payload
 *
 * Notes:
 *  *  -As an internal function, assumes the calling function checked for a null pointers
 *
 */
static uint16 TCTF_GetPayloadOffset(TCTF_ChannelService_t *channelService)
{
    uint16 offset = TCTF_PRIHDR_SIZE;

    if (channelService->HasSegHdr == TRUE)
    {
        offset += TCTF_SEGHDR_SIZE;
    }

    return offset;
}


/*
 * Function: TCTF_IsValidTf
 *
 * Purpose:
 *   Checks transfer frame for valid Master Channel Id, Virtual Channel Id, and MAP Id
 *
 * Arguments:
 *   tfPtr          : pointer to the transfer frame
 *   channelService : pointer to the identifiers and service type for the expected TF
 *
 * Return:
 *   status indicating whether the data is valid or fails a check
 *
 * Notes:
 *  - if tests are structured such that the minimum number of values are checked
 *    for a given service
 *  - TODO: add check on Frame Error Control Field, if included
 *
 */
boolean TCTF_IsValidTf(TCTF_Hdr_t *tfPtr, TCTF_ChannelService_t *channelService)
{
    boolean status = TRUE;

    if (tfPtr == NULL || channelService == NULL)
    {
        return FALSE;
    }

    /* Verify Master Channel Id for every TF */
    if ((TCTF_RD_VERSION(*tfPtr) != channelService->PacketVersionNumber) ||
        (TCTF_RD_SCID(*tfPtr)    != channelService->SpacecraftId))
    {
        status = FALSE;
    }
    else if (TCTF_SERVICE_MCF != channelService->Service)
    {
        /* Verify Virtual Channel Id for all services with a virtual channel */
        if (TCTF_RD_VCID(*tfPtr) != channelService->VirtualChannelId)
        {
            status = FALSE;
        }
        else if ((TCTF_SERVICE_VCP != channelService->Service) &&
                 (TCTF_SERVICE_VCA != channelService->Service) &&
                 (TCTF_SERVICE_VCF != channelService->Service))
        {
            /* Verify MAP Id for all MAP services */
            if (TCTF_RD_SEGHDR_MAPID(*tfPtr) != channelService->MapId)
            {
                status = FALSE;
            }
        }
    }

    /* Verify valid combination of Bypass and Control Command Flags */
    if ((TCTF_CONTROL_FRAME == TCTF_GetCtlCmdFlag(tfPtr) &&
        (TCTF_RD_BYPASS_FLG(*tfPtr) == 0)))
    {
        status = FALSE;
    }

    return status;
}
