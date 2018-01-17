/******************************************************************************/
/** \file  to_mission_cfg.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Mission Configuration Header File for TO Application
*
*   \par Limitations, Assumptions, External Events, and Notes:
*       - All Mission configuration files should be defined in apps/inc folder.
*
*   \par Modification History:
*     - 2015-01-09 | Guy de Carufel | Code Started
*     - 2016-05-11 | Allen Brown | Updated headers
*******************************************************************************/ 
#ifndef _TO_MISSION_CFG_H_
#define _TO_MISSION_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
** Pragmas
*/

/*
** Local Defines
*/

/*
** Include Files
*/
#include "cfe.h"

#include "to_perf_ids.h"
#include "to_msgids.h"
#include "to_msgdefs.h"


/* Define enable / disable commands. */ 
typedef struct
{
   uint8	CmdHeader[CFE_SB_CMD_HDR_SIZE];
   uint16   usRouteMask;                      /**< Route Mask to enable    */
} TO_EnableOutputCmd_t;


typedef struct
{
   uint8	CmdHeader[CFE_SB_CMD_HDR_SIZE];
   uint16   usRouteMask;                      /**< Route Mask to enable    */
} TO_DisableOutputCmd_t;


typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} TO_OutData_t;


typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint16  usCmdCnt;           /**< Count of all commands received           */
    uint16  usCmdErrCnt;        /**< Count of command errors                  */
    uint16  usMsgSubCnt;        /**< Count of subscribed messages by all 
                                     telemetry pipe.                          */
    uint16  usMsgSubErrCnt;     /**< Count of subscription errors             */
    uint16  usTblUpdateCnt;     /**< Count of table updates through CFE_TBL   */
    uint16  usTblErrCnt;        /**< Count of table update errors             */
    uint16  usConfigRoutes;     /**< Current mask of configured routes        */
    uint16  usEnabledRoutes;    /**< Current mask of enabled routes           */
    uint16  usPktCnt;           /**< Count of packet sent                     */
    uint16  usPktErrCnt;        /**< Count of packet processing errors        */
    uint16  usFrameErrCnt;      /**< Count of frame errors                    */
} TO_HkTlm_t;



/*
** Local Structure Declarations
*/

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

#ifdef __cplusplus
}
#endif

#endif /* _TO_MISSION_CFG_H_ */

/*==============================================================================
** End of file to_mission_cfg.h
**============================================================================*/
    
