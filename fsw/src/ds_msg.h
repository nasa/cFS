/************************************************************************
** File:
**   $Id: ds_msg.h 1.4.1.1 2017/02/02 14:11:31EST mdeschu Exp  $
**
**  Copyright (c) 2007-2014 United States Government as represented by the 
**  Administrator of the National Aeronautics and Space Administration. 
**  All Other Rights Reserved.  
**
**  This software was created at NASA's Goddard Space Flight Center.
**  This software is governed by the NASA Open Source Agreement and may be 
**  used, distributed and modified only pursuant to the terms of that 
**  agreement.
**
** Purpose: 
**  The CFS Data Storage (DS) Application header file
**
** Notes:
**
*************************************************************************/
#ifndef _ds_msg_h_
#define _ds_msg_h_

#include "cfe.h"

#include "ds_platform_cfg.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS application command packet formats                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
**  \brief No-Operation Command
**
**  For command details see #DS_NOOP_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

} DS_NoopCmd_t;


/*
**  \brief Reset Housekeeping Telemetry Command
**
**  For command details see #DS_RESET_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

} DS_ResetCmd_t;


/*
**  \brief Set Ena/Dis State For DS Application
**
**  For command details see #DS_SET_APP_STATE_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

    uint16  EnableState;                        /**< \brief Application enable/disable state */
    uint16  Padding;                            /**< \brief Structure Padding on 32-bit boundaries */

} DS_AppStateCmd_t;


/*
**  \brief Set File Selection For Packet Filter Table Entry
**
**  For command details see #DS_SET_FILTER_FILE_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

    uint16  MessageID;                          /**< \brief Message ID of existing entry in Packet Filter Table
                                                     \details DS defines Message ID zero to be unused */
    uint16  FilterParmsIndex;                   /**< \brief Index into Filter Parms Array */
    uint16  FileTableIndex;                     /**< \brief Index into Destination File Table */
    uint16  Padding;                            /**< \brief Structure Padding on 32-bit boundaries */

} DS_FilterFileCmd_t;


/*
**  \brief Set Filter Type For Packet Filter Table Entry
**
**  For command details see #DS_SET_FILTER_TYPE_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

    uint16  MessageID;                          /**< \brief Message ID of existing entry in Packet Filter Table
                                                     \details DS defines Message ID zero to be unused */
    uint16  FilterParmsIndex;                   /**< \brief Index into Filter Parms Array */
    uint16  FilterType;                         /**< \brief Filter type (packet count or time) */
    uint16  Padding;                            /**< \brief Structure Padding on 32-bit boundaries */

} DS_FilterTypeCmd_t;


/*
**  \brief Set Filter Parameters For Packet Filter Table Entry
**
**  For command details see #DS_SET_FILTER_PARMS_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

    uint16  MessageID;                          /**< \brief Message ID of existing entry in Packet Filter Table
                                                     \details DS defines Message ID zero to be unused */
    uint16  FilterParmsIndex;                   /**< \brief Index into Filter Parms Array */

    uint16  Algorithm_N;                        /**< \brief Algorithm value N (pass this many) */
    uint16  Algorithm_X;                        /**< \brief Algorithm value X (out of this many) */
    uint16  Algorithm_O;                        /**< \brief Algorithm value O (at this offset) */
    uint16  Padding;                            /**< \brief Structure Padding on 32-bit boundaries */

} DS_FilterParmsCmd_t;


/*
**  \brief Set Filename Type For Destination File Table Entry
**
**  For command details see #DS_SET_DEST_TYPE_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

	uint16  FileTableIndex;                     /**< \brief Index into Destination File Table */
    uint16  FileNameType;                       /**< \brief Filename type - count vs time */

} DS_DestTypeCmd_t;


/*
**  \brief Set Ena/Dis State For Destination File Table Entry
**
**  For command details see #DS_SET_DEST_STATE_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

	uint16  FileTableIndex;                     /**< \brief Index into Destination File Table */
    uint16  EnableState;                        /**< \brief File enable/disable state */

} DS_DestStateCmd_t;


/*
**  \brief Set Path Portion Of Filename For Destination File Table Entry
**
**  For command details see #DS_SET_DEST_PATH_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

    uint16  FileTableIndex;                     /**< \brief Index into Destination File Table */
    uint16  Padding;                            /**< \brief Structure Padding on 32-bit boundaries */

    char    Pathname[DS_PATHNAME_BUFSIZE];      /**< \brief Path portion of filename */

} DS_DestPathCmd_t;


/*
**  \brief Set Base Portion Of Filename For Destination File Table Entry
**
**  For command details see #DS_SET_DEST_BASE_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

    uint16  FileTableIndex;                     /**< \brief Index into Destination File Table */
    uint16  Padding;                            /**< \brief Structure Padding on 32-bit boundaries */

    char    Basename[DS_BASENAME_BUFSIZE];      /**< \brief Base portion of filename */

} DS_DestBaseCmd_t;


/*
**  \brief Set Extension Portion Of Filename For Destination File Table Entry
**
**  For command details see #DS_SET_DEST_EXT_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

    uint16  FileTableIndex;                     /**< \brief Index into Destination File Table */
    uint16  Padding;                            /**< \brief Structure Padding on 32-bit boundaries */

    char    Extension[DS_EXTENSION_BUFSIZE];    /**< \brief Extension portion of filename */

} DS_DestExtCmd_t;


/*
**  \brief Set Max File Size For Destination File Table Entry
**
**  For command details see #DS_SET_DEST_SIZE_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

    uint16  FileTableIndex;                     /**< \brief Index into Destination File Table */
    uint16  Padding;                            /**< \brief Structure Padding on 32-bit boundaries */

    uint32  MaxFileSize;                        /**< \brief Max file size (bytes) before re-open */

} DS_DestSizeCmd_t;


/*
**  \brief Set Max File Age For Destination File Table Entry
**
**  For command details see #DS_SET_DEST_AGE_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

    uint16  FileTableIndex;                     /**< \brief Index into Destination File Table */
    uint16  Padding;                            /**< \brief Structure Padding on 32-bit boundaries */

    uint32  MaxFileAge;                         /**< \brief Max file age (seconds) */

} DS_DestAgeCmd_t;


/*
**  \brief Set Sequence Portion Of Filename For Destination File Table Entry
**
**  For command details see #DS_SET_DEST_COUNT_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

    uint16  FileTableIndex;                     /**< \brief Index into Destination File Table */
    uint16  Padding;                            /**< \brief Structure Padding on 32-bit boundaries */

    uint32  SequenceCount;                      /**< \brief Sequence count portion of filename */

} DS_DestCountCmd_t;


/*
**  \brief Close Destination File
**
**  For command details see #DS_CLOSE_FILE_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

    uint16  FileTableIndex;                     /**< \brief Index into Destination File Table */
    uint16  Padding;                            /**< \brief Structure Padding on 32-bit boundaries */

} DS_CloseFileCmd_t;


/*
**  \brief Close All Destination Files
**
**  For command details see #DS_CLOSE_ALL_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

} DS_CloseAllCmd_t;


/*
**  \brief Get File Info Command
**
**  For command details see #DS_GET_FILE_INFO_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

} DS_GetFileInfoCmd_t;


/*
**  \brief Add Message ID To Packet Filter Table
**
**  For command details see #DS_ADD_MID_CC
*/
typedef struct
{
    uint8   CmdHeader[CFE_SB_CMD_HDR_SIZE];     /**< \brief cFE Software Bus command message header */

    uint16  MessageID;                          /**< \brief Message ID to add to Packet Filter Table */
    uint16  Padding;                            /**< \brief Structure Padding on 32-bit boundaries */

} DS_AddMidCmd_t;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS application telemetry formats                                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
**  \dstlm DS application housekeeping packet
*/
typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];     /**< \brief cFE Software Bus telemetry message header */
 
    uint8   CmdAcceptedCounter;                 /**< \dstlmmnemonic \DS_CMDPC
                                                     \brief Count of valid commands received */
    uint8   CmdRejectedCounter;                 /**< \dstlmmnemonic \DS_CMDEC
                                                     \brief Count of invalid commands received */
    uint8   DestTblLoadCounter;                 /**< \dstlmmnemonic \DS_DESTLOADCNT
                                                     \brief Count of destination file table loads */
    uint8   DestTblErrCounter;                  /**< \dstlmmnemonic \DS_DESTPTRERRCNT
                                                     \brief Count of failed attempts to get table data pointer */
    uint8   FilterTblLoadCounter;               /**< \dstlmmnemonic \DS_FILTERLOADCNT
                                                     \brief Count of packet filter table loads */
    uint8   FilterTblErrCounter;                /**< \dstlmmnemonic \DS_FILTERPTRERRCNT
                                                     \brief Count of failed attempts to get table data pointer */
    uint8   AppEnableState;                     /**< \dstlmmnemonic \DS_APPENASTATE
                                                     \brief Application enable/disable state */
    uint8   Spare8;                             /**< \brief Structure alignment padding */

    uint16  FileWriteCounter;                   /**< \dstlmmnemonic \DS_FILEWRITECNT
                                                     \brief Count of good destination file writes */
    uint16  FileWriteErrCounter;                /**< \dstlmmnemonic \DS_FILEWRITEERRCNT
                                                     \brief Count of bad destination file writes */
    uint16  FileUpdateCounter;                  /**< \dstlmmnemonic \DS_FILEUPDCNT
                                                     \brief Count of good updates to secondary header */
    uint16  FileUpdateErrCounter;               /**< \dstlmmnemonic \DS_FILEUPDERRCNT
                                                     \brief Count of bad updates to secondary header */
    uint32  DisabledPktCounter;                 /**< \dstlmmnemonic \DS_DISABLEDPKTCNT
                                                     \brief Count of packets discarded (DS was disabled) */
    uint32  IgnoredPktCounter;                  /**< \dstlmmnemonic \DS_IGNOREDPKTCNT
                                                     \brief Count of packets discarded.  Incoming packets will be discarded when:
                                                     <ul> <li> The File and/or Filter Table has failed to load </li>
                                                          <li> A packet (that is not a DS HK or command packet) has been received that is not 
                                                               listed in the Filter Table </li> </ul> */
    uint32  FilteredPktCounter;                 /**< \dstlmmnemonic \DS_FILTEREDPKTCNT
                                                     \brief Count of packets discarded (failed filter test) */
    uint32  PassedPktCounter;                   /**< \dstlmmnemonic \DS_PASSEDPKTCNT
                                                     \brief Count of packets that passed filter test */
    char    FilterTblFilename[OS_MAX_PATH_LEN]; /**< \dstlmmnemonic \DS_FILTERTBL
                                                     \brief Name of filter table file */
} DS_HkPacket_t;

      
/**
** \brief Current state of destination files
*/
typedef struct
{
    uint32  FileAge;                            /**< \dstlmmnemonic \DS_FILEAGE
                                                     \brief Current file age in seconds */
    uint32  FileSize;                           /**< \dstlmmnemonic \DS_FILESIZE
                                                     \brief Current file size in bytes */
    uint32  FileRate;                           /**< \dstlmmnemonic \DS_FILERATE
                                                     \brief Current file data rate (avg since HK) */
    uint32  SequenceCount;                      /**< \dstlmmnemonic \DS_FILESEQ
                                                     \brief Sequence count portion of filename */
    uint16  EnableState;                        /**< \dstlmmnemonic \DS_ENABLESTATE
                                                     \brief Current file enable/disable state */
    uint16  OpenState;                          /**< \dstlmmnemonic \DS_OPENSTATE
                                                     \brief Current file open/close state */
    char    FileName[DS_TOTAL_FNAME_BUFSIZE];   /**< \dstlmmnemonic \DS_FILENAME
                                                     \brief Current filename (path+base+seq+ext) */
} DS_FileInfo_t;


/**
**  \dstlm DS application file info packet
*/
typedef struct
{
    uint8   TlmHeader[CFE_SB_TLM_HDR_SIZE];     /**< \brief cFE Software Bus telemetry message header */
 
    DS_FileInfo_t FileInfo[DS_DEST_FILE_CNT];   /**< \dstlmmnemonic \DS_FILEINFO
                                                     \brief Current state of destination files */
} DS_FileInfoPkt_t;

      
#endif /* _ds_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
