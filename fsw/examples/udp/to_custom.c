/******************************************************************************/
/** \file  to_custom.c
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*  
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Function Definitions for Custom Layer of TO Application for UDP
*
*   \par
*     This file defines the functions for a custom implementation of the custom
*     layer of the TO application over UDP Socket.
*
*   \par API Functions Defined:
*     - TO_CustomInit() - Initialize the transport protocol
*     - TO_CustomAppCmds() - Process custom App Commands
*     - TO_CustomEnableOutputCmd() - Enable telemetry output
*     - TO_CustomDisableOutputCmd() - Disable telemetry output
*     - TO_CustomCleanup() - Cleanup callback to close transport channel.
*     - TO_CustomProcessData() - Send output data over transport protocol.
*
*   \par Private Functions Defined:
*     - TO_SendDataTypePktCmd() - Send Test packet (Reference to_lab app)
*
*   \par Limitations, Assumptions, External Events, and Notes:
*     - All input messages are CCSDS messages
*     - All config macros defined in to_platform_cfg.h
*
*   \par Modification History:
*     - 2015-01-09 | Guy de Carufel | Code Started
*     - 2015-06-02 | Guy de Carufel | Revised for new UDP API
*******************************************************************************/

/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe.h"
#include "network_includes.h"
#include "trans_udp.h"

#include "to_app.h"
#include "ci_msgids.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/
typedef struct
{
    IO_TransUdp_t   udp;        /**< UDP working              */
} TO_CustomData_t;

/*
** External Global Variables
*/
extern TO_AppData_t g_TO_AppData; 

/*
** Global Variables
*/

/*
** Local Variables
*/
static TO_CustomData_t g_TO_CustomData;

/*
** Local Function Definitions
*/
extern void TO_SendDataTypePktCmd(CFE_SB_MsgPtr_t);

/*******************************************************************************
** Custom Application Functions 
*******************************************************************************/

/******************************************************************************/
/** \brief Custom Initialization
*******************************************************************************/
int32 TO_CustomInit(void)
{
    int32 iStatus = TO_SUCCESS;
    
    /* Create socket for outgoing */
    if (IO_TransUdpCreateSocket(&g_TO_CustomData.udp) < 0)
    {
        iStatus = TO_ERROR;
        goto end_of_function;
    }

    /* NOTE: For this simple UDP example, we are only requiring The following
       3 messages.  If more message IDs should be included by default, add them
       here and update the TO_NUM_CRITICAL_MIDS value. */ 

    /* Set Critical Message Ids which must always be in config table. */
    g_TO_AppData.criticalMid[0] = TO_HK_TLM_MID;
    g_TO_AppData.criticalMid[1] = CI_HK_TLM_MID;
    g_TO_AppData.criticalMid[2] = CFE_EVS_EVENT_MSG_MID; 
    
    /* Route 0: Udp. Linked to CF Channel Index 0. */
    g_TO_AppData.routes[0].usExists = 1;
    g_TO_AppData.routes[0].sCfChnlIdx = 0;
    
end_of_function:
    return iStatus;
}

/******************************************************************************/
/** \brief Process of custom app commands 
*******************************************************************************/
int32 TO_CustomAppCmds(CFE_SB_Msg_t* pMsg)
{
    int32 iStatus = TO_SUCCESS;
    uint32 uiCmdCode = CFE_SB_GetCmdCode(pMsg);
    switch (uiCmdCode)
    {
        case TO_SEND_DATA_TYPE_CC:
            TO_SendDataTypePktCmd(pMsg);
            break;
        
        default:
            iStatus = -1;
            break;
    }

    return iStatus;
}

/******************************************************************************/
/** \brief Process of output telemetry
*******************************************************************************/
int32 TO_CustomProcessData(CFE_SB_Msg_t * pMsg, int32 size, int32 iTblIdx,
                           uint16 usRouteId)
{
    int32 iStatus = 0;

    if (g_TO_AppData.routes[0].usIsEnabled == 0 || usRouteId != 0)
    {
        goto end_of_function;
    }

    iStatus = IO_TransUdpSnd(&g_TO_CustomData.udp, (uint8 *) pMsg, size);

    if (iStatus < 0)
    {
        CFE_EVS_SendEvent(TO_CUSTOM_ERR_EID, CFE_EVS_ERROR,
                          "TO UDP sendto errno %d. Telemetry output disabled.", 
                          errno);
        g_TO_AppData.usOutputEnabled = 0;
    }

end_of_function:
    return iStatus;
}

/******************************************************************************/
/** \brief Custom Cleanup 
*******************************************************************************/
void TO_CustomCleanup(void)
{
    if (g_TO_AppData.usOutputEnabled)
    {
        CFE_EVS_SendEvent(TO_CUSTOM_INF_EID, CFE_EVS_INFORMATION, 
                          "TO - Closing Socket."); 
        IO_TransUdpCloseSocket(&g_TO_CustomData.udp);
    }

    return;
}

/******************************************************************************/
/** \brief Enable Output Command Response
*******************************************************************************/
int32 TO_CustomEnableOutputCmd(CFE_SB_Msg_t *pCmdMsg)
{
    int32 iStatus = IO_TRANS_UDP_NO_ERROR;
    int32 routeMask = TO_ERROR;
    char cDestIp[TO_MAX_IP_STRING_SIZE];
    uint16 usDestPort = 0;
    
    TO_EnableOutputCmd_t * pCustomCmd = (TO_EnableOutputCmd_t *) pCmdMsg;
    strncpy(cDestIp, pCustomCmd->cDestIp, sizeof(cDestIp));
    
    if (pCustomCmd->usDestPort > 0)
    {
        usDestPort = pCustomCmd->usDestPort;
    }
    else
    {
        usDestPort = TO_DEFAULT_DEST_PORT;
    }

    iStatus = IO_TransUdpSetDestAddr(&g_TO_CustomData.udp, pCustomCmd->cDestIp, 
                                     usDestPort);                               

    if (iStatus < 0)
    {
        goto end_of_function;
    }

    /* We are done configuring route 0 */
    TO_SetRouteAsConfigured(0);
    routeMask = 0x0001;

end_of_function:
    return routeMask; 
}

/******************************************************************************/
/** \brief Disable Output Command Response
*******************************************************************************/
int32 TO_CustomDisableOutputCmd(CFE_SB_Msg_t *pCmdMsg)
{
    /* Disable */
    g_TO_AppData.usOutputEnabled = 0;
    return TO_SUCCESS;
}


/*******************************************************************************
** Non standard custom Commands
*******************************************************************************/

/*==============================================================================
** End of file to_custom.c
**============================================================================*/
