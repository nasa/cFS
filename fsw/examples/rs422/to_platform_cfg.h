/******************************************************************************/
/** \file  to_platform_cfg.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Sample config file for TO Application with RS422 device
*
*   \par Limitations, Assumptions, External Events, and Notes:
*       - Make use of the setup.sh script to move / link this file to the 
*       {MISSION_HOME}/apps/to/fsw/platform_inc folder.
*
*   \par Modification History:
*     - 2015-01-09 | Guy de Carufel | Code Started
*******************************************************************************/
    
#ifndef _TO_PLATFORM_CFG_H_
#define _TO_PLATFORM_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
** Pragmas
*/

/*
** Local Defines
*/
#define TO_SCH_PIPE_DEPTH  10
#define TO_CMD_PIPE_DEPTH  10
#define TO_TLM_PIPE_DEPTH  10

#define TO_NUM_CRITICAL_MIDS   3

#define TO_MAX_TBL_ENTRIES    100
#define TO_WAKEUP_TIMEOUT     500

#define TO_CONFIG_TABLENAME "to_config"
#define TO_CONFIG_FILENAME "/cf/apps/to_config.tbl"

#define TO_GROUP_NUMBER_MASK    0xFF000000
#define TO_MULTI_GROUP_MASK     0x00FFFFFF

#define TO_CF_THROTTLE_SEM_NAME "CFTOSemId"

/*
** Include Files
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

/*
** Local Variables
*/

/*
** Local Function Prototypes
*/

#ifdef __cplusplus
}
#endif

#endif /* _TO_PLATFORM_CFG_H_ */

/*==============================================================================
** End of file to_platform_cfg.h
**============================================================================*/
    
