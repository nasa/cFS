/*******************************************************************************
 * File: tctf.h
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose:
 *   Provides the interface functionality for the receiver side of the Telecommand
 *   Space Data Link Transfer Frame
 *
 * Reference(s):
 *   - _TC Space Data Link Protocol_, CCSDS 232.0-B-2
 *
 * History:
 *   04/02/2015, A. Asp, Odyssey Space Research, LLC
 *    -Created
 *
 ******************************************************************************/

#ifndef _TC_TRANSFER_FRAME_H_
#define _TC_TRANSFER_FRAME_H_


#include "io_lib.h"

#define TCTF_FRAME_ERROR_CONTROL_INCLUDED  FALSE  /* Set for each mission. TRUE = each TF
                                                     has a frame error control field,
                                                     FALSE = otherwise */


/*------------------------------------------------------------------------------
 * Items below should not require user updates
 */

#define TCTF_PRIHDR_SIZE               5   /* avoid using 'sizeof' in case of padding */
#define TCTF_FRAME_ERROR_CONTROL_SIZE  2   /* number of octets for the frame
                                              error control field */
#define TCTF_SEGHDR_SIZE               1   /* number of octets for the segment header */


/* NOTE: Refer to CCSDS 232.0-B-3 on the following definition of services */
/* Define values to specify the service type to be used */
typedef enum
{
    TCTF_SERVICE_MAPP = 0,  /* MAP Packet             */
    TCTF_SERVICE_VCP  = 1,  /* Virtual Channel Packet */
    TCTF_SERVICE_MAPA = 2,  /* MAP Access             */
    TCTF_SERVICE_VCA  = 3,  /* Virtual Channel Access */
    TCTF_SERVICE_VCF  = 4,  /* Virtual Channel Frame  */
    TCTF_SERVICE_MCF  = 5   /* Master Channel Frame   */
} TCTF_Service_t;


typedef struct
{
    uint8 Octet[4];   /* Transfer frame version, flags, ids, length   */
    uint8 Sequence;   /* Sequence number                              */
    uint8 SegHdr;     /* Segment header, (use is specific to each vc) */
} TCTF_Hdr_t;

#define TCTF_HDR_SIZE   sizeof(TCTF_Hdr_t)


/*
 * Define multiplexing channel information related to expected transfer frames.
 * The segment header is required for services with more than one MAP and
 * services which transfer service data units (SDUs) larger than permitted in
 * a transfer frame.
 * Note that the error control field's presence if fixed over physical channel
 */
typedef struct
{
    uint16  Service;               /* Service type                          */
    uint16  PacketVersionNumber;   /* always 0 for current version of CCSDS */
    uint16  SpacecraftId;          /* Spacecraft ID stored in TC header     */
    uint16  VirtualChannelId;      /* Virtual ID stored in TC header        */
    uint16  MapId;                 /* Map ID stored in Segment Header       */
    boolean HasSegHdr;             /* Wether a Segment header is present    */
    boolean HasFrameErrCtl;        /* If frame has err ctrl field           */
} TCTF_ChannelService_t;


typedef enum
{
    TCTF_DATA_FRAME    = 0,
    TCTF_CONTROL_FRAME = 1
} TCTF_Frame_t;

typedef enum
{
    TCTF_CONTINUING_SEGMENT = 0,
    TCTF_FIRST_SEGMENT      = 1,
    TCTF_LAST_SEGMENT       = 2,
    TCTF_NO_SEGMENTATION    = 3,
    TCTF_NO_SEGHDR          = 255
} TCTF_Sequence_t;



uint16  TCTF_GetVersion(TCTF_Hdr_t *tfPtr);
uint16  TCTF_GetBypassFlag(TCTF_Hdr_t *tfPtr);
uint16  TCTF_GetCtlCmdFlag(TCTF_Hdr_t *tfPtr);
uint16  TCTF_GetScId(TCTF_Hdr_t *tfPtr);
uint16  TCTF_GetVcId(TCTF_Hdr_t *tfPtr);
uint16  TCTF_GetLength(TCTF_Hdr_t *tfPtr);
uint8   TCTF_GetSeqNum(TCTF_Hdr_t *tfPtr);
uint16  TCTF_GetSegHdrSeqFlags(TCTF_Hdr_t *tfPtr);
uint16  TCTF_GetSegHdrMapId(TCTF_Hdr_t *tfPtr);

uint16  TCTF_GetPayloadLength(TCTF_Hdr_t *tfPtr, TCTF_ChannelService_t *channelService);
uint16  TCTF_CopyData(uint8 *toBuffer, TCTF_Hdr_t *tfPtr, TCTF_ChannelService_t *channelService);

boolean TCTF_IsValidTf(TCTF_Hdr_t *tfPtr, TCTF_ChannelService_t *channelService);


#endif /* _TC_TRANSFER_FRAME_H_ */
