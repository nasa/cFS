/*
** $Id: sch_cmds.c 1.5 2015/03/01 14:01:41EST sstrege Exp  $
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
** Purpose: Scheduler (SCH) application command handling
**
** Author:
**
** Notes:
**
** $Log: sch_cmds.c  $
** Revision 1.5 2015/03/01 14:01:41EST sstrege 
** Added copyright information
** Revision 1.4 2012/06/26 13:56:25EDT lwalling 
** Added call to CFE_TBL_Modified() after change to shedule table entry enable/disable state
** Revision 1.3 2009/06/12 11:41:00PDT rmcgraw 
** DCR82191:1 Changed OS_Mem function calls to CFE_PSP_Mem
** Revision 1.2 2009/03/27 00:24:21EDT dkobe 
** Added consecutive noisy major frame counter and platform config parameter to compare counter to
** Revision 1.1 2008/10/16 15:08:34EDT dkobe 
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

#include "sch_msg.h"
#include "sch_events.h"
#include "sch_app.h"
#include "sch_cmds.h"
#include "sch_version.h"

#include "cfe_time_msg.h"


/*************************************************************************
**
** Macro definitions
**
**************************************************************************/

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
extern SCH_AppData_t           SCH_AppData;

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
/** \brief Process housekeeping request
**  
**  \par Description
**       Processes an on-board housekeeping request message.
**
**  \par Assumptions, External Events, and Notes:
**       This command does not affect the command execution counter
**       
**  \param [in]   MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                             references the software bus message 
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32 SCH_HousekeepingCmd(CFE_SB_MsgPtr_t MessagePtr);

/*
** Application command handlers
*/
/************************************************************************/
/** \brief Process noop command
**  
**  \par Description
**       Processes a noop ground command.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                             references the software bus message 
**
**  \sa #SCH_NOOP_CC
**
*************************************************************************/
void SCH_NoopCmd(CFE_SB_MsgPtr_t MessagePtr);

/************************************************************************/
/** \brief Process reset counters command
**  
**  \par Description
**       Processes a reset counters ground command which will reset
**       the Scheduler commmand error, command execution and performance
**       statistics counters to zero.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                             references the software bus message 
**
**  \sa #SCH_RESET_CC
**
*************************************************************************/
void SCH_ResetCmd(CFE_SB_MsgPtr_t MessagePtr);

/************************************************************************/
/** \brief Enable a Single Activity Command
**  
**  \par Description
**       Command to Enable a specific activity in the Schedule 
**       Definition Table.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   MessagePtr     A #CFE_SB_MsgPtr_t pointer that
**                               references the software bus message 
**       
**  \sa #SCH_ENABLE_CC, #SCH_DISABLE_CC, #SCH_ENABLE_GROUP_CC, #SCH_DISABLE_GROUP_CC
**
*************************************************************************/
void SCH_EnableCmd(CFE_SB_MsgPtr_t MessagePtr);

/************************************************************************/
/** \brief Disable a Single Activity Command
**  
**  \par Description
**       Command to Disable a specific activity in the Schedule 
**       Definition Table.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   MessagePtr     A #CFE_SB_MsgPtr_t pointer that
**                               references the software bus message 
**       
**  \sa #SCH_ENABLE_CC, #SCH_DISABLE_CC, #SCH_ENABLE_GROUP_CC, #SCH_DISABLE_GROUP_CC
**
*************************************************************************/
void SCH_DisableCmd(CFE_SB_MsgPtr_t MessagePtr);

/************************************************************************/
/** \brief Enable a Group and/or Multi-Group(s) Command
**  
**  \par Description
**       Command to Enable a single Group and/or one or more Multi-Groups
**       of activities.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   MessagePtr     A #CFE_SB_MsgPtr_t pointer that
**                               references the software bus message 
**       
**  \sa #SCH_ENABLE_CC, #SCH_DISABLE_CC, #SCH_ENABLE_GROUP_CC, #SCH_DISABLE_GROUP_CC
**
*************************************************************************/
void SCH_EnableGroupCmd(CFE_SB_MsgPtr_t MessagePtr);

/************************************************************************/
/** \brief Disable a Group and/or Multi-Group(s) Command
**  
**  \par Description
**       Command to Disable a single Group and/or one or more Multi-Groups
**       of activities.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   MessagePtr     A #CFE_SB_MsgPtr_t pointer that
**                               references the software bus message 
**       
**  \sa #SCH_ENABLE_CC, #SCH_DISABLE_CC, #SCH_ENABLE_GROUP_CC, #SCH_DISABLE_GROUP_CC
**
*************************************************************************/
void SCH_DisableGroupCmd(CFE_SB_MsgPtr_t MessagePtr);

/************************************************************************/
/** \brief Enables Major Frame Synchronization
**  
**  \par Description
**       Command to enable synchronization of Schedule Definition Table to
**       the Major Frame Sync signal.  The synchronization can become
**       unsynchronized when the Major Frame signal becomes noisy.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   MessagePtr     A #CFE_SB_MsgPtr_t pointer that
**                               references the software bus message 
**       
**  \sa #SCH_ENABLE_CC, #SCH_DISABLE_CC, #SCH_ENABLE_GROUP_CC, #SCH_DISABLE_GROUP_CC
**
*************************************************************************/
void SCH_EnableSyncCmd(CFE_SB_MsgPtr_t MessagePtr);

/************************************************************************/
/** \brief Creates and sends diagnostic message packet
**  
**  \par Description
**       Command to send the Scheduler diagnostic message.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   MessagePtr     A #CFE_SB_MsgPtr_t pointer that
**                               references the software bus message 
**       
*************************************************************************/
void SCH_SendDiagTlmCmd(CFE_SB_MsgPtr_t MessagePtr);

/************************************************************************/
/** \brief Updates appropriate command counters following command execution
**  
**  \par Description
**       This function updates the ground or on-board command counter or
**       command error counter depending upon the success of the command
**       and where it originated.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]   GoodCommand    Indicates the command was successfully
**                               performed (=TRUE) or contained an error
**                               (=FALSE). 
**       
*************************************************************************/
void SCH_PostCommandResult(boolean GoodCommand);

/************************************************************************/
/** \brief Verifies the length of the specified message
**  
**  \par Description
**       This function determines whether the specified message is of the
**       specified expected length.  If not, an event message is generated
**       and the appropriate command error counter is incremented.  If the
**       message length is appropriate, then the appropriate command counter
**       is incremented.
**
**  \par Assumptions, External Events, and Notes:
**       None
**       
**       
**  \param [in]   MessagePtr     A #CFE_SB_MsgPtr_t pointer that
**                               references the software bus message 
**       
**  \param [in]   ExpectedLength The size, in bytes, that the specified
**                               message should be equal to. 
**       
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
*************************************************************************/
int32 SCH_VerifyCmdLength (CFE_SB_MsgPtr_t MessagePtr, uint32 ExpectedLength);


/*******************************************************************
**
** SCH_AppPipe
**
** NOTE: For complete prolog information, see 'sch_cmds.h'
********************************************************************/

int32 SCH_AppPipe(CFE_SB_MsgPtr_t MessagePtr)
{
    int32           Result = CFE_SUCCESS;
    CFE_SB_MsgId_t  MessageID = 0;
    uint16          CommandCode = 0;

    MessageID = CFE_SB_GetMsgId(MessagePtr);
    switch (MessageID)
    {
        /*
        ** Housekeeping telemetry request
        */
        case SCH_SEND_HK_MID:
            Result = SCH_HousekeepingCmd(MessagePtr);
            break;

        /*
        ** SCH ground commands
        */
        case SCH_CMD_MID:

            CommandCode = CFE_SB_GetCmdCode(MessagePtr);
            switch (CommandCode)
            {
                case SCH_NOOP_CC:
                    SCH_NoopCmd(MessagePtr);
                    break;

                case SCH_RESET_CC:
                    SCH_ResetCmd(MessagePtr);
                    break;

                case SCH_ENABLE_CC:
                    SCH_EnableCmd(MessagePtr);
                    break;

                case SCH_DISABLE_CC:
                    SCH_DisableCmd(MessagePtr);
                    break;

                case SCH_ENABLE_GROUP_CC:
                    SCH_EnableGroupCmd(MessagePtr);
                    break;

                case SCH_DISABLE_GROUP_CC:
                    SCH_DisableGroupCmd(MessagePtr);
                    break;

                case SCH_ENABLE_SYNC_CC:
                    SCH_EnableSyncCmd(MessagePtr);
                    break;

                case SCH_SEND_DIAG_TLM_CC:
                    SCH_SendDiagTlmCmd(MessagePtr);
                    break;

                /*
                ** SCH ground commands with unknown command codes...
                */
                default:
                    CFE_EVS_SendEvent(SCH_CC_ERR_EID, CFE_EVS_ERROR,
                                      "Invalid command code: ID = 0x%04X, CC = %d",
                                      MessageID, CommandCode);

                    SCH_AppData.ErrCounter++;
                    break;
            }
            break;

        /*
        ** Unknown message ID's
        */
        default:
            CFE_EVS_SendEvent(SCH_MD_ERR_EID, CFE_EVS_ERROR,
                              "Msg with Invalid message ID Rcvd -- ID = 0x%04X",
                              MessageID);
            break;
    }

    return(Result);

} /* End of SCH_AppPipe() */


/*******************************************************************
**
** SCH_HousekeepingCmd
**
** NOTE: For complete prolog information, see above
********************************************************************/

int32 SCH_HousekeepingCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    int32  TableResult = SCH_SUCCESS;

    if(SCH_VerifyCmdLength(MessagePtr, sizeof(SCH_NoArgsCmd_t)) == SCH_SUCCESS)
    {
        /*
        ** Update contents of Housekeeping Packet
        */
        SCH_AppData.HkPacket.CmdCounter                   = SCH_AppData.CmdCounter;
        SCH_AppData.HkPacket.ErrCounter                   = SCH_AppData.ErrCounter;
        SCH_AppData.HkPacket.ScheduleActivitySuccessCount = SCH_AppData.ScheduleActivitySuccessCount;
        SCH_AppData.HkPacket.ScheduleActivityFailureCount = SCH_AppData.ScheduleActivityFailureCount;
        SCH_AppData.HkPacket.SlotsProcessedCount          = SCH_AppData.SlotsProcessedCount;
        SCH_AppData.HkPacket.SkippedSlotsCount            = SCH_AppData.SkippedSlotsCount;
        SCH_AppData.HkPacket.MultipleSlotsCount           = SCH_AppData.MultipleSlotsCount;
        SCH_AppData.HkPacket.SameSlotCount                = SCH_AppData.SameSlotCount;
        SCH_AppData.HkPacket.BadTableDataCount            = SCH_AppData.BadTableDataCount;
        SCH_AppData.HkPacket.TableVerifySuccessCount      = SCH_AppData.TableVerifySuccessCount;
        SCH_AppData.HkPacket.TableVerifyFailureCount      = SCH_AppData.TableVerifyFailureCount;
        SCH_AppData.HkPacket.TablePassCount               = SCH_AppData.TablePassCount;
        SCH_AppData.HkPacket.ValidMajorFrameCount         = SCH_AppData.ValidMajorFrameCount;
        SCH_AppData.HkPacket.MissedMajorFrameCount        = SCH_AppData.MissedMajorFrameCount;
        SCH_AppData.HkPacket.UnexpectedMajorFrameCount    = SCH_AppData.UnexpectedMajorFrameCount;
        SCH_AppData.HkPacket.MinorFramesSinceTone         = SCH_AppData.MinorFramesSinceTone;
        SCH_AppData.HkPacket.NextSlotNumber               = SCH_AppData.NextSlotNumber;
        SCH_AppData.HkPacket.LastSyncMETSlot              = SCH_AppData.LastSyncMETSlot;
        SCH_AppData.HkPacket.IgnoreMajorFrame             = SCH_AppData.IgnoreMajorFrame;
        SCH_AppData.HkPacket.UnexpectedMajorFrame         = SCH_AppData.UnexpectedMajorFrame;
        SCH_AppData.HkPacket.SyncToMET                    = SCH_AppData.SyncToMET;
        SCH_AppData.HkPacket.MajorFrameSource             = SCH_AppData.MajorFrameSource;
        
        /*
        ** Timestamps and send housekeeping packet
        */
        CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &SCH_AppData.HkPacket);
        CFE_SB_SendMsg((CFE_SB_Msg_t *) &SCH_AppData.HkPacket);

        /*
        ** Reset "high rate" event filters
        */
        CFE_EVS_ResetAllFilters();
    }

    /*
    ** Note:
    **
    **   The following table functions will give the cFE Table Manager
    **   a chance to update the tables used by this application.  If
    **   there is an error (very unlikely) the return value will cause
    **   us to fall out of the main process loop and terminate the SCH
    **   task.  It may sound extreme but there is nothing for the
    **   Scheduler to do if it cannot access both the message
    **   and schedule tables.
    */
    CFE_TBL_ReleaseAddress(SCH_AppData.ScheduleTableHandle);
    CFE_TBL_ReleaseAddress(SCH_AppData.MessageTableHandle);

    TableResult = SCH_AcquirePointers();

    return(TableResult);

} /* End of SCH_HousekeepingCmd() */


/*******************************************************************
**
** SCH_NoopCmd
**
** NOTE: For complete prolog information, see above
********************************************************************/

void SCH_NoopCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    if(SCH_VerifyCmdLength(MessagePtr, sizeof(SCH_NoArgsCmd_t)) != SCH_SUCCESS)
    {
        SCH_AppData.ErrCounter++;
    }
    else
    {
        /*
        ** This command is used primarily for "aliveness" testing
        */
        SCH_AppData.CmdCounter++;

        CFE_EVS_SendEvent(SCH_NOOP_CMD_EID, CFE_EVS_INFORMATION,
                          "NO-op command. Version %d.%d.%d.%d",
                          SCH_MAJOR_VERSION,
                          SCH_MINOR_VERSION,
                          SCH_REVISION,
                          SCH_MISSION_REV);
    }

    return;

} /* End of SCH_NoopCmd() */


/*******************************************************************
**
** SCH_ResetCmd
**
** NOTE: For complete prolog information, see above
********************************************************************/

void SCH_ResetCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    if(SCH_VerifyCmdLength(MessagePtr, sizeof(SCH_NoArgsCmd_t)) != SCH_SUCCESS)
    {
        SCH_AppData.ErrCounter++;
    }
    else
    {
        /*
        ** Reset housekeeping counters
        */
        SCH_AppData.CmdCounter      = 0;
        SCH_AppData.ErrCounter      = 0;

        SCH_AppData.ScheduleActivitySuccessCount = 0;
        SCH_AppData.ScheduleActivityFailureCount = 0;

        SCH_AppData.SlotsProcessedCount = 0;
        SCH_AppData.SkippedSlotsCount   = 0;
        SCH_AppData.MultipleSlotsCount  = 0;
        SCH_AppData.SameSlotCount       = 0;
        SCH_AppData.BadTableDataCount   = 0;

        SCH_AppData.TableVerifySuccessCount = 0;
        SCH_AppData.TableVerifyFailureCount = 0;
        
        SCH_AppData.ValidMajorFrameCount      = 0;
        SCH_AppData.MissedMajorFrameCount     = 0;
        SCH_AppData.UnexpectedMajorFrameCount = 0;

        CFE_EVS_SendEvent(SCH_RESET_CMD_EID, CFE_EVS_DEBUG,
                          "RESET command");
    }

    return;

} /* End of SCH_ResetCmd() */


/*******************************************************************
**
** SCH_EnableCmd
**
** NOTE: For complete prolog information, see above
********************************************************************/

void SCH_EnableCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    boolean         GoodCommand = FALSE;
    SCH_EntryCmd_t *EnableCmd = NULL;
    uint16          SlotNumber = 0; 
    uint16          EntryNumber = 0;
    uint16          TableIndex = 0;
    
    /*
    ** Extract contents of command
    */
    EnableCmd   = (SCH_EntryCmd_t *) MessagePtr;

    if(SCH_VerifyCmdLength(MessagePtr, sizeof(SCH_EntryCmd_t)) == SCH_SUCCESS)
    {
        SlotNumber  = EnableCmd->SlotNumber;
        EntryNumber = EnableCmd->EntryNumber;
        TableIndex  = (SlotNumber * SCH_ENTRIES_PER_SLOT) + EntryNumber;

        if ((SlotNumber >= SCH_TOTAL_SLOTS) || (EntryNumber >= SCH_ENTRIES_PER_SLOT))
        {
            /*
            ** Invalid command packet argument
            */
            CFE_EVS_SendEvent(SCH_ENABLE_CMD_ARG_ERR_EID, CFE_EVS_ERROR,
                              "ENABLE cmd: invalid argument, slot=%d (<%d), entry=%d (<%d)",
                              SlotNumber, SCH_TOTAL_SLOTS, 
                              EntryNumber, SCH_ENTRIES_PER_SLOT);
        }
        else if ((SCH_AppData.ScheduleTable[TableIndex].EnableState != SCH_ENABLED) &&
                 (SCH_AppData.ScheduleTable[TableIndex].EnableState != SCH_DISABLED))
        {
            /*
            ** Invalid schedule table enable state (unused or corrupt)
            */
            CFE_EVS_SendEvent(SCH_ENABLE_CMD_ENTRY_ERR_EID, CFE_EVS_ERROR,
                              "ENABLE command: invalid state = %d, slot = %d, entry = %d",
                              SCH_AppData.ScheduleTable[TableIndex].EnableState,
                              SlotNumber, EntryNumber);
        }
        else
        {
            /*
            ** Success
            */
            GoodCommand = TRUE;
    
            SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_ENABLED;
            CFE_TBL_Modified(SCH_AppData.ScheduleTableHandle);
    
            CFE_EVS_SendEvent(SCH_ENABLE_CMD_EID, CFE_EVS_DEBUG,
                              "ENABLE command: slot = %d, entry = %d",
                              SlotNumber, EntryNumber );
        }
    }

    SCH_PostCommandResult(GoodCommand);

    return;

} /* End of SCH_EnableCmd() */


/*******************************************************************
**
** SCH_DisableCmd
**
** NOTE: For complete prolog information, see above
********************************************************************/

void SCH_DisableCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    boolean         GoodCommand = FALSE;
    SCH_EntryCmd_t *DisableCmd  = NULL;
    uint16          SlotNumber  = 0;
    uint16          EntryNumber = 0;
    uint16          TableIndex  = 0;

    /*
    ** Extract contents of command
    */
    DisableCmd  = (SCH_EntryCmd_t *) MessagePtr;

    if(SCH_VerifyCmdLength(MessagePtr, sizeof(SCH_EntryCmd_t)) == SCH_SUCCESS)
    {
        SlotNumber  = DisableCmd->SlotNumber;
        EntryNumber = DisableCmd->EntryNumber;
        TableIndex  = (SlotNumber * SCH_ENTRIES_PER_SLOT) + EntryNumber;

        if ((SlotNumber >= SCH_TOTAL_SLOTS) || (EntryNumber >= SCH_ENTRIES_PER_SLOT))
        {
            /*
            ** Invalid command packet argument
            */
            CFE_EVS_SendEvent(SCH_DISABLE_CMD_ARG_ERR_EID, CFE_EVS_ERROR,
                              "DISABLE cmd: invalid argument, slot=%d (<%d), entry=%d (<%d)",
                              SlotNumber, SCH_TOTAL_SLOTS, 
                              EntryNumber, SCH_ENTRIES_PER_SLOT);
        }
        else if ((SCH_AppData.ScheduleTable[TableIndex].EnableState != SCH_ENABLED) &&
                 (SCH_AppData.ScheduleTable[TableIndex].EnableState != SCH_DISABLED))
        {
            /*
            ** Invalid schedule table enable state (unused or corrupt)
            */
            CFE_EVS_SendEvent(SCH_DISABLE_CMD_ENTRY_ERR_EID, CFE_EVS_ERROR,
                              "DISABLE command: invalid state = %d, slot = %d, entry = %d",
                              SCH_AppData.ScheduleTable[TableIndex].EnableState,
                              SlotNumber, EntryNumber);
        }
        else
        {
            /*
            ** Success
            */
            GoodCommand = TRUE;
    
            SCH_AppData.ScheduleTable[TableIndex].EnableState = SCH_DISABLED;
            CFE_TBL_Modified(SCH_AppData.ScheduleTableHandle);
    
            CFE_EVS_SendEvent(SCH_DISABLE_CMD_EID, CFE_EVS_DEBUG,
                              "DISABLE command: slot = %d, entry = %d",
                              SlotNumber, EntryNumber);
        }
    }

    SCH_PostCommandResult(GoodCommand);

    return;

} /* End of SCH_DisableCmd() */


/*******************************************************************
**
** SCH_EnableGroupCmd
**
** NOTE: For complete prolog information, see above
********************************************************************/

void SCH_EnableGroupCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    boolean              GoodCommand = FALSE;
    uint32               TblGroupNumber = 0;
    uint32               TblMultiGroup = 0;
    int32                LoopCount = 0;
    int32                MatchCount = 0;
    SCH_GroupCmd_t      *EnableCmd = NULL;
    SCH_ScheduleEntry_t *TableEntry = NULL;
    uint32               CmdGroupNumber = 0;
    uint32               CmdMultiGroup  = 0;

    /*
    ** Extract command parameters
    */
    EnableCmd = (SCH_GroupCmd_t *) MessagePtr;

    if(SCH_VerifyCmdLength(MessagePtr, sizeof(SCH_GroupCmd_t)) == SCH_SUCCESS)
    {
        TableEntry = &SCH_AppData.ScheduleTable[0];
        CmdGroupNumber = EnableCmd->GroupData & SCH_GROUP_NUMBER_BIT_MASK;
        CmdMultiGroup  = EnableCmd->GroupData & SCH_MULTI_GROUP_BIT_MASK;

        if ((CmdGroupNumber == SCH_UNUSED) && (CmdMultiGroup == SCH_UNUSED))
        {
            /*
            ** No groups selected
            */
            CFE_EVS_SendEvent(SCH_ENA_GRP_CMD_ERR_EID, CFE_EVS_ERROR,
                              "ENABLE GROUP command: invalid argument, no groups selected");
        }
        else
        {
            /*
            ** Search entire schedule table for group members
            */
            for (LoopCount = 0; LoopCount < SCH_TABLE_ENTRIES; LoopCount++)
            {
                /*
                ** Skip unused table entries
                */
                if (TableEntry->GroupData != SCH_UNUSED)
                {
                    TblGroupNumber = TableEntry->GroupData & SCH_GROUP_NUMBER_BIT_MASK;
                    TblMultiGroup  = TableEntry->GroupData & SCH_MULTI_GROUP_BIT_MASK;
    
                    /*
                    ** Look for matching table entries
                    */
                    if (((CmdGroupNumber != SCH_UNUSED) && (CmdGroupNumber == TblGroupNumber)) ||
                        ((CmdMultiGroup & TblMultiGroup) != SCH_UNUSED))
                    {
                        MatchCount++;
                        TableEntry->EnableState = SCH_ENABLED;
                    }
                }
    
                TableEntry++;
            }

            if (MatchCount > 0)
            {
                CFE_TBL_Modified(SCH_AppData.ScheduleTableHandle);
                CFE_EVS_SendEvent(SCH_ENA_GRP_CMD_EID, CFE_EVS_DEBUG,
                                  "ENABLE GROUP command: match count = %d",
                                  MatchCount);
                GoodCommand = TRUE;
            }
            else
            {
                CFE_EVS_SendEvent(SCH_ENA_GRP_NOT_FOUND_ERR_EID, CFE_EVS_ERROR,
                                  "ENABLE GROUP command: Neither Group %d nor Multi-Group 0x%06X found",
                                  (CmdGroupNumber>>24), CmdMultiGroup);
            }
        }
    }

    SCH_PostCommandResult(GoodCommand);

    return;

} /* End of SCH_EnableGroupCmd() */


/*******************************************************************
**
** SCH_DisableGroupCmd
**
** NOTE: For complete prolog information, see above
********************************************************************/

void SCH_DisableGroupCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    boolean              GoodCommand = FALSE;
    uint32               TblGroupNumber = 0;
    uint32               TblMultiGroup = 0;
    int32                LoopCount = 0;
    int32                MatchCount = 0;
    SCH_GroupCmd_t      *DisableCmd = NULL;
    SCH_ScheduleEntry_t *TableEntry = NULL;
    uint32               CmdGroupNumber = 0;
    uint32               CmdMultiGroup  = 0;

    /*
    ** Extract command parameters
    */
    DisableCmd = (SCH_GroupCmd_t *) MessagePtr;

    if(SCH_VerifyCmdLength(MessagePtr, sizeof(SCH_GroupCmd_t)) == SCH_SUCCESS)
    {
        TableEntry = &SCH_AppData.ScheduleTable[0];
        CmdGroupNumber = DisableCmd->GroupData & SCH_GROUP_NUMBER_BIT_MASK;
        CmdMultiGroup  = DisableCmd->GroupData & SCH_MULTI_GROUP_BIT_MASK;

        if ((CmdGroupNumber == SCH_UNUSED) && (CmdMultiGroup == SCH_UNUSED))
        {
            /*
            ** No groups selected
            */
            CFE_EVS_SendEvent(SCH_DIS_GRP_CMD_ERR_EID, CFE_EVS_ERROR,
                              "DISABLE GROUP command: invalid argument, no groups selected");
        }
        else
        {
            /*
            ** Search entire schedule table for group members
            */
            for (LoopCount = 0; LoopCount < SCH_TABLE_ENTRIES; LoopCount++)
            {
                /*
                ** Skip unused table entries
                */
                if (TableEntry->GroupData != SCH_UNUSED)
                {
                    TblGroupNumber = TableEntry->GroupData & SCH_GROUP_NUMBER_BIT_MASK;
                    TblMultiGroup  = TableEntry->GroupData & SCH_MULTI_GROUP_BIT_MASK;
    
                    /*
                    ** Look for matching table entries
                    */
                    if (((CmdGroupNumber != SCH_UNUSED) && (CmdGroupNumber == TblGroupNumber)) ||
                        ((CmdMultiGroup & TblMultiGroup) != SCH_UNUSED))
                    {
                        MatchCount++;
                        TableEntry->EnableState = SCH_DISABLED;
                    }
                }
    
                TableEntry++;
            }

            if (MatchCount > 0)
            {
                CFE_TBL_Modified(SCH_AppData.ScheduleTableHandle);
                CFE_EVS_SendEvent(SCH_DIS_GRP_CMD_EID, CFE_EVS_DEBUG,
                                  "DISABLE GROUP command: match count = %d",
                                  MatchCount);
                GoodCommand = TRUE;
            }
            else
            {
                CFE_EVS_SendEvent(SCH_DIS_GRP_NOT_FOUND_ERR_EID, CFE_EVS_ERROR,
                                  "DISABLE GROUP command: Neither Group %d nor Multi-Group 0x%06X found",
                                  (CmdGroupNumber>>24), CmdMultiGroup);
            }
        }
    }

    SCH_PostCommandResult(GoodCommand);

    return;

} /* End of SCH_DisableGroupCmd() */


/*******************************************************************
**
** SCH_EnableSyncCmd
**
** NOTE: For complete prolog information, see above
********************************************************************/

void SCH_EnableSyncCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    boolean   GoodCommand = FALSE;

    if(SCH_VerifyCmdLength(MessagePtr, sizeof(SCH_NoArgsCmd_t)) == SCH_SUCCESS)
    {
        GoodCommand = TRUE;
        
        SCH_AppData.IgnoreMajorFrame = FALSE;
        SCH_AppData.UnexpectedMajorFrame = FALSE;
        SCH_AppData.ConsecutiveNoisyFrameCounter = 0;
        
        CFE_EVS_SendEvent(SCH_ENA_SYNC_CMD_EID, CFE_EVS_DEBUG,
                          "Major Frame Synchronization Enabled");
    }

    SCH_PostCommandResult(GoodCommand);

    return;

} /* End of SCH_EnableSyncCmd() */




/*******************************************************************
**
** SCH_SendDiagTlmCmd
**
** NOTE: For complete prolog information, see above
********************************************************************/

void SCH_SendDiagTlmCmd(CFE_SB_MsgPtr_t MessagePtr)
{
    boolean              GoodCommand = FALSE;
    uint32               TblIndex = 0;
    uint32               WordIndex = 0;
    uint32               BitIndex = 0;
    SCH_ScheduleEntry_t *TableEntry = NULL;

    if(SCH_VerifyCmdLength(MessagePtr, sizeof(SCH_NoArgsCmd_t)) == SCH_SUCCESS)
    {
        GoodCommand = TRUE;
        
        /* Zero out the previous entry states */
        CFE_PSP_MemSet(&SCH_AppData.DiagPacket.EntryStates[0], 0x0, SCH_NUM_STATUS_BYTES_REQD);
        
        for (TblIndex=0; TblIndex<SCH_TABLE_ENTRIES; TblIndex++)
        {
            TableEntry = &SCH_AppData.ScheduleTable[TblIndex];
            WordIndex = TblIndex/8;             /* 8 states can fit in each word */
            BitIndex  = (7-(TblIndex%8))*2;     /* Determine bit pair, MSBs contain lowest index */
            
            if (TableEntry->EnableState == SCH_ENABLED)
            {
                SCH_AppData.DiagPacket.EntryStates[WordIndex] |= (1 << BitIndex);
                SCH_AppData.DiagPacket.MsgIDs[TblIndex] = 
                  CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)&SCH_AppData.MessageTable[SCH_AppData.ScheduleTable[TblIndex].MessageIndex]);
            }
            else if (TableEntry->EnableState == SCH_DISABLED)
            {
                SCH_AppData.DiagPacket.EntryStates[WordIndex] |= (2 << BitIndex);
                SCH_AppData.DiagPacket.MsgIDs[TblIndex] = 
                  CFE_SB_GetMsgId((CFE_SB_MsgPtr_t)&SCH_AppData.MessageTable[SCH_AppData.ScheduleTable[TblIndex].MessageIndex]);
            }
            else
            {
                SCH_AppData.DiagPacket.MsgIDs[TblIndex] = 0x0000;
            }
        }
        /*
        ** Timestamp and send diagnostic packet
        */
        CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &SCH_AppData.DiagPacket);
        CFE_SB_SendMsg((CFE_SB_Msg_t *) &SCH_AppData.DiagPacket);

        CFE_EVS_SendEvent(SCH_SEND_DIAG_CMD_EID, CFE_EVS_DEBUG,
                          "Transmitting Diagnostic Message");
    }

    SCH_PostCommandResult(GoodCommand);

    return;

} /* End of SCH_SendDiagTlmCmd() */


/*******************************************************************
**
** SCH_AcquirePointers
**
** NOTE: For complete prolog information, see 'sch_cmds.h'
********************************************************************/

int32 SCH_AcquirePointers(void)
{
    int32  Result;

    /*
    ** Let cFE manage the tables
    */
    CFE_TBL_Manage(SCH_AppData.ScheduleTableHandle);
    CFE_TBL_Manage(SCH_AppData.MessageTableHandle);

    /*
    ** Get a pointer to the schedule table
    */
    Result = CFE_TBL_GetAddress((void *)&SCH_AppData.ScheduleTable, 
                                         SCH_AppData.ScheduleTableHandle);

    if (Result > CFE_SUCCESS)
    {
        /*
        ** Change warning results to indicate "success"
        */
        Result = CFE_SUCCESS;
    }

    /*
    ** Repeat the process for the message table
    */
    if (Result == CFE_SUCCESS)
    {
        Result = CFE_TBL_GetAddress((void *)&SCH_AppData.MessageTable, 
                                             SCH_AppData.MessageTableHandle);
        if (Result > CFE_SUCCESS)
        {
            Result = CFE_SUCCESS;
        }
    }

    return(Result);

} /* End of SCH_AcquirePointers() */


/*******************************************************************
**
** SCH_VerifyCmdLength
**
** NOTE: For complete prolog information, see above
********************************************************************/

int32 SCH_VerifyCmdLength (CFE_SB_MsgPtr_t MessagePtr, uint32 ExpectedLength)
{
    int32               Status = SCH_SUCCESS;
    CFE_SB_MsgId_t      MessageID = 0;
    uint16              CommandCode = 0; 
    uint16              ActualLength = 0;
   
    ActualLength  = CFE_SB_GetTotalMsgLength(MessagePtr);
      
    if (ExpectedLength != ActualLength)
    {
        MessageID   = CFE_SB_GetMsgId(MessagePtr);
        CommandCode = CFE_SB_GetCmdCode(MessagePtr);   
         
        CFE_EVS_SendEvent(SCH_CMD_LEN_ERR_EID, CFE_EVS_ERROR,
                          "Cmd Msg with Bad length Rcvd: ID = 0x%04X, CC = %d, Exp Len = %d, Len = %d",
                          MessageID, CommandCode, ExpectedLength, ActualLength);

        Status = SCH_BAD_MSG_LENGTH_RC;
    }

    return Status;

} /* End of SCH_VerifyCmdLength () */


/*******************************************************************
**
** SCH_PostCommandResult
**
** NOTE: For complete prolog information, see above
********************************************************************/

void SCH_PostCommandResult(boolean GoodCommand)
{
    if (GoodCommand)
    {
        SCH_AppData.CmdCounter++;
    }
    else
    {
        SCH_AppData.ErrCounter++;
    }

    return;

} /* End of SCH_PostCommandResult() */



/************************/
/*  End of File Comment */
/************************/

