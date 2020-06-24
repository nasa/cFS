/*=======================================================================================
** File Name:  payload_app.h
**
** Title:  Header File for PAYLOAD Application
**
** $Author:    Paula Bosca
** $Revision: 1.1 $
** $Date:      2020-06-24
**
** Purpose:  To define PAYLOAD's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-06-24 | Paula Bosca | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _PAYLOAD_APP_H_
#define _PAYLOAD_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "payload_platform_cfg.h"
#include "payload_mission_cfg.h"
#include "payload_private_ids.h"
#include "payload_private_types.h"
#include "payload_perfids.h"
#include "payload_msgids.h"
#include "payload_msg.h"



/*
** Local Defines
*/
#define PAYLOAD_TIMEOUT_MSEC    1000

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[PAYLOAD_EVT_CNT];

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId; 
    uint16           usSchPipeDepth;
    char             cSchPipeName[OS_MAX_API_NAME];

    /* CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;
    uint16           usCmdPipeDepth;
    char             cCmdPipeName[OS_MAX_API_NAME];
    
    /* CFE telemetry pipe */
    CFE_SB_PipeId_t  TlmPipeId;
    uint16           usTlmPipeDepth;
    char             cTlmPipeName[OS_MAX_API_NAME];

    /* Task-related */
    uint32  uiRunStatus;
    
    /* Input data - from I/O devices or subscribed from other apps' output data.
       Data structure should be defined in payload/fsw/src/payload_private_types.h */
    PAYLOAD_InData_t   InData;

    /* Output data - to be published at the end of a Wakeup cycle.
       Data structure should be defined in payload/fsw/src/payload_private_types.h */
    PAYLOAD_OutData_t  OutData;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in payload/fsw/src/payload_msg.h */
    PAYLOAD_HkTlm_t  HkTlm;

    /* TODO:  Add declarations for additional private data here */
} PAYLOAD_AppData_t;

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/
int32  PAYLOAD_InitApp(void);
int32  PAYLOAD_InitEvent(void);
int32  PAYLOAD_InitData(void);
int32  PAYLOAD_InitPipe(void);

void  PAYLOAD_AppMain(void);

void  PAYLOAD_CleanupCallback(void);

int32  PAYLOAD_RcvMsg(int32 iBlocking);

void  PAYLOAD_ProcessNewData(void);
void  PAYLOAD_ProcessNewCmds(void);
void  PAYLOAD_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  PAYLOAD_ReportHousekeeping(void);
void  PAYLOAD_SendOutData(void);

boolean  PAYLOAD_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

#endif /* _PAYLOAD_APP_H_ */

/*=======================================================================================
** End of file payload_app.h
**=====================================================================================*/
    