/************************************************************************
** File:
**   $Id: ds_app.h 1.4 2017/01/25 12:05:26EST sstrege Exp  $
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
#ifndef _ds_app_h_
#define _ds_app_h_

#include "cfe.h"

#include "ds_appdefs.h"

#include "ds_platform_cfg.h"

#include "ds_table.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS application data structures                                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
** \brief Current state of destination files
*/
typedef struct
{
    int32   FileHandle;                         /**< \brief Current file handle */
    uint32  FileAge;                            /**< \brief Current file age in seconds */
    uint32  FileSize;                           /**< \brief Current file size in bytes */
    uint32  FileGrowth;                         /**< \brief Current file growth in bytes (since HK) */
    uint32  FileRate;                           /**< \brief File growth rate in bytes (at last HK) */
    uint32  FileCount;                          /**< \brief Current file sequence count */
    uint16  FileState;                          /**< \brief Current file enable/disable state */
    uint16  Unused;                             /**< \brief Unused - structure padding */
    char    FileName[DS_TOTAL_FNAME_BUFSIZE];   /**< \brief Current filename (path+base+seq+ext) */

} DS_AppFileStatus_t;


/* 
**  \brief DS global data structure definition
*/
typedef struct
{
    CFE_SB_PipeId_t         InputPipe;          /**< \brief Pipe Id for DS command pipe */    
   
    CFE_ES_CDSHandle_t      DataStoreHandle;    /**< \brief Critical Data Store (CDS) handle */
   
    CFE_TBL_Handle_t        FilterTblHandle;    /**< \brief Packet filter table handle */
    CFE_TBL_Handle_t        DestFileTblHandle;  /**< \brief Destination file table handle */
    
    DS_FilterTable_t       *FilterTblPtr;       /**< \brief Packet filter table data pointer */
    DS_DestFileTable_t     *DestFileTblPtr;     /**< \brief Destination file table data pointer */

    uint8   CmdAcceptedCounter;                 /**< \brief Count of valid commands received */
    uint8   CmdRejectedCounter;                 /**< \brief Count of invalid commands received */
    uint8   DestTblLoadCounter;                 /**< \brief Count of destination file table loads */
    uint8   DestTblErrCounter;                  /**< \brief Count of failed attempts to get table data pointer */
    uint8   FilterTblLoadCounter;               /**< \brief Count of packet filter table loads */
    uint8   FilterTblErrCounter;                /**< \brief Count of failed attempts to get table data pointer */
    uint8   AppEnableState;                     /**< \brief Application enable/disable state */
    uint8   Spare8;                             /**< \brief Structure alignment padding */

    uint16  FileWriteCounter;                   /**< \brief Count of good destination file writes */
    uint16  FileWriteErrCounter;                /**< \brief Count of bad destination file writes */
    uint16  FileUpdateCounter;                  /**< \brief Count of good updates to secondary header */
    uint16  FileUpdateErrCounter;               /**< \brief Count of bad updates to secondary header */

    uint32  DisabledPktCounter;                 /**< \brief Count of packets discarded (DS app disabled) */
    uint32  IgnoredPktCounter;                  /**< \brief Count of packets discarded (pkt has no filter) */
    uint32  FilteredPktCounter;                 /**< \brief Count of packets discarded (failed filter test) */
    uint32  PassedPktCounter;                   /**< \brief Count of packets that passed filter test */

#if (DS_FILE_HEADER_TYPE == DS_FILE_HEADER_GPM)
    uint32  CurrentPktTime;                     /**< \brief Seconds from current packet timestamp */
    uint32  LastPktTime[DS_DEST_FILE_CNT];      /**< \brief Seconds from last packet timestamp */
#endif

    DS_AppFileStatus_t FileStatus[DS_DEST_FILE_CNT]; /**< \brief Current state of destination files */

    DS_HashLink_t  HashLinks[DS_PACKETS_IN_FILTER_TABLE];     /**< \brief Hash table linked list elements */
    DS_HashLink_t *HashTable[DS_HASH_TABLE_ENTRIES];          /**< \brief Each hash table entry is a linked list */

} DS_AppData_t;


/* 
**  \brief DS global data structure reference
*/
extern DS_AppData_t DS_AppData;                 /**< \brief External reference to DS global data */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Prototypes for functions defined in ds_app.c                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*******************************************************************/
/*  \brief CFS Data Storage (DS) application entry point
**  
**  \par Description
**       DS application entry point and main process loop.
**
**  \par Assumptions, External Events, and Notes:
**       None
**/
void DS_AppMain(void);


/*******************************************************************/
/*  \brief Application initialization function
**
**  \par Description
**       Performs the following startup initialization:
**       - register DS application for cFE Event Services
**       - create a cFE Software Bus message pipe
**       - subscribe to DS commands via message pipe
**       - register DS filter and file destination tables
**       - load default filter and file destination tables
**       - subscribe to packets referenced in DS filter table
**       - generate startup initialization event message
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \retstmt Return codes from #CFE_TBL_Register         \endcode
**  \endreturns
*/
int32 DS_AppInitialize(void);


/*******************************************************************/
/*  \brief Software Bus message handler
**  
**  \par Description
**       Process packets received via Software Bus message pipe 
**       - may call application housekeeping request command handler
**       - may call 1Hz wakeup command handler (if enabled)
**       - may call application ground command handler
**       All packets are processed for possible data storage
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
*/
void DS_AppProcessMsg(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Application ground command handler
**  
**  \par Description
**       Call command code specific DS command handler function
**       Generate command error event for unknown command codes
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
*/
void DS_AppProcessCmd(CFE_SB_MsgPtr_t MessagePtr);


/*
** Process housekeeping request command...
*/
/*******************************************************************/
/*  \brief Application housekeeping request command handler
**  
**  \par Description
**       Check with cFE Table Services for table updates
**       Generate application housekeeping telemetry packet
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_HkPacket_t
*/
void DS_AppProcessHK(void);


/*******************************************************************/
/*  \brief Application packet storage pre-processor
**  
**  \par Description
**       This function verifies that DS storage is enabled and that
**       both DS tables (filter and file) are loaded before calling
**       the file storage function (#DS_FileStorePacket).
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Message ID (extracted from message pointer)
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #CFE_SB_MsgPtr_t
*/
void DS_AppStorePacket(CFE_SB_MsgId_t MessageID, CFE_SB_MsgPtr_t MessagePtr);

#endif /* _ds_app_h_ */

/************************/
/*  End of File Comment */
/************************/
