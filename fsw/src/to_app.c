/******************************************************************************/
/** \file  to_app.c
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*  
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Function Definitions for TO Application
*
*   \par
*     This source code file contains the application layer functions for TO.
*
*   \par
*     This file defines the application functions for the Telemetry Output (TO)
*     application. The application layer is responsible to interact with the cFE
*     Software bus, send the HK packet and the OutData packet. It is also
*     responsible for the management of the configuration table, and respond to
*     the TO commands. 
*
*   \par Functions Defined:
*     - TO_AppMain() - Main entry point. Initializes, then calls TO_RcvMsg. 
*     - TO_AppInit() - Initializes the TO Application
*     - TO_InitEvent() - Initializes the events
*     - TO_InitData() - Initializes HK and OutData packets.
*     - TO_InitTable() - Initializes the configuration table
*     - TO_InitPipe() - Initializes the pipes (Scheduler, telemetry, command)
*     - TO_ValidateTable() - Validate the message configuration table 
*     - TO_RcvMsg() - Pends on SB to perform main funtions.
*     - TO_ProcessTlmPipes() - Process all active telemetry pipes
*     - TO_ProcessNewData() - Process data on specific pipe
*     - TO_ProcessNewCmds() - Call appropriate fnct based on CMD MID.
*     - TO_ProcessNewAppCmds() - Call appropriate fnct based on CMD Code.
*     - TO_ReportHousekeeping() - Send to SB the HK packet.
*     - TO_SendOutData() - Send the OutData packet to the SB.
*     - TO_CleanupCallback() - Call custom cleanup.
*
*   \par Custom Functions Required:
*     - TO_CustomInit() - Initialize the routes and output devices
*     - TO_CustomAppCmds() - Response to custom commands 
*     - TO_CustomProcessData() - Process data over specific route
*     - TO_CustomCleanup() - Cleanup of route devices
*
*   \par Limitations, Assumptions, External Events, and Notes:
*     - All Custom functions are to be defined in to_custom.c
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
#include <string.h>
#include "cfe.h"
#include "to_app.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

/*
** External Global Variables
*/

/*
** Global Variables
*/
TO_AppData_t  g_TO_AppData;

/*
** Local Variables
*/

/******************************************************************************/
/** \brief Main Entry Point for TO Application
*******************************************************************************/
void TO_AppMain(void)
{
    int32  iStatus=CFE_SUCCESS;
    
    /* Register the Application with Executive Services */
    iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("TO - Failed to register the app (0x%08X)\n", 
                             iStatus);
        goto TO_AppMain_Exit_Tag;
    }

    /* Performance Log Entry stamp - #1 */
    CFE_ES_PerfLogEntry(TO_MAIN_TASK_PERF_ID);
    
    /* Perform Application initializations */
    if (TO_AppInit() != CFE_SUCCESS)
    {
        g_TO_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    }

    /* Application Main Loop. Call CFE_ES_RunLoop() to check for changes in the
    ** Application's status. If there is a request to kill this Application, 
    ** it will be passed in through the RunLoop call. */
    while (CFE_ES_RunLoop(&g_TO_AppData.uiRunStatus) == TRUE)
    {
        /* Performance Log Exit stamp */
        CFE_ES_PerfLogExit(TO_MAIN_TASK_PERF_ID);
        
        iStatus = TO_RcvMsg(g_TO_AppData.uiWakeupTimeout); 
    }

    /* Performance Log Exit stamp - #2 */
    CFE_ES_PerfLogExit(TO_MAIN_TASK_PERF_ID);
    
TO_AppMain_Exit_Tag:
    /* Exit the application */
    CFE_ES_ExitApp(g_TO_AppData.uiRunStatus);
} 
    
/******************************************************************************/
/** \brief Initialize the Event Filter Table.
*******************************************************************************/
int32 TO_AppInit(void)
{
    int32  iStatus=CFE_SUCCESS;

    /* Start as disabled. */
    g_TO_AppData.usOutputEnabled = 0;
    g_TO_AppData.usOutputActive = 0;
    g_TO_AppData.uiRunStatus = CFE_ES_APP_RUN;

    /* Init Events */
    if (TO_InitEvent() != CFE_SUCCESS)
    {
        iStatus = TO_ERROR;
        CFE_ES_WriteToSysLog("TO - Event Init failed. \n");
        goto TO_AppInit_Exit_Tag;
    }

    /* Init Data (Always succesful) */
    TO_InitData();

    /* Init Custom (Route configuration, etc.) */
    if (TO_CustomInit() != CFE_SUCCESS)
    {
        iStatus = TO_ERROR;
        CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
                          "TO - Custom Init failed.");
        goto TO_AppInit_Exit_Tag;
    }

    /* Init Config Table */
    if (TO_InitTable() != CFE_SUCCESS)
    {
        iStatus = TO_ERROR;
        CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
                          "TO - Table Init failed.");
        goto TO_AppInit_Exit_Tag;
    }

    /* Init Pipes */
    if (TO_InitPipe() != CFE_SUCCESS)
    {
        iStatus = TO_ERROR;
        CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
                          "TO - Pipe Init failed.");
        goto TO_AppInit_Exit_Tag;
    }
    
    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&TO_CleanupCallback);

TO_AppInit_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(TO_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("TO - Application failed to initialize\n");
    }

    return (iStatus);
}
    
/******************************************************************************/
/** \brief Initialize the Event Filter Table.
*******************************************************************************/
int32 TO_InitEvent(void)
{
    int32  iStatus=CFE_SUCCESS;
    int32  ii = 0;

    /* Create the event table */
    CFE_PSP_MemSet((void*)g_TO_AppData.EventTbl, 0x00, 
                   sizeof(g_TO_AppData.EventTbl));
    
    for (ii = 0; ii < TO_EVT_CNT; ++ii)
    {
        g_TO_AppData.EventTbl[ii].EventID = ii;
    }

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_TO_AppData.EventTbl,
                               TO_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("TO - Failed to register with EVS (0x%08X)\n", 
                             iStatus);
    }

    return (iStatus);
}


/******************************************************************************/
/** \brief Initialize Data
*******************************************************************************/
int32 TO_InitData(void)
{
    uint8 ii = 0;
    int32 iStatus=CFE_SUCCESS;
    char name[OS_MAX_API_NAME];
    char id[3];
    TO_TlmPipe_t *pTlmPipe;
    TO_CfChannel_t *pCfChnl;

    /* Init output data */
    CFE_PSP_MemSet((void*)&g_TO_AppData.OutData, 0x00, 
                   sizeof(g_TO_AppData.OutData));
    CFE_SB_InitMsg(&g_TO_AppData.OutData,
                   TO_OUT_DATA_MID, sizeof(g_TO_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    CFE_PSP_MemSet((void*)&g_TO_AppData.HkTlm, 0x00, sizeof(g_TO_AppData.HkTlm));
    CFE_SB_InitMsg(&g_TO_AppData.HkTlm,
                   TO_HK_TLM_MID, sizeof(g_TO_AppData.HkTlm), TRUE);

    /* Init wakeup count */
    g_TO_AppData.uiWakeupTimeout = TO_WAKEUP_TIMEOUT;
    g_TO_AppData.usWakeupCount = 0;

    /* Init critical MIDs */
    for (ii = 0; ii < TO_NUM_CRITICAL_MIDS; ++ii)
    {
        g_TO_AppData.criticalMid[ii] = 0;
    }

    /* Go Over every route */
    for (ii = 0; ii < TO_MAX_NUM_ROUTES; ++ii)
    {
        /* Initialize all pipes. May be overwritten in TO_CustomInit() */
        pTlmPipe = &g_TO_AppData.tlmPipes[ii];
        pTlmPipe->usTlmPipeDepth = TO_TLM_PIPE_DEPTH;
        CFE_PSP_MemSet((void*)pTlmPipe->cTlmPipeName, '\0', 
                       sizeof(pTlmPipe->cTlmPipeName));
        
        strncpy(name, "TO_TLM_PIPE_", OS_MAX_API_NAME-3);
        sprintf(id, "%u", ii);
        strcat(name, id);
        strncpy(pTlmPipe->cTlmPipeName, name, OS_MAX_API_NAME-1);

        /* Initialize the Route info. Config Routes in TO_CustomInit() */
        /* Must set usExist = 1 for routes to be used to_custom.c */
        g_TO_AppData.routes[ii].usWakePeriod = 1;
        g_TO_AppData.routes[ii].sCfChnlIdx = -1;
        g_TO_AppData.routes[ii].usExists = 0;
        g_TO_AppData.routes[ii].usIsConfig = 0;
        g_TO_AppData.routes[ii].usIsEnabled = 0;
    
    }

    /* Go over every CF Channel and initialize. 
     * 
     * NOTE: 
     * - Every CF channel included is enabled by default.
     * - Set cCfCntSemName in TO_CustomInit if used. 
     * - Two CF Channels must not share the same throttling semaphore name.
     * - CF Channels are associated to a route through route->sCfChnlIdx */
    for (ii = 0; ii < TO_NUM_CF_CHANNELS; ++ii)
    {
        pCfChnl = &g_TO_AppData.cfChnls[ii];
        pCfChnl->usIsEnabled = 1;
        pCfChnl->uiCfCntSemId = 0;
        strncpy(pCfChnl->cCfCntSemName, TO_CF_THROTTLE_SEM_NAME,
                OS_MAX_API_NAME);
    }

    return iStatus;
}
    

/******************************************************************************/
/** \brief Initialize Table
*******************************************************************************/
int32  TO_InitTable(void)
{
    int32 iStatus = CFE_SUCCESS;
    void * pTable;

    /* Register the table with cFE Table services. */
    iStatus = CFE_TBL_Register(&g_TO_AppData.tableHandle, TO_CONFIG_TABLENAME,
                               sizeof(TO_ConfigTable_t),
                               CFE_TBL_OPT_DEFAULT, 
                               (CFE_TBL_CallbackFuncPtr_t) TO_ValidateTable);

    if (iStatus != CFE_SUCCESS)
    {
    	CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
    			"CFE_TBL_Register() returned error 0x%08x.  Aborting table init.",
    			iStatus);

        goto end_of_function;
    }

    /* Load the TO configuration table file */
    iStatus = CFE_TBL_Load(g_TO_AppData.tableHandle, CFE_TBL_SRC_FILE,
                  		   TO_CONFIG_FILENAME);

    if (iStatus != CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
                  "CFE_TBL_Load() returned error 0x%08x.  Aborting table init.",
                  iStatus);

        goto end_of_function;
    }

    /* Manage the TO config table */
    iStatus = CFE_TBL_Manage(g_TO_AppData.tableHandle);
    if (iStatus != CFE_SUCCESS)
    {
    	CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
           "CFE_TBL_Manage() returned error 0x%08x.  Aborting table init.",
           iStatus);

        goto end_of_function;
    }

    /* Make sure the TO_Load Table is accessible */
    iStatus = CFE_TBL_GetAddress (&pTable, g_TO_AppData.tableHandle);
    
    /* Status should be CFE_TBL_INFO_UPDATED because we loaded it above */
    if (iStatus != CFE_TBL_INFO_UPDATED)
    {
    	CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
    			"CFE_TBL_GetAddress() returned error 0x%08x. "
                "Aborting table init.",
    			iStatus);

    	goto end_of_function;
    }

    /* Store the pointer */
    g_TO_AppData.pConfigTable = (TO_ConfigTable_t *) pTable;

    /* Register to receive TBL manage request commands for table updates. */
    iStatus = CFE_TBL_NotifyByMessage(g_TO_AppData.tableHandle, 
                                      TO_APP_CMD_MID,
                                      TO_MANAGE_TABLE_CC, 0);
    if (iStatus != CFE_SUCCESS)
    {
    	CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
    			"CFE_TBL_NotifyByMessage() returned error 0x%08x. "
                "Aborting table init.", iStatus);
    }
   
end_of_function:
    return iStatus;
}

/******************************************************************************/
/** \brief Initialize the Pipes
*******************************************************************************/
int32 TO_InitPipe(void)
{
    int32  iStatus=CFE_SUCCESS;
    int32  jj;
    TO_TlmPipe_t *pTlmPipe = NULL;
    TO_Route_t *pRoute = NULL;
    TO_CfChannel_t *pCfChnl = NULL;

    /* Init schedule pipe */
    g_TO_AppData.usSchPipeDepth = TO_SCH_PIPE_DEPTH;
    CFE_PSP_MemSet((void*)g_TO_AppData.cSchPipeName, '\0', 
                   sizeof(g_TO_AppData.cSchPipeName));
    strncpy(g_TO_AppData.cSchPipeName, "TO_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_TO_AppData.SchPipeId,
                                 g_TO_AppData.usSchPipeDepth,
                                 g_TO_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        CFE_SB_Subscribe(TO_WAKEUP_MID, g_TO_AppData.SchPipeId);
    }
    else
    {
        CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
                          "Failed to create SCH pipe (Error:0x%08x)", iStatus);
        goto TO_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_TO_AppData.usCmdPipeDepth = TO_CMD_PIPE_DEPTH ;
    CFE_PSP_MemSet((void*)g_TO_AppData.cCmdPipeName, '\0', 
                   sizeof(g_TO_AppData.cCmdPipeName));
    strncpy(g_TO_AppData.cCmdPipeName, "TO_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_TO_AppData.CmdPipeId,
                                 g_TO_AppData.usCmdPipeDepth,
                                 g_TO_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        CFE_SB_Subscribe(TO_APP_CMD_MID, g_TO_AppData.CmdPipeId);
        CFE_SB_Subscribe(TO_SEND_HK_MID, g_TO_AppData.CmdPipeId);
    }
    else
    {
        CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
                          "Failed to create CMD pipe (Error:0x%08x)", iStatus);
        goto TO_InitPipe_Exit_Tag;
    }

    /* Create route Pipes and CF Channels */
    for (jj = 0; jj < TO_MAX_NUM_ROUTES; ++jj)
    {
        pRoute = &g_TO_AppData.routes[jj];

        /* Route must exist in order to create a pipe link to CF channel */
        if (pRoute->usExists)
        {
            /* Create a route specific telemetry pipe */
            pTlmPipe = &g_TO_AppData.tlmPipes[jj];
            iStatus = CFE_SB_CreatePipe(&pTlmPipe->cfePipeId,
                                        pTlmPipe->usTlmPipeDepth,
                                        pTlmPipe->cTlmPipeName);

            if (iStatus != CFE_SUCCESS)
            {
                CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
                                  "Failed to create TLM pipe:%s, "
                                  "(CFE Error:0x%08x)", 
                                  pTlmPipe->cTlmPipeName, iStatus);
                goto TO_InitPipe_Exit_Tag;
            }
        
            /* Setup CF channel Semaphore for routes with channel */
            if (pRoute->sCfChnlIdx >= 0)
            {
                if (pRoute->sCfChnlIdx >= TO_NUM_CF_CHANNELS)
                {
                    CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
                                  "Route:%u, has invalid sCfChnlIdx of %d."
                                  " Only %u CF channels available. ",
                                  jj, pRoute->sCfChnlIdx, TO_NUM_CF_CHANNELS);
                    iStatus = TO_ERROR;
                    goto TO_InitPipe_Exit_Tag;
                }
                
                pCfChnl = &g_TO_AppData.cfChnls[pRoute->sCfChnlIdx];
                
                /* Initialize CF Counting Sem to Pipe Depth */
                /* NOTE: This will fail if two routes attempt to use the same 
                 * CF channel. */
                iStatus = OS_CountSemCreate(&pCfChnl->uiCfCntSemId, 
                                            pCfChnl->cCfCntSemName,
                                            pTlmPipe->usTlmPipeDepth, 0);
                if (iStatus != OS_SUCCESS)
                {
                    CFE_EVS_SendEvent(TO_INIT_ERR_EID, CFE_EVS_ERROR,
                                  "Failed to create counting semaphore "
                                  "for CF channel:%s for route ID:%u. "
                                  "(OSAL Error:%d)",
                                  pCfChnl->cCfCntSemName, jj, iStatus);
                    goto TO_InitPipe_Exit_Tag;
                }
            }
        }
    }
        
    /* Subscribe to messages on the telemetry pipe for all existing routes */
    iStatus = TO_SubscribeAllMsgs();
    if (iStatus != TO_SUCCESS)
    {
        goto TO_InitPipe_Exit_Tag;
    }

TO_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/******************************************************************************/
/** \brief Validate Table Load
*******************************************************************************/
int32 TO_ValidateTable(void* table)
{
    int32 iStatus = CFE_SUCCESS;
    int32 reachedUnused = 0;
    int32 ii, jj;

    TO_ConfigTable_t * pTable = (TO_ConfigTable_t *) table;

    /* Loop over all entries, looking for gaps and duplicate MID */
    for (ii = 0; ii < TO_MAX_TBL_ENTRIES; ii++)
    {
        TO_TableEntry_t *entry = &pTable->entries[ii];

        /* If the entry is in use */
        if (entry->usMsgId != TO_UNUSED_ENTRY &&
            entry->usMsgId != TO_REMOVED_ENTRY)
        {
            /* After an Unused entry found, there should be no more entries. */
            if (reachedUnused) 
            {
                CFE_EVS_SendEvent(TO_TBL_ERR_EID, CFE_EVS_ERROR,
                                  "Table Validation failed. "
                                  "Unused Entry before entry index:%d", ii);
                iStatus = TO_ERROR;
                goto end_of_function;
            }
            
            /* Look for duplicates */
            for (jj = ii + 1; jj < TO_MAX_TBL_ENTRIES; jj++)
            {
                TO_TableEntry_t *entryCmp = &pTable->entries[jj]; 
                
                if (entryCmp->usMsgId == TO_UNUSED_ENTRY)
                {
                    break;
                }
                else if (entryCmp->usMsgId != TO_REMOVED_ENTRY &&               
                         entry->usMsgId == entryCmp->usMsgId)
                {
                    CFE_EVS_SendEvent(TO_TBL_ERR_EID, CFE_EVS_ERROR,
                                       "Table Validation failed. "
                                       "Duplicate MID:0x%04x", entry->usMsgId);
                    
                    iStatus = TO_ERROR;
                    goto end_of_function;
                }
            }
        }
        else if (entry->usMsgId == TO_UNUSED_ENTRY)
        {
            reachedUnused = 1;
        }
    }

    /* Check that all critical messages are present. */
    for (ii = 0; ii < TO_NUM_CRITICAL_MIDS; ii++)
    {
        iStatus = TO_FindTableIndex(pTable, g_TO_AppData.criticalMid[ii]);

        if (g_TO_AppData.criticalMid[ii] != 0 && iStatus == TO_NO_MATCH)
        {
            CFE_EVS_SendEvent(TO_TBL_ERR_EID, CFE_EVS_ERROR,
                               "Table Validation failed. "
                               "Missing Critical MID:0x%04x", 
                               g_TO_AppData.criticalMid[ii]);
            
            iStatus = TO_ERROR;
            goto end_of_function;
        }
    }

    iStatus = CFE_SUCCESS;

end_of_function:    
    return iStatus;
}

/******************************************************************************/
/** \brief Receive Messages from Software Bus
*******************************************************************************/
int32 TO_RcvMsg(int32 iBlocking)
{
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_MsgPtr_t pMsg=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&pMsg, g_TO_AppData.SchPipeId, iBlocking);
        
    /* Performance Log Entry stamp - #2 */
    CFE_ES_PerfLogEntry(TO_MAIN_TASK_PERF_ID); 
    
    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(pMsg);
        switch (MsgId)
        {
            case TO_WAKEUP_MID:
                TO_ProcessNewCmds();
                TO_ProcessTlmPipes();
                TO_SendOutData();
                break;

            default:
                CFE_EVS_SendEvent(TO_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "Recvd invalid SCH usMsgId (0x%04X)", MsgId);
        }
    }
    /* Implementation may set usWakeupTimeout instead of relying on
     * Scheduler for wakeup message. */
    else if (iStatus == CFE_SB_TIME_OUT)
    {
        TO_ProcessNewCmds();
        TO_ProcessTlmPipes();
        TO_SendOutData();
    }
    else
    {
        CFE_EVS_SendEvent(TO_PIPE_ERR_EID, CFE_EVS_ERROR,
                         "TO: SB pipe read error (0x%08x), app will exit", 
                         iStatus);
        g_TO_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }
    
    return (iStatus);
}
    

/******************************************************************************/
/** \brief Process Telemetry Pipes
*******************************************************************************/
void TO_ProcessTlmPipes(void)
{
    uint8 ii = 0;
    uint16 wakeCnt = g_TO_AppData.usWakeupCount;

    /* Loop over every pipe and process data. */
    for (ii = 0; ii < TO_MAX_NUM_ROUTES; ++ii)
    {
        /* Only existing routes will have a pipe to process */
        if (g_TO_AppData.routes[ii].usExists)
        {
            /* Process the pipe if the period corresponds. */
            if (wakeCnt % g_TO_AppData.routes[ii].usWakePeriod == 0)
            {
                TO_ProcessNewData(&g_TO_AppData.tlmPipes[ii], ii);
            }
        }
        else
        {
            break;
        }
    }

    /* Increment the wakeup count */
    g_TO_AppData.usWakeupCount++;
    /* If we hit the max wakeup count, rollover to 0 */
    if (g_TO_AppData.usWakeupCount == TO_MAX_WAKEUP_COUNT)
    {
        g_TO_AppData.usWakeupCount = 0;
    }

    return;
}



/******************************************************************************/
/** \brief Process New Data
*******************************************************************************/
void TO_ProcessNewData(TO_TlmPipe_t *pTlmPipe, uint16 usRouteId)
{
    CFE_SB_MsgPtr_t         pTlmMsg=NULL;
    CFE_SB_MsgId_t          usMsgId = 0;
    boolean                 bGotNewMsg=TRUE;
    int32                   size = 0;
    int32                   iStatus = 0;
    int32                   iTblIdx = 0;
    TO_TableEntry_t         *pEntry=NULL;
    boolean                 bHasCfChnl=FALSE;
    int16                   sCfChnlIdx;
    uint16                  uiCntSemId; 
    OS_count_sem_prop_t     cntSemProp;

#ifdef TO_FRAMING_ENABLED
    /* Prepare framing mechanism, if applicable. */
    if (g_TO_AppData.usOutputEnabled && g_TO_AppData.usOutputActive &&
        g_TO_AppData.routes[usRouteId].usIsEnabled)
    {
        iStatus = TO_CustomFrameStart(usRouteId);
        if (iStatus != TO_SUCCESS)
        {
            /* Error event should be issued in TO_CustomFrameStart */
            goto end_of_function;
        }
    }
#endif
    
    /* Get the Counting Sem Id if applicable */
    sCfChnlIdx = g_TO_AppData.routes[usRouteId].sCfChnlIdx;
    if (sCfChnlIdx >= 0 && g_TO_AppData.cfChnls[sCfChnlIdx].usIsEnabled)
    {
        bHasCfChnl = TRUE;
        uiCntSemId = g_TO_AppData.cfChnls[sCfChnlIdx].uiCfCntSemId;
    }

    while (bGotNewMsg)
    {
        if (CFE_SB_RcvMsg(&pTlmMsg, pTlmPipe->cfePipeId, CFE_SB_POLL) == 
            CFE_SUCCESS)
        {
            /* Process if output is enabled and active. Otherwise, drop. */
            if (g_TO_AppData.usOutputEnabled && g_TO_AppData.usOutputActive)
            {
                usMsgId = CFE_SB_GetMsgId(pTlmMsg);
                iTblIdx = TO_FindTableIndex(g_TO_AppData.pConfigTable, usMsgId);

                if (iTblIdx == TO_NO_MATCH)
                {
                    CFE_EVS_SendEvent(TO_PIPE_ERR_EID, CFE_EVS_ERROR,
                                      "Received invalid MID on TlmPipe. "
                                      "MID:0x%04x", usMsgId);
                }
                /* Process message if the table entry is enabled for 
                 * this route. */
                else
                {
                    pEntry = &g_TO_AppData.pConfigTable->entries[iTblIdx];
                    
                    /* Process data over enabled routes for enabled messages */ 
                    if (pEntry->usState && 
                        (pEntry->usRouteMask & (1<<usRouteId)) &&
                        g_TO_AppData.routes[usRouteId].usIsEnabled)
                    {
                        size = CFE_SB_GetTotalMsgLength(pTlmMsg);
                        iStatus = TO_CustomProcessData(pTlmMsg, size, iTblIdx, 
                                                       usRouteId);
                    
                        if (iStatus < 0)
                        {
                            break;
                        }
                    }
                }
            }

            if(bHasCfChnl)
            {
                /* Give a CF Throttle count Semaphore if required */
                OS_CountSemGetInfo(uiCntSemId, &cntSemProp);
                if (cntSemProp.value < pTlmPipe->usTlmPipeDepth)
                {
                    /* If the current count is less than pipe depth, 
                     * give one count */
                    OS_CountSemGive(uiCntSemId);
                }
            }
        }
        else
        {
            bGotNewMsg = FALSE;
        }
    }

#ifdef TO_FRAMING_ENABLED
end_of_function:
    
    /* Complete and send a frame, if applicable. */
    /* NOTE: The CustomProcessData may have failed. Custom function responsible
       for dealing with status as provided. */
    if (g_TO_AppData.usOutputEnabled && g_TO_AppData.usOutputActive &&
        g_TO_AppData.routes[usRouteId].usIsEnabled)
    {
        TO_CustomFrameSend(usRouteId, iStatus);
    }

    return;
#endif
}
    

/******************************************************************************/
/** \brief Process New Commands
*******************************************************************************/
void TO_ProcessNewCmds(void)
{
    CFE_SB_MsgPtr_t pCmdMsg=NULL;
    CFE_SB_MsgId_t  usMsgId;
    boolean         bGotNewMsg=TRUE;

    while (bGotNewMsg)
    {
        if (CFE_SB_RcvMsg(&pCmdMsg, g_TO_AppData.CmdPipeId, CFE_SB_POLL) == 
            CFE_SUCCESS)
        {
            usMsgId = CFE_SB_GetMsgId(pCmdMsg);
            switch (usMsgId)
            {
                case TO_APP_CMD_MID:
                    TO_ProcessNewAppCmds(pCmdMsg);  
                    break;
                
                case TO_SEND_HK_MID:
                    TO_ReportHousekeeping();
                    break;

                default:
                    g_TO_AppData.HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(TO_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid CMD usMsgId (0x%04X)", 
                                      usMsgId);
                    break;
            }
        }
        else
        {
            bGotNewMsg = FALSE;
        }
    }
}
    
/******************************************************************************/
/** \brief Process New App Commands
*******************************************************************************/
void TO_ProcessNewAppCmds(CFE_SB_MsgPtr_t pMsg)
{
    int32 iStatus = TO_SUCCESS;
    uint32  uiCmdCode = 0;
    
    if (pMsg != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(pMsg);
        switch (uiCmdCode)
        {
            case TO_NOOP_CC:
                TO_NoopCmd(pMsg);
                break;

            case TO_RESET_CC:
                TO_ResetCmd(pMsg);
                break;
            
            case TO_ENABLE_OUTPUT_CC:
                TO_EnableOutputCmd(pMsg);
                break;

            case TO_DISABLE_OUTPUT_CC:
                TO_DisableOutputCmd(pMsg);
                break;                   

            case TO_PAUSE_OUTPUT_CC:
                TO_PauseOutputCmd(pMsg);
                break;

            case TO_RESUME_OUTPUT_CC:
                TO_ResumeOutputCmd(pMsg);
                break;
            
            case TO_ADD_TBL_ENTRY_CC:
                TO_AddTblEntryCmd(pMsg);
                break;

            case TO_REMOVE_TBL_ENTRY_CC:
                TO_RemoveTblEntryCmd(pMsg);
                break;   
           
            case TO_ENABLE_TBL_ENTRY_CC:
                TO_EnableTblEntryCmd(pMsg);
                break;

            case TO_DISABLE_TBL_ENTRY_CC:
                TO_DisableTblEntryCmd(pMsg);
                break;

            case TO_ENABLE_GROUP_CC:
                TO_EnableGroupCmd(pMsg);
                break;

            case TO_DISABLE_GROUP_CC:
                TO_DisableGroupCmd(pMsg);
                break;

            case TO_ENABLE_ALL_CC:
                TO_EnableAllCmd(pMsg);
                break;

            case TO_DISABLE_ALL_CC:
                TO_DisableAllCmd(pMsg);
                break;
            
            case TO_SET_ROUTE_BY_MID_CC:
                TO_SetRouteByMidCmd(pMsg);
                break;
            
            case TO_SET_ROUTE_BY_GROUP_CC:
                TO_SetRouteByGroupCmd(pMsg);
                break;

            case TO_MANAGE_TABLE_CC:
                TO_ManageTableCmd(pMsg);
                break;

            case TO_ACTIVATE_ROUTES_CC:
                TO_ActivateRoutesCmd(pMsg);
                break;
            
            case TO_DEACTIVATE_ROUTES_CC:
                TO_DeactivateRoutesCmd(pMsg);
                break;

            case TO_SET_ROUTE_PERIOD_CC:
                TO_SetRoutePeriodCmd(pMsg);
                break;

            case TO_SET_WAKEUP_TIMEOUT_CC:
                TO_SetWakeupTimeoutCmd(pMsg);
                break;


            /* Any other commands are assumed to be custom commands. */
            default:
                iStatus = TO_CustomAppCmds(pMsg);
               
                if (iStatus != TO_SUCCESS) 
                {
                    g_TO_AppData.HkTlm.usCmdErrCnt++;
                    CFE_EVS_SendEvent(TO_CMD_ERR_EID, CFE_EVS_ERROR,
                                      "Recvd invalid cmd code (%d)", uiCmdCode);
                }
                break;
        }
    }
}

/******************************************************************************/
/** \brief Report Housekeeping Packet
*******************************************************************************/
void TO_ReportHousekeeping(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_MsgPtr_t)&g_TO_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_MsgPtr_t)&g_TO_AppData.HkTlm);
}
    
/******************************************************************************/
/** \brief Send the OutData Packet
*******************************************************************************/
void TO_SendOutData(void)
{
    CFE_SB_TimeStampMsg((CFE_SB_MsgPtr_t)&g_TO_AppData.OutData);
    CFE_SB_SendMsg((CFE_SB_MsgPtr_t)&g_TO_AppData.OutData);
}

/******************************************************************************/
/** \brief Perform cleanup on shutdown
*******************************************************************************/
void TO_CleanupCallback(void)
{
    /* Call Custom Cleanup to close transport protocol, etc. */
    TO_CustomCleanup();
}
    
/*==============================================================================
** End of file to_app.c
**============================================================================*/
