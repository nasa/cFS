/************************************************************************
** File:
**   $Id: ds_app.c 1.4 2017/01/25 12:05:28EST sstrege Exp  $
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
**  The CFS Data Storage (DS) Application file containing the application
**  initialization routines, the main routine and the command interface.
**
** Notes:
**
*************************************************************************/

#include "cfe.h"

#include "ds_perfids.h"
#include "ds_msgids.h"

#include "ds_platform_cfg.h"
#include "ds_verify.h"

#include "ds_appdefs.h"

#include "ds_msg.h"
#include "ds_app.h"
#include "ds_cmds.h"
#include "ds_file.h"
#include "ds_table.h"
#include "ds_events.h"
#include "ds_msgdefs.h"
#include "ds_version.h"

#include "string.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_AppData -- application global data structure                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DS_AppData_t DS_AppData;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_AppMain() -- application entry point and main process loop   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_AppMain(void)
{
    CFE_SB_MsgPtr_t MessagePtr;
    int32 Result;
    uint32 RunStatus = CFE_ES_APP_RUN;

    /*
    ** Performance Log (start time counter)...
    */
    CFE_ES_PerfLogEntry(DS_APPMAIN_PERF_ID);

    /*
    ** Register application...
    */
    Result = CFE_ES_RegisterApp();

    /*
    ** Perform application specific initialization...
    */
    if (Result == CFE_SUCCESS)
    {
        Result = DS_AppInitialize();
    }

    /*
    ** Check for start-up error...
    */
    if (Result != CFE_SUCCESS)
    {
        /*
        ** Set request to terminate main loop...
        */
        RunStatus = CFE_ES_APP_ERROR;
    }

    /*
    ** Main process loop...
    */
    while (CFE_ES_RunLoop(&RunStatus))
    {
        /*
        ** Performance Log (stop time counter)...
        */
        CFE_ES_PerfLogExit(DS_APPMAIN_PERF_ID);

        /*
        ** Wait for next Software Bus message...
        */
        Result = CFE_SB_RcvMsg(&MessagePtr, DS_AppData.InputPipe, CFE_SB_PEND_FOREVER);

        /*
        ** Performance Log (start time counter)...
        */
        CFE_ES_PerfLogEntry(DS_APPMAIN_PERF_ID);

        /*
        ** Process Software Bus messages...
        */
        if (Result == CFE_SUCCESS)
        {
            DS_AppProcessMsg(MessagePtr);
        }
        else
        {
            /*
            ** Set request to terminate main loop...
            */
            RunStatus = CFE_ES_APP_ERROR;
        }

        /*
        ** Note: If there were some reason to exit normally
        **       (without error) then we would set
        **       RunStatus = CFE_ES_APP_EXIT
        */
    }

    /*
    ** Check for "fatal" process error...
    */
    if (Result != CFE_SUCCESS)
    {
        /*
        ** Send an event describing the reason for the termination...
        */
        CFE_EVS_SendEvent(DS_EXIT_ERR_EID, CFE_EVS_CRITICAL,
                         "Application terminating, err = 0x%08X", (unsigned int)Result);

        /*
        ** In case cFE Event Services is not working...
        */
        CFE_ES_WriteToSysLog("DS application terminating, err = 0x%08X\n", (unsigned int)Result);
    }

    /*
    ** Performance Log (stop time counter)...
    */
    CFE_ES_PerfLogExit(DS_APPMAIN_PERF_ID);

    /*
    ** Let cFE kill the application (and any child tasks)...
    */
    CFE_ES_ExitApp(RunStatus);

} /* End of DS_AppMain() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_AppInitialize() -- application initialization                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int32 DS_AppInitialize(void)
{
    int32 Result = CFE_SUCCESS;
    int32 i;

    /*
    ** Initialize global data structure...
    */
    CFE_PSP_MemSet(&DS_AppData, 0, sizeof(DS_AppData_t));

    DS_AppData.AppEnableState = DS_DEF_ENABLE_STATE;

    /*
    ** Mark files as closed (cFE uses zero as a valid file handle)...
    */
    for (i = 0; i < DS_DEST_FILE_CNT; i++)
    {
        DS_AppData.FileStatus[i].FileHandle = DS_CLOSED_FILE_HANDLE;
    }

    /*
    ** Initialize interface to cFE Event Services...
    */
    if (Result == CFE_SUCCESS)
    {
        Result = CFE_EVS_Register(NULL, 0, 0);

        if (Result != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(DS_INIT_ERR_EID, CFE_EVS_ERROR,
               "Unable to register for EVS services, err = 0x%08X", (unsigned int)Result);
        }
    }

    /*
    ** Create application Software Bus message pipe...
    */
    if (Result == CFE_SUCCESS)
    {
        Result = CFE_SB_CreatePipe(&DS_AppData.InputPipe,
                                    DS_APP_PIPE_DEPTH, DS_APP_PIPE_NAME);
        if (Result != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(DS_INIT_ERR_EID, CFE_EVS_ERROR,
               "Unable to create input pipe, err = 0x%08X", (unsigned int)Result);
        }
    }

    /*
    ** Subscribe to application housekeeping request command...
    */
    if (Result == CFE_SUCCESS)
    {
        Result = CFE_SB_Subscribe(DS_SEND_HK_MID, DS_AppData.InputPipe);

        if (Result != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(DS_INIT_ERR_EID, CFE_EVS_ERROR,
               "Unable to subscribe to HK request, err = 0x%08X", (unsigned int)Result);
        }
    }

    /*
    ** Subscribe to application commands...
    */
    if (Result == CFE_SUCCESS)
    {
        Result = CFE_SB_Subscribe(DS_CMD_MID, DS_AppData.InputPipe);

        if (Result != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(DS_INIT_ERR_EID, CFE_EVS_ERROR,
               "Unable to subscribe to DS commands, err = 0x%08X", (unsigned int)Result);
        }
    }

    /*
    ** Initialize application tables...
    */
    if (Result == CFE_SUCCESS)
    {
        Result = DS_TableInit();
    }

    /*
    ** Initialize access to Critical Data Store (CDS)...
    */
    if (Result == CFE_SUCCESS)
    {
        Result = DS_TableCreateCDS();
    }

    /*
    ** Generate application startup event message...
    */
    if (Result == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(DS_INIT_EID, CFE_EVS_INFORMATION,
                         "Application initialized, version %d.%d.%d.%d, data at 0x%08X",
                          DS_MAJOR_VERSION, DS_MINOR_VERSION,
                          DS_REVISION, DS_MISSION_REV, (unsigned int) &DS_AppData);
    }

    return(Result);

} /* End of DS_AppInitialize() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_AppProcessMsg() -- process Software Bus messages             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_AppProcessMsg(CFE_SB_MsgPtr_t MessagePtr)
{
    CFE_SB_MsgId_t MessageID = CFE_SB_GetMsgId(MessagePtr);
    uint16 ActualLength;
    uint16 ExpectedLength;

    switch (MessageID)
    {
        /*
        ** DS application commands...
        */
        case DS_CMD_MID:
            DS_AppProcessCmd(MessagePtr);
            if (DS_TableFindMsgID(MessageID) != DS_INDEX_NONE)
            {
                DS_AppStorePacket(MessageID, MessagePtr);
            }
            break;

        /*
        ** DS housekeeping request command...
        */
        case DS_SEND_HK_MID:

            ActualLength = CFE_SB_GetTotalMsgLength(MessagePtr);
            ExpectedLength = CFE_SB_CMD_HDR_SIZE;
            if (ExpectedLength != ActualLength)
            {
                CFE_EVS_SendEvent(DS_HK_REQUEST_ERR_EID, CFE_EVS_ERROR,
                   "Invalid HK request length: expected = %d, actual = %d",
                    ExpectedLength, ActualLength);
            }
            else
            {
                DS_AppProcessHK();
                if (DS_TableFindMsgID(MessageID) != DS_INDEX_NONE)
                {
                    DS_AppStorePacket(MessageID, MessagePtr);
                }
            }
            break;

        /*
        ** Unknown message ID's (must be something to store)...
        */
        default:
            DS_AppStorePacket(MessageID, MessagePtr);
            break;
    }

    return;

} /* End of DS_AppProcessMsg() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_AppProcessCmd() -- process application commands              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_AppProcessCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    uint16 CommandCode  = CFE_SB_GetCmdCode(MessagePtr);

    switch (CommandCode)
    {
        /*
        ** Do nothing command (aliveness test)...
        */
        case DS_NOOP_CC:
            DS_CmdNoop(MessagePtr);
            break;

        /*
        ** Set housekeeping telemetry counters to zero...
        */
        case DS_RESET_CC:
            DS_CmdReset(MessagePtr);
            break;

        /*
        ** Set DS application enable/disable state...
        */
        case DS_SET_APP_STATE_CC:
            DS_CmdSetAppState(MessagePtr);
            break;

        /*
        ** Set packet filter file index...
        */
        case DS_SET_FILTER_FILE_CC:
            DS_CmdSetFilterFile(MessagePtr);
            break;

        /*
        ** Set packet filter type (time vs count)...
        */
        case DS_SET_FILTER_TYPE_CC:
            DS_CmdSetFilterType(MessagePtr);
            break;

        /*
        ** Set packet filter algorithm parameters...
        */
        case DS_SET_FILTER_PARMS_CC:
            DS_CmdSetFilterParms(MessagePtr);
            break;

        /*
        ** Set destination file filename type (time vs count)...
        */
        case DS_SET_DEST_TYPE_CC:
            DS_CmdSetDestType(MessagePtr);
            break;

        /*
        ** Set destination file enable/disable state...
        */
        case DS_SET_DEST_STATE_CC:
            DS_CmdSetDestState(MessagePtr);
            break;

        /*
        ** Set destination file path portion of filename...
        */
        case DS_SET_DEST_PATH_CC:
            DS_CmdSetDestPath(MessagePtr);
            break;

        /*
        ** Set destination file base portion of filename...
        */
        case DS_SET_DEST_BASE_CC:
            DS_CmdSetDestBase(MessagePtr);
            break;

        /*
        ** Set destination file extension portion of filename...
        */
        case DS_SET_DEST_EXT_CC:
            DS_CmdSetDestExt(MessagePtr);
            break;

        /*
        ** Set destination file maximum size limit...
        */
        case DS_SET_DEST_SIZE_CC:
            DS_CmdSetDestSize(MessagePtr);
            break;

        /*
        ** Set destination file maximum age limit...
        */
        case DS_SET_DEST_AGE_CC:
            DS_CmdSetDestAge(MessagePtr);
            break;

        /*
        ** Set destination file sequence count portion of filename...
        */
        case DS_SET_DEST_COUNT_CC:
            DS_CmdSetDestCount(MessagePtr);
            break;

        /*
        ** Close destination file (next packet will re-open)...
        */
        case DS_CLOSE_FILE_CC:
            DS_CmdCloseFile(MessagePtr);
            break;

        /*
        ** Get file info telemetry packet...
        */
        case DS_GET_FILE_INFO_CC:
            DS_CmdGetFileInfo(MessagePtr);
            break;

        /*
        ** Add message ID to filter table...
        */
        case DS_ADD_MID_CC:
            DS_CmdAddMID(MessagePtr);
            break;

        /*
        ** Close all destination files (next packet will re-open)...
        */
        case DS_CLOSE_ALL_CC:
            DS_CmdCloseAll(MessagePtr);
            break;

        /*
        ** DS application command with unknown command code...
        */
        default:
            CFE_EVS_SendEvent(DS_CMD_CODE_ERR_EID, CFE_EVS_ERROR,
                             "Invalid command code: MID = 0x%04X, CC = %d",
                              DS_CMD_MID, CommandCode);

            DS_AppData.CmdRejectedCounter++;
            break;
    }

    return;

} /* End of DS_AppProcessCmd() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_AppProcessHK() -- process hk request command                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_AppProcessHK(void)
{
    DS_HkPacket_t HkPacket;
    int32 i;
    char FilterTblName[CFE_TBL_MAX_NAME_LENGTH];
    CFE_TBL_Info_t FilterTblInfo;

    /*
    ** Initialize housekeeping packet...
    */
    CFE_SB_InitMsg(&HkPacket, DS_HK_TLM_MID, sizeof(DS_HkPacket_t), TRUE);

    /*
    ** Process data storage file age limits...
    */
    DS_FileTestAge(DS_SECS_PER_HK_CYCLE);

    /*
    ** Take this opportunity to check for table updates...
    */
    DS_TableManageDestFile();
    DS_TableManageFilter();

    /*
    ** Copy application command counters to housekeeping telemetry packet...
    */
    HkPacket.CmdAcceptedCounter = DS_AppData.CmdAcceptedCounter;
    HkPacket.CmdRejectedCounter = DS_AppData.CmdRejectedCounter;

    /*
    ** Copy packet storage counters to housekeeping telemetry packet...
    */
    HkPacket.DisabledPktCounter = DS_AppData.DisabledPktCounter;
    HkPacket.IgnoredPktCounter  = DS_AppData.IgnoredPktCounter;
    HkPacket.FilteredPktCounter = DS_AppData.FilteredPktCounter;
    HkPacket.PassedPktCounter   = DS_AppData.PassedPktCounter;

    /*
    ** Copy file I/O counters to housekeeping telemetry packet...
    */
    HkPacket.FileWriteCounter     = DS_AppData.FileWriteCounter;
    HkPacket.FileWriteErrCounter  = DS_AppData.FileWriteErrCounter;
    HkPacket.FileUpdateCounter    = DS_AppData.FileUpdateCounter;
    HkPacket.FileUpdateErrCounter = DS_AppData.FileUpdateErrCounter;

    /*
    ** Copy configuration table counters to housekeeping telemetry packet...
    */
    HkPacket.DestTblLoadCounter   = DS_AppData.DestTblLoadCounter;
    HkPacket.DestTblErrCounter    = DS_AppData.DestTblErrCounter;
    HkPacket.FilterTblLoadCounter = DS_AppData.FilterTblLoadCounter;
    HkPacket.FilterTblErrCounter  = DS_AppData.FilterTblErrCounter;

    /*
    ** Copy app enable/disable state to housekeeping telemetry packet...
    */
    HkPacket.AppEnableState = DS_AppData.AppEnableState;

    /*
    ** Compute file growth rate from number of bytes since last HK request...
    */
    for (i = 0; i < DS_DEST_FILE_CNT; i++)
    {
        DS_AppData.FileStatus[i].FileRate = DS_AppData.FileStatus[i].FileGrowth / DS_SECS_PER_HK_CYCLE;
        DS_AppData.FileStatus[i].FileGrowth = 0;
    }

    /* Get the filter table info, put the file name in the hkp pkt. */
    sprintf(FilterTblName,"DS.%s",DS_FILTER_TBL_NAME);
    CFE_TBL_GetInfo(&FilterTblInfo, FilterTblName);
    strncpy(HkPacket.FilterTblFilename, FilterTblInfo.LastFileLoaded, OS_MAX_PATH_LEN);

    /*
    ** Timestamp and send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &HkPacket);
    CFE_SB_SendMsg((CFE_SB_Msg_t *) &HkPacket);

    return;

} /* End of DS_AppProcessHK() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* DS_AppStorePacket() -- packet storage pre-processor             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void DS_AppStorePacket(CFE_SB_MsgId_t MessageID, CFE_SB_MsgPtr_t MessagePtr)
{

    if (DS_AppData.AppEnableState == DS_DISABLED)
    {
        /*
        ** Application must be enabled in order to store data...
        */
        DS_AppData.DisabledPktCounter++;
    }
    else if ((DS_AppData.FilterTblPtr == (DS_FilterTable_t *) NULL) ||
             (DS_AppData.DestFileTblPtr == (DS_DestFileTable_t *) NULL))
    {
        /*
        ** Must have both tables loaded in order to store data...
        */
        DS_AppData.IgnoredPktCounter++;
    }
    else
    {
        /*
        ** Store packet (if permitted by filter table)...
        */
        DS_FileStorePacket(MessageID, MessagePtr);
    }

    return;

} /* End of DS_AppStorePacket() */

/************************/
/*  End of File Comment */
/************************/
