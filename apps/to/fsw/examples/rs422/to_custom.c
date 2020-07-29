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
*   \brief Function Definitions for Custom Layer of TO Application for RS422
*
*   \par
*     This file defines the functions for a custom implementation of the custom
*     layer of the TO application over an RS422 serial port. 
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
*     - 2015-01-09 | Guy de Carufel | Code Started
*******************************************************************************/

/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe.h"
#include "network_includes.h"
#include "trans_rs422.h"

#include "to_app.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/
typedef struct
{
    int32   iFileDesc;      /**< File Descriptor of serial port */
} TO_CustomData_t;

/*
** External Global Variables
*/
extern TO_AppData_t g_TO_AppData; 

/*
** Global Variables
*/
TO_CustomData_t g_TO_CustomData;

/*
** Local Variables
*/

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
    
    /* Set Critical Message Ids which must always be
     * in config table. */
    g_TO_AppData.criticalMid[0] = CFE_ES_SHELL_TLM_MID;
    g_TO_AppData.criticalMid[1] = CFE_EVS_EVENT_MSG_MID;
    g_TO_AppData.criticalMid[2] = CFE_SB_ALLSUBS_TLM_MID;

    /* Route 0: Serial */
    g_TO_AppData.routes[0].usExists = 1;
    
    /* Tie route 0 to CF channel 0 */
    g_TO_AppData.routes[0].sCfChnlIdx = 0;
    
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
    
    if (usRouteId == 0)
    {
        iSentSize = IO_TransRS422Write(g_TO_CustomData.iFileDesc, 
                                       (uint8 *) pMsg, 
                                       size);
        if (iSentSize < 0)
        {
            CFE_EVS_SendEvent(TO_CUSTOM_ERR_EID, CFE_EVS_ERROR,
                              "TO RS422 sendto errno %d. "
                              "Telemetry output disabled.", errno);
            g_TO_AppData.usOutputEnabled = 0;
            iStatus = TO_ERROR;
        }
        else if (iSentSize != size)
        {
            CFE_EVS_SendEvent(TO_CUSTOM_ERR_EID, CFE_EVS_ERROR,
                              "TO RS422 sent incomplete message.");
            iStatus = TO_ERROR;
        }
    }

    return iSentSize;
}


/******************************************************************************/
/** \brief Custom Cleanup 
*******************************************************************************/
void TO_CustomCleanup(void)
{
    /* Nothing to do here. The fileDescriptor is closed in CI. */
    return;
}

/******************************************************************************/
/** \brief Enable Output Command Response
*******************************************************************************/
int32 TO_CustomEnableOutputCmd(CFE_SB_Msg_t *cmdpMsg)
{
    int32 routeMask = 0x0001;
    
    TO_EnableOutputCmd_t * pCustomCmd = (TO_EnableOutputCmd_t *) cmdpMsg;
    g_TO_CustomData.iFileDesc = pCustomCmd->iFileDesc; 
    TO_SetRouteAsConfigured(0);
    
    return routeMask;
}

/******************************************************************************/
/** \brief Disable Output Command Response
*******************************************************************************/
int32 TO_CustomDisableOutputCmd(CFE_SB_Msg_t *cmdpMsg)
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
