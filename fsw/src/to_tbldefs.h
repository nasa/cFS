/******************************************************************************/
/** \file  to_tbldefs.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*  
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Define the configuration table for Telemetry Outputs
*
*   \par Limitations, Assumptions, External Events, and Notes:
*       Each table entry defines: 
*           1. The message id 
*           2. The qos and msgLength - Used for message subscription
*           3. The routeMask - For output message routing 
*           4. The groupData - To modify multiple entries with single command
*           5. The state - The state of the entry (enabled = 1 /disabled = 0)
*       A Valid Table:
*           1. No two entries with same MID
*           2. No gaps (all used entries are consecutive)
*           3. All g_TO_AppData.criticalMid are included in the table
*
*   \par Modification History:
*     - 2015-01-09 | Guy de Carufel | Code Started
*******************************************************************************/
#ifndef _TO_TBLDEFS_
#define _TO_TBLDEFS_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
** Includes
*******************************************************************************/
#include "cfe.h"
#include "to_platform_cfg.h"

/*******************************************************************************
** Macro Definitions
*******************************************************************************/
#ifdef MESSAGE_FORMAT_IS_CCSDS_VER_2
#define TO_UNUSED_ENTRY     ((uint32) 0x00FFFFFF+1)
#define TO_REMOVED_ENTRY    ((uint32) 0x00FFFFFF+2)
#else
#define TO_UNUSED_ENTRY     0
#define TO_REMOVED_ENTRY    1
#endif

/** \name Default Table Size */
/** \{ */
#ifndef TO_MAX_TBL_ENTRIES    
#define TO_MAX_TBL_ENTRIES      100
#endif
/** \} */

/*******************************************************************************
** Type Definitions
*******************************************************************************/
/** 
**  \brief TO Table Entry for message subscription and routing.
*/
typedef struct
{
      CFE_SB_MsgId_t      usMsgId;      /**< Message ID (must be unique)      */
      CFE_SB_Qos_t        qos;          /**< Quality of Service flag          */
      uint16              usMsgLimit;   /**< Max Num. of this Msgs in pipe    */
      uint16              usRouteMask;  /**< Bitwize Route Mask               */
      uint32              uiGroupData;  /**< Group data Mask                  */
      uint16              usFlag;       /**< Custom defined flag              */
      uint16              usState;      /**< Message ID is enabled = 1        */
} TO_TableEntry_t;

/** 
**  \brief TO Table definition 
*/
typedef struct
{
    TO_TableEntry_t     entries[TO_MAX_TBL_ENTRIES];
} TO_ConfigTable_t; 

#ifdef __cplusplus
}
#endif

#endif

/*==============================================================================
** End of file to_tbldefs.h
**============================================================================*/
