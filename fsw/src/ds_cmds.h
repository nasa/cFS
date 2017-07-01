/************************************************************************
**  $Id: ds_cmds.h 1.3 2017/01/25 12:05:30EST sstrege Exp  $
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
**  CFS Data Storage (DS) command handler header file
**
*************************************************************************/

#ifndef _ds_cmds_h_
#define _ds_cmds_h_

#include "cfe.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Prototypes for functions defined in ds_app.c                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*******************************************************************/
/*  \brief NOOP command handler
**  
**  \par Description
**       The NOOP command performs no specific function (no operation)
**       Validate command packet
**       - generate error event if invalid command packet length
**       Process valid command packets
**       - generate success event (event type = informational)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_NOOP_CC, #DS_NoopCmd_t
*/
void DS_CmdNoop(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief RESET command handler
**  
**  \par Description
**       Reset housekeeping telemetry counters command
**       Validate command packet
**       - generate error event if invalid command packet length
**       Process valid command packets
**       - set selected housekeeping telemetry counters to zero
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_RESET_CC, #DS_ResetCmd_t
*/
void DS_CmdReset(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Set application enable/disable state command handler
**  
**  \par Description
**       Set application enable/disable state command
**       Validate command packet
**       - generate error event if invalid command packet length
**       - generate error event if invalid enable/disable state
**       Process valid command packets
**       - update application enable/disable state
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_SET_APP_STATE_CC, #DS_AppStateCmd_t
*/
void DS_CmdSetAppState(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Set file index for filter table entry command handler
**  
**  \par Description
**       Set destination file index for filter table entry command
**       Validate command packet
**       - generate error event if invalid command packet length
**       - generate error event if invalid filter parameters index
**       - generate error event if invalid file table index
**       - generate error event if packet filter table is not loaded
**       - generate error event if message ID is not in filter table
**       Process valid command packets
**       - update file index for selected filter table entry
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_SET_FILTER_FILE_CC, #DS_FilterFileCmd_t
*/
void DS_CmdSetFilterFile(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Set filter type for filter table entry command handler
**  
**  \par Description
**       Set filter type for filter table entry command
**       Validate command packet
**       - generate error event if invalid command packet length
**       - generate error event if invalid filter parameters index
**       - generate error event if invalid filter type
**       - generate error event if packet filter table is not loaded
**       - generate error event if message ID is not in filter table
**       Process valid command packets
**       - update filter type for selected filter table entry
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_SET_FILTER_TYPE_CC, #DS_FilterTypeCmd_t
*/
void DS_CmdSetFilterType(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Set filter parameters for filter table entry command handler
**  
**  \par Description
**       Set filter parameters for filter table entry command
**       Validate command packet
**       - generate error event if invalid command packet length
**       - generate error event if invalid filter parameters index
**       - generate error event if invalid filter parameters
**       - generate error event if packet filter table is not loaded
**       - generate error event if message ID is not in filter table
**       Process valid command packets
**       - update filter parameters for selected filter table entry
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_SET_FILTER_PARMS_CC, #DS_FilterParmsCmd_t
*/
void DS_CmdSetFilterParms(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Set data storage filename type command handler
**  
**  \par Description
**       Modify the filename type for the selected entry in the
**       destination file definitions table.
**       Reject invalid command packets
**       - generate error event if invalid command packet length
**       - generate error event if invalid file table index
**       - generate error event if invalid filename type
**       - generate error event if file table is not loaded
**       Accept valid command packets
**       - update filename type for selected file table entry
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_SET_DEST_TYPE_CC, #DS_DestTypeCmd_t
*/
void DS_CmdSetDestType(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Set data storage enable/disable state command handler
**  
**  \par Description
**       Modify the enable/disable state for the selected entry
**       in the destination file definitions table.
**       Reject invalid command packets
**       - generate error event if invalid command packet length
**       - generate error event if invalid file table index
**       - generate error event if invalid enable/disable state
**       - generate error event if file table is not loaded
**       Accept valid command packets
**       - update enable/disable state for selected file table entry
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_SET_DEST_STATE_CC, #DS_DestStateCmd_t
*/
void DS_CmdSetDestState(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Set data storage file pathname command handler
**  
**  \par Description
**       Modify the path portion of the filename for the selected
**       entry in the destination file definitions table.
**       Reject invalid command packets
**       - generate error event if invalid command packet length
**       - generate error event if invalid file table index
**       - generate error event if invalid pathname characters
**       - generate error event if file table is not loaded
**       Accept valid command packets
**       - update file pathname for selected file table entry
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_SET_DEST_PATH_CC, #DS_DestPathCmd_t
*/
void DS_CmdSetDestPath(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Set data storage file basename command handler
**  
**  \par Description
**       Modify the base portion of the filename for the selected
**       entry in the destination file definitions table.
**       Reject invalid command packets
**       - generate error event if invalid command packet length
**       - generate error event if invalid file table index
**       - generate error event if invalid basename characters
**       - generate error event if file table is not loaded
**       Accept valid command packets
**       - update file basename for selected file table entry
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_SET_DEST_BASE_CC, #DS_DestBaseCmd_t
*/
void DS_CmdSetDestBase(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Set data storage file extension command handler
**  
**  \par Description
**       Modify the extension portion of the filename for the
**       selected entry in the destination file definitions table.
**       Reject invalid command packets
**       - generate error event if invalid command packet length
**       - generate error event if invalid file table index
**       - generate error event if invalid extension characters
**       - generate error event if file table is not loaded
**       Accept valid command packets
**       - update file extension for selected file table entry
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_SET_DEST_EXT_CC, #DS_DestExtCmd_t
*/
void DS_CmdSetDestExt(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Set data storage file size limit command handler
**  
**  \par Description
**       Modify the max file size limit for the selected entry
**       in the destination file definitions table.
**       Reject invalid command packets
**       - generate error event if invalid command packet length
**       - generate error event if invalid file table index
**       - generate error event if invalid file size limit
**       - generate error event if file table is not loaded
**       Accept valid command packets
**       - update max size limit for selected file table entry
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_SET_DEST_SIZE_CC, #DS_DestSizeCmd_t
*/
void DS_CmdSetDestSize(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Set data storage file age limit command handler
**  
**  \par Description
**       Modify the max file age limit for the selected entry
**       in the destination file definitions table.
**       Reject invalid command packets
**       - generate error event if invalid command packet length
**       - generate error event if invalid file table index
**       - generate error event if invalid file size limit
**       - generate error event if file table is not loaded
**       Accept valid command packets
**       - update max age limit for selected file table entry
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_SET_DEST_AGE_CC, #DS_DestAgeCmd_t
*/
void DS_CmdSetDestAge(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Set data storage filename sequence count command handler
**  
**  \par Description
**       Modify the filename sequence count for the selected
**       entry in the destination file definitions table.
**       Reject invalid command packets
**       - generate error event if invalid command packet length
**       - generate error event if invalid file table index
**       - generate error event if invalid filename sequence count
**       - generate error event if file table is not loaded
**       Accept valid command packets
**       - update filename sequence count for selected file table entry
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_SET_DEST_COUNT_CC, #DS_DestCountCmd_t
*/
void DS_CmdSetDestCount(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Close data storage file command handler
**  
**  \par Description
**       Close the selected data storage file. If this destination
**       is still enabled, another file will be opened upon receipt
**       of the next packet which passes the filter algorithm.
**       Reject invalid command packets
**       - generate error event if invalid command packet length
**       - generate error event if invalid destination file index
**       Accept valid command packets
**       - close the selected destination file
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_CLOSE_FILE_CC, #DS_CloseFileCmd_t
*/
void DS_CmdCloseFile(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Close all data storage files command handler
**  
**  \par Description
**       Close all open data storage files. If any open destination
**       file is still enabled, another file will be opened upon receipt
**       of the next packet which passes the filter algorithm.
**       Reject invalid command packets
**       - generate error event if invalid command packet length
**       Accept valid command packets
**       - close all open destination files
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_CLOSE_ALL_CC, #DS_CloseAllCmd_t
*/
void DS_CmdCloseAll(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Get file information telemetry packet command handler
**  
**  \par Description
**       Create and send a telemetry packet containing the current
**       status for all destination files.
**       Reject invalid command packets
**       - generate error event if invalid command packet length
**       Accept valid command packets
**       - generate file info telemetry packet
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_GET_FILE_INFO_CC, #DS_GetFileInfoCmd_t, #DS_FileInfoPkt_t
*/
void DS_CmdGetFileInfo(CFE_SB_MsgPtr_t MessagePtr);


/*******************************************************************/
/*  \brief Add Message ID to Packet Filter Table
**  
**  \par Description
**       Set MID selection for unused packet filter table entry
**       Reject invalid commands
**       - generate error event if invalid command packet length
**       - generate error event if MID argument is invalid (cannot be zero)
**       - generate error event if packet filter table is not loaded
**       - generate error event if MID is already in packet filter table
**       - generate error event if no unused packet filter table entries
**       Accept valid commands
**       - generate success event (event type = debug)
**
**  \par Assumptions, External Events, and Notes:
**       (none)
**       
**  \param [in]  Software Bus message pointer (#CFE_SB_MsgPtr_t)
**
**  \sa #DS_ADD_MID_CC, #DS_AddMidCmd_t
*/
void DS_CmdAddMID(CFE_SB_MsgPtr_t MessagePtr);


#endif /* _ds_cmds_h_ */

/************************/
/*  End of File Comment */
/************************/
