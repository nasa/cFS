/************************************************************************
**  $Id: ds_cmds.c 1.6 2017/01/25 12:05:32EST sstrege Exp  $
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
**  CFS Data Storage (DS) command handler functions
**
*************************************************************************/

#include "cfe.h"

#include "cfs_utils.h"

#include "ds_platform_cfg.h"
#include "ds_verify.h"

#include "ds_appdefs.h"
#include "ds_msgids.h"

#include "ds_msg.h"
#include "ds_app.h"
#include "ds_cmds.h"
#include "ds_file.h"
#include "ds_table.h"
#include "ds_events.h"
#include "ds_version.h"

#include "string.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdNoop() - NOOP command                                     */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdNoop(CFE_SB_MsgPtr_t MessagePtr)
{
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_NoopCmd_t);

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_NOOP_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid NOOP command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else
    {
        /*
        ** Do nothing except display "aliveness" event...
        */
        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_NOOP_CMD_EID, CFE_EVS_INFORMATION,
                         "NOOP command, Version %d.%d.%d.%d",
                          DS_MAJOR_VERSION, DS_MINOR_VERSION, DS_REVISION, DS_MISSION_REV);
    }

    return;

} /* End of DS_CmdNoop() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdReset() - reset hk telemetry counters command             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdReset(CFE_SB_MsgPtr_t MessagePtr)
{
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_ResetCmd_t);

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_RESET_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid RESET command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else
    {
        /*
        ** Reset application command counters...
        */
        DS_AppData.CmdAcceptedCounter = 0;
        DS_AppData.CmdRejectedCounter = 0;

        /*
        ** Reset packet storage counters...
        */
        DS_AppData.DisabledPktCounter = 0;
        DS_AppData.IgnoredPktCounter  = 0;
        DS_AppData.FilteredPktCounter = 0;
        DS_AppData.PassedPktCounter   = 0;

        /*
        ** Reset file I/O counters...
        */
        DS_AppData.FileWriteCounter     = 0;
        DS_AppData.FileWriteErrCounter  = 0;
        DS_AppData.FileUpdateCounter    = 0;
        DS_AppData.FileUpdateErrCounter = 0;

        /*
        ** Reset configuration table counters...
        */
        DS_AppData.DestTblLoadCounter   = 0;
        DS_AppData.DestTblErrCounter    = 0;
        DS_AppData.FilterTblLoadCounter = 0;
        DS_AppData.FilterTblErrCounter  = 0;

        CFE_EVS_SendEvent(DS_RESET_CMD_EID, CFE_EVS_DEBUG, "Reset counters command");
    }

    return;

} /* End of DS_CmdReset() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdSetAppState() - set application ena/dis state             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdSetAppState(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_AppStateCmd_t *DS_AppStateCmd = (DS_AppStateCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_AppStateCmd_t);

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_ENADIS_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid APP STATE command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_TableVerifyState(DS_AppStateCmd->EnableState) == FALSE)
    {
        /*
        ** Invalid enable/disable state...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_ENADIS_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid APP STATE command arg: app state = %d",
                          DS_AppStateCmd->EnableState);
    }
    else
    {
        /*
        ** Set new DS application enable/disable state...
        */
        DS_AppData.AppEnableState = DS_AppStateCmd->EnableState;

        /*
        ** Update the Critical Data Store (CDS)...
        */
        DS_TableUpdateCDS();

        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_ENADIS_CMD_EID, CFE_EVS_DEBUG,
                         "APP STATE command: state = %d",
                          DS_AppStateCmd->EnableState);
    }

    return;

} /* End of DS_CmdSetAppState() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdSetFilterFile() - set packet filter file index            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdSetFilterFile(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_FilterFileCmd_t *DS_FilterFileCmd = (DS_FilterFileCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_FilterFileCmd_t);
    DS_PacketEntry_t *pPacketEntry;
    DS_FilterParms_t *pFilterParms;
    int32 FilterTableIndex;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER FILE command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_FilterFileCmd->MessageID == DS_UNUSED)
    {
        /*
        ** Invalid packet messageID...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER FILE command arg: invalid messageID = 0x%4X",
                          DS_FilterFileCmd->MessageID);
    }
    else if (DS_FilterFileCmd->FilterParmsIndex >= DS_FILTERS_PER_PACKET)
    {
        /*
        ** Invalid packet filter parameters index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER FILE command arg: filter parameters index = %d",
                          DS_FilterFileCmd->FilterParmsIndex);
    }
    else if (DS_TableVerifyFileIndex(DS_FilterFileCmd->FileTableIndex) == FALSE)
    {
        /*
        ** Invalid destination file table index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER FILE command arg: file table index = %d",
                          DS_FilterFileCmd->FileTableIndex);
    }
    else if (DS_AppData.FilterTblPtr == (DS_FilterTable_t *) NULL)
    {
        /*
        ** Must have a valid packet filter table loaded...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER FILE command: packet filter table is not loaded");
    }
    else
    {
        /*
        ** Get the index of the filter table entry for this message ID...
        */
        FilterTableIndex = DS_TableFindMsgID(DS_FilterFileCmd->MessageID);

        if (FilterTableIndex == DS_INDEX_NONE)
        {
            /*
            ** Must not create - may only modify existing packet filter...
            */
            DS_AppData.CmdRejectedCounter++;

            CFE_EVS_SendEvent(DS_FILE_CMD_ERR_EID, CFE_EVS_ERROR,
                             "Invalid FILTER FILE command: Message ID 0x%04X is not in filter table",
                              DS_FilterFileCmd->MessageID);
        }
        else
        {
            /*
            ** Set new packet filter value (file table index)...
            */
            pPacketEntry = &DS_AppData.FilterTblPtr->Packet[FilterTableIndex];
            pFilterParms = &pPacketEntry->Filter[DS_FilterFileCmd->FilterParmsIndex];

            pFilterParms->FileTableIndex = DS_FilterFileCmd->FileTableIndex;

            /*
            ** Notify cFE that we have modified the table data...
            */
            CFE_TBL_Modified(DS_AppData.FilterTblHandle);

            DS_AppData.CmdAcceptedCounter++;

            CFE_EVS_SendEvent(DS_FILE_CMD_EID, CFE_EVS_DEBUG,
                             "FILTER FILE command: MID = 0x%04X, index = %d, filter = %d, file = %d",
                              DS_FilterFileCmd->MessageID, (int)FilterTableIndex,
                              DS_FilterFileCmd->FilterParmsIndex,
                              DS_FilterFileCmd->FileTableIndex);
        }
    }

    return;

} /* End of DS_CmdSetFilterFile() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdSetFilterType() - set pkt filter filename type            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdSetFilterType(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_FilterTypeCmd_t *DS_FilterTypeCmd = (DS_FilterTypeCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_FilterTypeCmd_t);
    DS_PacketEntry_t *pPacketEntry;
    DS_FilterParms_t *pFilterParms;
    int32 FilterTableIndex;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER TYPE command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_FilterTypeCmd->MessageID == DS_UNUSED)
    {
        /*
        ** Invalid packet messageID...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER TYPE command arg: invalid messageID = 0x%04X",
                          DS_FilterTypeCmd->MessageID);
    }
    else if (DS_FilterTypeCmd->FilterParmsIndex >= DS_FILTERS_PER_PACKET)
    {
        /*
        ** Invalid packet filter parameters index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER TYPE command arg: filter parameters index = %d",
                          DS_FilterTypeCmd->FilterParmsIndex);
    }
    else if (DS_TableVerifyType(DS_FilterTypeCmd->FilterType) == FALSE)
    {
        /*
        ** Invalid packet filter filename type...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER TYPE command arg: filter type = %d",
                          DS_FilterTypeCmd->FilterType);
    }
    else if (DS_AppData.FilterTblPtr == (DS_FilterTable_t *) NULL)
    {
        /*
        ** Must have a valid packet filter table loaded...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER TYPE command: packet filter table is not loaded");
    }
    else
    {
        /*
        ** Get the index of the filter table entry for this message ID...
        */
        FilterTableIndex = DS_TableFindMsgID(DS_FilterTypeCmd->MessageID);

        if (FilterTableIndex == DS_INDEX_NONE)
        {
            /*
            ** Must not create - may only modify existing packet filter...
            */
            DS_AppData.CmdRejectedCounter++;

            CFE_EVS_SendEvent(DS_FTYPE_CMD_ERR_EID, CFE_EVS_ERROR,
                             "Invalid FILTER TYPE command: Message ID 0x%04X is not in filter table",
                              DS_FilterTypeCmd->MessageID);
        }
        else
        {
            /*
            ** Set new packet filter value (filter type)...
            */
            pPacketEntry = &DS_AppData.FilterTblPtr->Packet[FilterTableIndex];
            pFilterParms = &pPacketEntry->Filter[DS_FilterTypeCmd->FilterParmsIndex];

            pFilterParms->FilterType = DS_FilterTypeCmd->FilterType;

            /*
            ** Notify cFE that we have modified the table data...
            */
            CFE_TBL_Modified(DS_AppData.FilterTblHandle);

            DS_AppData.CmdAcceptedCounter++;

            CFE_EVS_SendEvent(DS_FTYPE_CMD_EID, CFE_EVS_DEBUG,
                             "FILTER TYPE command: MID = 0x%04X, index = %d, filter = %d, type = %d",
                              DS_FilterTypeCmd->MessageID, (int)FilterTableIndex,
                              DS_FilterTypeCmd->FilterParmsIndex,
                              DS_FilterTypeCmd->FilterType);
        }
    }

    return;

} /* End of DS_CmdSetFilterType() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdSetFilterParms() - set packet filter parameters           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdSetFilterParms(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_FilterParmsCmd_t *DS_FilterParmsCmd = (DS_FilterParmsCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_FilterParmsCmd_t);
    DS_PacketEntry_t *pPacketEntry;
    DS_FilterParms_t *pFilterParms;
    int32 FilterTableIndex;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER PARMS command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_FilterParmsCmd->MessageID == DS_UNUSED)
    {
        /*
        ** Invalid packet messageID...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER PARMS command arg: invalid messageID = 0x%04X",
                          DS_FilterParmsCmd->MessageID);
    }
    else if (DS_FilterParmsCmd->FilterParmsIndex >= DS_FILTERS_PER_PACKET)
    {
        /*
        ** Invalid packet filter parameters index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER PARMS command arg: filter parameters index = %d",
                          DS_FilterParmsCmd->FilterParmsIndex);
    }
    else if (DS_TableVerifyParms(DS_FilterParmsCmd->Algorithm_N,
                                 DS_FilterParmsCmd->Algorithm_X,
                                 DS_FilterParmsCmd->Algorithm_O) == FALSE)
    {
        /*
        ** Invalid packet filter algorithm parameters...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER PARMS command arg: N = %d, X = %d, O = %d",
                          DS_FilterParmsCmd->Algorithm_N,
                          DS_FilterParmsCmd->Algorithm_X,
                          DS_FilterParmsCmd->Algorithm_O);
    }
    else if (DS_AppData.FilterTblPtr == (DS_FilterTable_t *) NULL)
    {
        /*
        ** Must have a valid packet filter table loaded...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid FILTER PARMS command: packet filter table is not loaded");
    }
    else
    {
        /*
        ** Get the index of the filter table entry for this message ID...
        */
        FilterTableIndex = DS_TableFindMsgID(DS_FilterParmsCmd->MessageID);

        if (FilterTableIndex == DS_INDEX_NONE)
        {
            /*
            ** Must not create - may only modify existing packet filter...
            */
            DS_AppData.CmdRejectedCounter++;

            CFE_EVS_SendEvent(DS_PARMS_CMD_ERR_EID, CFE_EVS_ERROR,
                             "Invalid FILTER PARMS command: Message ID 0x%04X is not in filter table",
                              DS_FilterParmsCmd->MessageID);
        }
        else
        {
            /*
            ** Set new packet filter values (algorithm)...
            */
            pPacketEntry = &DS_AppData.FilterTblPtr->Packet[FilterTableIndex];
            pFilterParms = &pPacketEntry->Filter[DS_FilterParmsCmd->FilterParmsIndex];

            pFilterParms->Algorithm_N = DS_FilterParmsCmd->Algorithm_N;
            pFilterParms->Algorithm_X = DS_FilterParmsCmd->Algorithm_X;
            pFilterParms->Algorithm_O = DS_FilterParmsCmd->Algorithm_O;

            /*
            ** Notify cFE that we have modified the table data...
            */
            CFE_TBL_Modified(DS_AppData.FilterTblHandle);

            DS_AppData.CmdAcceptedCounter++;

            CFE_EVS_SendEvent(DS_PARMS_CMD_EID, CFE_EVS_DEBUG,
                             "FILTER PARMS command: MID = 0x%04X, index = %d, filter = %d, N = %d, X = %d, O = %d",
                              DS_FilterParmsCmd->MessageID, (int)FilterTableIndex, DS_FilterParmsCmd->FilterParmsIndex,
                              pFilterParms->Algorithm_N, pFilterParms->Algorithm_X, pFilterParms->Algorithm_O);
        }
    }

    return;

} /* End of DS_CmdSetFilterParms() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdSetDestType() - set destination filename type             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdSetDestType(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_DestTypeCmd_t *DS_DestTypeCmd = (DS_DestTypeCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_DestTypeCmd_t);
    DS_DestFileEntry_t *pDest;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_NTYPE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST TYPE command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_TableVerifyFileIndex(DS_DestTypeCmd->FileTableIndex) == FALSE)
    {
        /*
        ** Invalid destination file table index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_NTYPE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST TYPE command arg: file table index = %d",
                          DS_DestTypeCmd->FileTableIndex);
    }
    else if (DS_TableVerifyType(DS_DestTypeCmd->FileNameType) == FALSE)
    {
        /*
        ** Invalid destination filename type...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_NTYPE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST TYPE command arg: filename type = %d",
                          DS_DestTypeCmd->FileNameType);
    }
    else if (DS_AppData.DestFileTblPtr == (DS_DestFileTable_t *) NULL)
    {
        /*
        ** Must have a valid destination file table loaded...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_NTYPE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST TYPE command: destination file table is not loaded");
    }
    else
    {
        /*
        ** Set new destination table filename type...
        */
        pDest = &DS_AppData.DestFileTblPtr->File[DS_DestTypeCmd->FileTableIndex];
        pDest->FileNameType = DS_DestTypeCmd->FileNameType;

        /*
        ** Notify cFE that we have modified the table data...
        */
        CFE_TBL_Modified(DS_AppData.DestFileTblHandle);

        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_NTYPE_CMD_EID, CFE_EVS_DEBUG,
                         "DEST TYPE command: file table index = %d, filename type = %d",
                          DS_DestTypeCmd->FileTableIndex, DS_DestTypeCmd->FileNameType);
    }

    return;

} /* End of DS_CmdSetDestType() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdSetDestState() - set dest file ena/dis state              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdSetDestState(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_DestStateCmd_t *DS_DestStateCmd = (DS_DestStateCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_DestStateCmd_t);

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_STATE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST STATE command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_TableVerifyFileIndex(DS_DestStateCmd->FileTableIndex) == FALSE)
    {
        /*
        ** Invalid destination file table index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_STATE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST STATE command arg: file table index = %d",
                          DS_DestStateCmd->FileTableIndex);
    }
    else if (DS_TableVerifyState(DS_DestStateCmd->EnableState) == FALSE)
    {
        /*
        ** Invalid destination file state...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_STATE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST STATE command arg: file state = %d",
                          DS_DestStateCmd->EnableState);
    }
    else if (DS_AppData.DestFileTblPtr == (DS_DestFileTable_t *) NULL)
    {
        /*
        ** Must have a valid destination file table loaded...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_STATE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST STATE command: destination file table is not loaded");
    }
    else
    {
        /*
        ** Set new destination table file state in table and in current status...
        */
        DS_AppData.DestFileTblPtr->File[DS_DestStateCmd->FileTableIndex].EnableState = DS_DestStateCmd->EnableState;
        DS_AppData.FileStatus[DS_DestStateCmd->FileTableIndex].FileState = DS_DestStateCmd->EnableState;

        /*
        ** Notify cFE that we have modified the table data...
        */
        CFE_TBL_Modified(DS_AppData.DestFileTblHandle);

        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_STATE_CMD_EID, CFE_EVS_DEBUG,
                         "DEST STATE command: file table index = %d, file state = %d",
                          DS_DestStateCmd->FileTableIndex, DS_DestStateCmd->EnableState);
    }

    return;

} /* End of DS_CmdSetDestState() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdSetDestPath() - set path portion of filename              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdSetDestPath(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_DestPathCmd_t *DS_DestPathCmd = (DS_DestPathCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_DestPathCmd_t);
    DS_DestFileEntry_t *pDest;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_PATH_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST PATH command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_TableVerifyFileIndex(DS_DestPathCmd->FileTableIndex) == FALSE)
    {
        /*
        ** Invalid destination file table index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_PATH_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST PATH command arg: file table index = %d",
                          (int)DS_DestPathCmd->FileTableIndex);
    }
    else if (CFS_VerifyString(DS_DestPathCmd->Pathname, DS_PATHNAME_BUFSIZE,
                              DS_STRING_REQUIRED, DS_FILENAME_TEXT) == FALSE)
    {
        /*
        ** Invalid destination pathname...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_PATH_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST PATH command arg: pathname");
    }
    else if (DS_AppData.DestFileTblPtr == (DS_DestFileTable_t *) NULL)
    {
        /*
        ** Must have a valid destination file table loaded...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_PATH_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST PATH command: destination file table is not loaded");
    }
    else
    {
        /*
        ** Set path portion of destination table filename...
        */
        pDest = &DS_AppData.DestFileTblPtr->File[DS_DestPathCmd->FileTableIndex];
        strcpy(pDest->Pathname, DS_DestPathCmd->Pathname);

        /*
        ** Notify cFE that we have modified the table data...
        */
        CFE_TBL_Modified(DS_AppData.DestFileTblHandle);

        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_PATH_CMD_EID, CFE_EVS_DEBUG,
                         "DEST PATH command: file table index = %d, pathname = '%s'",
                          (int)DS_DestPathCmd->FileTableIndex, DS_DestPathCmd->Pathname);
    }

    return;

} /* End of DS_CmdSetDestPath() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdSetDestBase() - set base portion of filename              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdSetDestBase(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_DestBaseCmd_t *DS_DestBaseCmd = (DS_DestBaseCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_DestBaseCmd_t);
    DS_DestFileEntry_t *pDest;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_BASE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST BASE command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_TableVerifyFileIndex(DS_DestBaseCmd->FileTableIndex) == FALSE)
    {
        /*
        ** Invalid destination file table index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_BASE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST BASE command arg: file table index = %d",
                          (int)DS_DestBaseCmd->FileTableIndex);
    }
    else if (CFS_VerifyString(DS_DestBaseCmd->Basename, DS_BASENAME_BUFSIZE,
                              DS_STRING_OPTIONAL, DS_FILENAME_TEXT) == FALSE)
    {
        /*
        ** Invalid destination base filename...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_BASE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST BASE command arg: base filename");
    }
    else if (DS_AppData.DestFileTblPtr == (DS_DestFileTable_t *) NULL)
    {
        /*
        ** Must have a valid destination file table loaded...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_BASE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST BASE command: destination file table is not loaded");
    }
    else
    {
        /*
        ** Set base portion of destination table filename...
        */
        pDest = &DS_AppData.DestFileTblPtr->File[DS_DestBaseCmd->FileTableIndex];
        strcpy(pDest->Basename, DS_DestBaseCmd->Basename);

        /*
        ** Notify cFE that we have modified the table data...
        */
        CFE_TBL_Modified(DS_AppData.DestFileTblHandle);

        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_BASE_CMD_EID, CFE_EVS_DEBUG,
                         "DEST BASE command: file table index = %d, base filename = '%s'",
                          (int)DS_DestBaseCmd->FileTableIndex, DS_DestBaseCmd->Basename);
    }

    return;

} /* End of DS_CmdSetDestBase() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdSetDestExt() - set extension portion of filename          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdSetDestExt(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_DestExtCmd_t *DS_DestExtCmd = (DS_DestExtCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_DestExtCmd_t);
    DS_DestFileEntry_t *pDest;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_EXT_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST EXT command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_TableVerifyFileIndex(DS_DestExtCmd->FileTableIndex) == FALSE)
    {
        /*
        ** Invalid destination file table index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_EXT_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST EXT command arg: file table index = %d",
                          (int)DS_DestExtCmd->FileTableIndex);
    }
    else if (CFS_VerifyString(DS_DestExtCmd->Extension, DS_EXTENSION_BUFSIZE,
                              DS_STRING_OPTIONAL, DS_FILENAME_TEXT) == FALSE)
    {
        /*
        ** Invalid destination filename extension...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_EXT_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST EXT command arg: extension");
    }
    else if (DS_AppData.DestFileTblPtr == (DS_DestFileTable_t *) NULL)
    {
        /*
        ** Must have a valid destination file table loaded...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_EXT_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST EXT command: destination file table is not loaded");
    }
    else
    {
        /*
        ** Set extension portion of destination table filename...
        */
        pDest = &DS_AppData.DestFileTblPtr->File[DS_DestExtCmd->FileTableIndex];
        strcpy(pDest->Extension, DS_DestExtCmd->Extension);

        /*
        ** Notify cFE that we have modified the table data...
        */
        CFE_TBL_Modified(DS_AppData.DestFileTblHandle);

        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_EXT_CMD_EID, CFE_EVS_DEBUG,
                         "DEST EXT command: file table index = %d, extension = '%s'",
                          (int)DS_DestExtCmd->FileTableIndex, DS_DestExtCmd->Extension);
    }

    return;

} /* End of DS_CmdSetDestExt() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdSetDestSize() - set maximum file size limit               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdSetDestSize(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_DestSizeCmd_t *DS_DestSizeCmd = (DS_DestSizeCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_DestSizeCmd_t);
    DS_DestFileEntry_t *pDest;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_SIZE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST SIZE command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_TableVerifyFileIndex(DS_DestSizeCmd->FileTableIndex) == FALSE)
    {
        /*
        ** Invalid destination file table index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_SIZE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST SIZE command arg: file table index = %d",
                          (int)DS_DestSizeCmd->FileTableIndex);
    }
    else if (DS_TableVerifySize(DS_DestSizeCmd->MaxFileSize) == FALSE)
    {
        /*
        ** Invalid destination file size limit...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_SIZE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST SIZE command arg: size limit = %d",
                          (int)DS_DestSizeCmd->MaxFileSize);
    }
    else if (DS_AppData.DestFileTblPtr == (DS_DestFileTable_t *) NULL)
    {
        /*
        ** Must have a valid destination file table loaded...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_SIZE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST SIZE command: destination file table is not loaded");
    }
    else
    {
        /*
        ** Set size limit for destination file...
        */
        pDest = &DS_AppData.DestFileTblPtr->File[DS_DestSizeCmd->FileTableIndex];
        pDest->MaxFileSize = DS_DestSizeCmd->MaxFileSize;

        /*
        ** Notify cFE that we have modified the table data...
        */
        CFE_TBL_Modified(DS_AppData.DestFileTblHandle);

        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_SIZE_CMD_EID, CFE_EVS_DEBUG,
                         "DEST SIZE command: file table index = %d, size limit = %d",
                          (int)DS_DestSizeCmd->FileTableIndex, (int)DS_DestSizeCmd->MaxFileSize);
    }

    return;

} /* End of DS_CmdSetDestSize() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdSetDestAge() - set maximum file age limit                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdSetDestAge(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_DestAgeCmd_t *DS_DestAgeCmd = (DS_DestAgeCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_DestAgeCmd_t);
    DS_DestFileEntry_t *pDest;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_AGE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST AGE command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_TableVerifyFileIndex(DS_DestAgeCmd->FileTableIndex) == FALSE)
    {
        /*
        ** Invalid destination file table index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_AGE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST AGE command arg: file table index = %d",
                          (int)DS_DestAgeCmd->FileTableIndex);
    }
    else if (DS_TableVerifyAge(DS_DestAgeCmd->MaxFileAge) == FALSE)
    {
        /*
        ** Invalid destination file age limit...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_AGE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST AGE command arg: age limit = %d",
                          (int)DS_DestAgeCmd->MaxFileAge);
    }
    else if (DS_AppData.DestFileTblPtr == (DS_DestFileTable_t *) NULL)
    {
        /*
        ** Must have a valid destination file table loaded...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_AGE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST AGE command: destination file table is not loaded");
    }
    else
    {
        /*
        ** Set age limit for destination file...
        */
        pDest = &DS_AppData.DestFileTblPtr->File[DS_DestAgeCmd->FileTableIndex];
        pDest->MaxFileAge = DS_DestAgeCmd->MaxFileAge;

        /*
        ** Notify cFE that we have modified the table data...
        */
        CFE_TBL_Modified(DS_AppData.DestFileTblHandle);

        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_AGE_CMD_EID, CFE_EVS_DEBUG,
                         "DEST AGE command: file table index = %d, age limit = %d",
                          (int)DS_DestAgeCmd->FileTableIndex, (int)DS_DestAgeCmd->MaxFileAge);
    }

    return;

} /* End of DS_CmdSetDestAge() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdSetDestCount() - set seq cnt portion of filename          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdSetDestCount(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_DestCountCmd_t *DS_DestCountCmd = (DS_DestCountCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_DestCountCmd_t);
    DS_AppFileStatus_t *FileStatus;
    DS_DestFileEntry_t *DestFile;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_SEQ_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST COUNT command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_TableVerifyFileIndex(DS_DestCountCmd->FileTableIndex) == FALSE)
    {
        /*
        ** Invalid destination file table index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_SEQ_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST COUNT command arg: file table index = %d",
                          (int)DS_DestCountCmd->FileTableIndex);
    }
    else if (DS_TableVerifyCount(DS_DestCountCmd->SequenceCount) == FALSE)
    {
        /*
        ** Invalid destination file sequence count...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_SEQ_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST COUNT command arg: sequence count = %d",
                          (int)DS_DestCountCmd->SequenceCount);
    }
    else if (DS_AppData.DestFileTblPtr == (DS_DestFileTable_t *) NULL)
    {
        /*
        ** Must have a valid destination file table loaded...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_SEQ_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST COUNT command: destination file table is not loaded");
    }
    else
    {
        /*
        ** Set next sequence count for destination file...
        */
        DestFile = &DS_AppData.DestFileTblPtr->File[DS_DestCountCmd->FileTableIndex];
        FileStatus = &DS_AppData.FileStatus[DS_DestCountCmd->FileTableIndex];

        /*
        ** Update both destination file table and current status...
        */
        DestFile->SequenceCount = DS_DestCountCmd->SequenceCount;
        FileStatus->FileCount = DS_DestCountCmd->SequenceCount;

        /*
        ** Notify cFE that we have modified the table data...
        */
        CFE_TBL_Modified(DS_AppData.DestFileTblHandle);

        /*
        ** Update the Critical Data Store (CDS)...
        */
        DS_TableUpdateCDS();

        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_SEQ_CMD_EID, CFE_EVS_DEBUG,
                         "DEST COUNT command: file table index = %d, sequence count = %d",
                          (int)DS_DestCountCmd->FileTableIndex, (int)DS_DestCountCmd->SequenceCount);
    }

    return;

} /* End of DS_CmdSetDestCount() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdCloseFile() - close destination file                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdCloseFile(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_CloseFileCmd_t *DS_CloseFileCmd = (DS_CloseFileCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_CloseFileCmd_t);

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_CLOSE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST CLOSE command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_TableVerifyFileIndex(DS_CloseFileCmd->FileTableIndex) == FALSE)
    {
        /*
        ** Invalid destination file table index...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_CLOSE_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST CLOSE command arg: file table index = %d",
                          (int)DS_CloseFileCmd->FileTableIndex);
    }
    else
    {
        /*
        ** Close destination file (if the file was open)...
        */
        if (DS_AppData.FileStatus[DS_CloseFileCmd->FileTableIndex].FileHandle != DS_CLOSED_FILE_HANDLE)
        {
            DS_FileUpdateHeader(DS_CloseFileCmd->FileTableIndex);
            DS_FileCloseDest(DS_CloseFileCmd->FileTableIndex);
        }

        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_CLOSE_CMD_EID, CFE_EVS_DEBUG,
                         "DEST CLOSE command: file table index = %d",
                          (int)DS_CloseFileCmd->FileTableIndex);
    }

    return;

} /* End of DS_CmdCloseFile() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdCloseAll() - close all open destination files             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdCloseAll(CFE_SB_MsgPtr_t MessagePtr)
{
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_CloseAllCmd_t);
    int32  i;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_CLOSE_ALL_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid DEST CLOSE ALL command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else
    {
        /*
        ** Close all open destination files...
        */
        for (i = 0; i < DS_DEST_FILE_CNT; i++)
        {
            if (DS_AppData.FileStatus[i].FileHandle != DS_CLOSED_FILE_HANDLE)
            {
                DS_FileUpdateHeader(i);
                DS_FileCloseDest(i);
            }
        }

        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_CLOSE_ALL_CMD_EID, CFE_EVS_DEBUG,
                         "DEST CLOSE ALL command");
    }

    return;

} /* End of DS_CmdCloseAll() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdGetFileInfo() - get file info packet                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdGetFileInfo(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_FileInfoPkt_t DS_FileInfoPkt;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_GetFileInfoCmd_t);
    int32 i;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_GET_FILE_INFO_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid GET FILE INFO command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else
    {
        /*
        ** Create and send a file info packet...
        */
        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_GET_FILE_INFO_CMD_EID, CFE_EVS_DEBUG, "GET FILE INFO command");

        /*
        ** Initialize file info telemetry packet...
        */
        CFE_SB_InitMsg(&DS_FileInfoPkt, DS_DIAG_TLM_MID, sizeof(DS_FileInfoPkt_t), TRUE);

        /*
        ** Process array of destination file info data...
        */
        for (i = 0; i < DS_DEST_FILE_CNT; i++)
        {
            /*
            ** Set file age and size...
            */
            DS_FileInfoPkt.FileInfo[i].FileAge  = DS_AppData.FileStatus[i].FileAge;
            DS_FileInfoPkt.FileInfo[i].FileSize = DS_AppData.FileStatus[i].FileSize;

            /*
            ** Set file growth rate (computed when process last HK request)...
            */
            DS_FileInfoPkt.FileInfo[i].FileRate = DS_AppData.FileStatus[i].FileRate;

            /*
            ** Set current filename sequence count...
            */
            DS_FileInfoPkt.FileInfo[i].SequenceCount = DS_AppData.FileStatus[i].FileCount;

            /*
            ** Set file enable/disable state...
            */
            if (DS_AppData.DestFileTblPtr == (DS_DestFileTable_t *) NULL)
            {
                DS_FileInfoPkt.FileInfo[i].EnableState = DS_DISABLED;
            }
            else
            {
                DS_FileInfoPkt.FileInfo[i].EnableState = DS_AppData.FileStatus[i].FileState;
            }

            /*
            ** Set file open/closed state...
            */
            if (DS_AppData.FileStatus[i].FileHandle == DS_CLOSED_FILE_HANDLE)
            {
                DS_FileInfoPkt.FileInfo[i].OpenState = DS_CLOSED;
            }
            else
            {
                DS_FileInfoPkt.FileInfo[i].OpenState = DS_OPEN;

                /*
                ** Set current open filename...
                */
                strcpy(DS_FileInfoPkt.FileInfo[i].FileName, DS_AppData.FileStatus[i].FileName);
            }
        }

        /*
        ** Timestamp and send file info telemetry packet...
        */
        CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &DS_FileInfoPkt);
        CFE_SB_SendMsg((CFE_SB_Msg_t *) &DS_FileInfoPkt);
    }

    return;

} /* End of DS_CmdGetFileInfo() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_CmdAddMID() - add message ID to packet filter table          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_CmdAddMID(CFE_SB_MsgPtr_t MessagePtr)
{
    DS_AddMidCmd_t *DS_AddMidCmd = (DS_AddMidCmd_t *) MessagePtr;
    uint16 ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
    uint16 ExpectedLength = sizeof(DS_AddMidCmd_t);
    DS_PacketEntry_t *pPacketEntry;
    DS_FilterParms_t *pFilterParms;
    int32 FilterTableIndex;
    int32 HashTableIndex;
    int32 i;

    if (ExpectedLength != ActualLength)
    {
        /*
        ** Invalid command packet length...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_ADD_MID_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid ADD MID command length: expected = %d, actual = %d",
                          ExpectedLength, ActualLength);
    }
    else if (DS_AddMidCmd->MessageID == DS_UNUSED)
    {
        /*
        ** Invalid packet message ID - can be anything but unused...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_ADD_MID_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid ADD MID command arg: invalid MID = 0x%4X",
                          DS_AddMidCmd->MessageID);
    }
    else if (DS_AppData.FilterTblPtr == (DS_FilterTable_t *) NULL)
    {
        /*
        ** Must have a valid packet filter table loaded...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_ADD_MID_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid ADD MID command: filter table is not loaded");
    }
    else if ((FilterTableIndex = DS_TableFindMsgID(DS_AddMidCmd->MessageID)) != DS_INDEX_NONE)
    {
        /*
        ** New message ID is already in packet filter table...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_ADD_MID_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid ADD MID command: MID = 0x%4X is already in filter table at index = %d",
                          DS_AddMidCmd->MessageID, (int)FilterTableIndex);
    }
    else if ((FilterTableIndex = DS_TableFindMsgID(DS_UNUSED)) == DS_INDEX_NONE)
    {
        /*
        ** Packet filter table has no unused entries...
        */
        DS_AppData.CmdRejectedCounter++;

        CFE_EVS_SendEvent(DS_ADD_MID_CMD_ERR_EID, CFE_EVS_ERROR,
                         "Invalid ADD MID command: filter table is full");
    }
    else
    {
        /*
        ** Initialize unused packet filter entry for new message ID...
        */
        pPacketEntry = &DS_AppData.FilterTblPtr->Packet[FilterTableIndex];

        pPacketEntry->MessageID = DS_AddMidCmd->MessageID;
        
        /* Add the message ID to the hash table as well */
        HashTableIndex = DS_TableAddMsgID(DS_AddMidCmd->MessageID, FilterTableIndex);

        for (i = 0; i < DS_FILTERS_PER_PACKET; i++)
        {
            pFilterParms = &pPacketEntry->Filter[i];

            pFilterParms->FileTableIndex = 0;
            pFilterParms->FilterType = DS_BY_COUNT;

            pFilterParms->Algorithm_N = 0;
            pFilterParms->Algorithm_X = 0;
            pFilterParms->Algorithm_O = 0;
        }

        CFE_SB_SubscribeEx(DS_AddMidCmd->MessageID, DS_AppData.InputPipe,
                           CFE_SB_Default_Qos, DS_PER_PACKET_PIPE_LIMIT);
        /*
        ** Notify cFE that we have modified the table data...
        */
        CFE_TBL_Modified(DS_AppData.FilterTblHandle);

        DS_AppData.CmdAcceptedCounter++;

        CFE_EVS_SendEvent(DS_ADD_MID_CMD_EID, CFE_EVS_DEBUG,
                         "ADD MID command: MID = 0x%04X, index = %d",
                          DS_AddMidCmd->MessageID, (int)FilterTableIndex);
    }

    return;

} /* End of DS_CmdAddMID() */


/************************/
/*  End of File Comment */
/************************/
