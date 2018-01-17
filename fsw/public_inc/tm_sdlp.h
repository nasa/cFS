/******************************************************************************/
/** \file  tm_sdlp.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
*   \author Alan A Asp, Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Header file for TM_SDLP Protocol
*
*   \par Limitations, Assumptions, External Events, and Notes:
*       - This library provides a service interface to the TMTF protocol.
*       - This implementation is based on Chapter 4 - without SDLS option.
*       - The VCA service is user defined
*       - The maximum frame length is user defined, and is dependent on the
*         channel coding startegy used.
*       - User may use the IO_LIB_UTIL_GenPseudoRandomSeq to generate an idle 
*         data sequence.
*
*   \par Modification History:
*     - 2015-04-26 | Alan A. Asp | OSR | Code Started (originally in tmtf.h)
*     - 2015-10-22 | Guy de Carufel | OSR | Migrated from tmtf.h. 
*           Major revision: structs, idle data, overflow, API.
*******************************************************************************/

#ifndef _TM_SDLP_H_
#define _TM_SDLP_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
** Includes
*******************************************************************************/
#include "io_lib.h"
#include "tmtf.h"


/*******************************************************************************
** Macro Definitions
*******************************************************************************/
#define TM_SDLP_SUCCESS                (0)
#define TM_SDLP_ERROR                 (-1)
#define TM_SDLP_INVALID_POINTER       (-2)
#define TM_SDLP_INVALID_LENGTH        (-3)
#define TM_SDLP_FRAME_NOT_INIT        (-4)
#define TM_SDLP_FRAME_NOT_READY       (-5)
#define TM_SDLP_OVERFLOW_FULL         (-6)

/*******************************************************************************
** Structure definitions
*******************************************************************************/
/** Following Structure is the user defined managed / configuration parameters 
 *  for all Transfer Frames over the physical channel. */
typedef struct
{
    uint16  scId;                /* Spacecraft ID (10 bits)                   */
    uint16  frameLength;         /* The length of the frame                   */
    uint8   hasErrCtrl;          /* Has the Error Control Field               */
} TM_SDLP_GlobalConfig_t;
    

/** Following Structure is the user defined managed /configuration parameters  
    for a a specific Channel (Master or virtual) */
typedef struct
{
    uint8   vcId;                /* Virtual channel ID (3 bits)               */
    uint8   dataType;            /* Type-0: VCP_SDU(packet), Type-1: VCA_SDU  */
    uint8   fshFlag;             /* The Sec Hdr flag                          */
    uint8   ocfFlag;             /* The value of the OCF flag (0/1)           */
    uint8   secHdrLength;        /* length of the secondary header data (1-63)*/
    uint8   isMaster;            /* Master Channel:1, Virtual Channel:0       */
    uint16  overflowSize;        /* Size of overflow buffer                   */
} TM_SDLP_ChannelConfig_t;


/** Working parameters for overflow buffer */
typedef struct
{
    uint16  buffSize;            /* Overflow buffer size                      */
    uint16  freeOctets;          /* number of free octets remaining in buffer */
    uint16  partialOctets;       /* Partial octets at the start of the 
                                    overflow queue                            */
    uint8 *dataStart;            /* Pointer to start of data (queue start)    */
    uint8 *dataEnd;              /* Pointer to end of data (queue end)        */
    uint8 *buffer;               /* Pointer to overflow buffer                */
} TM_SDLP_OverflowInfo_t;


/** Working paramters of frame */
typedef struct
{
    uint16  dataFieldLength;     /* Length of the data field                  */
    uint16  dataFieldOffset;     /* offset in octets from the start of frame 
                                    to the data field                         */
    uint16  ocfOffset;           /* Offset in octets from the start of frame 
                                    to the OCF                                */
    uint16  errCtrlOffset;       /* Offset in octets from the start of frame 
                                    to the Err Ctrl field                     */
    uint16  freeOctets;          /* Number of free octets remaining in the 
                                    data field                                */
    uint16  currentDataOffset;   /* Offset to next free data field octet from 
                                    start of frame                            */
    uint32  mutexId;             /* The mutex ID to protect the TF buffer 
                                    and overflow buffer                       */
    boolean isFirstHdrPtrSet;    /* Indicates if the TF first header pointer 
                                    has been set                              */
    boolean isReady;             /* Indicates the TF is ready to add data     */
    boolean isInitialized;       /* Indicates the TF is initialized           */
    TM_SDLP_OverflowInfo_t  overflowInfo;   /* Overflow Info Structure        */
    TM_SDLP_GlobalConfig_t  *globConfig;    /* Pointer to global config       */
    TM_SDLP_ChannelConfig_t *chnlConfig;    /* Pointer to channel config      */
    TMTF_PriHdr_t           *frame;         /* Pointer to Transfer frame      */
} TM_SDLP_FrameInfo_t;



/*******************************************************************************
** Function Declarations
*******************************************************************************/
/******************************************************************************/
/** \brief Initialize the Idle Packet
*
*   \par Description/Algorithm
*       Initializes an Idle Data Buffer with a repeating pattern sequence. 
*
*   \par Assumptions, External Events, and Notes:
*       - The Idle Packet is of type CFE_SB_Msg_t with a CCSDS Primary Hdr.
*       - The MID of the Idle Buffer Message is set to a SSP Idle Packet (0x3ff)
*       - The message has no secondary header.
*       - User may use the IO_LIB_UTIL_GenPseudoRandomSeq function to generate 
*         idle data pattern. Pattern should be "sufficiently" random.
*       - The Idle packet buffer length must be at least as large as the 
*         frameLength.
*       - The IdlePacket is used in both AddIdlePacket and SetOidFrame
*       - The Length parameter in the CCSDS HDR is not constant.
*
*   \param[in,out] pIdlePacket      Pointer to the Idle Buffer with CCSDS HDR.
*   \param[in]     pIdlePattern     A bit pattern to repeat in idle data
*   \param[in]     bufferLength     Length of the Idle Buffer in bytes.
*   \param[in]     patternBitLength Length of the repeating pattern in bits.
*
*   \return TM_SDLP_SUCCESS             If successful.
*   \return TM_SDLP_INVALID_POINTER     If a input pointer is NULL
*   \return TM_SDLP_INVALID_LENGTH      If an input length is invalid
*
*   \see 
*       #TM_SDLP_AddData
*       #TM_SDLP_AddIdlePacket
*       #TM_SDLP_SetOidFrame
*       #IO_LIB_UTIL_GenPseudoRandomSeq
*******************************************************************************/
int32 TM_SDLP_InitIdlePacket(CFE_SB_Msg_t *pIdlePacket, uint8 *pIdlePattern,
                             uint16 bufferLength, uint32 patternBitLength);

/******************************************************************************/
/** \brief Initialize a specific Channel
*
*   \par Description/Algorithm
*       This function will populate the Channel and overflow info structures 
*       based on provided configuration data and buffer pointers.
*
*   \par Assumptions, External Events, and Notes:
*       - User is responsible for setting the Packet order flag (1 bit),
*         The Segment Length (2 bits) and TF First Header Pointer Field if the
*         VCA_SDU data type is used [TM_SDLP 4.1.2.7]. Set these after calling
*         this function.
*       - The idle data sequence must be >= as frame data field length.
*
*   \param[out] pFrameInfo     Pointer to the Frame info/working struct.
*   \param[out] pTfBuffer      Pointer to the Transfer Frame buffer 
*   \param[out] pOfBuffer      Pointer to the Overflow buffer
*   \param[in] pGlobalConfig   Pointer to the Global configuration struct.
*   \param[in] pChannelConfig  Pointer to the Channel configuration struct.
*
*   \return TM_SDLP_SUCCESS             If successful.
*   \return TM_SDLP_INVALID_POINTER     If a input pointer is NULL
*   \return TM_SDLP_INVALID_LENGTH      If frame length is too short
*
*   \see
*       #TMTF_SetPacketOrderFlag
*       #TMTF_SetSegmentLengthId
*       #TMTF_SetFirstHdrPtr
*******************************************************************************/
int32 TM_SDLP_InitChannel(TM_SDLP_FrameInfo_t *pFrameInfo, 
                          uint8 *pTfBuffer, uint8 *pOfBuffer,
                          TM_SDLP_GlobalConfig_t *pGlobalConfig, 
                          TM_SDLP_ChannelConfig_t *pChannelConfig);

                          
/******************************************************************************/
/** \brief Check if frame currently has data
*
*   \par Description/Algorithm
*       This function will return whether the frame has data or not
*
*   \par Assumptions, External Events, and Notes:
*       - Function does not check if frame has been initalized or started.
*
*   \param[in] pFrameInfo  Pointer to the Frame info/working struct.
*
*   \return Frame has Data (0=no, 1=yes)
*   \return TM_SDLP_INVALID_POINTER    If a input pointer is NULL
*
*   \see 
*       #TM_SDLP_AddData
*******************************************************************************/
int32 TM_SDLP_FrameHasData(TM_SDLP_FrameInfo_t *pFrameInfo);


/******************************************************************************/
/** \brief Add a Packet to Transfer Frame
*
*   \par Description/Algorithm
*       This function will add a CFE packet to a provided transfer frame.
*
*   \par Assumptions, External Events, and Notes:
*       - A CFE packet is a Virtual Channel Packet Proto. Data Unit (VCP-PDU)
*       - This function calls TM_SDLP_AddData
*       - Multiversion multiplexing is not-implemented [4.2.2.3]
*
*   \param[in,out] pFrameInfo  Pointer to the Frame info/working struct.
*   \param[in]     pPacket  Pointer to the CFE Packet (VCPDU)
*
*   \return Frame FreeOctets
*   \return TM_SDLP_INVALID_POINTER    If a input pointer is NULL
*   \return TM_SDLP_FRAME_NOT_INIT     If frame has not been initialized
*   \return TM_SDLP_FRAME_NOT_READY    If frame has not been started
*   \return TM_SDLP_OVERFLOW_FULL      Data dropped. The overflow buffer is full
*
*   \see 
*       #TM_SDLP_AddData
*******************************************************************************/
int32 TM_SDLP_AddPacket(TM_SDLP_FrameInfo_t *pFrameInfo,
                        CFE_SB_Msg_t *pPacket);
                           


/******************************************************************************/
/** \brief Add an Idle packet to transfer frame
*
*   \par Description/Algorithm
*       Copies an idle packet as specified in Space Packet Protocol to fill all
*       free octets in TF data field. The data of the idle packet is based on 
*       the supplied idle pattern.
*
*   \par Assumptions, External Events, and Notes:
*       - The Idle packet may be segmented if the TF data field does not have 
*         enough space left by saving the extra octets to the overflow buffer.
*       - The minium idle packet length is 7 Octets (6 hdr, 1 data).
*       - This function calls TM_SDLP_AddData
*       - The supplied Packet must have the IdlePacket MID.
*       - User may use InitIdlePacket to initialize the Idle Data with a 
*         user specified repeating pattern.
*
*   \param[in,out] pFrameInfo    Pointer to the Frame info/working struct.
*   \param[in]     pIdlePacket   Pointer to the idle Buffer
*
*   \return TM_SDLP_SUCCESS            If successful (no free octets).
*   \return TM_SDLP_INVALID_POINTER    If a input pointer is NULL
*   \return TM_SDLP_FRAME_NOT_INIT     If frame has not been initialized
*   \return TM_SDLP_FRAME_NOT_READY    If frame has not been started
*   \return TM_SDLP_OVERFLOW_FULL      Data dropped. The overflow buffer is full
*
*   \see 
*       #TM_SDLP_AddData
*       #TM_SDLP_GenPseudoRandomSeq
*******************************************************************************/
int32 TM_SDLP_AddIdlePacket(TM_SDLP_FrameInfo_t *pFrameInfo,
                            CFE_SB_Msg_t *pIdlePacket);


/******************************************************************************/
/** \brief Add a Virtual Channel Access (VCA) PDU to the Transfer Frame
*
*   \par Description/Algorithm
*       Copies a VCA data buffer to the TF data field at the next free octet.
*
*   \par Assumptions, External Events, and Notes:
*       - The user is responsible for setting the FirstHdrPtr of the TF.
*       - The dataLength should be the same for all PDUs added to a specific TF
*
*   \param[in,out] pFrameInfo  Pointer to the Frame info/working struct.
*   \param[in]     pData       Pointer to data buffer
*   \param[in]     dataLength  Length of data to copy
*
*   \return Frame FreeOctets
*   \return TM_SDLP_INVALID_POINTER    If a input pointer is NULL
*   \return TM_SDLP_FRAME_NOT_INIT     If frame has not been initialized
*   \return TM_SDLP_FRAME_NOT_READY    If frame has not been started
*   \return TM_SDLP_OVERFLOW_FULL      Data dropped. The overflow buffer is full
*
*   \see
*       #TM_SDLP_AddData
*******************************************************************************/
int32 TM_SDLP_AddVcaData(TM_SDLP_FrameInfo_t *pFrameInfo, uint8 *pData, 
                         uint16 dataLength);


/******************************************************************************/
/** \brief Start a transfer frame  
*
*   \par Description/Algorithm
*       Start a new transfer frame by copying any data from the overflow buffer
*       into the empty transfer frame buffer. This readies the frame to accept
*       new data.
*
*   \par Assumptions, External Events, and Notes:
*       - The transfer frame has no data in it's data field prior to call. 
*
*   \param[in,out] pFrameInfo   Pointer to the Frame info/working struct.
*
*   \return TM_SDLP_SUCCESS             If successful.
*   \return TM_SDLP_INVALID_POINTER     If a input pointer is NULL
*   \return TM_SDLP_ERROR               Data field is not empty
*
*   \see 
*       #TM_SDLP_AddIdlePacket
*******************************************************************************/
int32 TM_SDLP_StartFrame(TM_SDLP_FrameInfo_t *pFrameInfo);


/******************************************************************************/
/** \brief Set a Frame with Only Idle Data (OID)
*
*   \par Description/Algorithm
*       Copies the Idle Pattern of an Only Idle Data (OID) frame into the 
*       frame data field and sets the first header pointer to the appropriate
*       value. 
*
*   \par Assumptions, External Events, and Notes:
*       - The user is responsible for providing an idle buffer with
*         sufficient randomness.
*       - The pIdlePacket is of type CFE_SB_Msg_t; Only the user data of 
*         the message is copied into the TF data field.
*       - It is recommended that a dedicated VC hold the OID frame.
*
*   \param[in,out] pFrameInfo    Pointer to the Frame info/working struct.
*   \param[in]     pIdlePacket   Pointer to the idle buffer.
*
*   \return TM_SDLP_SUCCESS             If successful.
*   \return TM_SDLP_INVALID_POINTER     If a input pointer is NULL
*
*   \see 
*******************************************************************************/
int32 TM_SDLP_SetOidFrame(TM_SDLP_FrameInfo_t *pFrameInfo,
                          CFE_SB_Msg_t *pIdlePacket);



/******************************************************************************/
/** \brief Complete a Frame to ready for transmission
*
*   \par Description/Algorithm
*       Fills out the final TF information include the frame counters, adds the
*       OCF if available, updates the first header pointer if not set, and
*       executes the error control field computation if included.
*
*   \par Assumptions, External Events, and Notes:
*       - This function represents the Virtual Channel Frame (VCF) Service or
*         the Master Channel Frame (MCF) Service, depending on the input
*         FrameInfo.
*       - User is responsible for filling frame with idle data if it is
*         incomplete prior to call. Call AddIdlePacket or SetOidFrame.
*
*   \param[in,out] pFrameInfo   Pointer to the Frame info/working struct.
*   \param[in,out] pMcFrameCnt  Pointer to Master Channel Frame Count.
*   \param[in]     pOcf         Pointer to Operational Control Field
*
*   \return TM_SDLP_SUCCESS             If successful.
*   \return TM_SDLP_INVALID_POINTER     If a input pointer is NULL
*
*   \see 
*       #TM_SDLP_AddIdlePacket
*       #TM_SDLP_SetOidFrame
*******************************************************************************/
int32 TM_SDLP_CompleteFrame(TM_SDLP_FrameInfo_t *pFrameInfo,
                            uint8 *pMcFrameCnt, uint8 *pOcf);


#ifdef __cplusplus
}
#endif

#endif /* _TM_SDLP_H_ */

/*==============================================================================
** End of file tm_sdlp.h
**============================================================================*/
