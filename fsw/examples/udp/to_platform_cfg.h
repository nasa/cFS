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
*   \brief Sample config file for TO Application with UDP device
*
*   \par Limitations, Assumptions, External Events, and Notes:
*       - Make use of the setup.sh script to move / link this file to the 
*       {MISSION_HOME}/apps/to/fsw/platform_inc folder.
*       - Overwrite any default settings by defining them here.
*       - Define any implementation specific settings. 
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
#define TO_NUM_CRITICAL_MIDS     3
#define TO_DEFAULT_DEST_PORT     5011 


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
    
