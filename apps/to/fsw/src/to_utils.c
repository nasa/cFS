/******************************************************************************/
/** \file  to_utils.c
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*  
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Function Definitions of utility functions
*
*   \par
*       This file defines utility functions used by other TO functions.
*
*   \par API Functions Defined:
*     - TO_FindEmptyTableIndex - Find an empty table index
*     - TO_FindTableIndex - Find a table index based on MID
*     - TO_SetStateByGroup - Set the state (enable/disable) by groupData
*     - TO_SetRouteByGroup - Set the routeMask by groupData
*     - TO_SetAllEntryState - Set the state (enable/disable) for all entries
*     - TO_DisableRoute - Disable a route ID and revised hk->usEnabledRoutes
*     - TO_ValidateRouteMask - Check that all routes in routeMask exist
*     - TO_GetRouteMask - Get the route mask at table index
*     - TO_GetMessageId - Get message ID at table index
*     - TO_VerifyCmdLength - Verify length of command message
*     - TO_SubscribeAllMsgs - Subscribe all messages to route pipes by routeMask
*     - TO_SubscribeMsg - Subscribe message to route pipes by routeMask
*     - TO_UnsubscribeAllMsgs - Unsub. all messages for route pipes by routeMask
*     - TO_UnsSubscribeMsg - Unsub. message for route pipes by routeMask
*
*   \par Private Functions Defined:
*
*   \par Limitations, Assumptions, External Events, and Notes:
*     - Utilities used by to_cmds.c and to_custom.c
*
*   \par Modification History:
*     - 2015-01-09 | Guy de Carufel | Code Started
*******************************************************************************/

/*
** Include Files
*/
#include "to_app.h"

/*
** External Global Variables
*/
extern TO_AppData_t g_TO_AppData;


/******************************************************************************/
/** \brief Find Empty Table Index.
*******************************************************************************/
int32 TO_FindEmptyTableIndex(void)
{
    uint32 ii = 0;
    uint32 tableIdx = TO_TBL_FULL_ERR;
    TO_TableEntry_t  *pEntry = NULL;
    
    /* Find either an unused entry or an entry that has been
     * deleted. */
    for (ii = 0; ii < TO_MAX_TBL_ENTRIES; ii++)
    {
        pEntry = &g_TO_AppData.pConfigTable->entries[ii];
        if (pEntry->usMsgId == TO_UNUSED_ENTRY ||
            pEntry->usMsgId == TO_REMOVED_ENTRY)
        {
            tableIdx = ii;
            break;
        }
    }

    return tableIdx;
}

/******************************************************************************/
/** \brief Find Table Index by MID.
*******************************************************************************/
int32 TO_FindTableIndex(TO_ConfigTable_t *pTable, 
                        CFE_SB_MsgId_t  usMsgId)
{
    uint32 ii = 0;
    uint32 tableIdx = TO_NO_MATCH;
    TO_TableEntry_t  *pEntry = NULL;
    
    for (ii = 0; ii < TO_MAX_TBL_ENTRIES; ii++)
    {
        pEntry = &pTable->entries[ii];
        /* If we've reached an unused entry, it doesn't exist. */
        if (pEntry->usMsgId == TO_UNUSED_ENTRY)
        {
            break;
        }
        else if (pEntry->usMsgId == usMsgId)
        {
            tableIdx = ii;
            break;
        }
    }

    return tableIdx;
}

/******************************************************************************/
/** \brief Set state (Enable/Disable) based on Group.
*******************************************************************************/
int32 TO_SetStateByGroup(uint32 uiGroupData, uint16 usEnableFlag)
{
    int32 iStatus = TO_NO_MATCH;
    uint32 ii = 0;
    TO_TableEntry_t  *pEntry = NULL;
    uint32 uiCmdGroup = uiGroupData & TO_GROUP_NUMBER_MASK;
    uint32 uiCmdMulti = uiGroupData & TO_MULTI_GROUP_MASK;
    uint32 uiEntryGroup = 0;
    uint32 uiEntryMulti = 0;
    int16 newState = 0;

    if (uiCmdGroup == 0 && uiCmdMulti == 0)
    {
        iStatus = TO_BAD_ARG_ERR;
        goto end_of_function;
    }

    for (ii = 0; ii < TO_MAX_TBL_ENTRIES; ii++)
    {
        pEntry = &g_TO_AppData.pConfigTable->entries[ii];
        
        if (pEntry->usMsgId == TO_UNUSED_ENTRY)
        {
            break;
        }
        
        uiEntryGroup = pEntry->uiGroupData & TO_GROUP_NUMBER_MASK;
        uiEntryMulti = pEntry->uiGroupData & TO_MULTI_GROUP_MASK;

        if ((uiEntryGroup == uiCmdGroup) || (uiEntryMulti & uiCmdMulti))
        {
            if (pEntry->usState != usEnableFlag)
            {
                newState = 1;
            }
            
            pEntry->usState = usEnableFlag;
            iStatus = TO_SUCCESS;
        }
    }

    if (iStatus == TO_SUCCESS && newState == 0)
    {
        iStatus = TO_NO_EFFECT;
    }

end_of_function:
    return iStatus;
}
        
/******************************************************************************/
/** \brief Set usRouteMask based on Group.
*******************************************************************************/
int32 TO_SetRouteByGroup(uint32 uiGroupData, uint16 usRouteMask)
{
    int32 iStatus = TO_NO_MATCH;
    uint32 ii = 0;
    TO_TableEntry_t  *pEntry = NULL;
    uint32 uiCmdGroup = uiGroupData & TO_GROUP_NUMBER_MASK;
    uint32 uiCmdMulti = uiGroupData & TO_MULTI_GROUP_MASK;
    uint32 uiEntryGroup = 0;
    uint32 uiEntryMulti = 0;
    int16 newState = 0;

    if (uiCmdGroup == 0 && uiCmdMulti == 0)
    {
        iStatus = TO_BAD_ARG_ERR;
        goto end_of_function;
    }

    for (ii = 0; ii < TO_MAX_TBL_ENTRIES; ii++)
    {
        pEntry = &g_TO_AppData.pConfigTable->entries[ii];
        
        /* No more entries. We're done. */
        if (pEntry->usMsgId == TO_UNUSED_ENTRY)
        {
            break;
        }
        
        uiEntryGroup = pEntry->uiGroupData & TO_GROUP_NUMBER_MASK;
        uiEntryMulti = pEntry->uiGroupData & TO_MULTI_GROUP_MASK;

        if ((uiEntryGroup == uiCmdGroup) || (uiEntryMulti & uiCmdMulti))
        {
            if (pEntry->usRouteMask != usRouteMask)
            {
                newState = 1;
            }
            
            pEntry->usRouteMask = usRouteMask; 
            iStatus = TO_SUCCESS;
        }
    }
    
    if (iStatus == TO_SUCCESS && newState == 0)
    {
        iStatus = TO_NO_EFFECT;
    }
    
end_of_function:
    return iStatus;
}

/******************************************************************************/
/** \brief Set State for all used table entries
*******************************************************************************/
int32 TO_SetAllEntryState(uint16 usEnableFlag)
{
    int32 iStatus = TO_SUCCESS;
    TO_TableEntry_t  *pEntry = NULL;
    uint32 ii = 0;
    int32 newState = 0;

    for (ii = 0; ii < TO_MAX_TBL_ENTRIES; ii++)
    {
        pEntry = &g_TO_AppData.pConfigTable->entries[ii];
        
        /* If we hit TO_UNUSED, we are done. */
        if (pEntry->usMsgId == TO_UNUSED_ENTRY)
        {
            break;
        }

        if (pEntry->usState != usEnableFlag)
        {
            newState = 1;
        }

        pEntry->usState = usEnableFlag;
    }

    if (newState == 0)
    {
        iStatus = TO_NO_EFFECT;
    }

    return iStatus;
}


/******************************************************************************/
/** \brief Set Route as configured
*******************************************************************************/
void TO_SetRouteAsConfigured(uint16 routeId)
{
    /* Set the route as configured */
    g_TO_AppData.routes[routeId].usIsConfig = 1;
    g_TO_AppData.HkTlm.usConfigRoutes |= (1 << routeId);
}

/******************************************************************************/
/** \brief Set Route as unconfigured
*******************************************************************************/
void TO_SetRouteAsUnconfigured(uint16 routeId)
{
    /* Set the route as configured */
    g_TO_AppData.routes[routeId].usIsConfig = 0;
    g_TO_AppData.HkTlm.usConfigRoutes &= ~(1 << routeId);
}


/******************************************************************************/
/** \brief Disable Route ID
*******************************************************************************/
void TO_DisableRoute(uint16 routeId)
{
    g_TO_AppData.routes[routeId].usIsEnabled = 0;       
    g_TO_AppData.HkTlm.usEnabledRoutes &= ~(1 << routeId);
}


/******************************************************************************/
/** \brief Verify RouteMask compared to configured routes.
*******************************************************************************/
int32 TO_ValidateRouteMask(uint16 usRouteMask)
{
    int32 iStatus = TO_SUCCESS;
    uint16 ii;
    
    /* Validate Route. The route must exist to be valid. */
    for (ii = 0; ii < TO_MAX_NUM_ROUTES; ++ii)
    {
        if ((!g_TO_AppData.routes[ii].usExists << ii) & usRouteMask)
        {
            iStatus = TO_ERROR;
            break;
        }
    }
        
    return iStatus;
}

/******************************************************************************/
/** \brief Get the route mask based at table index 
*******************************************************************************/
uint16 TO_GetRouteMask(int32 tblIdx)
{
    if (tblIdx >= TO_MAX_TBL_ENTRIES)
    {
        return 0x0000;
    }
    
    return g_TO_AppData.pConfigTable->entries[tblIdx].usRouteMask;
}

/******************************************************************************/
/** \brief Get the Message ID based at table index 
*******************************************************************************/
CFE_SB_MsgId_t TO_GetMessageID(int32 tblIdx)
{
    if (tblIdx >= TO_MAX_TBL_ENTRIES)
    {
        return 0;
    }
    
    return g_TO_AppData.pConfigTable->entries[tblIdx].usMsgId;
}


/******************************************************************************/
/** \brief Verify the command length against expected length
*******************************************************************************/
boolean TO_VerifyCmdLength(CFE_SB_MsgPtr_t pMsg,
                           uint16 usExpectedLen)
{
    boolean bResult=FALSE;
    uint16  usMsgLen=0;

    if (pMsg != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(pMsg);

        if (usExpectedLen == usMsgLen)
        {
            bResult = TRUE;
        }
        else
        {
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(pMsg);
            uint16 usCmdCode = CFE_SB_GetCmdCode(pMsg);

            CFE_EVS_SendEvent(TO_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "Rcvd invalid msgLen: usMsgId=0x%04X, "
                              "cmdCode=%d, msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
                              
            g_TO_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}



/******************************************************************************/
/** \brief Subscribe all Messages to appropriate pipes 
*******************************************************************************/
int32 TO_SubscribeAllMsgs(void)
{
    uint16             ii;
    int32              iStatus = TO_SUCCESS;
    TO_TableEntry_t   *pEntry   = NULL;
    
    for (ii = 0; ii < TO_MAX_TBL_ENTRIES; ++ii)
    {
        pEntry = &g_TO_AppData.pConfigTable->entries[ii];
        if (pEntry == NULL)
        {
            iStatus = TO_ERROR;
            break;
        }
        
        if (pEntry->usMsgId != TO_UNUSED_ENTRY && 
            pEntry->usMsgId != TO_REMOVED_ENTRY)
        {
            iStatus = TO_SubscribeMsg(pEntry);

            if (iStatus != TO_SUCCESS)
            {
                break;
            }
        }
        /* We've reached the end of used entries. */
        else if (pEntry->usMsgId == TO_UNUSED_ENTRY)
        {
            break;
        }
    }

    return iStatus;
}


/******************************************************************************/
/** \brief Subscribe Message to appropriate pipes 
*******************************************************************************/
int32 TO_SubscribeMsg(TO_TableEntry_t *pEntry)
{
    uint16             jj;
    TO_TlmPipe_t      *pTlmPipe = NULL;
    int32              iStatus = TO_SUCCESS;

    if (pEntry == NULL)
    {
        iStatus = TO_BAD_ARG_ERR;
        goto end_of_function;
    }

    for (jj = 0; jj < TO_MAX_NUM_ROUTES; ++jj)
    {
        pTlmPipe = &g_TO_AppData.tlmPipes[jj];
        
        /* Subscribe the message to the current route pipe if it exists. */
        if (pEntry->usRouteMask & (1 << jj) && g_TO_AppData.routes[jj].usExists)
        {
            iStatus = CFE_SB_SubscribeEx(pEntry->usMsgId, 
                                         pTlmPipe->cfePipeId,
                                         pEntry->qos, 
                                         pEntry->usMsgLimit);
        
            if (iStatus != CFE_SUCCESS)
            {
                g_TO_AppData.HkTlm.usMsgSubErrCnt++;
                CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
                    "TO Pipe:%s failed to subscribe to MID 0x%04x",
                     pTlmPipe->cTlmPipeName, pEntry->usMsgId);
                break;
            }
            else
            {
                g_TO_AppData.HkTlm.usMsgSubCnt++;
            }
        }
    }

end_of_function:
    return iStatus;
}


/******************************************************************************/
/** \brief Unsubscribe all Messages to appropriate pipes 
*******************************************************************************/
int32 TO_UnsubscribeAllMsgs(TO_ConfigTable_t *pConfigTable)
{
    uint16             ii;
    int32              iStatus = TO_SUCCESS;
    TO_TableEntry_t   *pEntry   = NULL;

    if (pConfigTable == NULL)
    {
        iStatus = TO_BAD_ARG_ERR;
        goto end_of_function;
    }

    for (ii = 0; ii < TO_MAX_TBL_ENTRIES; ++ii)
    {
        pEntry = &pConfigTable->entries[ii];
        
        if (pEntry->usMsgId != TO_UNUSED_ENTRY && 
            pEntry->usMsgId != TO_REMOVED_ENTRY)
        {
            iStatus = TO_UnsubscribeMsg(pEntry);

            if (iStatus != TO_SUCCESS)
            {
                break;
            }
        }
        /* We've reached the end of used entries. */
        else if (pEntry->usMsgId == TO_UNUSED_ENTRY)
        {
            break;
        }
    }

end_of_function:
    return iStatus;
}


/******************************************************************************/
/** \brief Unsubscribe Message to appropriate pipes 
*******************************************************************************/
int32 TO_UnsubscribeMsg(TO_TableEntry_t  *pEntry)
{
    uint16             jj;
    TO_TlmPipe_t      *pTlmPipe = NULL;
    int32              iStatus = TO_SUCCESS;
    
    if (pEntry == NULL)
    {
        iStatus = TO_BAD_ARG_ERR;
        goto end_of_function;
    }
    
    for (jj = 0; jj < TO_MAX_NUM_ROUTES; ++jj)
    {
        pTlmPipe = &g_TO_AppData.tlmPipes[jj];
        
        /* Unsubscribe the message to the current route pipe. */
        if (pEntry->usRouteMask & (1 << jj) && g_TO_AppData.routes[jj].usExists)
        {
            iStatus = CFE_SB_Unsubscribe(pEntry->usMsgId, 
                                         pTlmPipe->cfePipeId);
        
            if (iStatus != CFE_SUCCESS)
            {
                g_TO_AppData.HkTlm.usMsgSubErrCnt++;
                CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
                    "TO Pipe:%s failed to unsubscribe to MID 0x%04x",
                     pTlmPipe->cTlmPipeName, pEntry->usMsgId);
                break;
            }
            else
            {
                g_TO_AppData.HkTlm.usMsgSubCnt--;
            }
        }
    }
    
end_of_function:
    return iStatus;
}



/*==============================================================================
** End of file to_utils.c
**============================================================================*/
