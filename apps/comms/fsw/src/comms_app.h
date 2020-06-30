/*=======================================================================================
** File Name:  comms_app.h
**
** Title:  Header File for COMMS Application
**
** $Author:    Paula Bosca
** $Revision: 1.1 $
** $Date:      2020-06-24
**
** Purpose:  To define COMMS's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-06-24 | Paula Bosca | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _COMMS_APP_H_
#define _COMMS_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "comms_platform_cfg.h"
#include "comms_mission_cfg.h"
#include "comms_private_ids.h"
#include "comms_private_types.h"
#include "comms_perfids.h"
#include "comms_msgids.h"
#include "comms_msg.h"



/*
** Local Defines
*/
#define COMMS_TIMEOUT_MSEC    1000

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[COMMS_EVT_CNT];

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
       Data structure should be defined in comms/fsw/src/comms_private_types.h */
    COMMS_InData_t   InData;

    /* Output data - to be published at the end of a Wakeup cycle.
       Data structure should be defined in comms/fsw/src/comms_private_types.h */
    COMMS_OutData_t  OutData;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in comms/fsw/src/comms_msg.h */
    COMMS_HkTlm_t  HkTlm;

    /* TODO:  Add declarations for additional private data here */
} COMMS_AppData_t;

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
int32  COMMS_InitApp(void);
int32  COMMS_InitEvent(void);
int32  COMMS_InitData(void);
int32  COMMS_InitPipe(void);

void  COMMS_AppMain(void);

void  COMMS_CleanupCallback(void);

int32  COMMS_RcvMsg(int32 iBlocking);

void  COMMS_ProcessNewData(void);
void  COMMS_ProcessNewCmds(void);
void  COMMS_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  COMMS_ReportHousekeeping(void);
void  COMMS_SendOutData(void);

boolean  COMMS_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

#endif /* _COMMS_APP_H_ */

/*=======================================================================================
** End of file comms_app.h
**=====================================================================================*/
    