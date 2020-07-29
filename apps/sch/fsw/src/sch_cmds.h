/************************************************************************
** File:
**   $Id: sch_cmds.h 1.4 2017/06/21 15:29:00EDT mdeschu Exp  $
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
**  The CFS Scheduler (SCH) Application header file
**
** Notes:
**
**
*************************************************************************/
#ifndef _sch_cmds_
#define _sch_cmds_

/*************************************************************************
**
** Include section
**
**************************************************************************/

#include "cfe.h"

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

/*************************************************************************
**
** Exported data
**
**************************************************************************/

/*************************************************************************
**
** Exported functions
**
**************************************************************************/
/************************************************************************/
/** \brief Process a command pipe message
**  
**  \par Description
**       Processes a single software bus command pipe message. Checks
**       the message and command IDs and calls the appropriate routine
**       to handle the command.
**       
**  \par Assumptions, External Events, and Notes:
**       None
**       
**  \param [in]  MessagePtr   A #CFE_SB_MsgPtr_t pointer that
**                            references the software bus message 
**
**  \returns
**  \retcode #CFE_SUCCESS  \retdesc \copydoc CFE_SUCCESS \endcode
**  \retstmt Return codes from #CFE_EVS_Register         \endcode
**  \retstmt Return codes from #CFE_SB_CreatePipe        \endcode
**  \retstmt Return codes from #CFE_SB_Subscribe         \endcode
**  \endreturns
**
**  \sa #CFE_SB_RcvMsg
**
*************************************************************************/
int32 SCH_AppPipe(CFE_SB_MsgPtr_t MessagePtr);

/************************************************************************/
/** \brief Manages Scheduler's Schedule and Message Definition Tables
**  
**  \par Description
**       This function manages the contents of the Schedule and Message
**       Definition Tables.
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
int32 SCH_AcquirePointers(void);

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

#endif /* _sch_cmds_ */

/************************/
/*  End of File Comment */
/************************/
