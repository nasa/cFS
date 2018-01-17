/*******************************************************************************
 * File: tmtf.h
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose:
 *   Provide the Transfer Frame format API for the TM_SDLP Service.
 *
 * Reference(s):
 *   - _TM Space Data Link Protocol_, CCSDS 132.0-B-1_ (Issue 1, Sept. 2003)
 *   - _Space Packet Protocol_, CCSDS 133.0-B-1_ (Issue 1, Sept. 2003)
 *   - _A Painless Guide To CRC Error Detection Algorithm_ (Version 3, 1993),
 *      Ross N. Williams.  http://www.ross.net/crc/download/crc_v3.txt
 *
 * Notes:
 *  -The TM Transfer Frame is the protocol data unit (PDU) of the Telemetry 
 *   Space Data Link Protocol (TM-SDLP).
 *
 * History:
 *   04/26/2015, A. Asp, Odyssey Space Research, LLC
 *    -Created
 *   10/22/2015, G. de Carufel, Odyssey Space Research, LLC
 *    -Moved all services to tm_sdlp.h
 *
 ******************************************************************************/

#ifndef _TM_TRANSFER_FRAME_H_
#define _TM_TRANSFER_FRAME_H_

#include "common_types.h"

/*------------------------------------------------------------------------------
 * Items below should not require user updates
 */

/* Return codes */
#define TMTF_SUCCESS            (0)
#define TMTF_ERROR             (-1)
#define TMTF_INVALID_POINTER   (-2)
#define TMTF_INVALID_SECHDR    (-3)
#define TMTF_INVALID_LENGTH    (-4)

/* Fixed parameters to compute CRC */
#define TMTF_CRC_INIT_REGISTRY  0xffffU
#define TMTF_CRC_POLYNOMIAL     0x11021UL

/* Max number of virtual channels */
#define TMTF_MAX_VC                 8  

/* Fixed values */
#define TMTF_VERSION                1
#define TMTF_FSH_VERSION            0

#define TMTF_PRIHDR_LENGTH          6  
#define TMTF_SECHDR_MAX_LENGTH      63 
#define TMTF_OCF_LENGTH             4  
#define TMTF_ERR_CTRL_FIELD_LENGTH  2  
#define TMTF_NO_FIRST_HDR_PTR       (0x07 << 8 | 0xFF)
#define TMTF_OID_FIRST_HDR_PTR      (0x07 << 8 | 0xFE)


typedef struct
{
    uint8 Id[2];               /* MC id, VC id, OCF flag      */
    uint8 McFrameCount;        /* Master Channel Frame Count  */
    uint8 VcFrameCount;        /* Virtual Channel Frame Count */
    uint8 DataFieldStatus[2];  /* TF Data Field Status        */
} TMTF_PriHdr_t;

/*
 * Function: TMTF_LibInit
 *
 * Purpose:
 *   Initialize the static TMTF CRC Table
 *
 * Arguments:
 *
 * Return:
 *   TMTF_SUCCESS          Always returns success.
 *
 * Notes:
 *   - Called by IO_LibInit()
 */
int32 TMTF_LibInit(void);



/*
 * Function: TMTF_SetVersion
 *
 * Purpose:
 *   Sets the version number for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *   val  : value to set the version number
 *
 * Return:
 *   TMTF_SUCCESS          if the value is set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 * Notes:
 *   For CCSDS 132.0-B-1, Sept. 2003, the Transfer Frame Version Number
 *   shall be set to binary '00'.
 *
 */
int32 TMTF_SetVersion(TMTF_PriHdr_t *tfPtr, uint16 val);


/*
 * Function: TMTF_SetScId
 *
 * Purpose:
 *   Set spacecraft ID for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *   val  : value to set the spacecraft ID
 *
 * Return:
 *   TMTF_SUCCESS          if the value is set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 */
int32 TMTF_SetScId(TMTF_PriHdr_t *tfPtr, uint16 val);


/*
 * Function: TMTF_SetVcId
 *
 * Purpose:
 *   Set virtual channel ID for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *   val  : value to set the virtual channel ID
 *
 * Return:
 *   TMTF_SUCCESS          if the value is set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 */
int32 TMTF_SetVcId(TMTF_PriHdr_t *tfPtr, uint16 val);


/*
 * Function: TMTF_SetOcfFlag
 *
 * Purpose:
 *   Set operational control field flag for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *   val  : boolean value to set the flag
 *
 * Return:
 *   TMTF_SUCCESS          if the value is set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 */
int32 TMTF_SetOcfFlag(TMTF_PriHdr_t *tfPtr, boolean val);


/*
 * Function: TMTF_GetMcId
 *
 * Purpose:
 *   Get the Master Channel Id (Version Num + SCID) 
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *
 * Return:
 *   Master Channel ID (VersionNumber + SCID) 
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 */
int32 TMTF_GetMcId(TMTF_PriHdr_t *tfPtr);

/*
 * Function: TMTF_GetGlobalVcId
 *
 * Purpose:
 *   Get the global VC Id (MCID + VCID) (not including OCF)
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *
 * Return:
 *   Global Virtual Channel ID (MCID + VCID) 
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 */
int32 TMTF_GetGlobalVcId(TMTF_PriHdr_t *tfPtr);



/*
 * Function: TMTF_SetMcFrameCount
 *
 * Purpose:
 *   Set master channel frame count for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *   val  : value to set the master channel frame count
 *
 * Return:
 *   TMTF_SUCCESS          if the value is set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 */
int32 TMTF_SetMcFrameCount(TMTF_PriHdr_t *tfPtr, uint16 val);


/*
 * Function: TMTF_SetVcFrameCount
 *
 * Purpose:
 *   Set virtual channel frame count for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *   val  : value to set the virtual channel frame count
 *
 * Return:
 *   TMTF_SUCCESS          if the value is set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 */
int32 TMTF_SetVcFrameCount(TMTF_PriHdr_t *tfPtr, uint16 val);


/*
 * Function: TMTF_IncrVcFrameCount
 *
 * Purpose:
 *   Increment virtual channel frame count for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *
 * Return:
 *   TMTF_SUCCESS          if the value is set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 */
int32 TMTF_IncrVcFrameCount(TMTF_PriHdr_t *tfPtr);


/*
 * Function: TMTF_SetSecHdrFlag
 *
 * Purpose:
 *   Set flag indicating the presence or absence of the secondary header for
 *   the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *   val  : boolean value to set the flag
 *
 * Return:
 *   TMTF_SUCCESS          if the value is set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 */
int32 TMTF_SetSecHdrFlag(TMTF_PriHdr_t *tfPtr, boolean val);


/*
 * Function: TMTF_SetSyncFlag
 *
 * Purpose:
 *   Set the sync flag for the transfer frame
 *     FALSE = octet-synchronized and forward ordered packets or Idle Data inserted
 *     TRUE  = VCA_SDU inserted
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *   val  : boolean value to set the flag
 *
 * Return:
 *   TMTF_SUCCESS          if the value is set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 */
int32 TMTF_SetSyncFlag(TMTF_PriHdr_t *tfPtr, boolean val);


/*
 * Function: TMTF_SetPacketOrderFlag
 *
 * Purpose:
 *   Set the packet order flag for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *   val  : boolean value to set the flag
 *
 * Return:
 *   TMTF_SUCCESS          if the value is set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 */
int32 TMTF_SetPacketOrderFlag(TMTF_PriHdr_t *tfPtr, boolean val);


/*
 * Function: TMTF_SetSegLengthId
 *
 * Purpose:
 *   Set the segment length ID for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *   val  : value to set the segment length ID
 *
 * Return:
 *   TMTF_SUCCESS          if the value is set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 */
int32 TMTF_SetSegLengthId(TMTF_PriHdr_t *tfPtr, uint16 val);


/*
 * Function: TMTF_SetFirstHdrPtr
 *
 * Purpose:
 *   Set the location of the first header pointer for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *   val  : value to set the first header pointer
 *
 * Return:
 *   TMTF_SUCCESS          if the value is set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *
 * Notes:
 *  -Constants TMTF_NO_FIRST_HDR_PTR and TMTF_OID_FIRST_HDR_PTR defined to be used
 *   for 'val' in situations where there's no first header pointer in the current
 *   TF and there's only idle data in the packet, respectively.
 *
 */
int32 TMTF_SetFirstHdrPtr(TMTF_PriHdr_t *tfPtr, uint16 val);


/*
 * Function: TMTF_SetSecHdrLength
 *
 * Purpose:
 *   Set the size of the secondary header for the transfer frame
 *
 * Arguments:
 *   tfPtr: pointer to the transfer frame
 *   val  : value to set the length of the secondary header.  This value is one
 *          octet less than the actual length of the secondary header.
 *
 * Return:
 *   TMTF_SUCCESS          if the value was set
 *   TMTF_INVALID_POINTER  if the input pointer is NULL
 *   TMTF_INVALID_SECHDR   if the secondary header flag is not set
 *   TMTF_INVALID_LENGTH   if the length is too short (0 octets) or too long (> 63 octets)
 *
 */
int32 TMTF_SetSecHdrLength(TMTF_PriHdr_t *tfPtr, uint8 val);


/*
 * Function: TMTF_SetSecHdrData
 *
 * Purpose:
 *   Set the Data of the secondary header for the transfer frame
 *
 * Arguments:
 *   tfPtr : pointer to the transfer frame
 *   data  : pointer to the data to be copied to the secondary header data field
 *   length: number of octets to copy
 *
 * Return:
 *   TMTF_SUCCESS          if the data is copied
 *   TMTF_INVALID_POINTER  if an input pointer is NULL
 *   TMTF_INVALID_SECHDR   if the secondary header flag is not set
 *   TMTF_INVALID_LENGTH   if the length of the data being copied is larger than
 *                           the secondary header data field
 *
 * Notes:
 *
 */
int32 TMTF_SetSecHdrData(TMTF_PriHdr_t *tfPtr, uint8 *data, uint8 length);


/*
 * Function: TMTF_SetOcf
 *
 * Purpose:
 *   Copies the input data to the Operational Control Field (OCF) of the TF
 *
 * Arguments:
 *   tfPtr : pointer to the transfer frame
 *   data  : pointer to the data to be copied
 *   offset: number of octets from the start of the frame to the OCF
 *
 * Return:
 *   TMTF_SUCCESS          if the data is copied
 *   TMTF_INVALID_POINTER  if an input pointer is NULL
 *   TMTF_ERROR            if the OCF flag is not set for the frame
 *
 * Notes:
 *
 */
int32 TMTF_SetOcf(TMTF_PriHdr_t *tfPtr, uint8 *data, uint16 offset);



/*
 * Function: TMTF_UpdateErrCtrlField
 *
 * Purpose:
 *   Calculates the value of the error control field and copies it to the TF trailer
 *
 * Arguments:
 *   tfPtr : pointer to the transfer frame
 *   offset: number of octets from the start of the frame to the error control field
 *
 * Return:
 *   TMTF_SUCCESS          if the value is successfully calculated and copied
 *   TMTF_INVALID_POINTER  if an input pointer is NULL
 *
 * Notes:
 *
 */
int32 TMTF_UpdateErrCtrlField(TMTF_PriHdr_t *tfPtr, uint16 offset);

#endif
