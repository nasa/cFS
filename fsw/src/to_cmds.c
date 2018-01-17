/******************************************************************************/
/** \file  to_cmds.c
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*  
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Function Definitions of command responses
*
*   \par
*       This file defines command responses for TO functions.
*
*   \par Cmd Functions Defined:
*     - TO_NoopCmd - No-operation command. Returns version number.
*     - TO_ResetCmd - Reset Housekeeping counters.
*     - TO_EnableOutputCmd - Enable output with call to custom fnct.
*     - TO_DisableOutputCmd - Disable output with call to custom fnct. 
*     - TO_PauseOutputCmd - Pause output (set outputActive = 0)
*     - TO_ActivateRoutesCmd - Activate routes (set usIsEnabled = 1)
*     - TO_DeactivateRoutesCmd - Deactivate routes (set usIsEnabled = 0) 
*     - TO_ResumeOutputCmd - Resume output (set outputActive = 1)
*     - TO_AddTblEntryCmd - Add a new entry to config table.
*     - TO_RemoveTblEntryCmd - Remove a table entry by MID.
*     - TO_EnableTblEntryCmd - Enable a table entry by MID.
*     - TO_DisableTblEntryCmd - Disable a table entry by MID.
*     - TO_EnableGroupCmd - Enable table entries by Group.
*     - TO_DisableGroupCmd - Disable table entries by Group.
*     - TO_EnableAllCmd - Enable all table entries.
*     - TO_DisableAllCmd - Disable all table entries.
*     - TO_SetRouteByMidCmd - Set table entry RouteMask by MID.
*     - TO_SetRouteByGroupCmd - Set table entries RouteMask by Group.
*     - TO_ManageTableCmd - Manage Table cmd (sent by CFE_TBL)
*     - TO_SetRoutePeriodCmd - Set routes' wakeup period.
*     - TO_SetWakeupTimeoutcmd - Set the TO Wakeup Timeout.
*
*   \par Custom Functions Required:
*     - TO_CustomEnableOutputCmd - Custom response to enable output cmd.
*     - TO_CustomDisableOutputCmd - Custom response to disable output cmd. 
*
*   \par Limitations, Assumptions, External Events, and Notes:
*
*   \par Modification History:
*     - 2015-01-09 | Guy de Carufel | Code Started
*******************************************************************************/
#include "to_app.h"
#include "to_cmds.h"
#include "cfe_tbl_msg.h"

extern TO_AppData_t     g_TO_AppData;

/* Optional Custom Commands. 
 * Call these through the TO_CustomAppCmds function in to_custom.c if desired. 
 * Add an extern prototype definition in the to_custom.c file. */
void TO_SendDataTypePktCmd(CFE_SB_MsgPtr_t);


/******************************************************************************/
/** \brief Noop Command (TO_NOOP_CC)
*******************************************************************************/
void TO_NoopCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_NoArgCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID,
                          CFE_EVS_INFORMATION,
                          "No-op command. Version %d.%d.%d.%d",
                          TO_MAJOR_VERSION,
                          TO_MINOR_VERSION,
                          TO_REVISION,
                          TO_MISSION_REV);
    } 
}


/******************************************************************************/
/** \brief Reset Command (TO_RESET_CC)
*******************************************************************************/
void TO_ResetCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_NoArgCmd_t)))
    {
        /* Note: the subCnt is not reset. */
        
        g_TO_AppData.HkTlm.usCmdCnt = 0;
        g_TO_AppData.HkTlm.usCmdErrCnt = 0;
        g_TO_AppData.HkTlm.usMsgSubErrCnt = 0;
        g_TO_AppData.HkTlm.usTblUpdateCnt = 0;
        g_TO_AppData.HkTlm.usTblErrCnt = 0;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd RESET cmd (%d)", TO_RESET_CC);
    }
}


/******************************************************************************/
/** \brief Enable Output Command (TO_ENABLE_OUTPUT_CC)
*******************************************************************************/
void TO_EnableOutputCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    int32 ii = 0;
    int32 routeMask = 0;

    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_EnableOutputCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd ENABLE_OUTPUT cmd (%d)", 
                          TO_ENABLE_OUTPUT_CC);

        /* NOTES: 
            - The CustomEnableOutputCmd is responsible for doing any 
              necessary device configuration based on the routeMask. 
            - The custom layer is also responsible for validating the 
              routeMask command parameter, if present. 
              May use TO_ValidateRouteMask utility function.
        */
        
        /* The custom function respond with the routeMask that it enabled. */
        routeMask = TO_CustomEnableOutputCmd(pCmdMsg);
        
        if (routeMask > TO_MAX_ROUTE_MASK)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              " Route mask exceeds max route mask." 
                              " ENABLE_OUTPUT Cmd failed.");
        }
        else if (routeMask >= 0)
        {
            /* Set all routes by mask as enabled */
            for (ii = 0; ii < TO_MAX_NUM_ROUTES; ++ii)
            {
                if (routeMask & (1 << ii))
                {
                    if (!g_TO_AppData.routes[ii].usExists ||
                        !g_TO_AppData.routes[ii].usIsConfig)
                    {
                        g_TO_AppData.HkTlm.usCmdErrCnt++;
                        CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                          "Attempting to enable route:%u which "
                          "does not exist or is not configured.", ii);
                        goto end_of_command;
                    }
                              
                    g_TO_AppData.routes[ii].usIsEnabled = 1;
                }
            }

            g_TO_AppData.usOutputEnabled = 1;
            g_TO_AppData.usOutputActive = 1;

            /* Update housekeeping usEnabledRoutes */
            g_TO_AppData.HkTlm.usEnabledRoutes |= routeMask;
            
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "ENABLE OUTPUT CMD Succesful for Routes:0x%04x",
                              routeMask);
        }
        else
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              " ENABLE OUTPUT CMD failed.");
        }
    }

end_of_command:
    return;
}


/******************************************************************************/
/** \brief Disable Output Command (TO_DISABLE_OUTPUT_CC)
*******************************************************************************/
void TO_DisableOutputCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    int32 iStatus = TO_SUCCESS;
    int32 routeMask = TO_SUCCESS;
    int32 ii;
    
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_DisableOutputCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd DISABLE_OUTPUT cmd (%d)", 
                          TO_DISABLE_OUTPUT_CC);

        if (g_TO_AppData.usOutputEnabled == 0)
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "Output already disabled. "
                              "DISABLE_OUTPUT CMD ignored.");
            goto end_of_command;
        }

        /* NOTE: Examples of Custom implementation actions:
            - Ignore command 
            - Disable all output (usOutputEnabled = 0
            - Disable specific routes by returning routeMask */
        iStatus = TO_CustomDisableOutputCmd(pCmdMsg);
        
        if (iStatus == TO_SUCCESS)
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "DISABLE_OUTPUT CMD Succesful.");
        }
        /* If a positive value is returned, disable specified route mask */
        else if (iStatus > 0)
        {
            routeMask = iStatus;
            
            if (routeMask > TO_MAX_ROUTE_MASK)
            {
                g_TO_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                                  " Route mask exceeds max route mask." 
                                  " DISABLE_OUTPUT CMD failed.");
                goto end_of_command;
            }

            /* Set all routes by mask as disabled */
            for (ii = 0; ii < TO_MAX_NUM_ROUTES; ++ii)
            {
                if (routeMask & (1 << ii))
                {
                    g_TO_AppData.routes[ii].usIsEnabled = 0;
                }
            }

            g_TO_AppData.HkTlm.usEnabledRoutes  &= ~routeMask;

            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                    "DISABLE_OUTPUT CMD Successful for Routes:0x%04x",
                    routeMask);
        }
        else
        {
            /* Error message issued by TO_CustomDisableOutputCmd() */
            /* Increment error counter if appropriate within Custom function. */
        }
    }

end_of_command:
    return;
}


/******************************************************************************/
/** \brief Activate Routes Command (TO_ACTIVATE_ROUTES_CC)
*******************************************************************************/
void TO_ActivateRoutesCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    uint16 ii;

    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_RouteMaskArgCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd ACTIVATE_ROUTES cmd (%d)", 
                          TO_ACTIVATE_ROUTES_CC);

        TO_RouteMaskArgCmd_t *cmd = (TO_RouteMaskArgCmd_t *) pCmdMsg;

        for (ii = 0; ii < TO_MAX_NUM_ROUTES; ++ii)
        {
            if (cmd->usRouteMask & (1 << ii))
            {
                if (g_TO_AppData.routes[ii].usIsConfig == 0)
                {
                    g_TO_AppData.HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                      "Route ID:%u is not configured and so can't be enabled. "
                      "ACTIVATE ROUTES with route mask:0x%04x failed.",
                      ii, cmd->usRouteMask);
                      goto end_of_command;
                }
               
                if (g_TO_AppData.routes[ii].usIsEnabled == 1)
                {
                    CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                      "Route ID:%u is already enabled.", ii);
                }

                g_TO_AppData.routes[ii].usIsEnabled = 1;
            }
        }

        g_TO_AppData.HkTlm.usEnabledRoutes |= cmd->usRouteMask;

        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                      "Routes 0x%04x succesfully enabled.", cmd->usRouteMask);
    }

end_of_command:
    return;
}


/******************************************************************************/
/** \brief Deactivate Routes Command (TO_DEACTIVATE_ROUTES_CC)
*******************************************************************************/
void TO_DeactivateRoutesCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    uint16 ii;

    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_RouteMaskArgCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd DEACTIVATE_ROUTES cmd (%d)", 
                          TO_DEACTIVATE_ROUTES_CC);

        TO_RouteMaskArgCmd_t *cmd = (TO_RouteMaskArgCmd_t *) pCmdMsg;

        for (ii = 0; ii < TO_MAX_NUM_ROUTES; ++ii)
        {
            if (cmd->usRouteMask & (1 << ii))
            {
                g_TO_AppData.routes[ii].usIsEnabled = 0;
            }
        }
        
        g_TO_AppData.HkTlm.usEnabledRoutes &= ~cmd->usRouteMask;

        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                    "Routes 0x%04x succesfully disabled.", cmd->usRouteMask);
    }

    return;
}


/******************************************************************************/
/** \brief Pause Output Command (TO_PAUSE_OUTPUT_CC)
*******************************************************************************/
void TO_PauseOutputCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_NoArgCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd PAUSE_OUTPUT cmd (%d)", 
                          TO_PAUSE_OUTPUT_CC);
        
        if (g_TO_AppData.usOutputEnabled == 0)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Output is disabled - can't pause. "
                              "PAUSE_OUTPUT CMD failed.");
            goto end_of_command;
        }
        
        g_TO_AppData.usOutputActive = 0;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "PAUSE_OUTPUT CMD Succesful.");
    }

end_of_command:
    return;
}


/******************************************************************************/
/** \brief Resume Output Command (TO_RESUME_OUTPUT_CC)
*******************************************************************************/
void TO_ResumeOutputCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_NoArgCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd RESUME_OUTPUT cmd (%d)", 
                          TO_RESUME_OUTPUT_CC);
        
        if (g_TO_AppData.usOutputEnabled == 0)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Output is disabled - "
                              "send enable instead of resume. "
                              "RESUME_OUTPUT CMD failed.");
            goto end_of_command;
        }
        
        g_TO_AppData.usOutputActive = 1;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "RESUME_OUTPUT CMD Succesful."); 
    }

end_of_command:
    return;
}


/******************************************************************************/
/** \brief Add a Table Entry (TO_ADD_TBL_ENTRY_CC)
*******************************************************************************/
void TO_AddTblEntryCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    TO_AddTblEntryCmd_t * pCmd = (TO_AddTblEntryCmd_t *) pCmdMsg;
    TO_TableEntry_t     *pEntry = NULL;
    int32 index = 0;
    int32 exists = 0;
    int32 iStatus = TO_SUCCESS;

    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_AddTblEntryCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd ADD_TBL_ENTRY cmd (%d)", 
                          TO_ADD_TBL_ENTRY_CC);
        
        /* Note that the RouteMask is not validated here as table entries 
           are permitted to have routeMasks which include non-existing routes. 
           Those routes will simply be ignored. */
        
        /* Validate MID */
        if (pCmd->usMsgId == TO_REMOVED_ENTRY || 
            pCmd->usMsgId == TO_UNUSED_ENTRY)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Received invalid MSG ID: 0x%04x."
                              " TO_ADD_TBL_ENTRY CMD failed.",
                              pCmd->usMsgId);
            goto end_of_command;
        }

        index = TO_FindEmptyTableIndex();
        
        if (index == TO_TBL_FULL_ERR)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "No empty table entries."
                              " TO_ADD_TBL_ENTRY CMD failed.");
        }
        else 
        {
            /* Verify table for duplicate usMsgIds */
            exists = TO_FindTableIndex(g_TO_AppData.pConfigTable, pCmd->usMsgId);

            if (exists != TO_NO_MATCH)
            {
                g_TO_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                                  "Pre-existing table entry with MID:0x%04x"
                                  " TO_ADD_TBL_ENTRY CMD failed.", pCmd->usMsgId);
                goto end_of_command;
            }
            
            pEntry = &g_TO_AppData.pConfigTable->entries[index];
            pEntry->usMsgId     = pCmd->usMsgId;
            pEntry->qos         = pCmd->qos;      
            pEntry->usMsgLimit  = pCmd->usMsgLimit; 
            pEntry->usRouteMask = pCmd->usRouteMask;

            /* Subscribe tlm pipes to new msgid according to routeMask */
            iStatus = TO_SubscribeMsg(pEntry);
            if (iStatus != CFE_SUCCESS)
            {
                g_TO_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                                  "TO_ADD_TBL_ENTRY CMD failed.", pCmd->usMsgId);
                /* Reset entry */
                pEntry->usMsgId         = 0;
                pEntry->qos.Priority    = 0;
                pEntry->qos.Reliability = 0;
                pEntry->usMsgLimit      = 0;
                pEntry->usRouteMask     = 0;
                
                goto end_of_command;
            }
            
            /* Set table entry. */
            pEntry->uiGroupData = pCmd->uiGroupData;
            pEntry->usFlag      = pCmd->usFlag;
            pEntry->usState     = pCmd->usState;
            
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "Succesfully added TBL entry. "
                              "Index:%d, MsgID:0x%04x, RouteMask:0x%04x, "
                              "usMsgLimit:%d, uiGroupData:0x%08x, initState:%u",
                              index, pEntry->usMsgId, pEntry->usRouteMask,
                              pEntry->usMsgLimit, pEntry->uiGroupData, 
                              pEntry->usState);
        }
    }

end_of_command:
    return;
}


/******************************************************************************/
/** \brief Remove a Table Entry by MID (TO_REMOVE_TBL_ENTRY_CC)
*******************************************************************************/
void TO_RemoveTblEntryCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    TO_MidArgCmd_t * pCmd = (TO_MidArgCmd_t *) pCmdMsg;
    TO_TableEntry_t *pEntry = NULL;
    int32 index = 0;
    int32 iStatus = TO_SUCCESS;
    
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_MidArgCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd REMOVE_TBL_ENTRY cmd (%d)", 
                          TO_REMOVE_TBL_ENTRY_CC);

        /* Validate MID */
        if (pCmd->usMsgId == TO_REMOVED_ENTRY || 
            pCmd->usMsgId == TO_UNUSED_ENTRY)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Received invalid MSG ID: 0x%04x."
                              " TO_REMOVE_TBL_ENTRY CMD failed.",
                              pCmd->usMsgId);
            goto end_of_command;
        }

        /* Find the Table entry corresponding to MID */
        index = TO_FindTableIndex(g_TO_AppData.pConfigTable, pCmd->usMsgId);

        if (index < 0)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "MSG ID not found."
                              " TO_REMOVE_TBL_ENTRY CMD failed.");
            goto end_of_command;
        }
        
        pEntry = &g_TO_AppData.pConfigTable->entries[index];

        /* Unsubscribe the MID from the Tlm Pipes */
        iStatus = TO_UnsubscribeMsg(pEntry);
        if (iStatus != TO_SUCCESS)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "TO_REMOVE_TBL_ENTRY CMD failed.");
            goto end_of_command;
        }

        /* Set the table entry as removed. */
        pEntry->usMsgId = TO_REMOVED_ENTRY;

        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Succesfully Removed TBL entry. "
                          "Index:%d, MsgId:0x%04x",
                          index, pCmd->usMsgId);
    }

end_of_command:
    return;
}


/******************************************************************************/
/** \brief Enable a Table Entry by MID (TO_ENABLE_TBL_ENTRY_CC)
*******************************************************************************/
void TO_EnableTblEntryCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    TO_MidArgCmd_t * pCmd = (TO_MidArgCmd_t *) pCmdMsg;
    TO_TableEntry_t *pEntry = NULL;
    int32 index = 0;
    
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_MidArgCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd ENABLE_TBL_ENTRY cmd (%d)", 
                          TO_ENABLE_TBL_ENTRY_CC);
        
        /* Validate MID */
        if (pCmd->usMsgId == TO_REMOVED_ENTRY || 
            pCmd->usMsgId == TO_UNUSED_ENTRY)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Received invalid MSG ID: 0x%04x."
                              " TO_ENABLE_TBL_ENTRY CMD failed.",
                              pCmd->usMsgId);
            goto end_of_command;
        }
        
        index = TO_FindTableIndex(g_TO_AppData.pConfigTable, pCmd->usMsgId);

        if (index < 0)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "MSG ID not found."
                              " TO_ENABLE_TBL_ENTRY CMD failed.");
            goto end_of_command;
        }
        
        /* Set the table entry as enabled. */
        pEntry = &g_TO_AppData.pConfigTable->entries[index];
        
        if (pEntry->usState == 1)
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "Table Entry idx:%u for MID:0x%04x already "
                              "enabled. TO_ENABLE_TBL_ENTRY CMD ignored.",
                              index, pCmd->usMsgId);
            goto end_of_command;
        }
        
        pEntry->usState     = 1;

        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Succesfully Enabled TBL entry:%u, MID:0x%04x",
                          index, pCmd->usMsgId);
    }

end_of_command:
    return;
}


/******************************************************************************/
/** \brief Disable a Table Entry by MID (TO_DISABLE_TBL_ENTRY_CC)
*******************************************************************************/
void TO_DisableTblEntryCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    TO_MidArgCmd_t * pCmd = (TO_MidArgCmd_t *) pCmdMsg;
    TO_TableEntry_t * pEntry = NULL;
    int32 index = 0;
    
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_MidArgCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd DISABLE_TBL_ENTRY cmd (%d)", 
                          TO_DISABLE_TBL_ENTRY_CC);
        
        /* Validate MID */
        if (pCmd->usMsgId == TO_REMOVED_ENTRY || 
            pCmd->usMsgId == TO_UNUSED_ENTRY)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Received invalid MSG ID: 0x%04x."
                              " TO_DISABLE_TBL_ENTRY CMD failed.",
                              pCmd->usMsgId);
            goto end_of_command;
        }
        
        index = TO_FindTableIndex(g_TO_AppData.pConfigTable, pCmd->usMsgId);

        if (index < 0)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "MSG ID not found."
                              " TO_DISABLE_TBL_ENTRY CMD failed.");
            goto end_of_command;
        }
        
        /* Set the table entry as enabled. */
        pEntry = &g_TO_AppData.pConfigTable->entries[index];
        
        if (pEntry->usState == 0)
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "Table Entry idx:%u for MID:0x%04x already "
                              "disabled. TO_DISABLE_TBL_ENTRY CMD ignored.",
                              index, pCmd->usMsgId);
            goto end_of_command;
        }
        pEntry->usState = 0;

        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Succesfully Disabled TBL entry:%u, MID:0x%04x",
                          index, pCmd->usMsgId);
    }

end_of_command:
    return;
}


/******************************************************************************/
/** \brief Enable Table Entries by Group (TO_ENABLE_GROUP_CC)
*******************************************************************************/
void TO_EnableGroupCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    TO_GroupArgCmd_t * pCmd = (TO_GroupArgCmd_t *) pCmdMsg;
    int32 iStatus = TO_SUCCESS;

    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_GroupArgCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd ENABLE_GROUP cmd (%d)", 
                          TO_ENABLE_GROUP_CC);
        
        iStatus = TO_SetStateByGroup(pCmd->uiGroupData, 1);

        if (iStatus == TO_BAD_ARG_ERR)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Bad uiGroupData argument." 
                              "ENABLE_GROUP CMD failed.");
        }
        else if (iStatus == TO_NO_MATCH)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "No group match found. "
                              "ENABLE_GROUP CMD failed.");
        }
        else if (iStatus == TO_NO_EFFECT)
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "Group already enabled. "
                              "ENABLE_GROUP CMD ignored.");
        }
        else if (iStatus == TO_SUCCESS)
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "ENABLE GROUP CMD Successful. GroupData:0x%08x",
                              pCmd->uiGroupData);
        }
    }
}


/******************************************************************************/
/** \brief Disable Table Entries by Group (TO_DISABLE_GROUP_CC)
*******************************************************************************/
void TO_DisableGroupCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    TO_GroupArgCmd_t * pCmd = (TO_GroupArgCmd_t *) pCmdMsg;
    int32 iStatus = TO_SUCCESS;

    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_GroupArgCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd DISABLE_GROUP cmd (%d)", 
                          TO_DISABLE_GROUP_CC);
        
        iStatus = TO_SetStateByGroup(pCmd->uiGroupData, 0);

        if (iStatus == TO_BAD_ARG_ERR)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Bad uiGroupData argument." 
                              "DISABLE_GROUP CMD Ignored.");
        }
        else if (iStatus == TO_NO_MATCH)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "No group match found."
                              "DISABLE_GROUP CMD Ignored.");
        }
        else if (iStatus == TO_NO_EFFECT)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Group already disabled."
                              "DISABLE_GROUP CMD Ignored.");
        }
        else if (iStatus == TO_SUCCESS)
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "DISABLE GROUP CMD Successful. GroupData:0x%08x",
                              pCmd->uiGroupData);
        }
    }
}


/******************************************************************************/
/** \brief Enable All Used Table Entries (TO_ENABLE_ALL_CC)
*******************************************************************************/
void TO_EnableAllCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    int32 iStatus = TO_SUCCESS;

    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_NoArgCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd ENABLE ALL cmd (%d)", 
                          TO_ENABLE_ALL_CC);
        
        iStatus = TO_SetAllEntryState(1);

        if (iStatus == TO_NO_EFFECT) 
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "All table entries already enabled. "
                              "ENABLE_ALL CMD Ignored.");
        }
        else 
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "All Entries enabled.");
        }
    }
}


/******************************************************************************/
/** \brief Disable All Used Table Entries (TO_DISABLE_ALL_CC)
*******************************************************************************/
void TO_DisableAllCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    int32 iStatus = TO_SUCCESS;
    
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_NoArgCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd DISABLE ALL cmd (%d)", 
                          TO_DISABLE_ALL_CC);
        
        iStatus = TO_SetAllEntryState(0);
        
        if (iStatus == TO_NO_EFFECT) 
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "All table entries already disabled. "
                              "DISABLE_ALL CMD Ignored.");
        }
        else 
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "All Entries disabled.");
        }
    }
}


/******************************************************************************/
/** \brief Set table entry routeMask by MID (TO_SET_ROUTE_BY_MID_CC)
*******************************************************************************/
void TO_SetRouteByMidCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    TO_SetRouteByMidCmd_t * pCmd = (TO_SetRouteByMidCmd_t *) pCmdMsg;
    TO_TableEntry_t * pEntry = NULL;
    int32 index = 0;
    
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_SetRouteByMidCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd SET_ROUTE_BY_MID cmd (%d)", 
                          TO_SET_ROUTE_BY_MID_CC);
        
        /* Note that the RouteMask is not validated here as table entries 
           are permitted to have routeMasks which include non-existing routes. 
           Those routes will simply be ignored. */
        
        /* Validate MID */
        if (pCmd->usMsgId == TO_REMOVED_ENTRY || 
            pCmd->usMsgId == TO_UNUSED_ENTRY)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Received invalid MSG ID: 0x%04x."
                              " TO_SET_ROUTE_BY_MID CMD failed.",
                              pCmd->usMsgId);
            goto end_of_command;
        }
        
        index = TO_FindTableIndex(g_TO_AppData.pConfigTable, pCmd->usMsgId);

        if (index < 0)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "MSG ID not found."
                              " TO_SET_ROUTE_BY_MID CMD failed.");
            goto end_of_command;
        }
        
        /* Set the table entry route */
        pEntry = &g_TO_AppData.pConfigTable->entries[index];
        pEntry->usRouteMask = pCmd->usRouteMask;

        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Succesfully set usRouteMask=0x%04x for entry:%u, "
                          "MID:0x%04x",
                          pCmd->usRouteMask, index, pCmd->usMsgId);
    }

end_of_command:
    return;
}


/******************************************************************************/
/** \brief Set table entry routeMask by Group (TO_SET_ROUTE_BY_GROUP_CC)
*******************************************************************************/
void TO_SetRouteByGroupCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    TO_SetRouteByGroupCmd_t * pCmd = (TO_SetRouteByGroupCmd_t *) pCmdMsg;
    int32 iStatus = TO_SUCCESS;
    
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_SetRouteByGroupCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd SET_ROUTE_BY_GROUP cmd (%d)", 
                          TO_SET_ROUTE_BY_GROUP_CC);
        
        /* Note that the RouteMask is not validated here as table entries 
           are permitted to have routeMasks which include non-existing routes.
           Those routes will simply be ignored. */
        
        iStatus = TO_SetRouteByGroup(pCmd->uiGroupData, pCmd->usRouteMask);
        
        if (iStatus == TO_BAD_ARG_ERR)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "SET_ROUTE_BY_GROUP CMD - Bad uiGroupData arg." 
                              " CMD Ignored.");
        }
        else if (iStatus == TO_NO_MATCH)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "SET_ROUTE_BY_GROUP CMD - No group match found."
                              " CMD Ignored.");
        }
        else if (iStatus == TO_NO_EFFECT)
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "Route already set for group. "
                              "SET_ROUTE_BY_GROUP CMD ignored.");
        }
        else if (iStatus == TO_SUCCESS)
        {
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "SET_ROUTE_BY_GROUP CMD Successful. "
                              "usRouteMask=0x%04x, uGroupData=0x%08x",
                              pCmd->usRouteMask, pCmd->uiGroupData);
        }
    }

    return;
}


/******************************************************************************/
/** \brief Manage the Table - Hook for TBL Services (TO_MANAGE_TABLE_CC)
*******************************************************************************/
void TO_ManageTableCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    int32 iStatus = CFE_SUCCESS;
    TO_ConfigTable_t *pTable = NULL;
    TO_ConfigTable_t oldTable;

    if (TO_VerifyCmdLength(pCmdMsg, sizeof(CFE_TBL_NotifyCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd MANAGE TABLE CMD (%d)", 
                          TO_MANAGE_TABLE_CC);

        /* Temporarily save the previous table */
        memcpy((void *) &oldTable, (void *) g_TO_AppData.pConfigTable, 
               sizeof(TO_ConfigTable_t));

        /* Release the table pointer so that CFE can update it. */
        iStatus = CFE_TBL_ReleaseAddress(g_TO_AppData.tableHandle); 
        if (iStatus != CFE_SUCCESS)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            g_TO_AppData.HkTlm.usTblErrCnt++;
            CFE_EVS_SendEvent(TO_TBL_ERR_EID, CFE_EVS_ERROR,
               "CFE_TBL_ReleaseAddress() returned error 0x%08x. "
               "TO_ManageTable failed.",
               iStatus);
            goto end_of_command;
        }

        /* This is where the table gets updated. */
        iStatus = CFE_TBL_Manage(g_TO_AppData.tableHandle);
        if (iStatus != CFE_SUCCESS && iStatus != CFE_TBL_INFO_UPDATED)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            g_TO_AppData.HkTlm.usTblErrCnt++;
            CFE_EVS_SendEvent(TO_TBL_ERR_EID, CFE_EVS_ERROR,
              "CFE_TBL_Manage() returned error 0x%08x.  TO_ManageTable failed.",
              iStatus);
        
            /* Re-acquire table address. */
            CFE_TBL_GetAddress ((void **) &pTable, g_TO_AppData.tableHandle);
            g_TO_AppData.pConfigTable = pTable;

            goto end_of_command;
        }

        iStatus = CFE_TBL_GetAddress ((void **) &pTable, 
                                      g_TO_AppData.tableHandle);
        /* Status should be CFE_TBL_INFO_UPDATED because we loaded it above */
        if (iStatus == CFE_TBL_INFO_UPDATED)
        {
            /* Store the new table pointer */
            g_TO_AppData.pConfigTable = pTable;
            
            /* Unsubscribe all messages from old table */
            iStatus = TO_UnsubscribeAllMsgs(&oldTable);
            if (iStatus != CFE_SUCCESS)
            {
                g_TO_AppData.HkTlm.usCmdErrCnt++;
                g_TO_AppData.HkTlm.usTblErrCnt++;
                CFE_EVS_SendEvent(TO_PIPE_ERR_EID, CFE_EVS_ERROR,
                    "Unsubscription of messages failed. "
                    "Reverting back table. TO_ManageTable failed.");
                memcpy((void *) pTable, (void *) &oldTable, 
                       sizeof(TO_ConfigTable_t));
                iStatus = TO_SubscribeAllMsgs();
                goto end_of_command;
            }
            
            /* Subscribe to all messages in new table */
            iStatus = TO_SubscribeAllMsgs();
            if (iStatus != CFE_SUCCESS)
            {
                g_TO_AppData.HkTlm.usCmdErrCnt++;
                g_TO_AppData.HkTlm.usTblErrCnt++;
                CFE_EVS_SendEvent(TO_PIPE_ERR_EID, CFE_EVS_ERROR,
                    "Subscription of messages failed. "
                    "Reverting back table. TO_ManageTable failed.");
                memcpy((void *) pTable, (void *) &oldTable, 
                       sizeof(TO_ConfigTable_t));
                iStatus = TO_SubscribeAllMsgs();
                goto end_of_command;
            }
            
            g_TO_AppData.HkTlm.usTblUpdateCnt++;
            CFE_EVS_SendEvent(TO_TBL_INF_EID, CFE_EVS_INFORMATION,
                              "ConfigTable updated succesfully. ");
        }
        else if (iStatus == CFE_SUCCESS)
        {
            /* Store the pointer */
            g_TO_AppData.pConfigTable = pTable;
            
            CFE_EVS_SendEvent(TO_TBL_INF_EID, CFE_EVS_INFORMATION,
                              "ConfigTable did not change.");
        }
        else 
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            g_TO_AppData.HkTlm.usTblErrCnt++;
            CFE_EVS_SendEvent(TO_TBL_ERR_EID, CFE_EVS_ERROR,
                    "CFE_TBL_GetAddress() returned error 0x%08x. "  
                    "TO_ManageTable failed.",
                    iStatus);
        }
    }

end_of_command:
    return;
}


/******************************************************************************/
/** \brief Set the route period by RouteMask (TO_SET_ROUTE_PERIOD_CC)
*******************************************************************************/
void TO_SetRoutePeriodCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    TO_SetRoutePeriodCmd_t * pCmd = (TO_SetRoutePeriodCmd_t *) pCmdMsg;
    uint16 ii;
    
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_SetRoutePeriodCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd SET_ROUTE_PERIOD cmd (%d)", 
                          TO_SET_ROUTE_PERIOD_CC);
        
        /* Validate RouteMask */
        if (TO_ValidateRouteMask(pCmd->usRouteMask) != TO_SUCCESS)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Received invalid RouteMask: 0x%04x. "
                              "RouteMask includes non-existing routes. "
                              "TO_SET_ROUTE_PERIOD CMD failed.",
                              pCmd->usRouteMask);
            goto end_of_command;
        }
   
        /* Validate Wake Period value */
        if (pCmd->usWakePeriod > TO_MAX_WAKEUP_COUNT) 
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Received invalid WakePeriod:%d. "
                              "Period must be <= TO_MAX_WAKEUP_COUNT:%u. "
                              "TO_SET_ROUTE_PERIOD CMD failed.",
                              pCmd->usWakePeriod, TO_MAX_WAKEUP_COUNT);
            goto end_of_command;
        }
        
        if (TO_MAX_WAKEUP_COUNT % pCmd->usWakePeriod != 0)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Received invalid WakePeriod:%d. "
                              "Wake Period must be a factor of %u. "
                              "TO_SET_ROUTE_PERIOD CMD failed.",
                              pCmd->usWakePeriod, TO_MAX_WAKEUP_COUNT);
            goto end_of_command;
        }

        /* Go over every route and set route period. */
        for (ii = 0; ii < TO_MAX_NUM_ROUTES; ++ii)
        {
            if (pCmd->usRouteMask & (1<<ii))
            {
                g_TO_AppData.routes[ii].usWakePeriod = pCmd->usWakePeriod;
            }
        }

        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "SET_ROUTE_PERIOD CMD Successful. "
                          "usRouteMask=0x%04x, usWakePeriod=%d",
                          pCmd->usRouteMask, pCmd->usWakePeriod);
    }

end_of_command:
    return;
}


/******************************************************************************/
/** \brief Set Wakeup Timeout (TO_SET_WAKEUP_TIMEOUT_CC)
*******************************************************************************/
void TO_SetWakeupTimeoutCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    TO_SetWakeupTimeoutCmd_t * pCmd = (TO_SetWakeupTimeoutCmd_t *) pCmdMsg;
    
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_SetWakeupTimeoutCmd_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "Recvd SET_WAKEUP_TIMEOUT cmd (%d)", 
                          TO_SET_ROUTE_PERIOD_CC);
        
        /* Validate input timeout value */
        if (pCmd->uiWakeupTimeout != CFE_SB_PEND_FOREVER &&
            pCmd->uiWakeupTimeout < TO_MIN_WAKEUP_TIMEOUT)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
            CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                              "Cmd Timeout value %d must be >= to "
                              "TO_MIN_WAKEUP_TIMEOUT: %d. "
                              "TO_SET_WAKEUP_TIMEOUT CMD failed.",
                              pCmd->uiWakeupTimeout, TO_MIN_WAKEUP_TIMEOUT);
            goto end_of_command;
        }
        
        g_TO_AppData.uiWakeupTimeout = pCmd->uiWakeupTimeout;

        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "SET_WAKEUP_TIMEOUT CMD Successful. "
                          "wakeup Timeout:%d (ms).", pCmd->uiWakeupTimeout);
    }

end_of_command:
    return;
}


/******************************************************************************/
/*  Optional Commands. Include these commands in TO_CustomAppCmds()           */
/******************************************************************************/

/******************************************************************************/
/** \brief Convenience Command: Send Test Data Packet 
 *
 *  \par 
 *      This is a legacy command that was present in to_lab application, 
 *      useful for determining byte packing / order on platform with respect
 *      to ground displays. Should only be used during development.
*******************************************************************************/
void TO_SendDataTypePktCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    TO_TypeDefPacket_t testPacket;
    int16           i;
    char            string_variable[10] = "ABCDEFGHIJ";
    
    
    if (TO_VerifyCmdLength(pCmdMsg, sizeof(CFE_SB_CmdHdr_t)))
    {
        g_TO_AppData.HkTlm.usCmdCnt++;
        CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                          "TO - Recvd SEND_DATA_TYPE cmd (%d)", 
                          TO_SEND_DATA_TYPE_CC);

        /* initialize data types packet */
        CFE_SB_InitMsg(&testPacket,
                       TO_DATA_TYPE_MID,
                       sizeof(testPacket), TRUE);

        CFE_SB_TimeStampMsg((CFE_SB_MsgPtr_t) &testPacket);

        /* initialize the packet data */
        testPacket.synch = 0x6969;
        
        testPacket.bit1 = 1;
        testPacket.bit2 = 0;
        testPacket.bit34 = 2;
        testPacket.bit56 = 3;
        testPacket.bit78 = 1;
        testPacket.nibble1 = 0xA;
        testPacket.nibble2 = 0x4;
        
        testPacket.bl1 = FALSE;
        testPacket.bl2 = TRUE;
        testPacket.b1 = 16;
        testPacket.b2 = 127;
        testPacket.b3 = 0x7F;
        testPacket.b4 = 0x45;
        testPacket.w1 = 0x2468;
        testPacket.w2 = 0x7FFF;
        testPacket.dw1 = 0x12345678;
        testPacket.dw2 = 0x87654321;
        testPacket.f1 = 90.01;
        testPacket.f2 = .0000045;
        testPacket.df1 = 99.9;
        testPacket.df2 = .4444;

        for (i=0; i < 10; i++) 
        {
            testPacket.str[i] = string_variable[i];
        }

        CFE_SB_SendMsg((CFE_SB_Msg_t *)&testPacket);
    }
}


/*==============================================================================
** End of file to_cmds.c
**============================================================================*/
