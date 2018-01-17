/******************************************************************************/
/** \file  to_cmds.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*  
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Header file for command responses for TO
*
*   \par Limitations, Assumptions, External Events, and Notes:
*       - Functions are called from TO_ProcessNewAppCmds in to_app.c
*       - Command functions are defined in to_cmds.c
*       - Custom functions are defined in to_custom.c
*       - Command codes defined in to_msgdefs.h
*
*   \par Modification History:
*     - 2015-01-09 | Guy de Carufel | Code Started
*     - 2016-05-11 | Allen Brown | Updated comments
*******************************************************************************/

#ifndef _TO_CMDS_H_
#define _TO_CMDS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
** Includes
*******************************************************************************/
#include "cfe.h"

/*******************************************************************************
** Command Structure definitions
*******************************************************************************/
typedef struct
{
    uint8           ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} TO_NoArgCmd_t;


typedef struct
{
    uint8           ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    CFE_SB_MsgId_t  usMsgId;      /**< Msg Id parameter */
} TO_MidArgCmd_t;

typedef struct
{
    uint8           ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16          usRouteMask;  /**< Route Mask   */
} TO_RouteMaskArgCmd_t;


typedef struct
{
    uint8           ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32          uiGroupData;  /**< GroupData for entry selection */
} TO_GroupArgCmd_t;

typedef struct
{
    uint8           ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    CFE_SB_MsgId_t  usMsgId;      /**< Message ID (must be unique)      */
    CFE_SB_Qos_t    qos;          /**< Quality of Service flag          */
    uint16          usMsgLimit;   /**< Max Num. of this Msgs in pipe    */
    uint16          usRouteMask;  /**< Bitwize Route Mask               */
    uint32          uiGroupData;  /**< Group data Mask                  */
    uint16          usFlag;       /**< Custom defined flag              */
    uint16          usState;      /**< Message ID is enabled = 1        */
} TO_AddTblEntryCmd_t;

typedef struct
{
    uint8           ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    CFE_SB_MsgId_t  usMsgId;      /**< MsgId parameter */
    uint16          usRouteMask;  /**< RouteMask to set table entry */
} TO_SetRouteByMidCmd_t;

typedef struct
{
    uint8           ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32          uiGroupData;  /**< GroupData for entry selection  */
    uint16          usRouteMask;  /**< RouteMask to set table entries */
    uint16          spare;        /**< Padding */
} TO_SetRouteByGroupCmd_t;

typedef struct
{
    uint8           ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint16          usRouteMask;   /**< RouteMask to set table entries */
    uint16          usWakePeriod;  /**< Modulus of TO Wakeup rate      */
} TO_SetRoutePeriodCmd_t;

typedef struct
{
    uint8           ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32          uiWakeupTimeout;    /**< New wakeup timeout in ms */
} TO_SetWakeupTimeoutCmd_t;


typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint16             synch;
    
    uint16             bit1:1;
    uint16             bit2:1;
    uint16             bit34:2;
    uint16             bit56:2;
    uint16             bit78:2;
    
    uint16             nibble1:4;
    uint16             nibble2:4;
    
    uint8              bl1, bl2;       /* boolean */
    
    int8               b1, b2, b3, b4;
    int16              w1,w2;
    
    int32              dw1, dw2;
    float              f1, f2;
    double             df1, df2;
    char               str[10];
} TO_TypeDefPacket_t;


/*******************************************************************************
** Application Command Function Declarations (defined in to_cmds.c)
*******************************************************************************/

/******************************************************************************/
/** \brief No-Operations command (TO_NOOP_CC)
*
*   \par Description/Algorithm
*       Increment command counter and issue event with TO version number.
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_NoArgCmd_t
*******************************************************************************/
void TO_NoopCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Reset Counter command (TO_RESET_CC)
*
*   \par Description/Algorithm
*       Reset Housekeeping counters, with the exception of usMsgSubCnt.
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_NoArgCmd_t
*******************************************************************************/
void TO_ResetCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Enable Output Command (TO_ENABLE_OUTPUT_CC)
*
*   \par Description/Algorithm
*       - Call TO_CustomEnableOutputCmd.
*       - Set usOutputEnable and usOutputActive to 1.
*       - Enable all routes (usIsEnabled = 1) based on returned routeMask. 
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_EnableOutputCmd_t
*
*   \see
*       #TO_CustomEnableOutputCmd
*******************************************************************************/
void TO_EnableOutputCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Disable Output Command (TO_DISABLE_OUTPUT_CC)
*
*   \par Description/Algorithm
*       - Call TO_CustomDisableOutputCmd.
*       - Set usOutputEnable and usOutputActive to 1.
*       - Enable all routes (usIsEnabled = 1) based on returned routeMask. 
*
*   \par Assumptions, External Events, and Notes:
*       - Command ignored if usOutputEnabled = 0
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_DisableOutputCmd_t
*******************************************************************************/
void TO_DisableOutputCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Activate Routes Command (TO_ACTIVATE_ROUTES_CC)
*
*   \par Description/Algorithm
*       - Enable routes based on route mask. 
*
*   \par Assumptions, External Events, and Notes:
*       - All routes in routeMask must be configured in order to be enabled
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_RouteMaskArgCmd_t
*******************************************************************************/
void TO_ActivateRoutesCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Deactivate Routes Command (TO_DEACTIVATE_ROUTES_CC)
*
*   \par Description/Algorithm
*       - Disable routes based on route mask. 
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_RouteMaskArgCmd_t
*******************************************************************************/
void TO_DeactivateRoutesCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Pause Output Command (TO_PAUSE_OUTPUT_CC)
*
*   \par Description/Algorithm
*       - Set usOutputActive to 0
*
*   \par Assumptions, External Events, and Notes:
*       - Output must be enabled (usOutputEnabled) to be able to Pause.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_NoArgCmd_t
*******************************************************************************/
void TO_PauseOutputCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Resume Output Command (TO_RESUME_OUTPUT_CC)
*
*   \par Description/Algorithm
*       - Set usOutputActive to 1
*
*   \par Assumptions, External Events, and Notes:
*       - Output must be enabled (usOutputEnabled) to be able to Resume.
*       - Cmd ignored if output is active.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_NoArgCmd_t
*******************************************************************************/
void TO_ResumeOutputCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Add a Table Entry (TO_ADD_TBL_ENTRY_CC)
*
*   \par Description/Algorithm
*       - Validate MID in command 
*       - Verify table is not full
*       - Verify that MID is not already in table
*       - Add new table entry to available table index 
*       - Subscribe route pipes to new entry's MID based on entry's routeMask
*
*   \par Assumptions, External Events, and Notes:
*       - New table entry must have unique MID (not present in table)
*       - Route mask in table entry may include non-existing routes
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_AddTblEntryCmd_t
*******************************************************************************/
void TO_AddTblEntryCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Remove a Table Entry by MID (TO_REMOVE_TBL_ENTRY_CC)
*
*   \par Description/Algorithm
*       - Validate MID in command 
*       - Find table entry of MID
*       - Unsubscribe route pipes for MID based on entry's routeMask
*       - Set table entry as TO_REMOVED_ENTRY 
*
*   \par Assumptions, External Events, and Notes:
*       - An entry with MID must be present
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_MidArgCmd_t
*******************************************************************************/
void TO_RemoveTblEntryCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Enable a Table Entry by MID (TO_ENABLE_TBL_ENTRY_CC)
*
*   \par Description/Algorithm
*       - Validate MID in command 
*       - Find table entry of MID
*       - Enable table entry (usState = 1)
*
*   \par Assumptions, External Events, and Notes:
*       - Command is ignored if entry is already enabled
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_MidArgCmd_t
*******************************************************************************/
void TO_EnableTblEntryCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Disable a Table Entry by MID (TO_DISABLE_TBL_ENTRY_CC)
*
*   \par Description/Algorithm
*       - Validate MID in command 
*       - Find table entry of MID
*       - Disable table entry (usState = 0)
*
*   \par Assumptions, External Events, and Notes:
*       - Command is ignored if entry is already disabled
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_MidArgCmd_t
*******************************************************************************/
void TO_DisableTblEntryCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Enable Table Entries by Group (TO_ENABLE_GROUP_CC)
*
*   \par Description/Algorithm
*       - Find all entries with matching GroupData
*       - Set matching entrie's state to 1 (enabled)
*
*   \par Assumptions, External Events, and Notes:
*       - Command has no effect if no entry has matching GroupData
*       - Command is ignored if entries with GroupData is already enabled
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_GroupArgCmd_t
*******************************************************************************/
void TO_EnableGroupCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Disable Table Entries by Group (TO_DISABLE_GROUP_CC)
*
*   \par Description/Algorithm
*       - Find all entries with matching GroupData
*       - Set matching entrie's state to 0 (disabled)
*
*   \par Assumptions, External Events, and Notes:
*       - Command has no effect if no entry has matching GroupData
*       - Command is ignored if entries with GroupData is already disabled
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_GroupArgCmd_t
*******************************************************************************/
void TO_DisableGroupCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Enable All Used Table Entries (TO_ENABLE_ALL_CC)
*
*   \par Description/Algorithm
*       - Set all non-empty entry state to 1 (enabled)
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_NoArgCmd_t
*******************************************************************************/
void TO_EnableAllCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Disable All Used Table Entries (TO_DISABLE_ALL_CC)
*
*   \par Description/Algorithm
*       - Set all non-empty entry state to 1 (disabled)
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_NoArgCmd_t
*******************************************************************************/
void TO_DisableAllCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Set table entry routeMask by MID (TO_SET_ROUTE_BY_MID_CC)
*
*   \par Description/Algorithm
*       - Validate MID
*       - Find table entry
*       - Set table entry RouteMask
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_SetRouteByMidCmd_t
*******************************************************************************/
void TO_SetRouteByMidCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Set table entry routeMask by Group (TO_SET_ROUTE_BY_GROUP_CC)
*
*   \par Description/Algorithm
*       - Find table entries matching input group
*       - Set found table entries' RouteMask
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_SetRouteByGroupCmd_t
*******************************************************************************/
void TO_SetRouteByGroupCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Manage the Table - Hook for TBL Services (TO_ManageTable_CC)
*
*   \par Description/Algorithm
*       - Call CFE_TBL_ReleaseAddress
*       - Call CFE_TBL_Manage
*       - Call CFE_TBL_GetAddress
*       - Save new table address
*       - Unsubscribe all tlm pipes from messages in previous table, according
*         to routeMask
*       - Subscribe all tlm pipes from messages in new table according to
*         routeMask
*
*   \par Assumptions, External Events, and Notes:
*       - This command should only be called by CFE_TBL services in response
*         to a table activate command (CFE_TBL_ACTIVATE_CC).
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type CFE_TBL_NotifyCmd_t
*******************************************************************************/
void TO_ManageTableCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Set the route period by RouteMask (TO_SET_ROUTE_PERIOD_CC)
*
*   \par Description/Algorithm
*       - Validate input routeMask
*       - Set the route wakeup period of all routes in routeMask
*
*   \par Assumptions, External Events, and Notes:
*       - To be valid, all routes within routeMask must exist 
*       - The input wakePeriod must be a factor of TO_MAX_WAKEUP_COUNT
*         (TO_MAX_WAKEUP_COUNT % wakePeriod == 0)
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_SetRoutePeriodCmd_t
*******************************************************************************/
void TO_SetRoutePeriodCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Set Wakeup Timeout (TO_SET_WAKEUP_TIMEOUT_CC)
*
*   \par Description/Algorithm
*       - Set a new value for the Wakeup Timeout in ms
*
*   \par Assumptions, External Events, and Notes:
*       - The input value must be greater or equal to TO_MIN_WAKEUP_TIMEOUT
*       - May be used to set the rate of execution of the TO application
*         if no TO_WAKEUP_MID is issued by the SCH application.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pCmdMsg Cmd of type TO_SetWakeupTimeoutCmd_t
*******************************************************************************/
void TO_SetWakeupTimeoutCmd(CFE_SB_MsgPtr_t);

#ifdef __cplusplus
}
#endif

#endif

/*==============================================================================
** End of file to_cmds.h
**============================================================================*/
