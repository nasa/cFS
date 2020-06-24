/*=======================================================================================
** File Name:  eps_app.h
**
** Title:  Header File for EPS Application
**
** $Author:    Paula Bosca
** $Revision: 1.1 $
** $Date:      2020-06-24
**
** Purpose:  To define EPS's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-06-24 | Paula Bosca | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _EPS_APP_H_
#define _EPS_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "eps_platform_cfg.h"
#include "eps_mission_cfg.h"
#include "eps_private_ids.h"
#include "eps_private_types.h"
#include "eps_perfids.h"
#include "eps_msgids.h"
#include "eps_msg.h"



/*
** Local Defines
*/
#define EPS_TIMEOUT_MSEC    1000

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[EPS_EVT_CNT];

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
       Data structure should be defined in eps/fsw/src/eps_private_types.h */
    EPS_InData_t   InData;

    /* Output data - to be published at the end of a Wakeup cycle.
       Data structure should be defined in eps/fsw/src/eps_private_types.h */
    EPS_OutData_t  OutData;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in eps/fsw/src/eps_msg.h */
    EPS_HkTlm_t  HkTlm;

    /* TODO:  Add declarations for additional private data here */
} EPS_AppData_t;

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
int32  EPS_InitApp(void);
int32  EPS_InitEvent(void);
int32  EPS_InitData(void);
int32  EPS_InitPipe(void);

void  EPS_AppMain(void);

void  EPS_CleanupCallback(void);

int32  EPS_RcvMsg(int32 iBlocking);

void  EPS_ProcessNewData(void);
void  EPS_ProcessNewCmds(void);
void  EPS_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  EPS_ReportHousekeeping(void);
void  EPS_SendOutData(void);

boolean  EPS_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

#endif /* _EPS_APP_H_ */

/*=======================================================================================
** End of file eps_app.h
**=====================================================================================*/
    