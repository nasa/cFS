/*******************************************************************************
 * File: cop1.c
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose:
 *   Provide the functionality for the receiver for the Communications Operation
 *   Procedure-1 (COP-1), i.e. the Frame Acceptance and Reporting Mechanism-1
 *   (FARM-1).
 *
 * Reference:
 *   -_Communications Operation Procedure-1 Recommended Standard_, CCSDS 232.1-B-2,
 *    September, 2010
 *   -_TC Space Data Link Protocol Recommended Standard_, CCSDS 232.0-B-2, September,
 *    2010
 *
 * Notes:
 *   -The COP1_ProcessFrame function is the main entry point for handling Telecommand
 *    Transfer Frames (TF).  A 'Higher Procedure', i.e. a function which calls
 *    COP1_ProcessFrame, shall indicate the availability of a buffer large enough
 *    to hold the TF by passing in a non-null destination buffer.  To signal that
 *    there is not enough space available at the moment, the calling function
 *    shall pass a NULL destination buffer pointer.  The Communication Link
 *    Control Word (CLCW) will be updated to the 'Wait' state as necessary based
 *    on this signal mechanism.
 *
 *   -Many of the functions to process transfer frames reuse code from Morpheus
 *
 *   -The use of 'boolean' values for setters/getters assumes the underlying value
 *    for TRUE = 1, and the underlying value for FALSE = 0.  If this were not so,
 *    the CLCW flags would not be set or returned with the expected values.
 *
 * History:
 *   03/16/2015  Alan Asp, Odyssey Space Research, LLC
 *    * Created
 *
 ******************************************************************************/


#include "cop1.h"
#include "io_lib_events.h"
#include "cfe.h"


typedef enum
{
    COP1_CMD_UNKNOWN = 0,
    COP1_CMD_UNLOCK  = 1,
    COP1_CMD_SETVR   = 2
} COP1_Commands;

typedef enum
{
    COP1_SEQ_UNKNOWN  = 0,
    COP1_SEQ_PASS     = 1,
    COP1_SEQ_NEGATIVE = 2,
    COP1_SEQ_POSITIVE = 3,
    COP1_SEQ_LOCKOUT  = 4
} COP1_SequenceValidation;


static const uint16 COP1_CLCW_SIZE        = 4; /* number of octets for the CLCW */
static const uint16 COP1_TF_CMDSEQ_OFFSET = 7; /* octet offset in a BC transfer frame to
                                                  the commanded sequence number */
static const uint16 COP1_COP_IN_EFFECT    = 1; /* value to set the COP in Effect field for COP1 */


/*------------------------------------------------------------------------------
 *
 * Macros for reading and writing the fields in a Communications Link Control
 * Word (CLCW).
 * All of the macros are used in a similar way:
 *
 *   COP1_RD_CLCW_xxx(clcw)        -- Read field xxx from CLCW.
 *   COP1_WR_CLCW_xxx(clcw,value)  -- Write value to field xxx of CLCW.
 *
 * Note that clcw is a reference to the actual CLCW structure,
 * not to a pointer to the structure.  If using a pointer, one must
 * refer to the structure as *pointer.
 *
 * The COP1_WR macros may refer to the 'clcw' more than once; thus
 * the expression for 'clcw' must NOT contain any side effects.
 *
 *----------------------------------------------------------------------------*/
#define COP1_RD_CLCW_CTRLWORDTYPE(clcw)      ((clcw).Status >> 7)
#define COP1_WR_CLCW_CTRLWORDTYPE(clcw,val)  ((clcw).Status = ((clcw).Status & 0x7F) | \
                                                              ((val) << 7))

#define COP1_RD_CLCW_VERSION(clcw)           (((clcw).Status & 0x60) >> 5)
#define COP1_WR_CLCW_VERSION(clcw,val)       ((clcw).Status = ((clcw).Status & 0x9F) | \
                                                              (((val) << 5) & 0x60))

#define COP1_RD_CLCW_STATUS(clcw)            (((clcw).Status & 0x1C) >> 2)
#define COP1_WR_CLCW_STATUS(clcw,val)        ((clcw).Status = ((clcw).Status & 0xE3) | \
                                                              (((val) << 2) & 0x1C))

#define COP1_RD_CLCW_COPEFFECT(clcw)         ((clcw).Status & 0x03)
#define COP1_WR_CLCW_COPEFFECT(clcw,val)     ((clcw).Status = ((clcw).Status & 0xFC) | \
                                                              ((val) & 0x03))

#define COP1_RD_CLCW_VC(clcw)                (((clcw).Channel & 0xFC) >> 2)
#define COP1_WR_CLCW_VC(clcw,val)            ((clcw).Channel = ((clcw).Channel & 0x03) | \
                                                               ((val) << 2))

#define COP1_RD_CLCW_NORF_FLG(clcw)          ((clcw).Flags >> 7)
#define COP1_WR_CLCW_NORF_FLG(clcw,val)      ((clcw).Flags = ((clcw).Flags & 0x7F) | \
                                                             ((val) << 7))

#define COP1_RD_CLCW_NOBITLOCK_FLG(clcw)     (((clcw).Flags & 0x40) >> 6)
#define COP1_WR_CLCW_NOBITLOCK_FLG(clcw,val) ((clcw).Flags = ((clcw).Flags & 0xBF) | \
                                                             (((val) & 0x01) << 6))

#define COP1_RD_CLCW_LOCKOUT_FLG(clcw)       (((clcw).Flags & 0x20) >> 5)
#define COP1_WR_CLCW_LOCKOUT_FLG(clcw,val)   ((clcw).Flags = ((clcw).Flags & 0xDF) | \
                                                             (((val) & 0x01) << 5))

#define COP1_RD_CLCW_WAIT_FLG(clcw)          (((clcw).Flags & 0x10) >> 4)
#define COP1_WR_CLCW_WAIT_FLG(clcw,val)      ((clcw).Flags = ((clcw).Flags & 0xEF) | \
                                                             (((val) & 0x01) << 4))

#define COP1_RD_CLCW_RETRAN_FLG(clcw)        (((clcw).Flags & 0x08) >> 3)
#define COP1_WR_CLCW_RETRAN_FLG(clcw,val)    ((clcw).Flags = ((clcw).Flags & 0xF7) | \
                                                             (((val) & 0x01) << 3))

#define COP1_RD_CLCW_FARMB_CTR(clcw)         (((clcw).Flags >> 1) & 0x03)
#define COP1_WR_CLCW_FARMB_CTR(clcw,val)     ((clcw).Flags = ((clcw).Flags & 0xF9) | \
                                                             (((val) & 0x03) << 1))

#define COP1_INCR_CLCW_FARMB_CTR(clcw)       ((clcw).Flags = ((clcw).Flags & 0xF9) | \
                                                             (((clcw).Flags + 0x02) & 0x06))


/*------------------------------------------------------------------------------
 *
 * Prototypes for internal functions
 *
 *----------------------------------------------------------------------------*/
static int32    COP1_BypassTf(TCTF_Hdr_t *tfPtr, COP1_Clcw_t *clcwPtr, uint8 *toBuffer,
                              TCTF_ChannelService_t *channelService);
static int32    COP1_AcceptTf(TCTF_Hdr_t *tfPtr, COP1_Clcw_t *clcwPtr, uint8 *toBuffer,
                             TCTF_ChannelService_t *channelService);
static uint16  COP1_GetTfCommand(TCTF_Hdr_t *tfPtr);
static uint8   COP1_GetTfCommandedVr(TCTF_Hdr_t *tfPtr);
static uint16  COP1_CheckTfSequence(uint8 seqNum, uint8 expSeqNum);
static boolean COP1_isInWrappedRange(uint8 lower, uint8 value, uint8 upper);



/*
 * Function: COP1_InitClcw
 *
 * Purpose:
 *   Initialize the CLCW
 *
 * Arguments:
 *   clcwPtr     : pointer to the CLCW
 *   vcId        : the value of the virtual channel this CLCW reports on
 *
 * Return:
 *   void
 *
 * Notes:
 *  -Zeroing out the CLCW initially sets several values to their static defaults:
 *     - Control Word Type (always 0)
 *     - CLCW Version Number (b'00')
 *
 */
int32 COP1_InitClcw(COP1_Clcw_t *clcwPtr, uint16 vcId)
{
    if (clcwPtr == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_COP1_EID, CFE_EVS_ERROR,
                          "COP1 Error: NULL clcw input.");

        return COP1_BADINPUT_ERR;
    }

    CFE_PSP_MemSet(clcwPtr, 0, COP1_CLCW_SIZE);

    COP1_WR_CLCW_COPEFFECT(*clcwPtr, COP1_COP_IN_EFFECT);

    COP1_WR_CLCW_VC(*clcwPtr, vcId);

    return COP1_SUCCESS;
}


/*
 * Function: COP1_GetClcwCtrlWordType
 *
 * Purpose:
 *   Return the CLCW control word type
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *
 * Return:
 *   The control word type
 *
 * Notes:
 *
 */
uint16 COP1_GetClcwCtrlWordType(COP1_Clcw_t *clcwPtr)
{
    if (clcwPtr == NULL)
    {
        return 0;
    }

    return COP1_RD_CLCW_CTRLWORDTYPE(*clcwPtr);
}


/*
 * Function: COP1_GetClcwVersion
 *
 * Purpose:
 *   Return the CLCW version
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *
 * Return:
 *   The version
 *
 * Notes:
 *
 */
uint16 COP1_GetClcwVersion(COP1_Clcw_t *clcwPtr)
{
    if (clcwPtr == NULL)
    {
        return 0;
    }

    return COP1_RD_CLCW_VERSION(*clcwPtr);
}


/*
 * Function: COP1_SetClcwStatus
 *
 * Purpose:
 *   Set the CLCW status
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *   value  : the value to set the status
 *
 * Return:
 *   void
 *
 * Notes:
 *
 */
void COP1_SetClcwStatus(COP1_Clcw_t *clcwPtr, uint16 value)
{
    if (clcwPtr == NULL)
    {
        return;
    }

    COP1_WR_CLCW_STATUS(*clcwPtr, value);
}


/*
 * Function: COP1_GetClcwStatus
 *
 * Purpose:
 *   Return the CLCW status
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *
 * Return:
 *   The status
 *
 * Notes:
 *
 */
uint16 COP1_GetClcwStatus(COP1_Clcw_t *clcwPtr)
{
    if (clcwPtr == NULL)
    {
        return 0;
    }

    return COP1_RD_CLCW_STATUS(*clcwPtr);
}


/*
 * Function: COP1_GetClcwCopEffect
 *
 * Purpose:
 *   Return the CLCW COP in Effect field
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *
 * Return:
 *   The COP in Effect value
 *
 * Notes:
 *
 */
uint16 COP1_GetClcwCopEffect(COP1_Clcw_t *clcwPtr)
{
    if (clcwPtr == NULL)
    {
        return 0;
    }

    return COP1_RD_CLCW_COPEFFECT(*clcwPtr);
}


/*
 * Function: COP1_GetClcwVcId
 *
 * Purpose:
 *   Return the CLCW virtual channel ID
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *
 * Return:
 *   The virtual channel ID
 *
 * Notes:
 *
 */
uint16 COP1_GetClcwVcId(COP1_Clcw_t *clcwPtr)
{
    if (clcwPtr == NULL)
    {
        return 0;
    }

    return COP1_RD_CLCW_VC(*clcwPtr);
}


/*
 * Function: COP1_SetClcwNoRf
 *
 * Purpose:
 *   Set the 'no radio frequency available' flag
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *   value  : the value to set the 'no radio frequency available flag'
 *
 * Return:
 *   void
 *
 * Notes:
 *
 */
void COP1_SetClcwNoRf(COP1_Clcw_t *clcwPtr, boolean value)
{
    if (clcwPtr == NULL)
    {
        return;
    }

    COP1_WR_CLCW_NORF_FLG(*clcwPtr, value);
}


/*
 * Function: COP1_GetClcwNoRf
 *
 * Purpose:
 *   Return the CLCW 'no radio frequency available' flag
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *
 * Return:
 *   The 'no radio frequency available' flag
 *
 * Notes:
 *
 */
boolean COP1_GetClcwNoRf(COP1_Clcw_t *clcwPtr)
{
    if (clcwPtr == NULL)
    {
        return 0;
    }

    return COP1_RD_CLCW_NORF_FLG(*clcwPtr);
}


/*
 * Function: COP1_SetClcwNoBitlock
 *
 * Purpose:
 *   Set the 'no bitlock' flag
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *   value  : the value to set the 'no bitlock' flag
 *
 * Return:
 *   void
 *
 * Notes:
 *
 */
void COP1_SetClcwNoBitlock(COP1_Clcw_t *clcwPtr, boolean value)
{
    if (clcwPtr == NULL)
    {
        return;
    }

    COP1_WR_CLCW_NOBITLOCK_FLG(*clcwPtr, value);
}


/*
 * Function: COP1_GetClcwNoBitlock
 *
 * Purpose:
 *   Return the CLCW 'no bitlock' flag
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *
 * Return:
 *   The 'no bitlock' flag
 *
 * Notes:
 *
 */
boolean COP1_GetClcwNoBitlock(COP1_Clcw_t *clcwPtr)
{
    if (clcwPtr == NULL)
    {
        return 0;
    }

    return COP1_RD_CLCW_NOBITLOCK_FLG(*clcwPtr);
}


/*
 * Function: COP1_GetClcwLockout
 *
 * Purpose:
 *   Return the CLCW lockout flag
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *
 * Return:
 *   The lockout flag
 *
 * Notes:
 *
 */
boolean COP1_GetClcwLockout(COP1_Clcw_t *clcwPtr)
{
    if (clcwPtr == NULL)
    {
        return 0;
    }

    return COP1_RD_CLCW_LOCKOUT_FLG(*clcwPtr);
}


/*
 * Function: COP1_GetClcwWait
 *
 * Purpose:
 *   Return the CLCW wait flag
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *
 * Return:
 *   The wait flag
 *
 * Notes:
 *
 */
boolean COP1_GetClcwWait(COP1_Clcw_t *clcwPtr)
{
    if (clcwPtr == NULL)
    {
        return 0;
    }

    return COP1_RD_CLCW_WAIT_FLG(*clcwPtr);
}


/*
 * Function: COP1_GetClcwRetransmit
 *
 * Purpose:
 *   Return the CLCW retransmit flag
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *
 * Return:
 *   The retransmit flag
 *
 * Notes:
 *
 */
boolean COP1_GetClcwRetransmit(COP1_Clcw_t *clcwPtr)
{
    if (clcwPtr == NULL)
    {
        return 0;
    }

    return COP1_RD_CLCW_RETRAN_FLG(*clcwPtr);
}


/*
 * Function: COP1_GetClcwFarmbCtr
 *
 * Purpose:
 *   Return the CLCW FARM-B counter
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *
 * Return:
 *   The FARM-B counter
 *
 * Notes:
 *
 */
uint16 COP1_GetClcwFarmbCtr(COP1_Clcw_t *clcwPtr)
{
    if (clcwPtr == NULL)
    {
        return 0;
    }

    return COP1_RD_CLCW_FARMB_CTR(*clcwPtr);
}


/*
 * Function: COP1_GetClcwReport
 *
 * Purpose:
 *   Return the report value
 *
 * Arguments:
 *   clcwPtr: pointer to the CLCW
 *
 * Return:
 *   The report value
 *
 * Notes:
 *
 */
uint8 COP1_GetClcwReport(COP1_Clcw_t *clcwPtr)
{
    if (clcwPtr == NULL)
    {
        return 0;
    }

    return clcwPtr->Report;
}


/*
 * Function: COP1_ProcessFrame
 *
 * Purpose:
 *   Primary access point for processing a transfer frame.  Verifies the TF and
 *   either responds to valid TF command frames or copies the data unit to a
 *   destination buffer.
 *
 * Arguments:
 *   toBuffer      : pointer to the destination buffer (Out)
 *   clcwPtr       : pointer to the CLCW (In/Out)
 *   tfPtr         : pointer to the transfer frame (In)
 *   channelService: pointer to the identifiers and service type for the expected TF (In)
 *
 * Return:
 *   number of bytes copied to toBuffer or an error code
 *
 * Notes:
 *
 */
int32 COP1_ProcessFrame(uint8* toBuffer, COP1_Clcw_t *clcwPtr, TCTF_Hdr_t *tfPtr,
                        TCTF_ChannelService_t *channelService)
{
    int32 retVal = 0;

    if (tfPtr == NULL || clcwPtr == NULL)
    {
        CFE_EVS_SendEvent(IO_LIB_COP1_EID, CFE_EVS_ERROR,
                          "COP1 Error: COP1_ProcessFrame() Bad input.");
        return COP1_BADINPUT_ERR;
    }

    /* Process frame if this is the correct destination service */
    if (TRUE == TCTF_IsValidTf(tfPtr, channelService))
    {
        if (TCTF_GetBypassFlag(tfPtr))
        {
            /* Process the bypass command */
            retVal = COP1_BypassTf(tfPtr, clcwPtr, toBuffer, channelService);
        }
        else
        {
            /* Process the Accept TF. */
            retVal = COP1_AcceptTf(tfPtr, clcwPtr, toBuffer, channelService);
        }
    }
    else
    {
        CFE_EVS_SendEvent(IO_LIB_COP1_EID, CFE_EVS_INFORMATION,
                          "COP1 Info: Received invalid transfer frame.");
        retVal = COP1_INVALID_TF_ERR;
    }

    return retVal;
}


/*
 * Function: COP1_BypassTf
 *
 * Purpose:
 *   Process the bypass frame
 *
 * Arguments:
 *   tfPtr         : pointer to the transfer frame
 *   clcwPtr       : pointer to the CLCW
 *   toBuffer      : pointer to the destination buffer for the transfer frame data unit
 *   channelService: pointer to the identifiers and service type for the expected TF
 *
 * Return:
 *   number of bytes copied to toBuffer or an error code
 *
 * Notes:
 *  -As an internal function, assumes the calling function checked for null TF pointer
 *  -Ref CCSDS 232.1-B-2, Table 6-1: FARM-1 State Table
 *
 */
static int32 COP1_BypassTf(TCTF_Hdr_t *tfPtr, COP1_Clcw_t *clcwPtr, uint8 *toBuffer,
                           TCTF_ChannelService_t *channelService)
{
    uint16 command = 0;
    int32 retVal   = 0;

    if (TCTF_CONTROL_FRAME == TCTF_GetCtlCmdFlag(tfPtr))
    {
        command = COP1_GetTfCommand(tfPtr);
        if (COP1_CMD_UNLOCK == command)
        {
            /* Unlock the channel */
            COP1_WR_CLCW_RETRAN_FLG(*clcwPtr, 0);
            COP1_WR_CLCW_WAIT_FLG(*clcwPtr, 0);
            COP1_WR_CLCW_LOCKOUT_FLG(*clcwPtr, 0);

            COP1_INCR_CLCW_FARMB_CTR(*clcwPtr);

            CFE_EVS_SendEvent(IO_LIB_COP1_EID, CFE_EVS_INFORMATION,
                              "COP1 Info: Cmd Transfer Frame UNLOCKED.");
        }
        else if (COP1_CMD_SETVR == command)
        {
            /* Set next expected frame number (FARM-1 must be 'unlocked' first) */
            if (0 == COP1_RD_CLCW_LOCKOUT_FLG(*clcwPtr))
            {
                COP1_WR_CLCW_RETRAN_FLG(*clcwPtr, 0);
                COP1_WR_CLCW_WAIT_FLG(*clcwPtr, 0);
                clcwPtr->Report = COP1_GetTfCommandedVr(tfPtr);

                CFE_EVS_SendEvent(IO_LIB_COP1_EID, CFE_EVS_INFORMATION,
                                  "COP1 Info: Cmd Transfer Frame counter set (SETVR).");
            }
            else /* Invalid Control Command Frame, discard and do nothing */
            {
                CFE_EVS_SendEvent(IO_LIB_COP1_EID, CFE_EVS_ERROR,
                                  "COP1 Error: FARM-1 must be unlocked "
                                  "prior to SETVR");
                retVal = COP1_FARM1_ERR;
            }

            COP1_INCR_CLCW_FARMB_CTR(*clcwPtr);
        }
        else /* An unrecognized command */
        {
            CFE_EVS_SendEvent(IO_LIB_COP1_EID, CFE_EVS_ERROR,
                    "Invalid Bypass Control Command.");
            retVal = COP1_FARM1_ERR;
        }
    }
    else  /* Data frame */
    {
        COP1_INCR_CLCW_FARMB_CTR(*clcwPtr);
        retVal = (int32) TCTF_CopyData(toBuffer, tfPtr, channelService);
    }

    return retVal;
}


/*
 * Function: COP1_AcceptTf
 *
 * Purpose:
 *   Process the accept frame
 *
 * Arguments:
 *   tfPtr         : pointer to the transfer frame
 *   clcwPtr       : pointer to the CLCW
 *   toBuffer      : pointer to the destination buffer for the transfer frame data unit
 *   channelService: pointer to the identifiers and service type for the expected TF
 *
 * Return:
 *   number of bytes copied to toBuffer or an error code
 *
 * Notes:
 *  -As an internal function, assumes the calling function checked for a null TF pointer
 *  -Ref CCSDS 232.1-B-2, Table 6-1: FARM-1 State Table.  Implementation varies slightly
 *   for event number E3, as the retransmit flag is set regardless of 'Wait' state.
 *  -To signal a lack of buffer space, the 'Higher Procedures' must transmit a NULL
 *   destination buffer.
 *  -Assumes the input buffer to copy the transfer frame data to can handle the maximum
 *   transfer frame payload size.
 *
 */
static int32 COP1_AcceptTf(TCTF_Hdr_t *tfPtr, COP1_Clcw_t *clcwPtr, uint8 *toBuffer,
                          TCTF_ChannelService_t *channelService)
{
    uint8  seqNum    = tfPtr->Sequence;
    uint8  expSeqNum = clcwPtr->Report;
    uint16 sequence  = COP1_SEQ_UNKNOWN;
    int32  retVal    = 0;

    /* Verify the VC is not in lockout mode */
    if (0 == COP1_RD_CLCW_LOCKOUT_FLG(*clcwPtr))
    {
        /* Perform the sequence validation */
        sequence  = COP1_CheckTfSequence(seqNum, expSeqNum);

        switch (sequence)
        {
            case COP1_SEQ_PASS:
                if (toBuffer != NULL)
                {
                    /* Success case */
                    /* Clear 'Wait' state, retransmit flag, update clcw expected seq. number */
                    COP1_WR_CLCW_WAIT_FLG(*clcwPtr, 0);
                    COP1_WR_CLCW_RETRAN_FLG(*clcwPtr, 0);
                    clcwPtr->Report++;
                    retVal = (int32) TCTF_CopyData(toBuffer, tfPtr, channelService);
                }
                else
                {
                    /* No buffer available, enter the 'Wait' state */
                    COP1_WR_CLCW_WAIT_FLG(*clcwPtr, 1);
                    COP1_WR_CLCW_RETRAN_FLG(*clcwPtr, 1);
                    retVal = COP1_BADINPUT_ERR;
                }

                break;

            case COP1_SEQ_POSITIVE:
                /* Inside the positive part of the sliding window, retransmit expected frame */
                COP1_WR_CLCW_RETRAN_FLG(*clcwPtr, 1);
                CFE_EVS_SendEvent(IO_LIB_COP1_EID, CFE_EVS_ERROR,
                                  "COP1 Error: Inside Trans Frame Pos Window Edge");
                retVal = COP1_FARM1_ERR;
                break;

            case COP1_SEQ_NEGATIVE:
                /* Inside the negative edge of the sliding window, drop the frame */
                CFE_EVS_SendEvent(IO_LIB_COP1_EID, CFE_EVS_ERROR,
                                  "COP1 Error: Inside Trans Frame Neg Window Edge");
                retVal = COP1_FARM1_ERR;
                break;

            case COP1_SEQ_LOCKOUT:
                /* Outside of the sliding window, enter 'Lockout' state */
                COP1_WR_CLCW_LOCKOUT_FLG(*clcwPtr, 1);
                CFE_EVS_SendEvent(IO_LIB_COP1_EID, CFE_EVS_ERROR,
                                  "COP1 Error: Transfer Frame Lockout Mode Entered.");
                retVal = COP1_FARM1_ERR;
                break;

            default:
                break;
        }
    }
    else
    {
        /* System is in lockout mode  */
        CFE_EVS_SendEvent(IO_LIB_COP1_EID, CFE_EVS_ERROR,
                          "COP1 Error: Cmd Transfer Frame Rejected, In Lockout Mode.");
        retVal = COP1_FARM1_ERR;
    }

    return retVal;
}


/*
 * Function: COP1_GetTfCommand
 *
 * Purpose:
 *   Return the command value of the transfer frame
 *
 * Arguments:
 *   tfPtr  : pointer to the transfer frame
 *
 * Return:
 *   An enumerated value representing the command
 *
 * Notes:
 *  -As an internal function, assumes the calling function checked for a null pointer
 *  -Command bit patterns from CCSDS 232.0-B-2
 *
 */
static uint16 COP1_GetTfCommand(TCTF_Hdr_t *tfPtr)
{
    uint8 *bytePtr = (uint8 *)tfPtr;
    uint16 command = COP1_CMD_UNKNOWN;
    uint16 offset  = TCTF_PRIHDR_SIZE;

    if (bytePtr[offset] == 0x00)
    {
        command = COP1_CMD_UNLOCK;
    }

    if (bytePtr[offset]   == 0x82 &&
        bytePtr[offset+1] == 0x00)
    {
        command = COP1_CMD_SETVR;
    }

    return command;
}


/*
 * Function: COP1_GetTfCommandedVr
 *
 * Purpose:
 *   Return the commanded receiver frame sequence number of the next transfer frame
 *
 * Arguments:
 *   tfPtr  : pointer to the transfer frame
 *
 * Return:
 *   The commanded receiver frame sequence number
 *
 * Notes:
 *  -As an internal function, assumes the calling function checked for a null pointer
 *  -For a transfer frame with control command, there is no segment header (offset from
 *   start of frame is constant)
 *
 */
static uint8 COP1_GetTfCommandedVr(TCTF_Hdr_t *tfPtr)
{
    uint8 *bytePtr = (uint8 *)tfPtr;
    uint16 offset  = COP1_TF_CMDSEQ_OFFSET;

    return bytePtr[offset];
}

/*
 * Function: COP1_CheckTfSequence
 *
 * Purpose:
 *   Checks the COP-1 transfer frame sequence number against the expected value.
 *
 * Arguments:
 *   seqNum    : sequence number of the transfer frame, a.k.a. N(S)
 *   expSeqNum : expected sequence number of the transfer frame, a.k.a. V(R)
 *
 * Returns:
 *   Enumeration value indicating whether the sequence number is correct, inside the
 *   positive edge of the sliding window, inside the negative edge of the sliding window,
 *   or outside the sliding window and thus in the lockout area.
 *
 * Notes:
 *  -The COP1_isInWrappedRange checks both bounds, inclusive, so a check for equality
 *   with the expected value needs to be performed first
 *
 */
static uint16 COP1_CheckTfSequence(uint8 seqNum, uint8 expSeqNum)
{
    const uint8 HALF_SLIDING_WINDOW_WIDTH = (uint8)(COP1_SLIDING_WINDOW_WIDTH/2);

    uint16 status        = COP1_SEQ_UNKNOWN;
    uint8  posWindowEdge = expSeqNum + HALF_SLIDING_WINDOW_WIDTH - 1;
    uint8  negWindowEdge = expSeqNum - HALF_SLIDING_WINDOW_WIDTH;

    if (seqNum == expSeqNum)
    {
        status = COP1_SEQ_PASS;
    }
    /* Positive window */
    else if (COP1_isInWrappedRange(expSeqNum, seqNum, posWindowEdge))
    {
        status = COP1_SEQ_POSITIVE;
    }
    /* Negative window */
    else if (COP1_isInWrappedRange(negWindowEdge, seqNum, expSeqNum))
    {
        status = COP1_SEQ_NEGATIVE;
    }
    else
    {
        status = COP1_SEQ_LOCKOUT;
    }

    return status;
}


/*
 * Function: COP1_isInWrappedRange
 *
 * Purpose:
 *   Checks whether 'value' is between 'lower' and 'upper' given integer wrapping
 *
 * Arguments:
 *   lower    : lower bounds, inclusive
 *   value    : value being checked if in range
 *   upper    : upper bounds, inclusive
 *
 * Returns:
 *   True/false value indicating whether 'value' is between 'lower' and 'upper'
 *
 * Notes:
 *
 */
static boolean COP1_isInWrappedRange(uint8 lower, uint8 value, uint8 upper)
{
    boolean result = FALSE;

    if (upper < lower)
    {
        result =  (lower <= value) || (value <= upper);
    }
    else
    {
        result = (lower <= value) && (value <= upper);
    }

    return result;
}


