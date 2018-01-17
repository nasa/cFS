/******************************************************************************/
/** \file  to_app.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*  
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Header file for the TO application
*
*   \par Limitations, Assumptions, External Events, and Notes:
*       - Application functions are defined in to_app.c
*       - Command functions are declared in to_cmds.h and defined in to_cmds.c
*       - Utilities are defined in to_utils.c
*       - Custom functions are defined in to_custom.c
*       - A route is configured through to_custom.c
*       - SEND_HK is subscribed to the command pipe. The wakeup rate should
*         generally be set faster than the SEND_HK rate, otherwise HK packets
*         will be dropped. SEND_HK is required to get housekeeping data.
*       - The TO_WAKEUP_TIMEOUT value may be used instead of Scheduler 
*         table for app processing rate. Timeout rate can be changed by cmd.
*
*   \par Modification History:
*     - 2015-01-09 | Guy de Carufel | Code Started
*     - 2016-05-11 | Allen Brown | Updated comments
*******************************************************************************/
    
#ifndef _TO_APP_H_
#define _TO_APP_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
** Includes
*******************************************************************************/
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "osconfig.h"

#include "to_platform_cfg.h"
#include "to_mission_cfg.h"
#include "to_events.h"
#include "to_tbldefs.h"
#include "to_cmds.h"

/*******************************************************************************
** Macro Definitions
*******************************************************************************/
/** \name Version numbers */
/** \{ */
#define TO_MAJOR_VERSION     1
#define TO_MINOR_VERSION     0
#define TO_REVISION          0
#define TO_MISSION_REV       0
/** \} */

/** \name Return codes */
/** \{ */
#define TO_SUCCESS            0
#define TO_ERROR            (-1)
#define TO_TBL_FULL_ERR     (-2)
#define TO_BAD_ARG_ERR      (-3)
#define TO_NO_MATCH         (-4)
#define TO_NO_EFFECT        (-5)
/** \} */

#define TO_MAX_NUM_ROUTES   16
#define TO_MAX_ROUTE_MASK   0xffffL


/** Default values of macros if not defined in to_platform_cfg.h */
/** \name Default Settings */
/** \{ */
#ifndef TO_WAKEUP_TIMEOUT     
#define TO_WAKEUP_TIMEOUT       500
#endif
#ifndef TO_MIN_WAKEUP_TIMEOUT     
#define TO_MIN_WAKEUP_TIMEOUT   500
#endif

/* NOTE: Changing this value will impact what are valid uiWakePeriod
 * values for a route. Valid values are any factors of this max count. */
#ifndef TO_MAX_WAKEUP_COUNT
#define TO_MAX_WAKEUP_COUNT     20000
#endif

#ifndef TO_SCH_PIPE_DEPTH
#define TO_SCH_PIPE_DEPTH       10
#endif

#ifndef TO_CMD_PIPE_DEPTH  
#define TO_CMD_PIPE_DEPTH       10
#endif
#ifndef TO_TLM_PIPE_DEPTH  
#define TO_TLM_PIPE_DEPTH       10
#endif

#ifndef TO_NUM_CRITICAL_MIDS   
#define TO_NUM_CRITICAL_MIDS    0
#endif

#ifndef TO_GROUP_NUMBER_MASK    
#define TO_GROUP_NUMBER_MASK    0xFF000000
#endif
#ifndef TO_MULTI_GROUP_MASK     
#define TO_MULTI_GROUP_MASK     0x00FFFFFF
#endif

#ifndef TO_CONFIG_TABLENAME 
#define TO_CONFIG_TABLENAME     "to_config"
#endif
#ifndef TO_CONFIG_FILENAME 
#define TO_CONFIG_FILENAME      "/cf/apps/to_config.tbl"
#endif

#ifndef TO_NUM_CF_CHANNELS      
#define TO_NUM_CF_CHANNELS      1
#endif

#ifndef TO_CF_THROTTLE_SEM_NAME 
#define TO_CF_THROTTLE_SEM_NAME "CFTOSemId"
#endif
/** \} */


/*******************************************************************************
** Structure definitions
*******************************************************************************/
/** /brief TO Telemetry Pipes */
typedef struct
{
    CFE_SB_PipeId_t  cfePipeId;                     /**< Telemetry Pipe ID    */
    uint16           usTlmPipeDepth;                /**< Telemetry Pipe depth */
    char             cTlmPipeName[OS_MAX_API_NAME]; /**< Telemetry Pipe name  */
} TO_TlmPipe_t; 

typedef struct
{
    uint16  usWakePeriod;  /**< Multiple of TO rate when route should 
                                be processed (and associated pipe.            */
    int16   sCfChnlIdx;    /**< Index of CF channel (-1 if none)              */
    uint16  usExists;      /**< A Route exists (Pipe to be created)           */
    uint16  usIsConfig;    /**< Route is configured (through to_custom.c)     */
    uint16  usIsEnabled;   /**< Route is enabled (Telemetry processed)        */
} TO_Route_t;

typedef struct
{
    uint16  usIsEnabled;    /**< Channel is Enabled (default:true)            */
    uint32  uiCfCntSemId;   /**< The CF Throttling Counting Semaphore ID.     
                                 Set by TO_InitPipe.                          */
    char    cCfCntSemName[OS_MAX_API_NAME];
                            /**< Name of CF throttling semaphore              */
} TO_CfChannel_t;


/** /brief AppData Structure Defenition */
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[TO_EVT_CNT];      /**< Event Filter Table.  */

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId;                     /**< Schedule Pipe ID     */
    uint16           usSchPipeDepth;                /**< Schedule Pipe depth  */
    char             cSchPipeName[OS_MAX_API_NAME]; /**< Schedule Pipe name   */

    /* CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;                     /**< Command Pipe ID      */
    uint16           usCmdPipeDepth;                /**< Command Pipe depth   */
    char             cCmdPipeName[OS_MAX_API_NAME]; /**< Command Pipe name    */

    /* Routing structures */
    TO_Route_t       routes[TO_MAX_NUM_ROUTES];     /**< Output Routes        */
    TO_TlmPipe_t     tlmPipes[TO_MAX_NUM_ROUTES];   /**< Telemetry Pipes      */
    TO_CfChannel_t   cfChnls[TO_NUM_CF_CHANNELS];   /**< CF Channels          */

    /* Task-related */
    uint32  uiRunStatus;      /**< Application Run Status                     */
    
    /* Wakeup schedule */
    uint32  uiWakeupTimeout;  /**< Wakeup timeout in ms                       */
    uint16  usWakeupCount;    /**< Wakeup cnts, for route wake period check.  */

    /* Output data (Data destined for other applications over SB)
       Data structure defined in $MISSION/apps/inc/{MISSION}_to_types.h */
    TO_OutData_t  OutData;      /**< Output SB Data Packet                    */

    /* Housekeeping telemetry 
       Data structure defined in $MISSION/apps/inc/{MISSION}_to_types.h */
    TO_HkTlm_t  HkTlm;          /**< Housekeeping Packet                      */

    /* MIDs which must be present in the config tbl Set in TO_CustomInit. */
    CFE_SB_MsgId_t criticalMid[TO_NUM_CRITICAL_MIDS]; /**< Critical msg Ids   */
   
    /* Configuration tables */
    TO_ConfigTable_t  *pConfigTable;     /**< Pointer to the config table     */
    CFE_TBL_Handle_t   tableHandle;      /**< Table handle                    */

    /* Output flags. 
       TO_CustomProcessData called when output is enabled and active. */
    uint16         usOutputEnabled;  /**< Output enabled(by enable/disable)   */
    uint16         usOutputActive;   /**< Output active (by pause/resume)     */
                                             
} TO_AppData_t;


/*******************************************************************************
** Application Function Declarations (defined in to_app.c)
*******************************************************************************/

/******************************************************************************/
/** \brief Main Entry Point for TO Application
*
*   \par Description/Algorithm
*       This function is the main entry point of the TO Application. It 
*       performs the following:
*       1. Registers the Application with cFE
*       2. Initializes the application through TO_AppInit
*       3. Loops over the TO_RcvMsg function to perform main function.
*       4. Exit application on kill signal or error
*
*   \par Assumptions, External Events, and Notes:
*       - The TO_MAIN_TASK_PERF_ID Entered in TO_RcvMsg
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns None
*
*   \see 
*       #TO_AppInit
*       #TO_RcvMsg
*       #TO_CleanupCallback
*******************************************************************************/
void  TO_AppMain(void);

/******************************************************************************/
/** \brief Initialize The Application
*
*   \par Description/Algorithm
*        High level initialization function.  Calls in order:
*        1. TO_InitEvent
*        2. TO_InitData
*        3. TO_CustomInit
*        4. TO_InitTable
*        5. TO_InitPipe
*        6. Installs the Cleanup Callback function.
*
*   \par Assumptions, External Events, and Notes:
*       - Output is initially disabled
*       - The TO_CustomInit is defined in to_custom.c
*       - TO_CustomInit is called before table and pipe init so that  
*         criticalMID array can be set, as well as route, pipe and CF Channel 
*         config.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns
*   \retcode #CFE_SUCCESS \retdesc  \copydoc CFE_SUCCESS \endcode
*   \retcode #TO_ERROR \retdesc Initialization Error \endcode
*
*   \see 
*       #TO_InitEvent
*       #TO_InitData
*       #TO_InitTable
*       #TO_InitPipe
*       #TO_CustomInit
*******************************************************************************/
int32  TO_AppInit(void);

/******************************************************************************/
/** \brief Initialize the Event Filter Table.
*
*   \par Description/Algorithm
*        1. Set the EventTbl EventIds based on ids defined in to_events.h
*        2. Register the events with cFE Table services.
*
*   \par Assumptions, External Events, and Notes:
*        - All Events are intialized as unfiltered.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns
*   \retcode #CFE_SUCCESS \retdesc \copydoc CFE_SUCCESS \endcode
*   \retstmt Any of the error codes from #CFE_EVS_Register    \endstmt
*   \endreturns
*
*   \see 
*       #TO_AppInit
*******************************************************************************/
int32  TO_InitEvent(void);

/******************************************************************************/
/** \brief Initialize Data
*
*   \par Description/Algorithm
*        Initialize the Housekeeping Packet, the OutData Packet, pipes and
*        route parameters.
*
*   \par Assumptions, External Events, and Notes:
*       - All Routes are initialized as non-existing and non-configured.
*       - All route telemetry pipes given TO_TLM_PIPE_DEPTH depth.
*       - Routes configuration can be overwritten in TO_CustomInit. 
*       - The default CF Channel is not associated with any route by default.
*       - If more than 1 CF Channel are used, their CfCntSemName must be set in
*         TO_CustomInit.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns
*   \retcode #CFE_SUCCESS \retdesc \copydoc CFE_SUCCESS \endcode
*
*   \see 
*       #TO_AppInit
*******************************************************************************/
int32  TO_InitData(void);

/******************************************************************************/
/** \brief Initialize Table
*
*   \par Description/Algorithm
*        Initialize the configuration through the following steps:
*        1. Registers table with CFE TBL services
*        2. Loads the table to CFE TBL services
*        3. Manages the table with CFE TBL services
*        4. Verifies Table load
*        5. Stores the table pointer
*        6. Registers with CFE TBL manager for notify message.
*
*   \par Assumptions, External Events, and Notes:
*       - The table is validated on Load through TO_ValidateTable
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns
*   \retcode #CFE_SUCCESS \retdesc \copydoc CFE_SUCCESS \endcode
*   \retstmt Any of the error codes from #CFE_TBL_Register    \endstmt
*   \retstmt Any of the error codes from #CFE_TBL_Load        \endstmt
*   \retstmt Any of the error codes from #CFE_TBL_Manage      \endstmt
*   \retstmt Any of the error codes from #CFE_TBL_GetAddress  \endstmt
*   \retstmt Any of the error codes from #CFE_TBL_NotifyByMessage  \endstmt
*
*   \see 
*       #TO_AppInit
*******************************************************************************/
int32  TO_InitTable(void);

/******************************************************************************/
/** \brief Initialize the Pipes
*
*   \par Description/Algorithm
*        Initialize the Scheduler, Tlm and Cmd Pipes. 
*        - The Scheduler pipe is subscribed to the WAKEUP message.
*        - The Command pipe is subscribed to TO_APP_CMD_MID and TO_SEND_HK.
*        - Each enabled route will create a corresponding telemetry pipe.
*        - A counting semaphore for CF app throttling is created for each 
*          CF channel referenced by a route. Each counting semaphore 
*          created is initialized to the pipe depth of the corresponding
*          route telemetry pipe.
*        - Each telemetry pipe is subscribed to all message IDs included in the
*          configuration table for the corresponding route.
*
*   \par Assumptions, External Events, and Notes:
*        - The CfCntSemName of each CF Channel used must be distinct. Set in 
*          TO_CustomInit.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns
*   \retcode #CFE_SUCCESS \retdesc \copydoc CFE_SUCCESS \endcode
*   \retstmt Any of the error codes from #CFE_SB_Pipe    \endstmt
*   \endreturns
*
*   \see 
*       #TO_AppInit
*******************************************************************************/
int32  TO_InitPipe(void);

/******************************************************************************/
/** \brief Validate Table Load
*
*   \par Description/Algorithm
*       Validate the table load by verifying the following:
*       - All Message IDs should be unique (no repeating MIDs)
*       - All used table entries are sequential (no gaps)
*       - All criticalMID message IDs are present in table.
*
*   \par Assumptions, External Events, and Notes:
*       - An Event is issued on any validation error.
*       - This function is called by cFE TBL services on a new table load.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns
*   \retcode #CFE_SUCCESS \retdesc \copydoc CFE_SUCCESS \endcode
*   \retcode #TO_ERROR \retdesc Validation Failed \endcode
*   \endreturns
*
*   \see 
*       #TO_InitTable
*******************************************************************************/
int32  TO_ValidateTable(void *);

/******************************************************************************/
/** \brief Receive Messages from Software Bus
*
*   \par Description/Algorithm
*       - Pend on the SchPipe for TO_WAKEUP_MID. 
*       - On wakeup, call TO_ProcessNewCmds, TO_ProcessNewData and 
*         TO_SendOutData.
*       - On timed-out, perform all three functions as on wakeup.
*
*   \par Assumptions, External Events, and Notes:
*       - Timeout based on uiWakeupTimeout, with default of TO_WAKEUP_TIMEOUT 
*       - Quit app on error status.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] iBlocking blocking timeout (set to CFE_SB_PEND_FOREVER)
*
*   \returns
*   \retcode #CFE_SUCCESS \retdesc \copydoc CFE_SUCCESS \endcode
*   \retstmt Any of the error codes from #CFE_SB_RcvMsg \endstmt
*
*   \see 
*       #TO_AppMain
*******************************************************************************/
int32  TO_RcvMsg(int32 iBlocking);

/******************************************************************************/
/** \brief ProcessTlmPipes 
*
*   \par Description/Algorithm
*       - Loop over every created tlm pipe (one per existing TO Route) 
*         and call ProccessNewData on that pipe based on route's wake period.  
*       - Increment wakeup count and force rollover if TO_MAX_WAKEUP_COUNT is
*         reached.
*
*   \par Assumptions, External Events, and Notes:
*       - This function is called in response to the TO_WAKEUP_MID or timeout 
*         of SchPipe.
*       - Messages will be processed based on the routePeriod parameter of 
*         each route. Process if wakeupCnt % route->wakePeriod == 0.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns None
*
*   \see 
*       #TO_RcvMsg
*       #TO_ProcessNewData
*******************************************************************************/
void   TO_ProcessTlmPipes(void);

/******************************************************************************/
/** \brief Process New Data over route telemetry pipe
*
*   \par Description/Algorithm
*       - Retrieve all new messages over telemetry pipe for specific route.
*       - If the message received is enabled according to the config table, Call 
*         TO_CustomProcessData for message. 
*       - If the route is associated with a CF Channel, manage the counting 
*         semaphore count for CF App throttling.
*       - Optionally, call TO_CustomFrameStart and TO_CustomFrameSend before 
*         and after processing all pipe messages if TO_FRAMING_ENABLED is 
*         defined.
*
*   \par Assumptions, External Events, and Notes:
*       - Messages are dropped from pipe if either usOutputEnabled or 
*         usOutputActive is false.
*       - If framing is enabled, messages should be sent over route device
*         (eg. UDP) in TO_CustomFrameSend. TO_CustomProcessData should be used
*         to add messages to frame. TO_CustomFrameStart should prepare 
*         new frame.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in]     pTlmPipe Pointer to current telemetry pipe structure.
*   \param[in]     routeId Index of current route.
*
*   \returns None
*
*   \see 
*       #TO_ProcessTlmPipes
*       #TO_CustomProcessData
*******************************************************************************/
void   TO_ProcessNewData(TO_TlmPipe_t *pTlmPipe, uint16 usRouteId);

/******************************************************************************/
/** \brief Process New Commands
*
*   \par Description/Algorithm
*        - Loop over Cmd Pipe for any new messages. 
*        - Call TO_ProcessNewAppCmds on the receipt of the TO_APP_CMD_MID. 
*        - Call TO_ReportHousekeeping on receipt of TO_SEND_HK_MID.
*        - Error otherwise. 
*
*   \par Assumptions, External Events, and Notes:
*       - This function is called in response to the TO_WAKEUP_MID or timeout on
*         SchPipe.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns None
*
*   \see 
*       #TO_RcvMsg
*       #TO_ProcessNewAppCmds
*       #TO_ReportHousekeeping
*******************************************************************************/
void   TO_ProcessNewCmds(void);

/******************************************************************************/
/** \brief Process New Application Commands
*
*   \par Description/Algorithm
*       Perform appropriate response based on the received command code.
*
*   \par Assumptions, External Events, and Notes:
*       - This function is called by TO_ProcessNewCmds on receipt of new cmd.
*       - The command length of each command is verified
*       - Any other command codes is considered custom, and processed by
*         TO_CustomAppCmds.
*       - Command message structures and function declaration are defined in 
*         to_cmds.h
*       - Command response functions are defined in to_cmds.c
*       - Command Codes are defined in to_msgdefs.h
*       - Custom command messages are defined in {MISSION}_to_types.h
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns None
*
*   \see 
*       #TO_ProcessNewCmds
*       #TO_CustomAppCmds
*       #ALL_TO_CMDS
*******************************************************************************/
void   TO_ProcessNewAppCmds(CFE_SB_MsgPtr_t);

/******************************************************************************/
/** \brief Report Housekeeping Packet
*
*   \par Description/Algorithm
*       Send the housekeeping packet to the software bus.
*
*   \par Assumptions, External Events, and Notes:
*       - This function is called in response to the TO_SEND_HK_MID on
*         CmdPipe by TO_ProcessNewCmds.
*       - The default HK packet is defined in to_hktlm.h.
*       - Default HK packet may be included in {MISSION}_to_types.h or 
*         redefined therein.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns None
*
*   \see 
*       #TO_ProcessNewCmds
*******************************************************************************/
void   TO_ReportHousekeeping(void);

/******************************************************************************/
/** \brief Send the OutData Packet
*
*   \par Description/Algorithm
*       Send the OutData packet to the software bus.
*
*   \par Assumptions, External Events, and Notes:
*       - This function is called in response to the TO_WAKEUP_MID or timeout of
*         schPipe.
*       - The OutData packet is defined in {MISSION}_to_types.h.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns None
*
*   \see 
*       #TO_RcvMsg
*******************************************************************************/
void   TO_SendOutData(void);

/******************************************************************************/
/** \brief Perform cleanup on shutdown
*
*   \par Call the custom cleanup function to close I/O channels, etc. 
*
*   \par Assumptions, External Events, and Notes:
*       - TO_CustomCleanup is defined in to_custom.c
*       - This function gets called on CFE_ES_ExitApp from TO_AppMain.
*
*   \param None
*
*   \returns None
*
*   \see 
*       #TO_AppMain
*       #TO_AppInit
*       #TO_CustomCleanup
*******************************************************************************/
void   TO_CleanupCallback(void);


/*******************************************************************************
** Utility Function Declarations (defined in to_utils.c)
*******************************************************************************/

/******************************************************************************/
/** \brief Find Empty Table Index.
*
*   \par Description/Algorithm
*       Return the table index of the first entry that is unused.
*
*   \par Assumptions, External Events, and Notes:
*       - An entry that was removed by TO_REMOVE_TBL_ENTRY is considired empty.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns tableIdx index number of empty entry 
*   \retcode #TO_TBL_FULL_ERR \retdesc No empty entries - tbl full. \endcode
*   \endreturns
*
*   \see 
*       #TO_AddTblEntryCmd
*******************************************************************************/
int32   TO_FindEmptyTableIndex(void);

/******************************************************************************/
/** \brief Find Table Index by MID.
*
*   \par Description/Algorithm
*       Return the table entry corresponding to the MID.
*
*   \par Assumptions, External Events, and Notes:
*       - Will return TO_NO_MATCH if no table entry found.
*
*   \param[in] pTable Pointer to the table to search.
*   \param[in] usMsgId Message Id to find.
*
*   \returns tableIdx Table index number of MID 
*   \retcode #TO_NO_MATCH \retdesc No matching entry w/ usMsgId \endcode
*   \endreturns
*
*   \see 
*       #TO_AddTblEntryCmd
*       #TO_RemoveTblEntryCmd
*       #TO_EnableTblEntryCmd
*       #TO_DisableTblEntryCmd
*       #TO_SetRouteByMidCmd
*       #TO_ManageTableCmd
*******************************************************************************/
int32   TO_FindTableIndex(TO_ConfigTable_t* pTable, CFE_SB_MsgId_t usMsgId);

/******************************************************************************/
/** \brief Set state (Enable/Disable) based on Group.
*
*   \par Description/Algorithm
*       Set the state of the table entries to disable / enable based on the
*       supplied groupData.
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in] groupData GroupData paramater to evaluate.
*   \param[in] enableFlag enable = 1, disable = 0
*
*   \returns
*   \retcode #TO_SUCCESS \retdesc Succesful change \endcode
*   \retcode #TO_BAD_ARG_ERR \retdesc Bad input argument \endcode
*   \retcode #TO_NO_MATCH \retdesc No matching entry w/ groupData \endcode
*   \retcode #TO_NO_EFFECT \retdesc function had no effect \endcode
*   \endreturns
*
*   \see 
*       #TO_EnableGroupCmd
*       #TO_DisableGroupCmd
*******************************************************************************/
int32   TO_SetStateByGroup(uint32 groupData, uint16 enableFlag);

/******************************************************************************/
/** \brief Set routeMask based on Group.
*
*   \par Description/Algorithm
*       Set the routeMask of the table entries based on the supplied groupData.
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in] groupData GroupData paramater to evaluate.
*   \param[in] routeMask Route Mask to set entry with
*
*   \returns
*   \retcode #TO_SUCCESS \retdesc Succesful change \endcode
*   \retcode #TO_BAD_ARG_ERR \retdesc Bad input argument \endcode
*   \retcode #TO_NO_MATCH \retdesc No matching entry w/ groupData \endcode
*   \retcode #TO_NO_EFFECT \retdesc function had no effect \endcode
*   \endreturns
*
*   \see 
*       #TO_EnableGroupCmd
*       #TO_DisableGroupCmd
*******************************************************************************/
int32   TO_SetRouteByGroup(uint32 uiGroupData, uint16 usRouteMask);

/******************************************************************************/
/** \brief Set State for all used table entries
*
*   \par Description/Algorithm
*       Set the state for all table entries (enable /disable)
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in] usEnableFlag Enable = 1, Disable = 0
*
*   \returns 
*   \retcode #TO_SUCCESS \retdesc Succesful change \endcode
*   \retcode #TO_NO_EFFECT \retdesc function had no effect \endcode
*   \endreturns
*
*   \see 
*       #TO_EnableAllTblEntriesCmd
*       #TO_DisableAllTblEntriesCmd
*******************************************************************************/
int32    TO_SetAllEntryState(uint16 usEnableFlag);

/******************************************************************************/
/** \brief Set Route as configured
*
*   \par Description/Algorithm
*       Set route as configured and updates HK->usConfigRoutes.
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in] uRouteId RouteId that was configured
*
*   \returns None 
*******************************************************************************/
void TO_SetRouteAsConfigured(uint16 usRouteId);

/******************************************************************************/
/** \brief Set Route as unconfigured
*
*   \par Description/Algorithm
*       Set route as unconfigured and updates HK->usConfigRoutes.
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in] uRouteId RouteId That was unconfigured
*
*   \returns None 
*******************************************************************************/
void TO_SetRouteAsUnconfigured(uint16 usRouteId);

/******************************************************************************/
/** \brief Disable specific route
*
*   \par Description/Algorithm
*       Disables and update housekeeping usEnabledRoutes parameter.
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in] uRouteId RouteId To disable
*
*   \returns None 
*******************************************************************************/
void TO_DisableRoute(uint16 usRouteId);

/******************************************************************************/
/** \brief Validate RouteMask against existing routes
*
*   \par Description/Algorithm
*       Verify that all routes within the route mask is an existing route.
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in] uRouteMask RouteMask to validate
*
*   \returns 
*   \retcode #TO_SUCCESS \retdesc RouteMask is valid \endcode
*   \retcode #TO_ERROR \retdesc RouteMask is invalid \endcode
*   \endreturns
*
*   \see 
*       #TO_SetRoutePeriodCmd
*       #TO_CustomEnableOutputCmd
*******************************************************************************/
int32 TO_ValidateRouteMask(uint16 usRouteMask);

/******************************************************************************/
/** \brief Get route mask by table index
*
*   \par Description/Algorithm
*       Return the RouteMask based on the supplied table index
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in] iTblIndex Table Index
*
*   \returns Route Mask or 0x0000 if bad input table index.
*******************************************************************************/
uint16 TO_GetRouteMask(int32 tblIdx);

/******************************************************************************/
/** \brief Get message ID by table index
*
*   \par Description/Algorithm
*       Return the Message ID based on the supplied table index
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in] iTblIndex Table Index
*
*   \returns MID or 0 if bad input table index.
*******************************************************************************/
CFE_SB_MsgId_t TO_GetMessageID(int32 tblIdx);

/******************************************************************************/
/** \brief Verify the command length against expected length
*
*   \par Description/Algorithm
*       Get the message length through the SB API and compare it with the passed
*       in expected length.  If not equal, issue an error event, increment the
*       uiCmdErrCnt and return false.
*
*   \par Assumptions, External Events, and Notes:
*       - Call this function for all received commands to verify the length.
*       - Also to be used in the custom layer.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pMsg Pointer to the CCSDS message.
*   \param[in] expectedLen Expected Command message length.
*
*   \returns True or False
*
*   \see 
*       #ALL_TO_CMDS
*       #TO_CustomAppCmds
*******************************************************************************/
boolean TO_VerifyCmdLength(CFE_SB_MsgPtr_t, uint16);

/******************************************************************************/
/** \brief Subscribe route pipes to all config table messages
*
*   \par Description/Algorithm
*       All pipes of existing routes will subscribe to all messages
*       According to configuration table.
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*
*   \returns
*   \retcode #TO_SUCCESS \retdesc Succesful \endcode
*   \retcode #TO_ERRROR \retdesc Error occurred in CFE_SB_SubscribeEx \endcode 
*   \endreturns
*
*   \see 
*       #TO_InitPipe
*       #TO_SubscribeMsg
*******************************************************************************/
int32 TO_SubscribeAllMsgs(void);

/******************************************************************************/
/** \brief Subscribe route pipes to specified message
*
*   \par Description/Algorithm
*       All pipes corresponding to enabled routes will subscribe to message
*       according to configuration table.
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pEntry The Table entry of the message ID to subscribe 
*
*   \returns
*   \retcode #TO_SUCCESS \retdesc Succesful \endcode
*   \retcode #TO_BAD_ARG_ERR \retdesc Bad input argument \endcode 
*   \retcode #TO_ERRROR \retdesc Error occurred in CFE_SB_SubscribeEx \endcode 
*   \endreturns
*
*   \see 
*       #TO_AddTblEntryCmd
*       #TO_ManageTableCmd
*       #TO_SubscribeAllMsgs
*******************************************************************************/
int32 TO_SubscribeMsg(TO_TableEntry_t *pEntry);

/******************************************************************************/
/** \brief Unsubscribe route pipes to all config table messages
*
*   \par Description/Algorithm
*       All pipes corresponding to existing routes will unsubscribe to all 
*       messages according to configuration table.
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in]  pConfigTable  The Config Table to use
*
*   \returns
*   \retcode #TO_SUCCESS \retdesc Succesful \endcode
*   \retcode #TO_BAD_ARG_ERR \retdesc Bad input argument \endcode 
*   \retcode #TO_ERRROR \retdesc Error occurred in CFE_SB_Unsubscribe \endcode 
*   \endreturns
*
*   \see 
*       #TO_UnsubscribeMsg
*******************************************************************************/
int32 TO_UnsubscribeAllMsgs(TO_ConfigTable_t *pConfigTable);

/******************************************************************************/
/** \brief Unsubscribe route pipes to specified message
*
*   \par Description/Algorithm
*       All pipes corresponding to enabled routes will unsubscribe to message
*       according to configuration table.
*
*   \par Assumptions, External Events, and Notes:
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in] pEntry The Table entry of the message ID to unsubscribe 
*
*   \returns
*   \retcode #TO_SUCCESS \retdesc Succesful \endcode
*   \retcode #TO_BAD_ARG_ERR \retdesc Bad input argument \endcode 
*   \retcode #TO_ERRROR \retdesc Error occurred in CFE_SB_Unsubscribe \endcode 
*   \endreturns
*
*   \see 
*       #TO_RemoveTblEntryCmd
*       #TO_ManageTableCmd
*       #TO_UnsubscribeAllMsgs
*******************************************************************************/
int32 TO_UnsubscribeMsg(TO_TableEntry_t *pEntry);


/*******************************************************************************
** Required Custom Functions (defined in to_custom.c)
*******************************************************************************/

/******************************************************************************/
/** \brief Custom Initialization
*
*   \par Description/Algorithm
*       This function is mainly responsible to initialize the transport
*       protocol(s). It also configures the criticalMid array, routes, pipe
*       names and CF channel semaphore names.
*
*   \par Assumptions, External Events, and Notes:
*       - Configuration macros defined in to_platform_cfg.h should be used when
*         configuring the transport protocol(s).
*       - The criticalMid array is used to ensure that these MID are always
*         present in the table.
*       - Any existing route in implementation must has isExist set to 1.
*       - Any CF channels used should have it's cfCntSemName set, and have
*         the corresponding route sCfChnlIdx set.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in,out] g_TO_CustomData TO Global Custom Data
*
*   \returns 
*   \retcode #TO_SUCCESS \retdesc Succesful \endcode
*   \retcode #TO_BAD_ARG_ERR \retdesc Bad input argument \endcode 
*   \retcode #TO_ERROR \retdesc Initialization Error \endcode
*
*   \see 
*       #TO_AppInit
*       #TO_ValidateTable
*******************************************************************************/
int32   TO_CustomInit(void);

/******************************************************************************/
/** \brief Process of custom app commands 
*
*   \par Description/Algorithm
*       This function is responsible to process any custom app commands. Make
*       use of same pattern as in TO_ProcessNewAppCmds, with a switch case on
*       the received command code of the message. This function is called on 
*       any user defined non-generic commands from TO_ProcessNewAppCmds. Return
*       TO_ERROR if the command code is not recognized.
*
*   \par Assumptions, External Events, and Notes:
*       - This function is called in response to the TO_APP_CMD_MID, called 
*       from the TO_ProcessNewAppCmds function, on a custom command code.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in,out] g_TO_CustomData TO Global Custom Data
*   \param[in] pCmdMsg The pointer to the (CCSDS) app command message.
*
*   \returns
*   \retcode #TO_SUCCESS \retdesc Success \endcode
*   \retcode #TO_ERROR \retdesc Bad Command code \endcode
*
*   \see 
*       #TO_ProcessNewAppCmds
*       #TO_VerifyCmdLength
*       #CFE_SB_GetCmdCode
*******************************************************************************/
int32   TO_CustomAppCmds(CFE_SB_MsgPtr_t pCmdMsg);


/******************************************************************************/
/** \brief Start a new frame 
*
*   \par Description/Algorithm
*       This function may be used to prepare / start a new frame before 
*       adding individual packets. 
*
*   \par Assumptions, External Events, and Notes:
*       - This function is only useful if multi-packet framing is performed
*       - If an error occurs, function is responsible for issuing event.
*       - Refer to the multi_tf example for an example using TM_SDLP library.
*       - Method only required if TO_FRAMING_ENABLED macro is defined
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in,out] g_TO_CustomData TO Global Custom Data
*   \param[in] usRouteId The current Route ID to use
*
*   \returns
*   \retcode #TO_SUCCESS \retdesc Success \endcode
*   \retcode #TO_ERROR \retdesc Error occured \endcode
*
*   \see 
*       #TO_ProcessNewData
*******************************************************************************/
int32   TO_CustomFrameStart(uint16 usRouteId);


/******************************************************************************/
/** \brief Complete and send frame 
*
*   \par Description/Algorithm
*       This function may be used to complete a frame and send over I/O device. 
*       Specific implementation is user specific, but may include operations 
*       such as filling frame with empty data, setting frame headers before 
*       sending, and sending over appropriate transport device.
*
*   \par Assumptions, External Events, and Notes:
*       - This function is only useful if multi-packet framing is performed
*       - If an error occurs, function is responsible for issuing event.
*       - Refer to the multi_tf example for an example using TM_SDLP library.
*       - Method only required if TO_FRAMING_ENABLED macro is defined.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in,out] g_TO_CustomData TO Global Custom Data
*   \param[in] usRouteId The current Route ID to use
*   \param[in] iInStatus Status from previous call 
*
*   \returns
*   \retcode #TO_SUCCESS \retdesc Success \endcode
*   \retcode #TO_ERROR \retdesc Error occured \endcode
*
*   \see 
*       #TO_ProcessNewData
*******************************************************************************/
int32   TO_CustomFrameSend(uint16 usRouteId, int32 iInStatus);


/******************************************************************************/
/** \brief Custom Process Data
*
*   \par Description/Algorithm
*       This function is responsible to process all outgoing telemetry
*       receveived over the telemetry pipe. The custom implementation should
*       follow this general pattern:
*       1. Look at the table entry routeMask and perform what follows according
*          to the route.
*       2. Perform any format change for outgoing telemetry. Use local buffer if
*          required.
*       3. Call any I/O data link protocol services for packet framing
*       4. Send message over appropriate transport protocol (if applicable)
*
*   \par Assumptions, External Events, and Notes:
*       - This function is called by TO_ProcessNewData.
*       - The state (enabled/disable) of the table entry is verified as enabled
*         in TO_ProcessNewData.
*       - Make use of the routeMask as a bit Mask (eg. routeMask & 0x01)
*       - Refer to IO_LIB for available protocols.
*       - If Framing is enabled, this function is responsible for adding
*         individual CFS packets to the frame, before the full frame is sent
*         over TO_CustomFrameSend.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in,out] g_TO_CustomData TO Global Custom Data
*   \param[in] pCmdMsg   The pointer to the (CCSDS) app command message.
*   \param[in] size      The size of the message
*   \param[in] tblIdx    The table index associated with this message
*   \param[in] usRouteId The current Route ID to use
*
*   \returns
*   \retcode #TO_SUCCESS \retdesc Success \endcode
*   \retcode #TO_ERROR \retdesc Error on send \endcode
*
*   \see 
*       #TO_ProcessNewData
*       #TO_CustomFrameSend
*******************************************************************************/
int32   TO_CustomProcessData(CFE_SB_MsgPtr_t pTlmMsg, int32 size, int32 tblIdx,
                             uint16 usRouteId);

/******************************************************************************/
/** \brief Custom Cleanup 
*
*   \par Description/Algorithm
*       This function will close any transport protocols (if required)
*       in response to exiting the application.  
*
*   \par Assumptions, External Events, and Notes:
*       - This function is called from TO_CleanupCallback.
*
*   \param[in,out] g_TO_CustomData TO Global Custom Data
*
*   \returns None 
*
*   \see 
*       #TO_AppMain
*       #TO_CleanupCallback
*******************************************************************************/
void    TO_CustomCleanup(void);

/******************************************************************************/
/** \brief Enable Output Command Response
*
*   \par Description/Algorithm
*       This function extends the TO_EnableOutputCmd function in order to 
*       perform any necessary transport protocol configurations to enable
*       specific routes based on input command. 
*       - The function should configure transport protocols based on 
*         routes to enable.
*       - Call TO_SetRouteAsConfigured for each route ID configured.
*       - Return the routeMask of newly configured routes.
*
*   \par Assumptions, External Events, and Notes:
*       - This function is called from TO_EnableOutputCmd
*       - TO_ValidateRouteMask may be called on input routeMask.
*       - TO_SetRouteAsConfigured must be called on newly configured routes.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in,out] g_TO_CustomData TO Global Custom Data
*   \param[in] pCmdMsg The pointer to the (CCSDS) command message.
*
*   \returns
*   A positive value is interpreted as route mask to enable
*   \retcode #TO_SUCCESS \retdesc Success \endcode
*   \retcode #TO_ERROR \retdesc Error Occured \endcode
*
*   \see 
*       #TO_EnableOutputCmd
*       #TO_CustomDisableOutputCmd
*       #TO_ValidateRouteMask
*       #TO_SetRouteAsConfigured
*******************************************************************************/
int32 TO_CustomEnableOutputCmd(CFE_SB_MsgPtr_t pCmdMsg);

/******************************************************************************/
/** \brief Disable Output Command Response
*
*   \par Description/Algorithm
*       This function extends the TO_DisableOutputCmd function in order to 
*       perform any necessary desired actions to disable output.
*       The function should set usOutputEnabled to 0 if all output is to be
*       disabled. 
*       - If command is used to disable specific routes, function may close
*         transport protocols and set routes as unconfigured with
*         TO_SetRouteAsUnconfigured. The route would then need to be
*         reconfigured through EnableOutputCmd. Otherwise it could
*         leave them as configured and return a routeMask to only disable them.
*       - Return a routeMask for routes that must be disabled.
*
*   \par Assumptions, External Events, and Notes:
*       - This function is called from TO_DisableOutputCmd
*       - Implementation may choose to ignore command, by returning TO_ERROR.
*         Issue appropriate event.
*
*   \param[in,out] g_TO_AppData TO Global Application Data
*   \param[in,out] g_TO_CustomData TO Global Custom Data
*   \param[in] pCmdMsg The pointer to the (CCSDS) command message.
*
*   \returns
*   A positive value is interpreted as route mask to disable 
*   \retcode #TO_SUCCESS \retdesc Success \endcode
*   \retcode #TO_ERROR \retdesc Error Occured \endcode
*
*   \see 
*       #TO_DisableOutputCmd
*       #TO_CustomEnabledOutputCmd
*       #TO_SetRouteAsUnconfigured
*******************************************************************************/
int32 TO_CustomDisableOutputCmd(CFE_SB_MsgPtr_t);

#ifdef __cplusplus
}
#endif

#endif /* _TO_APP_H_ */

/*==============================================================================
** End of file to_app.h
**============================================================================*/
