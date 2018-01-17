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
*   \brief Function Definitions for Custom Layer of TO Multi Devices with TFs.
*
*   \par
*     This file defines the functions for a custom implementation of the custom
*     layer of the TO application with transfer frames sent over multiple
*     devices (RS422 and UDP) with the Space Data link protocol Transfer frames.
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
*     - All output messages are Transfer Frames 
*     - All config macros defined in to_platform_cfg.h
*
*   \par Modification History:
*     - 2015-12-23 | Guy de Carufel | Code Started
*******************************************************************************/

/*
** Pragmas
*/

/*
** Include Files
*/
#include "cfe.h"
#include "network_includes.h"
#include "io_lib_utils.h"
#include "trans_udp.h"
#include "trans_rs422.h"
#include "tm_sdlp.h"
#include "tm_sync.h"
#include "cop1.h"

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
    TM_SDLP_FrameInfo_t frameInfo;
    TM_SDLP_ChannelConfig_t vcConfig;
    uint8 ocfBuff[4];
    uint8 ofBuff[TO_CUSTOM_TF_OVERFLOW_SIZE];
} TO_CustomVChnl_t;

typedef struct
{
    TM_SDLP_GlobalConfig_t mcConfig;
    uint8                  mcFrameCnt;
    TO_CustomVChnl_t       vc;
} TO_CustomMChnl_t;

typedef struct
{
    TO_CustomMChnl_t    mc;
    uint8               buffer[TO_CUSTOM_TF_SIZE + TM_SYNC_ASM_SIZE];
} TO_CustomPChnl_t;

typedef struct
{
    IO_TransUdp_t       udp;
    TO_CustomPChnl_t    pc;
} TO_CustomSocketPChnl_t;

typedef struct
{
    int32               portFd;
    TO_CustomPChnl_t    pc;
} TO_CustomSerialPChnl_t;

typedef struct
{
    TO_CustomSocketPChnl_t  socket;
    TO_CustomSerialPChnl_t  serial;
    uint8                   idleBuff[TO_CUSTOM_TF_IDLE_SIZE];
} TO_CustomData_t;

/*
** External Global Variables
*/
extern TO_AppData_t g_TO_AppData; 

/*
** Global Variables
*/
static TO_CustomData_t g_TO_CustomData;

/*
** Local Variables
*/
static uint8           idlePattern[32];
static CFE_SB_Msg_t   *pIdlePacket = (CFE_SB_Msg_t *) &g_TO_CustomData.idleBuff;
static const uint16    iCaduSize = TO_CUSTOM_TF_SIZE + TM_SYNC_ASM_SIZE; 

/*
** Local Function Definitions
*/
extern void TO_SendDataTypePktCmd(CFE_SB_MsgPtr_t);
static void TO_CustomSetOcfCmd(CFE_SB_Msg_t *pCmdMsg);
static int32 TO_CustomProcessPacket(CFE_SB_Msg_t *pMsg, uint16 usRouteId);
static TO_CustomPChnl_t * TO_CustomGetChnl(uint16 usRouteId);
static int32 TO_CustomProcessSizeSent(int32, int32, uint16);

/*******************************************************************************
** Custom Application Functions 
*******************************************************************************/

/******************************************************************************/
/** \brief Custom Initialization
*******************************************************************************/
int32 TO_CustomInit(void)
{
    int32 iStatus = TO_SUCCESS;
    TO_CustomPChnl_t    *pChnl;
    
    /* Create socket for outgoing */
    if (IO_TransUdpCreateSocket(&g_TO_CustomData.socket.udp) < 0)
    {
        iStatus = TO_ERROR;
        goto end_of_function;
    }
    
    /* Set Critical Message Ids which must always be
     * in config table. */
    g_TO_AppData.criticalMid[0] = TO_HK_TLM_MID;
    g_TO_AppData.criticalMid[1] = CI_HK_TLM_MID;

    /* Initialize Idle pattern as pseudo-random sequence. */
    IO_LIB_UTIL_GenPseudoRandomSeq(&idlePattern[0], 0xa9, 0xff);
    
    /* Initialize Idle packet with repeating idle pattern */
    TM_SDLP_InitIdlePacket(pIdlePacket, &idlePattern[0],
                           TO_CUSTOM_TF_IDLE_SIZE, 255);

    /* Initialize Master channels */
    g_TO_CustomData.serial.pc.mc.mcConfig.scId = CFE_SPACECRAFT_ID;
    g_TO_CustomData.serial.pc.mc.mcConfig.frameLength = TO_CUSTOM_TF_SIZE;
    g_TO_CustomData.serial.pc.mc.mcConfig.hasErrCtrl = TO_CUSTOM_TF_ERR_CTRL;
    g_TO_CustomData.serial.pc.mc.mcFrameCnt = 0;
    
    g_TO_CustomData.socket.pc.mc.mcConfig.scId = CFE_SPACECRAFT_ID;
    g_TO_CustomData.socket.pc.mc.mcConfig.frameLength = TO_CUSTOM_TF_SIZE;
    g_TO_CustomData.socket.pc.mc.mcConfig.hasErrCtrl = TO_CUSTOM_TF_ERR_CTRL;
    g_TO_CustomData.socket.pc.mc.mcFrameCnt = 0;

    /* NOTE: We are setting VC ID to: Socket: 0, Serial: 1 */

    /* Set channel config table */
    TM_SDLP_ChannelConfig_t chnlConfig[TO_CUSTOM_NUM_CHNL] = 
    {
        {0, 0, 0, 1, 0, 0, TO_CUSTOM_TF_OVERFLOW_SIZE},
        {1, 0, 0, 1, 0, 0, TO_CUSTOM_TF_OVERFLOW_SIZE}
    };
    
    pChnl = &g_TO_CustomData.socket.pc;
    CFE_PSP_MemCpy((void *) &pChnl->mc.vc.vcConfig, (void *) &chnlConfig[0], 
                   sizeof(TM_SDLP_ChannelConfig_t));

    if (TM_SDLP_InitChannel(&pChnl->mc.vc.frameInfo, 
                            &pChnl->buffer[TM_SYNC_ASM_SIZE],
                            &pChnl->mc.vc.ofBuff[0],
                            &pChnl->mc.mcConfig,
                            &pChnl->mc.vc.vcConfig) < 0)
    {
        iStatus = TO_ERROR;
        goto end_of_function;
    }

    pChnl = &g_TO_CustomData.serial.pc;
    CFE_PSP_MemCpy((void *) &pChnl->mc.vc.vcConfig, (void *) &chnlConfig[0], 
                   sizeof(TM_SDLP_ChannelConfig_t));

    if (TM_SDLP_InitChannel(&pChnl->mc.vc.frameInfo, 
                            &pChnl->buffer[TM_SYNC_ASM_SIZE],
                            &pChnl->mc.vc.ofBuff[0],
                            &pChnl->mc.mcConfig,
                            &pChnl->mc.vc.vcConfig) < 0)
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
        
        case TO_SET_OCF_DATA_CC:
            TO_CustomSetOcfCmd(pMsg);
            break;

        default:
            iStatus = TO_ERROR;
            break;
    }

    return iStatus;
}

/******************************************************************************/
/** \brief Process data packet 
*******************************************************************************/
int32 TO_CustomProcessData(CFE_SB_Msg_t * pMsg, int32 size, int32 iTblIdx,
                           uint16 usRouteId)
{
    return TO_CustomProcessPacket(pMsg, usRouteId);
}


/******************************************************************************/
/** \brief Start a new frame, copying overflow data if present.
*******************************************************************************/
int32 TO_CustomFrameStart(uint16 usRouteId)
{
    int32 iStatus = TO_SUCCESS;
    TO_CustomPChnl_t *pChnl         = NULL;
    TM_SDLP_FrameInfo_t *pFrameInfo = NULL;
    
    pChnl = TO_CustomGetChnl(usRouteId);
    if (!pChnl)
    {
        CFE_EVS_SendEvent(TO_CUSTOM_ERR_EID, CFE_EVS_ERROR,
                          "TO_CustomFrameStart Error: Invalid Route ID:%u",
                          usRouteId);
        iStatus = TO_ERROR;
        goto end_of_function;
    }
        
    /* Start Frame */
    pFrameInfo = &pChnl->mc.vc.frameInfo;
    iStatus = TM_SDLP_StartFrame(pFrameInfo);

end_of_function:
    return iStatus;
}


/******************************************************************************/
/** \brief Complete frame and send 
*******************************************************************************/
int32 TO_CustomFrameSend(uint16 usRouteId, int32 iInStatus)
{
    int32 iCaduSize = 0;
    int32 iSentSize = 0;
    int32 iStatus = TO_SUCCESS;

    TO_CustomPChnl_t    *pChnl          = NULL; 
    TM_SDLP_FrameInfo_t *pFrameInfo     = NULL; 
    uint8               *pMcFrameCnt    = NULL; 
    uint8               *pOcf           = NULL;

    pChnl = TO_CustomGetChnl(usRouteId);
    if (!pChnl)
    {
        CFE_EVS_SendEvent(TO_CUSTOM_ERR_EID, CFE_EVS_ERROR,
                          "TO_CustomFrameSend Error: Invalid Route ID:%u",
                          usRouteId);
        iStatus = TO_ERROR;
        goto end_of_function;
    }
    
    /* Set Pointers */
    pFrameInfo     = &pChnl->mc.vc.frameInfo;
    pMcFrameCnt    = &pChnl->mc.mcFrameCnt;
    pOcf           = &pChnl->mc.vc.ocfBuff[0];

    /* Check if there is packets, otherwise, fill with OID. */
    iStatus = TM_SDLP_FrameHasData(pFrameInfo);
    if (iStatus == 1)
    {
        /* Add an idle packet to fill remaining free space */
        iStatus = TM_SDLP_AddIdlePacket(pFrameInfo, pIdlePacket);
    }
    else if (iStatus == 0)
    {
        /* Set frame as Only Idle Data (OID) */
        iStatus = TM_SDLP_SetOidFrame(pFrameInfo, pIdlePacket);
    }
    
    if (iStatus != TO_SUCCESS)
    {
        goto end_of_function;
    }

    /* Complete Frame */
    iStatus = TM_SDLP_CompleteFrame(pFrameInfo, pMcFrameCnt, pOcf);
    if (iStatus != TO_SUCCESS)
    {
        goto end_of_function;
    }
    
    /* Synchronize frame into CADU */ 
    iCaduSize = TM_SYNC_Synchronize(pChnl->buffer, TM_SYNC_ASM_STR, 
                                    TM_SYNC_ASM_SIZE,
                                    TO_CUSTOM_TF_SIZE, 
                                    TO_CUSTOM_TF_RANDOMIZE);
    if (iCaduSize < 0)
    {
        iStatus = TO_ERROR;
        goto end_of_function;
    }    

    /* Send Frame */
    if (usRouteId == 0)
    {
        iSentSize = IO_TransUdpSnd(&g_TO_CustomData.socket.udp, 
                                   &g_TO_CustomData.socket.pc.buffer[0], 
                                   iCaduSize);
    }
    else if (usRouteId == 1)
    {
        iSentSize = IO_TransRS422Write(g_TO_CustomData.serial.portFd, 
                                       &g_TO_CustomData.serial.pc.buffer[0], 
                                       iCaduSize);
    }

    iStatus = TO_CustomProcessSizeSent(iCaduSize, iSentSize, 0);

    /* If Process Data Failed, disable route. */
    if (iInStatus != TM_SDLP_SUCCESS)
    {
        TO_DisableRoute(usRouteId);

        CFE_EVS_SendEvent(TO_CUSTOM_ERR_EID, CFE_EVS_ERROR,
                          "TO_CustomFrameSend Error: Failed input status. "
                          "Disabling route:%u.",
                          usRouteId);
    }

end_of_function:
    return iStatus;
}


/******************************************************************************/
/** \brief Process a Packet and add to frame
*******************************************************************************/
int32 TO_CustomProcessPacket(CFE_SB_Msg_t *pMsg, uint16 usRouteId)
{
    int32 iStatus = TO_SUCCESS;
    TO_CustomPChnl_t *pChnl         = NULL;
    TM_SDLP_FrameInfo_t *pFrameInfo = NULL;
    
    pChnl = TO_CustomGetChnl(usRouteId);
    if (!pChnl)
    {
        /* This should never happen since route ID is checked several times 
           before */
        CFE_EVS_SendEvent(TO_CUSTOM_ERR_EID, CFE_EVS_ERROR,
                          "TO_CustomProcessPacket Error: Invalid Route ID:%u",
                          usRouteId);
        iStatus = TO_ERROR;
        goto end_of_function;
    }
        
    pFrameInfo     = &pChnl->mc.vc.frameInfo;

    /* Add Packet */
    iStatus = TM_SDLP_AddPacket(pFrameInfo, pMsg);
    if (iStatus >= 0)
    {
        iStatus = TO_SUCCESS;
    }

end_of_function:
    return iStatus; 
}


/******************************************************************************/
/** \brief Get the physical channel based on route id
*******************************************************************************/
TO_CustomPChnl_t * TO_CustomGetChnl(uint16 usRouteId)
{
    TO_CustomPChnl_t *pChnl = NULL;
    
    if (usRouteId == 0)
    {
        pChnl = &g_TO_CustomData.socket.pc;
    }
    else if(usRouteId == 1)
    {
        pChnl = &g_TO_CustomData.serial.pc;
    }

    return pChnl;
}


/******************************************************************************/
/** \brief Check Data Sent Size (Local)
*******************************************************************************/
int32 TO_CustomProcessSizeSent(int32 size, int32 iSentSize, uint16 routeId)
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
        /* NOTE: If this happens, that means a partial frame was sent. Your
           Ground support equipment must be able to handle partial frames.
           For example, ITOS does not handle this case well and goes into
           a weird state, where every following TF is corrupted. 
           This condition indicates that throttling is necessary. */
        
        CFE_EVS_SendEvent(TO_CUSTOM_ERR_EID, CFE_EVS_ERROR,
            "TO sent incomplete message (Insuficient bandwidth likely). " 
            "ROUTE ID:%u, MsgSize:%d, SentSize:%d. Route disabled.", 
            routeId, size, iSentSize);
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
        IO_TransUdpCloseSocket(&g_TO_CustomData.socket.udp);
    }
    
    return;
}


/******************************************************************************/
/** \brief Set the OCF trailer with the CLCW - Internal Cmd.
*******************************************************************************/
void TO_CustomSetOcfCmd(CFE_SB_Msg_t *pCmdMsg)
{
    TO_CustomSetOcfCmd_t *cmd = (TO_CustomSetOcfCmd_t *) pCmdMsg;

    if (TO_VerifyCmdLength(pCmdMsg, sizeof(TO_CustomSetOcfCmd_t)))
    {
        switch (COP1_GetClcwVcId(&cmd->clcw))
        {
            case 0:
                CFE_PSP_MemCpy(&g_TO_CustomData.socket.pc.mc.vc.ocfBuff[0],
                               &cmd->clcw, 4);
                break;

            case 1:
                CFE_PSP_MemCpy(&g_TO_CustomData.serial.pc.mc.vc.ocfBuff[0],
                               &cmd->clcw, 4);
                break;
                
            default:
                CFE_EVS_SendEvent(TO_CUSTOM_ERR_EID, CFE_EVS_ERROR,
                          "Received invalid Channel ID in TO_SET_OCF_DATA_CC");
        }
    } 
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
    
    iStatus = IO_TransUdpSetDestAddr(&g_TO_CustomData.socket.udp, 
                                     pCustomCmd->cDestIp, 
                                     usDestPort); 
    if (iStatus < 0)
    {
        goto end_of_function;
    }

    g_TO_CustomData.serial.portFd = pCustomCmd->iFileDesc; 

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
