/*
 * File: to_stubs.c
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose:
 *  Stub out various functions not stubbed out by the UT-Assert code
 */

#include <sys/socket.h>
#include <string.h>
#include <stdio.h>

#include "cfe.h"

#include "to_stubs.h"
#include "to_app.h"
#include "to_cmds.h"

extern TO_AppData_t  g_TO_AppData;
extern void TO_SendDataTypePktCmd(CFE_SB_MsgPtr_t);
int32 Ut_OS_CountSemGetInfoHook(uint32 sem_id, OS_count_sem_prop_t *count_prop);

Ut_TO_ReturnCodeTable_t     Ut_TO_ReturnCodeTable[UT_TO_MAX_INDEX];

void Ut_TO_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UT_TO_MAX_INDEX) {
        Ut_TO_ReturnCodeTable[Index].Value = RtnVal;
        Ut_TO_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported Index In SetReturnCode Call %u\n", Index);
    }
}


boolean Ut_TO_UseReturnCode(uint32 Index)
{
    if (Ut_TO_ReturnCodeTable[Index].Count > 0) {
        Ut_TO_ReturnCodeTable[Index].Count--;
        if (Ut_TO_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}



/* Functions normally declared in Custom File (to_custom.c) */
int32 TO_CustomInit(void)
{
    if (Ut_TO_UseReturnCode(UT_TO_CUSTOMINIT_INDEX))
        return Ut_TO_ReturnCodeTable[UT_TO_CUSTOMINIT_INDEX].Value;

    /* Set Critical Message Ids which must always be in config table. */
    g_TO_AppData.criticalMid[0] = TO_HK_TLM_MID;
    
    /* Route 0: Udp. Linked to CF Channel Index 0. */
    g_TO_AppData.routes[0].usExists = 1;
    g_TO_AppData.routes[0].sCfChnlIdx = 0;
    
    return TO_SUCCESS;
}


int32 TO_CustomAppCmds(CFE_SB_MsgPtr_t pCmdMsg)
{
    uint32 uiCmdCode = CFE_SB_GetCmdCode(pCmdMsg);

    if (Ut_TO_UseReturnCode(UT_TO_CUSTOMAPPCMDS_INDEX))
        return Ut_TO_ReturnCodeTable[UT_TO_CUSTOMAPPCMDS_INDEX].Value;

    switch (uiCmdCode)
    {
        case TO_SEND_DATA_TYPE_CC:
            TO_SendDataTypePktCmd(pCmdMsg);
            break;
        
        default:
            g_TO_AppData.HkTlm.usCmdCnt++;
            CFE_EVS_SendEvent(TO_CMD_INF_EID, CFE_EVS_INFORMATION,
                              "Received Custom Cmd (%d)",
                              uiCmdCode);
            break;
    }

    return TO_SUCCESS;
}


/* This implementation simply outputs packet to console by default. */
int32   TO_CustomProcessData(CFE_SB_MsgPtr_t pTlmMsg, int32 size, int32 tblIdx,
                             uint16 usRouteId)
{
    if (Ut_TO_UseReturnCode(UT_TO_CUSTOMPROCESSDATA_INDEX))
    {
        g_TO_AppData.HkTlm.usPktErrCnt++;
        return Ut_TO_ReturnCodeTable[UT_TO_CUSTOMPROCESSDATA_INDEX].Value;
    }

    g_TO_AppData.HkTlm.usPktCnt++;
    UtPrintf("Packet added to frame %u: ", usRouteId);
    UtPrintx(pTlmMsg, size);

    return TO_SUCCESS;
}

int32 TO_CustomFrameStart(uint16 usRouteId)
{
    if (Ut_TO_UseReturnCode(UT_TO_CUSTOMFRAMESTART_INDEX))
        return Ut_TO_ReturnCodeTable[UT_TO_CUSTOMFRAMESTART_INDEX].Value;
    
    return TO_SUCCESS;
}


int32 TO_CustomFrameSend(uint16 usRouteId, int32 iStatus)
{
    if (iStatus == TO_ERROR)
    {
        g_TO_AppData.HkTlm.usFrameErrCnt++;
        return iStatus;
    }
    
    if (Ut_TO_UseReturnCode(UT_TO_CUSTOMFRAMESEND_INDEX))
        return Ut_TO_ReturnCodeTable[UT_TO_CUSTOMFRAMESEND_INDEX].Value;
    
    printf("Frame Sent on route %u.\n", usRouteId);
    
    return TO_SUCCESS;
}



void TO_CustomCleanup(void)
{
    return;
}

/* Simple set whichever route is received as configured */
int32 TO_CustomEnableOutputCmd(CFE_SB_MsgPtr_t pCmdMsg)
{
    TO_EnableOutputCmd_t * pCustomCmd = (TO_EnableOutputCmd_t *) pCmdMsg;
    int32 routeMask = TO_ERROR;
    
    if (Ut_TO_UseReturnCode(UT_TO_CUSTOMENABLEOUTPUTCMD_INDEX))
        return Ut_TO_ReturnCodeTable[UT_TO_CUSTOMENABLEOUTPUTCMD_INDEX].Value;
    
    if (pCustomCmd->usRouteMask & (1<<0))
    {
        routeMask |= (1<<0);
        TO_SetRouteAsConfigured(0);
    }
    if (pCustomCmd->usRouteMask & (1<<1))
    {
        routeMask |= (1<<1);
        TO_SetRouteAsConfigured(1);
    }

    return routeMask;
}

int32 TO_CustomDisableOutputCmd(CFE_SB_Msg_t *pCmdMsg)
{
    TO_DisableOutputCmd_t *pCmd = (TO_DisableOutputCmd_t *) pCmdMsg;
    
    int32 value;
    if (Ut_TO_UseReturnCode(UT_TO_CUSTOMDISABLEOUTPUTCMD_INDEX))
    {
        value = Ut_TO_ReturnCodeTable[UT_TO_CUSTOMDISABLEOUTPUTCMD_INDEX].Value;
        if (value < 0)
        {
            g_TO_AppData.HkTlm.usCmdErrCnt++;
        }
        return value;
    }
    
    g_TO_AppData.usOutputEnabled = 0;

    return pCmd->usRouteMask;
}

int32 Ut_OS_CountSemGetInfoHook(uint32 sem_id, OS_count_sem_prop_t *count_prop)
{
    (void) sem_id;
    
    count_prop->value = 0;
    return OS_SUCCESS;
}

