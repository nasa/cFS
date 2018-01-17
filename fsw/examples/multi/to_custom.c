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
*   \brief Function Definitions for Custom Layer of TO Multi Devices
*
*   \par
*     This file defines the functions for a custom implementation of the custom
*     layer of the TO application with temeletry output to multiple devices
*     (RS422 and UDP)
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
*     - Both CI and TO makes use of the same RS422 device
*     - All config macros defined in to_platform_cfg.h
*
*   \par Modification History:
*     - 2015-06-03 | Guy de Carufel | Code Started
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
#include "trans_rs422.h"

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
    int32           iFileDesc;      /**< File Descriptor of serial port */
    IO_TransUdp_t   udp;            /**< UDP working                    */
    uint8           buffer[2000];   
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
static int32 TO_CustomProcessSizeSent(int32, int32, int32,uint16);

/*******************************************************************************
** Custom Application Functions 
*******************************************************************************/

/******************************************************************************/
/** \brief Custom Initialization
*******************************************************************************/
int32 TO_CustomInit(void)
{
    int32 iStatus = TO_SUCCESS;
    
    /* Set Critical Message Ids which must always be
     * in config table. */
    g_TO_AppData.criticalMid[0] = TO_HK_TLM_MID;
    g_TO_AppData.criticalMid[1] = CI_HK_TLM_MID;

    /* Create socket for outgoing */
    if (IO_TransUdpCreateSocket(&g_TO_CustomData.udp) < 0)
    {
        iStatus = TO_ERROR;
        goto end_of_function;
    }
    
    /* Route 0: Udp */
    g_TO_AppData.routes[0].usExists = 1;
    /* Route 1: Serial */
    g_TO_AppData.routes[1].usExists = 1;

    /* Tie route 0 to CF channel 0 */
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
            iStatus = TO_ERROR;
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
    int32 iSentSize = 0;
    int32 iStatus = TO_SUCCESS;
    int32 iReturn = TO_SUCCESS;

    CFE_PSP_MemCpy((void *) &g_TO_CustomData.buffer[0], (void *) pMsg, size);

    /* For route 0, use socket */
    if (usRouteId == 0)
    {
        iSentSize = IO_TransUdpSnd(&g_TO_CustomData.udp, 
                                   &g_TO_CustomData.buffer[0], size);
        iStatus = TO_CustomProcessSizeSent(size, iSentSize, iTblIdx, 0);
        if (iStatus != TO_SUCCESS)
        {
            iReturn = TO_ERROR;
        }
    }
    /* For route 1, use serial port */
    if (usRouteId == 1)
    {
        iSentSize = IO_TransRS422Write(g_TO_CustomData.iFileDesc, 
                                       &g_TO_CustomData.buffer[0], size);
        iStatus = TO_CustomProcessSizeSent(size, iSentSize, iTblIdx, 1);
        if (iStatus != TO_SUCCESS)
        {
            iReturn = TO_ERROR;
        }
    }

    return iReturn;
}


/******************************************************************************/
/** \brief Check Data Sent Size (Local)
*******************************************************************************/
int32 TO_CustomProcessSizeSent(int32 size, int32 iSentSize, int32 iTblIdx,
                               uint16 routeId)
{
    int32 iStatus = TO_SUCCESS;
    
    if (iSentSize < 0)
    {
        CFE_EVS_SendEvent(TO_CUSTOM_ERR_EID, CFE_EVS_ERROR,
                          "TO Output errno %d. Route ID:%u disabled ",
                          errno, routeId);
        TO_DisableRoute(routeId);
        iStatus = TO_ERROR;
    }
    else if (iSentSize != size)
    {
        CFE_SB_MsgId_t  usMsgId = TO_GetMessageID(iTblIdx);
        CFE_EVS_SendEvent(TO_CUSTOM_ERR_EID, CFE_EVS_ERROR,
            "TO sent incomplete message (Insuficient bandwidth likely). " 
            "MID:%d, ROUTE ID:%u, MsgSize:%d, SentSize:%d. Route disabled.", 
            usMsgId, routeId, size, iSentSize);
        TO_DisableRoute(routeId);
        iStatus = TO_ERROR;
    }

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

    g_TO_CustomData.iFileDesc = pCustomCmd->iFileDesc; 

    CFE_EVS_SendEvent(TO_CUSTOM_INF_EID, CFE_EVS_INFORMATION, 
                      "Serial Output Device File Descriptor Set.");
    
    /* Both routes are now configured */
    TO_SetRouteAsConfigured(0);
    TO_SetRouteAsConfigured(1);

    /* Enable both routes 0 and 1. */
    routeMask = 0x0003;

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
