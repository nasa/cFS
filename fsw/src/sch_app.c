/*
** $Id: sch_app.c 1.15 2015/03/01 14:01:50EST sstrege Exp  $
**
**  Copyright © 2007-2014 United States Government as represented by the 
**  Administrator of the National Aeronautics and Space Administration. 
**  All Other Rights Reserved.  
**
**  This software was created at NASA's Goddard Space Flight Center.
**  This software is governed by the NASA Open Source Agreement and may be 
**  used, distributed and modified only pursuant to the terms of that 
**  agreement.
**
** Purpose: Scheduler (SCH) application
**
** Author:
**
** Notes:
**
** $Log: sch_app.c  $
** Revision 1.15 2015/03/01 14:01:50EST sstrege 
** Added copyright information
** Revision 1.14 2012/06/26 13:56:10EDT lwalling 
** Added call to CFE_TBL_Modified() after change to shedule table entry enable/disable state
** Revision 1.13 2011/06/30 17:38:56PDT aschoeni 
** updated OS_SUCCESS to CFE_SUCCESS for custom earlyinit
** Revision 1.12 2011/06/30 15:33:08EDT aschoeni 
** removed customizable components to separate file
** Revision 1.11 2011/06/30 14:03:39EDT aschoeni 
** Added sch library for inhibiting schedule
** Revision 1.10 2011/06/02 16:19:26EDT aschoeni 
** Updated platform config and verification
** Revision 1.9 2011/05/24 18:08:13EDT aschoeni 
** Moved Major frame event outside of callback
** Revision 1.8 2011/03/30 17:25:43EDT aschoeni 
** Added a minimum message ID limit for defined messages
** Revision 1.7 2011/03/30 16:05:00EDT aschoeni 
** Added error codes for primary key in each table (enablestate and msgid)
** Revision 1.6 2011/03/29 18:49:51EDT aschoeni 
** MessageIndex of 0 is now considered a bad schedule table entry.
** Revision 1.5 2009/03/27 09:58:19EDT dkobe 
** Changed Table Validation Return Codes to be negative to avoid cFE 5.2 TBL Services bug
** Revision 1.4 2009/03/27 00:24:23EDT dkobe 
** Added consecutive noisy major frame counter and platform config parameter to compare counter to
** Revision 1.3 2009/03/26 16:05:08EDT dkobe 
** Added logic for determining worst case number of slots that could be executed in response to a poor minor frame timer signal.
** Revision 1.2 2009/03/26 15:42:04EDT dkobe 
** Moved 1 Hz Registration to after Startup Synchronization so that 1 Hz signals are not received too early.
** Revision 1.1 2008/10/16 15:08:32EDT dkobe 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/sch/fsw/src/project.pj
*/

/*************************************************************************
**
** Include section
**
**************************************************************************/

#include "cfe.h"

#include "sch_msgids.h"
#include "sch_perfids.h"
#include "sch_platform_cfg.h"

#if SCH_LIB_PRESENCE == 1
#include "sch_api.h"
#endif

#include "sch_custom.h"
#include "sch_msg.h"
#include "sch_events.h"
#include "sch_app.h"
#include "sch_cmds.h"
#include "sch_version.h"

#include "cfe_time_msg.h"
#include "sch_verify.h"

/*************************************************************************
**
** Macro definitions
**
**************************************************************************/

/*
** Time Semaphore Characteristics
*/
#define SCH_SEM_NAME     "SCH_TIME_SEM"
#define SCH_SEM_VALUE    0
#define SCH_SEM_OPTIONS  0

/*
** SDT Table Validation Error Codes
*/
#define SCH_SDT_GARBAGE_ENTRY    (-1)
#define SCH_SDT_NO_FREQUENCY     (-2)
#define SCH_SDT_BAD_REMAINDER    (-3)
#define SCH_SDT_BAD_ACTIVITY     (-4)
#define SCH_SDT_BAD_MSG_INDEX    (-5)
#define SCH_SDT_BAD_ENABLE_STATE (-6)

/*
** MDT Table Validation Error Codes
*/
#define SCH_MDT_GARBAGE_ENTRY   (-1)
#define SCH_MDT_INVALID_LENGTH  (-2)
#define SCH_MDT_BAD_MSG_ID      (-3)

/*************************************************************************
**
** Type definitions
**
**************************************************************************/

/*
** (none)
*/

/*************************************************************************
**
** Imported data
**
**************************************************************************/

/*
** (none)
*/

/*************************************************************************
**
** Exported data
**
**************************************************************************/

/*
** Application global data
*/
SCH_AppData_t           SCH_AppData;

/*************************************************************************
**
** File data
**
**************************************************************************/

/*
** (none)
*/

/*************************************************************************
** Local function prototypes
**************************************************************************/

/************************************************************************/
/** \brief Initialize the Scheduler CFS application
**  
**  \par Description
**       Scheduler application initialization routine. This 
**       function performs all the required startup steps to 
**       get the application registered with the cFE services so
**       it can begin to receive command messages. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32 SCH_AppInit(void);

/************************************************************************/
/** \brief Initialize the cFE Events with CFS Scheduler Application
**  
**  \par Description
**       This function performs those steps required to initialize the
**       relationship between the CFS Scheduler and the cFE Events Services. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32 SCH_EvsInit(void);

/************************************************************************/
/** \brief Initialize cFE Software Bus with CFS Scheduler Application
**  
**  \par Description
**       This function performs those steps required to initialize the
**       relationship between the CFS Scheduler and the cFE Software Bus. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32 SCH_SbInit(void);

/************************************************************************/
/** \brief Initialize cFE Table Services with CFS Scheduler Application
**  
**  \par Description
**       This function performs those steps required to initialize the
**       relationship between the CFS Scheduler and the cFE Table Services. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32 SCH_TblInit(void);

/************************************************************************/
/** \brief Initialize cFE Table Services with cFE Time Services and OSAL timer
**  
**  \par Description
**       This function performs those steps required to initialize the
**       relationship between the CFS Scheduler and cFE Time Services as
**       well as with the OS Abstraction Layer Timer Interface. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32 SCH_TimerInit(void);

/************************************************************************/
/** \brief Processes the Schedule Table to determine next Minor Frame
**  
**  \par Description
**       This function performs the highest level operations associated with
**       processing the Schedule Definition Table.  It determines which
**       minor frame schedule definitions are to be processed during this
**       cycle. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32 SCH_ProcessScheduleTable(void);

/************************************************************************/
/** \brief Processes the next minor frame in schedule definition table
**  
**  \par Description
**       This function performs the high level operations associated with
**       processing a minor frame definition in the Schedule Definition
**       Table. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32  SCH_ProcessNextSlot(void);

/************************************************************************/
/** \brief Processes the next entry in the current minor frame
**  
**  \par Description
**       This function processes the next entry in the current minor frame
**       as defined in the schedule definition table. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
*************************************************************************/
void   SCH_ProcessNextEntry(SCH_ScheduleEntry_t *NextEntry, int32 EntryNumber);

/************************************************************************/
/** \brief Processes commands received from cFE Software Bus
**  
**  \par Description
**       This function pulls messages from command pipe and processes
**       them accordingly. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32  SCH_ProcessCommands(void);

/************************************************************************/
/** \brief Validates contents of Schedule Definition Table
**  
**  \par Description
**       This function is called by table services when a validation of
**       the Schedule Definition Table is required. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32 SCH_ValidateScheduleData(void *TableData);

/************************************************************************/
/** \brief Validates contents of Message Definition Table
**  
**  \par Description
**       This function is called by table services when a validation of
**       the Message Definition Table is required. 
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32 SCH_ValidateMessageData(void *TableData);


/*************************************************************************
**
** Function definitions
**
**************************************************************************/

/*******************************************************************
**
** SCH_AppMain
**
** NOTE: For complete prolog information, see 'sch_app.h'
********************************************************************/

void SCH_AppMain(void)
{
    int32  Status    = CFE_SUCCESS;
    uint32 RunStatus = CFE_ES_APP_RUN;

    /*
    ** Performance Log (start time counter)
    */
    CFE_ES_PerfLogEntry(SCH_APPMAIN_PERF_ID);

    /*
    ** Register application
    */
    Status = CFE_ES_RegisterApp();

    /*
    ** Perform application specific initialization
    */
    if (Status == CFE_SUCCESS)
    {
        Status = SCH_AppInit();
    }
    
    /* If no errors were detected during initialization, then wait for everyone to start */
    if (Status == CFE_SUCCESS)
    {
        Status = SCH_CustomLateInit();
        if (Status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(SCH_MAJOR_FRAME_SUB_ERR_EID, CFE_EVS_ERROR,
                              "Error initializing Timers (RC=0x%08X)", 
                              Status);    
        }
    }

    /*
    ** Check for start-up error
    */
    if (Status != CFE_SUCCESS)
    {
        /*
        ** Set request to terminate main loop
        */
        RunStatus = CFE_ES_APP_ERROR;
    }

    /*
    ** Main process loop
    */
    while (CFE_ES_RunLoop(&RunStatus))
    {
        /*
        ** Performance Log (stop time counter)
        */
        CFE_ES_PerfLogExit(SCH_APPMAIN_PERF_ID);

        /*
        ** Wait for the next slot (Major or Minor Frame)
        */
        Status = OS_BinSemTake(SCH_AppData.TimeSemaphore);

        /*
        ** Performance Log (start time counter)
        */
        CFE_ES_PerfLogEntry(SCH_APPMAIN_PERF_ID);

        /*
        ** Report if during the previous frame the major has determined to be unstable
        */
        if (SCH_AppData.IgnoreMajorFrame == TRUE)
        {
            if (SCH_AppData.IgnoreMajorFrameMsgSent == FALSE)
            {
                CFE_EVS_SendEvent(SCH_NOISY_MAJOR_FRAME_ERR_EID, CFE_EVS_ERROR, 
                                  "Major Frame Sync too noisy (Slot %d). Disabling synchronization.", 
                                  SCH_AppData.MinorFramesSinceTone);
                SCH_AppData.IgnoreMajorFrameMsgSent = TRUE;
            }
        }
        else
        {
            SCH_AppData.IgnoreMajorFrameMsgSent = FALSE;
        }

        /*
        ** Process schedule table activities
        */
#if SCH_LIB_PRESENCE == 1
        if ((Status == OS_SUCCESS) &&
            (SCH_GetProcessingState() == TRUE))
        {
            Status = SCH_ProcessScheduleTable();
        }
#else
        if (Status == OS_SUCCESS)
        {
            Status = SCH_ProcessScheduleTable();
        }
#endif

        /*
        ** Note: If there were some reason to exit the task
        **       normally (without error) then we would set
        **       RunStatus = CFE_ES_APP_EXIT
        */
        if (Status != SCH_SUCCESS)
        {
            /*
            ** Set request to terminate main loop
            */
            RunStatus = CFE_ES_APP_ERROR;
        }

    } /* End of while */

    /*
    ** Check for "fatal" process error
    */
    if ((Status != OS_SUCCESS) || (Status != SCH_SUCCESS))
    {
        /*
        ** Send an event describing the reason for the termination
        */
        CFE_EVS_SendEvent(SCH_APP_EXIT_EID, CFE_EVS_CRITICAL, 
                          "SCH App: terminating, err = 0x%08X", Status);

        /*
        ** In case cFE Event Services is not working
        */
        CFE_ES_WriteToSysLog("SCH App terminating, err = 0x%08X\n", Status);
    }
 
    /*
    ** Perform any custom cleanup
    */
    SCH_CustomCleanup();

    /*
    ** Performance Log (stop time counter)
    */
    CFE_ES_PerfLogExit(SCH_APPMAIN_PERF_ID);

    /*
    ** Let cFE kill the task (and any child tasks)
    */
    CFE_ES_ExitApp(RunStatus);

} /* End of SH_AppMain() */


/*******************************************************************
**
** SCH_AppInit
**
** NOTE: For complete prolog information, see above
********************************************************************/

int32 SCH_AppInit(void)
{
    int32 Status = CFE_SUCCESS;

    /*
    ** Initialize Application Processing Counters
    */
    SCH_AppData.SlotsProcessedCount = 0;
    SCH_AppData.SkippedSlotsCount   = 0;
    SCH_AppData.MultipleSlotsCount  = 0;
    SCH_AppData.SameSlotCount       = 0;
    SCH_AppData.ScheduleActivitySuccessCount = 0;
    SCH_AppData.ScheduleActivityFailureCount = 0;

    /*
    ** Initialize Command Counters
    */
    SCH_AppData.CmdCounter = 0;
    SCH_AppData.ErrCounter = 0;
    
    /*
    ** Get our assigned Application ID
    */
    Status = CFE_ES_GetAppID(&SCH_AppData.AppID);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SCH App: Unable to obtain own AppID, RC=0x%08X\n", Status);
        return(Status);
    }
    
    /*
    ** Initialize Event Services Interface
    */
    Status = SCH_EvsInit();
    if (Status != CFE_SUCCESS)
    {
        return(Status);
    }
    
    Status = SCH_SbInit();
    if (Status != CFE_SUCCESS)
    {
        return(Status); 
    }
    
    /*
    ** Initialize application tables
    */
    Status = SCH_TblInit();
    if (Status != CFE_SUCCESS)
    {
        return(Status);
    }

    /*
    ** Initialize timer interfaces
    */
    Status = SCH_TimerInit();
    if (Status != CFE_SUCCESS)
    {
        return(Status);
    }

    /*
    ** Application startup event message
    */
    Status = CFE_EVS_SendEvent(SCH_INITSTATS_INF_EID,
                               CFE_EVS_INFORMATION,
                               "SCH Initialized. Version %d.%d.%d.%d",
                               SCH_MAJOR_VERSION,
                               SCH_MINOR_VERSION,
                               SCH_REVISION,
                               SCH_MISSION_REV);
 
    return(Status);

} /* End of SCH_AppInit() */


/*******************************************************************
**
** SCH_EvsInit
**
** NOTE: For complete prolog information, see above
********************************************************************/

int32 SCH_EvsInit(void)
{
    int32   Status = CFE_SUCCESS;

    SCH_AppData.EventFilters[0].EventID = SCH_SAME_SLOT_EID;
    SCH_AppData.EventFilters[0].Mask    = CFE_EVS_FIRST_ONE_STOP;

    SCH_AppData.EventFilters[1].EventID = SCH_MULTI_SLOTS_EID;
    SCH_AppData.EventFilters[1].Mask    = CFE_EVS_FIRST_ONE_STOP;

    SCH_AppData.EventFilters[2].EventID = SCH_SKIPPED_SLOTS_EID;
    SCH_AppData.EventFilters[2].Mask    = CFE_EVS_FIRST_ONE_STOP;

    SCH_AppData.EventFilters[3].EventID = SCH_CORRUPTION_EID;
    SCH_AppData.EventFilters[3].Mask    = CFE_EVS_FIRST_TWO_STOP;

    SCH_AppData.EventFilters[4].EventID = SCH_PACKET_SEND_EID;
    SCH_AppData.EventFilters[4].Mask    = CFE_EVS_FIRST_ONE_STOP;

    /*
    ** Register for event services
    */
    Status = CFE_EVS_Register(SCH_AppData.EventFilters, SCH_FILTER_COUNT, CFE_EVS_BINARY_FILTER);
    if (Status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("SCH App: Error Registering For Event Services, RC=0x%08X\n", Status);
    }

    return(Status);
    
} /* End of SCH_EvsInit() */


/*******************************************************************
**
** SCH_SbInit
**
** NOTE: For complete prolog information, see above
********************************************************************/

int32 SCH_SbInit(void)
{
    int32 Status = CFE_SUCCESS;
    
    SCH_AppData.MsgPtr  = (CFE_SB_MsgPtr_t) NULL;
    SCH_AppData.CmdPipe = 0;
    
    /*
    ** Initialize housekeeping packet (clear user data area)
    */
    CFE_SB_InitMsg(&SCH_AppData.HkPacket, SCH_HK_TLM_MID, sizeof(SCH_HkPacket_t), TRUE);
    
    /*
    ** Initialize diagnostic packet (clear user data area)
    */
    CFE_SB_InitMsg(&SCH_AppData.DiagPacket, SCH_DIAG_TLM_MID, sizeof(SCH_DiagPacket_t), TRUE);

    /*
    ** Create Software Bus message pipe
    */
    Status = CFE_SB_CreatePipe(&SCH_AppData.CmdPipe, SCH_PIPE_DEPTH, SCH_PIPE_NAME);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SCH_CR_PIPE_ERR_EID, CFE_EVS_ERROR,
                          "Error Creating SB Pipe, RC=0x%08X", Status);
        return(Status);
    }

    /*
    ** Subscribe to Housekeeping request commands
    */
    Status = CFE_SB_Subscribe(SCH_SEND_HK_MID, SCH_AppData.CmdPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SCH_SUB_HK_REQ_ERR_EID, CFE_EVS_ERROR,
                          "Error Subscribing to HK Request(MID=0x%04X), RC=0x%08X", 
                          SCH_SEND_HK_MID, Status);    
        return(Status);
    }

    /*
    ** Subscribe to SCH ground command packets
    */
    Status = CFE_SB_Subscribe(SCH_CMD_MID, SCH_AppData.CmdPipe);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SCH_SUB_GND_CMD_ERR_EID, CFE_EVS_ERROR,
                          "Error Subscribing to GND CMD(MID=0x%04X), RC=0x%08X", 
                          SCH_CMD_MID, Status);    
        return(Status);
    }

    return(Status);
    
} /* End of SCH_SbInit() */


/*******************************************************************
**
** SCH_TblInit
**
** NOTE: For complete prolog information, see above
********************************************************************/

int32 SCH_TblInit(void)
{
    uint32 TableSize = 0;
    int32  Status = CFE_SUCCESS;

    /*
    ** Initialize SCH table variables
    */
    SCH_AppData.ScheduleTable = (SCH_ScheduleEntry_t *) NULL;
    SCH_AppData.MessageTable  = (SCH_MessageEntry_t  *) NULL;

    SCH_AppData.ScheduleTableHandle = CFE_TBL_BAD_TABLE_HANDLE;
    SCH_AppData.MessageTableHandle  = CFE_TBL_BAD_TABLE_HANDLE;
    
    SCH_AppData.BadTableDataCount       = 0;
    SCH_AppData.TableVerifySuccessCount = 0;
    SCH_AppData.TableVerifyFailureCount = 0;
    SCH_AppData.TablePassCount          = 0;

    /*
    ** Register schedule definition table
    */
    TableSize = SCH_TABLE_ENTRIES * sizeof (SCH_ScheduleEntry_t);

    Status = CFE_TBL_Register(&SCH_AppData.ScheduleTableHandle,
                               SCH_SCHEDULE_TABLE_NAME,
                               TableSize,
                               CFE_TBL_OPT_DEFAULT,
                               SCH_ValidateScheduleData);

    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SCH_SDT_REG_ERR_EID, CFE_EVS_ERROR,
                          "Error Registering SDT, RC=0x%08X", 
                          Status);    
        return(Status);
    }

    /*
    ** Register message definition table
    */
    TableSize = SCH_MAX_MESSAGES * sizeof (SCH_MessageEntry_t);

    Status = CFE_TBL_Register(&SCH_AppData.MessageTableHandle,
                               SCH_MESSAGE_TABLE_NAME,
                               TableSize,
                               CFE_TBL_OPT_DEFAULT,
                               SCH_ValidateMessageData);

    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SCH_MDT_REG_ERR_EID, CFE_EVS_ERROR,
                          "Error Registering MDT, RC=0x%08X", 
                          Status);    
        return(Status);
    }

    /*
    ** Load default schedule definition table data
    */
    Status = CFE_TBL_Load(SCH_AppData.ScheduleTableHandle,
                          CFE_TBL_SRC_FILE,
                          (const void *) SCH_SCHEDULE_FILENAME);

    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SCH_SDT_LOAD_ERR_EID, CFE_EVS_ERROR,
                          "Error (RC=0x%08X) Loading SDT with %s", 
                          Status, SCH_SCHEDULE_FILENAME);    
        return(Status);
    }

    /*
    ** Load default message definition table data
    */
    Status = CFE_TBL_Load(SCH_AppData.MessageTableHandle,
                          CFE_TBL_SRC_FILE,
                          (const void *) SCH_MESSAGE_FILENAME);

    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SCH_MDT_LOAD_ERR_EID, CFE_EVS_ERROR,
                          "Error (RC=0x%08X) Loading MDT with %s", 
                          Status, SCH_MESSAGE_FILENAME);    
        return(Status);
    }


    /*
    ** Get pointers to table data
    */
    Status = SCH_AcquirePointers();

    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SCH_ACQ_PTR_ERR_EID, CFE_EVS_ERROR,
                          "Error Acquiring Tbl Ptrs (RC=0x%08X)", 
                          Status);    
        return(Status);
    }

    return(Status);

} /* End of SCH_TblInit() */


/*******************************************************************
**
** SCH_TimerInit
**
** NOTE: For complete prolog information, see above
********************************************************************/

int32 SCH_TimerInit(void)
{
    int32             Status = CFE_SUCCESS;
    
    /*
    ** Start off assuming Major Frame synch is normal
    ** and should be coming at any moment
    */
    SCH_AppData.IgnoreMajorFrame     = FALSE;
    SCH_AppData.IgnoreMajorFrameMsgSent = FALSE;
    SCH_AppData.UnexpectedMajorFrame = FALSE;
    SCH_AppData.SyncToMET            = SCH_NOT_SYNCHRONIZED;
    SCH_AppData.MajorFrameSource     = SCH_MAJOR_FS_NONE;
    SCH_AppData.NextSlotNumber       = 0;
    SCH_AppData.MinorFramesSinceTone = SCH_TIME_SYNC_SLOT;
    SCH_AppData.LastSyncMETSlot      = 0;
    SCH_AppData.SyncAttemptsLeft     = 0;
    
    SCH_AppData.UnexpectedMajorFrameCount   = 0;
    SCH_AppData.MissedMajorFrameCount       = 0;
    SCH_AppData.ValidMajorFrameCount        = 0;
    SCH_AppData.WorstCaseSlotsPerMinorFrame = 1;
    
    /*
    ** Configure Major Frame and Minor Frame sources
    */
    SCH_AppData.ClockAccuracy = SCH_WORST_CLOCK_ACCURACY;
    /*
    ** Create the timer to be used for minor frames
    */
    Status = SCH_CustomEarlyInit();
    
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SCH_MINOR_FRAME_TIMER_CREATE_ERR_EID, CFE_EVS_ERROR,
                          "Error creating Timer (RC=0x%08X)", 
                          Status);    
        return(Status);
    }
    
    /*
    ** Determine if the timer has an acceptable clock accuracy
    */
    if (SCH_AppData.ClockAccuracy > SCH_WORST_CLOCK_ACCURACY)
    {
        CFE_EVS_SendEvent(SCH_MINOR_FRAME_TIMER_ACC_WARN_EID, CFE_EVS_INFORMATION,
                          "OS Timer Accuracy (%d > reqd %d usec) requires Minor Frame MET sync",
                          SCH_AppData.ClockAccuracy, SCH_WORST_CLOCK_ACCURACY);
        
        /* Synchronize Minor Frame Timing with Mission Elapsed Time to keep from losing slots */
        SCH_AppData.SyncToMET = SCH_MINOR_SYNCHRONIZED;

        /* Calculate how many slots we may have to routinely process on each Minor Frame Wakeup */
        SCH_AppData.WorstCaseSlotsPerMinorFrame = ((SCH_AppData.ClockAccuracy * 2) / SCH_NORMAL_SLOT_PERIOD) + 1;
    }
    
    /*
    ** Create main task semaphore (given by MajorFrameCallback and MinorFrameCallback)
    */
    Status = OS_BinSemCreate(&SCH_AppData.TimeSemaphore, SCH_SEM_NAME, SCH_SEM_VALUE, SCH_SEM_OPTIONS);
    if (Status != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(SCH_SEM_CREATE_ERR_EID, CFE_EVS_ERROR,
                          "Error creating Main Loop Timing Semaphore (RC=0x%08X)", 
                          Status);    
        return(Status);
    }
    
    return(Status);
    
} /* End of SCH_TimerInit() */


/*******************************************************************
**
** SCH_ProcessScheduleTable
**
** NOTE: For complete prolog information, see above
********************************************************************/

int32 SCH_ProcessScheduleTable(void)
{
    uint32 CurrentSlot;
    uint32 ProcessCount;
    int32  Result = CFE_SUCCESS;

    /*
    ** Get the slot we should be at 
    */
    CurrentSlot = SCH_CustomGetCurrentSlotNumber();

    /*
    ** Compute the number of slots we need to process (watch for rollover)
    */
    if (CurrentSlot < SCH_AppData.NextSlotNumber)
    {
        ProcessCount = SCH_TOTAL_SLOTS - SCH_AppData.NextSlotNumber;
        ProcessCount += (CurrentSlot + 1);
    }
    else
    {
        ProcessCount = (CurrentSlot - SCH_AppData.NextSlotNumber) + 1;
    }

    /*
    ** Correct for the following conditions observed when minor frame driven
    ** by a clock with poor accuracy
    **
    **   1) Wake up a little too late for just 1 slot
    **      symptom = multi slots event followed by same slot event
    **
    **   2) Wake up a little too early for just 1 slot
    **      symptom = same slot event followed by multi slots event
    */
    if (ProcessCount == 2)
    {
        /*
        ** If we want to do 2 slots but last time was OK then assume we
        **    are seeing condition #1 above.  By doing just 1 slot now,
        **    there will still be 1 to do when the next wakeup occurs
        **    and we will avoid both events.  But, if we really are in
        **    a delayed state, we will process both slots when we wake
        **    up next time because then the last time will NOT be OK.
        */
        if (SCH_AppData.LastProcessCount == 1)
        {
            ProcessCount = 1;
        }
        SCH_AppData.LastProcessCount = 2;
    }
    else if (ProcessCount == SCH_TOTAL_SLOTS)
    {
        /*
        ** Same as previous comment except in reverse order.
        */
        if (SCH_AppData.LastProcessCount != SCH_TOTAL_SLOTS)
        {
            ProcessCount = 1;
        }
        SCH_AppData.LastProcessCount = SCH_TOTAL_SLOTS;
    }
    else
    {
        SCH_AppData.LastProcessCount = ProcessCount;
    }

    /*
    ** If current slot = next slot - 1, assume current slot did not increment
    */
    if (ProcessCount == SCH_TOTAL_SLOTS)
    {
        SCH_AppData.SameSlotCount++;

        CFE_EVS_SendEvent(SCH_SAME_SLOT_EID, CFE_EVS_DEBUG,
                          "Slot did not increment: slot = %d",
                          CurrentSlot);
        ProcessCount = 0;
    }

    /*
    ** If we are too far behind, jump forward and do just the current slot
    */
    if (ProcessCount > SCH_MAX_LAG_COUNT)
    {
        SCH_AppData.SkippedSlotsCount++;

        CFE_EVS_SendEvent(SCH_SKIPPED_SLOTS_EID, CFE_EVS_ERROR,
                          "Slots skipped: slot = %d, count = %d",
                          SCH_AppData.NextSlotNumber, (ProcessCount - 1));

        /*
        ** Update the pass counter if we are skipping the rollover slot
        */
        if (CurrentSlot < SCH_AppData.NextSlotNumber)
        {
            SCH_AppData.TablePassCount++;
        }

        /*
        ** Process ground commands if we are skipping the time synch slot
        ** NOTE: This assumes the Time Synch Slot is the LAST Schedule slot
        **       (see definition of SCH_TIME_SYNC_SLOT in sch_app.h)
        ** Ground commands should only be processed at the end of the schedule table
        ** so that Group Enable/Disable commands do not change the state of entries
        ** in the middle of a schedule.
        */
        if ((SCH_AppData.NextSlotNumber + ProcessCount) > SCH_TIME_SYNC_SLOT)
        {
            Result = SCH_ProcessCommands();
        }

        SCH_AppData.NextSlotNumber = CurrentSlot;
        ProcessCount = 1;
    }

    /*
    ** Don't try to catch up all at once, just do a couple
    */
    if (ProcessCount > SCH_MAX_SLOTS_PER_WAKEUP)
    {
        ProcessCount = SCH_MAX_SLOTS_PER_WAKEUP;
    }

    /*
    ** Keep track of multi-slot processing
    */
    if (ProcessCount > 1)
    {
        SCH_AppData.MultipleSlotsCount++;

        /* Generate an event message if not syncing to MET or when there is more than two being processed */
        if ((ProcessCount > SCH_AppData.WorstCaseSlotsPerMinorFrame) || (SCH_AppData.SyncToMET == SCH_NOT_SYNCHRONIZED))
        {
            CFE_EVS_SendEvent(SCH_MULTI_SLOTS_EID, CFE_EVS_INFORMATION,
                              "Multiple slots processed: slot = %d, count = %d",
                              SCH_AppData.NextSlotNumber, ProcessCount);
        }
    }

    /*
    ** Process the slots (most often this will be just one)
    */
    while ((ProcessCount != 0) && (Result == CFE_SUCCESS))
    {
        Result = SCH_ProcessNextSlot();

        ProcessCount--;
    }

    return(Result);

} /* End of SCH_ProcessScheduleTable() */


/*******************************************************************
**
** SCH_ProcessNextSlot
**
** NOTE: For complete prolog information, see above
********************************************************************/

int32 SCH_ProcessNextSlot(void)
{
    int32 Result = CFE_SUCCESS;
    int32 EntryNumber;
    int32 SlotIndex;
    SCH_ScheduleEntry_t *NextEntry;

    SlotIndex = SCH_AppData.NextSlotNumber * SCH_ENTRIES_PER_SLOT;
    NextEntry = &SCH_AppData.ScheduleTable[SlotIndex];

    /*
    ** Process each (enabled) entry in the schedule table slot
    */
    for (EntryNumber = 0; EntryNumber < SCH_ENTRIES_PER_SLOT; EntryNumber++)
    {
        if (NextEntry->EnableState == SCH_ENABLED)
        {
            SCH_ProcessNextEntry(NextEntry, EntryNumber);
        }

        NextEntry++;
    }

    /*
    ** Process ground commands in the slot reserved for time synch
    ** Ground commands should only be processed at the end of the schedule table
    ** so that Group Enable/Disable commands do not change the state of entries
    ** in the middle of a schedule.
    */
    if (SCH_AppData.NextSlotNumber == SCH_TIME_SYNC_SLOT)
    {
        Result = SCH_ProcessCommands();
    }

    /*
    ** Maintain "next" schedule table slot index
    */
    SCH_AppData.NextSlotNumber++;

    if (SCH_AppData.NextSlotNumber == SCH_TOTAL_SLOTS)
    {
        SCH_AppData.NextSlotNumber = 0;
        SCH_AppData.TablePassCount++;
    }

    /*
    ** Maintain "total slots processed" counter
    */
    SCH_AppData.SlotsProcessedCount++;

    return(Result);

} /* End of SCH_ProcessNextSlot() */


/*******************************************************************
**
** SCH_ProcessNextEntry
**
** NOTE: For complete prolog information, see above
********************************************************************/

void SCH_ProcessNextEntry(SCH_ScheduleEntry_t *NextEntry, int32 EntryNumber)
{
    int32   Status;
    uint32  Remainder;
    uint16 *Message;

    /*
    ** Check for invalid table entry
    **
    ** (run time corruption -- data was verified at table load)
    */
    if ((NextEntry->MessageIndex >= SCH_MAX_MESSAGES) ||
        (NextEntry->Frequency    == SCH_UNUSED)       ||
        (NextEntry->Type         != SCH_ACTIVITY_SEND_MSG) ||
        (NextEntry->Remainder    >= NextEntry->Frequency))
    {
        SCH_AppData.BadTableDataCount++;

        /*
        ** Too much data for just one event
        */
        CFE_EVS_SendEvent(SCH_CORRUPTION_EID, CFE_EVS_ERROR,
                          "Corrupt data error (1): slot = %d, entry = %d",
                          SCH_AppData.NextSlotNumber, EntryNumber);

        CFE_EVS_SendEvent(SCH_CORRUPTION_EID, CFE_EVS_ERROR,
                          "Corrupt data error (2): msg = %d, freq = %d, type = %d, rem = %d",
                          NextEntry->MessageIndex,
                          NextEntry->Frequency,
                          NextEntry->Type,
                          NextEntry->Remainder);

        /*
        ** Disable entry to avoid repeating this error
        */
        NextEntry->EnableState = SCH_DISABLED;
        CFE_TBL_Modified(SCH_AppData.ScheduleTableHandle);
    }
    else
    {
        /*
        ** Look for entry active on this particular pass through table
        */
        Remainder = SCH_AppData.TablePassCount % NextEntry->Frequency;

        if (Remainder == NextEntry->Remainder)
        {
            Message = SCH_AppData.MessageTable[NextEntry->MessageIndex].MessageBuffer;
            Status = CFE_SB_SendMsg((CFE_SB_Msg_t *) Message);
            
            /* If additional activity types are added in the future, a switch statement */
            /* would be useful, as shown below:                                         */
            /* NOTE: The "default" clause should never be able to be executed but is    */
            /*       required by Flight Software Branch Coding Standards                */
       /*
        *   switch(NextEntry->Type)
        *   {
        *       case SCH_ACTIVITY_SEND_MSG:
        *           Message = SCH_AppData.MessageTable[NextEntry->MessageIndex].MessageBuffer;
        *           Status = CFE_SB_SendMsg((CFE_SB_Msg_t *) Message);
        *           break;
        *            
        *       default:
        *           Status = SCH_UNKNOWN_ACTIVITY;
        *           break;
        *   }
        */
            if (Status == CFE_SUCCESS)
            {
                SCH_AppData.ScheduleActivitySuccessCount++;
            }
            else
            {
                SCH_AppData.ScheduleActivityFailureCount++;

                CFE_EVS_SendEvent(SCH_PACKET_SEND_EID, CFE_EVS_ERROR,
                                  "Activity error: slot = %d, entry = %d, err = 0x%08X",
                                  SCH_AppData.NextSlotNumber, EntryNumber, Status);
            }
        }
    }

    return;

} /* End of SCH_ProcessNextEntry() */


/*******************************************************************
**
** SCH_ProcessCommands
**
** NOTE: For complete prolog information, see above
********************************************************************/

int32 SCH_ProcessCommands(void)
{
    int32 Status = CFE_SUCCESS;

    while (Status == CFE_SUCCESS)
    {
        /*
        ** Process pending Software Bus messages
        */
        Status = CFE_SB_RcvMsg(&SCH_AppData.MsgPtr, SCH_AppData.CmdPipe, CFE_SB_POLL);

        if (Status == CFE_SUCCESS)
        {
            Status = SCH_AppPipe(SCH_AppData.MsgPtr);
        }
    }

    if (Status == CFE_SB_NO_MESSAGE)
    {
        /*
        ** It's OK to not get a message -- we are polling
        */
        Status = CFE_SUCCESS;
    }

    return(Status);

} /* End of SCH_ProcessCommands() */


/*******************************************************************
**
** SCH_ValidateScheduleData
**
** NOTE: For complete prolog information, see above
********************************************************************/

int32 SCH_ValidateScheduleData(void *TableData)
{
    SCH_ScheduleEntry_t *TableArray = (SCH_ScheduleEntry_t *) TableData;
    int32 EntryResult = CFE_SUCCESS;
    int32 TableResult = CFE_SUCCESS;
    int32 TableIndex;

    uint8  EnableState;
    uint8  Type;
    uint16 Frequency;
    uint16 Remainder;
    uint16 MessageIndex;
    uint32 GroupData;

    int32 GoodCount   = 0;
    int32 BadCount    = 0;
    int32 UnusedCount = 0;

    /*
    ** Verify each entry in pending SCH schedule table
    */
    for (TableIndex = 0; TableIndex < SCH_TABLE_ENTRIES; TableIndex++)
    {
        EnableState  = TableArray[TableIndex].EnableState;
        Type         = TableArray[TableIndex].Type;
        Frequency    = TableArray[TableIndex].Frequency;
        Remainder    = TableArray[TableIndex].Remainder;
        MessageIndex = TableArray[TableIndex].MessageIndex;
        GroupData    = TableArray[TableIndex].GroupData;

        EntryResult  = CFE_SUCCESS;

        if (EnableState == SCH_UNUSED)
        {
            /*
            ** If enable state is unused, then all fields must be unused
            */
            if ((Frequency != SCH_UNUSED) ||
                (Remainder != SCH_UNUSED) ||
                (GroupData != SCH_UNUSED) ||
                (Type      != SCH_UNUSED) ||
                (MessageIndex != SCH_UNUSED))
            {
                EntryResult = SCH_SDT_GARBAGE_ENTRY;
                BadCount++;
            }
            else
            {
                UnusedCount++;
            }
        }
        else if ((EnableState == SCH_ENABLED) || (EnableState == SCH_DISABLED))
        {
            /*
            ** If enable state is used, then verify all fields
            **
            **  - Frequency must be non-zero
            **  - Remainder must be < Frequency
            **  - Type must be SCH_ACTIVITY_SEND_MSG
            **  - MessageIndex must be non-zero (reserved value = "unused")
            **  - MessageIndex must be < SCH_MAX_MESSAGES
            */
            if (Frequency == SCH_UNUSED)
            {
                EntryResult = SCH_SDT_NO_FREQUENCY;
            }
            else if (Remainder >= Frequency)
            {
                EntryResult = SCH_SDT_BAD_REMAINDER;
            }
            else if (Type != SCH_ACTIVITY_SEND_MSG)
            {
                EntryResult = SCH_SDT_BAD_ACTIVITY;
            }
            else if (MessageIndex == 0)
            {
                EntryResult = SCH_SDT_BAD_MSG_INDEX;
            }
            else if (MessageIndex >= SCH_MAX_MESSAGES)
            {
                EntryResult = SCH_SDT_BAD_MSG_INDEX;
            }
            
            if (EntryResult != CFE_SUCCESS)
            {
                BadCount++;
            }
            else
            {
                GoodCount++;
            }
        }
        else
        {
            EntryResult = SCH_SDT_BAD_ENABLE_STATE;
            BadCount++;
        }

        /*
        ** Send event for "first" error found
        */
        if ((EntryResult != CFE_SUCCESS) && (TableResult == CFE_SUCCESS))
        {
            TableResult = EntryResult;

            CFE_EVS_SendEvent(SCH_SCHEDULE_TBL_ERR_EID, CFE_EVS_ERROR,
                              "Schedule tbl verify error - idx[%d] ena[%d] typ[%d] fre[%d] rem[%d] msg[%d] grp[0x%08X]",
                              TableIndex, EnableState, Type, Frequency, Remainder, MessageIndex, GroupData);
        }
    }

    /*
    ** Send event describing results
    */
    CFE_EVS_SendEvent(SCH_SCHEDULE_TABLE_EID, CFE_EVS_DEBUG,
                      "Schedule table verify results -- good[%d] bad[%d] unused[%d]",
                      GoodCount, BadCount, UnusedCount);
    /*
    ** Maintain table verification statistics
    */
    if (TableResult == CFE_SUCCESS)
    {
        SCH_AppData.TableVerifySuccessCount++;
    }
    else
    {
        SCH_AppData.TableVerifyFailureCount++;
    }

    return(TableResult);

} /* End of SCH_ValidateScheduleData() */


/*******************************************************************
**
** SCH_ValidateMessageData
**
** NOTE: For complete prolog information, see above
********************************************************************/

int32 SCH_ValidateMessageData(void *TableData)
{
    SCH_MessageEntry_t *TableArray = (SCH_MessageEntry_t *) TableData;
    int32 EntryResult = CFE_SUCCESS;
    int32 TableResult = CFE_SUCCESS;
    int32 TableIndex;
    int32 BufferIndex;


    uint16         *MessageBuffer;
    uint16         *UserDataPtr;

    uint16          MessageLength;
    CFE_SB_MsgId_t  MessageID;
    CFE_SB_MsgId_t  MaxValue = (CFE_SB_MsgId_t) SCH_MDT_MAX_MSG_ID;
    CFE_SB_MsgId_t  MinValue = (CFE_SB_MsgId_t) SCH_MDT_MIN_MSG_ID;

    int32 GoodCount   = 0;
    int32 BadCount    = 0;
    int32 UnusedCount = 0;

    /*
    ** Verify each entry in pending SCH Message table
    */
    for (TableIndex = 0; TableIndex < SCH_MAX_MESSAGES; TableIndex++)
    {
        EntryResult = CFE_SUCCESS;
        BufferIndex = 0;

        MessageBuffer = &TableArray[TableIndex].MessageBuffer[0];
        MessageID     = CFE_SB_GetMsgId((CFE_SB_MsgPtr_t) MessageBuffer);
        MessageLength = CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t) MessageBuffer);

        if (MessageID == SCH_UNUSED_MID)
        {
            /*
            ** If message ID is unused, then look for junk in user data portion
            */
            UnusedCount++;
            UserDataPtr = (uint16 *)CFE_SB_GetUserData((CFE_SB_MsgPtr_t) MessageBuffer);
            while (UserDataPtr < &TableArray[TableIndex+1].MessageBuffer[0])
            {
                if (*UserDataPtr != SCH_UNUSED)
                {
                    EntryResult = SCH_MDT_GARBAGE_ENTRY;
                    BadCount++;
                    UnusedCount--;
                    break;
                }
                UserDataPtr++;
            }
        }
        else if ((MessageID <= MaxValue) && 
                 (MessageID >= MinValue))
        {
            /*
            ** If message ID is valid, then check message length
            */
            if ((MessageLength > (SCH_MAX_MSG_WORDS * 2)) ||
                (MessageLength < (SCH_MIN_MSG_WORDS * 2)) ||
               ((MessageLength & 1) != 0))
            {
                EntryResult = SCH_MDT_INVALID_LENGTH;
                BadCount++;
            }
            else
            {
                GoodCount++;
            }
        }
        else
        {
            EntryResult = SCH_MDT_BAD_MSG_ID;
            BadCount++;
        }

        /*
        ** Save index of "first" error found
        */
        if ((EntryResult != CFE_SUCCESS) && (TableResult == CFE_SUCCESS))
        {
            TableResult = EntryResult;

            CFE_EVS_SendEvent(SCH_MESSAGE_TBL_ERR_EID, CFE_EVS_ERROR,
                              "Message tbl verify err - idx[%d] mid[0x%X] len[%d] buf[%d]",
                              TableIndex, MessageID, MessageLength, BufferIndex);
        }
    }

    /*
    ** Send event describing results
    */
    CFE_EVS_SendEvent(SCH_MESSAGE_TABLE_EID, CFE_EVS_DEBUG,
                      "Message tbl verify results - good[%d] bad[%d] unused[%d]",
                      GoodCount, BadCount, UnusedCount);
    /*
    ** Maintain table verification statistics
    */
    if (TableResult == CFE_SUCCESS)
    {
        SCH_AppData.TableVerifySuccessCount++;
    }
    else
    {
        SCH_AppData.TableVerifyFailureCount++;
    }

    return(TableResult);

} /* End of SCH_ValidateMessageData() */


/************************/
/*  End of File Comment */
/************************/

