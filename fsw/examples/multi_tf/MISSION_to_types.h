/******************************************************************************/
/** \file  MISSION_to_types.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Command and telemetry data strucutres for TO application (Multi)
*
*   \par
*       This header file contains definitions of command and telemetry data
*       structures for TO applications for the Multi transport protocol example.
*
*   \par Limitations, Assumptions, External Events, and Notes:
*     - Make use of the setup.sh script to move / link this file to the
*     {MISSION_HOME}/apps/inc/ folder.
*     - Standard command messages are defined in to_cmds.h
*     - Default HK Telemetry structure is defined in to_hktlm.h
*
*   \par Modification History:
*     - 2015-06-02 | Guy de Carufel | Code Started
*     - 2015-09-22 | Guy de Carufel | Moved hktlm to to_hktlm.h
*******************************************************************************/
#ifndef _MISSION_TO_TYPES_H_
#define _MISSION_TO_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "cfe.h"
#include "../to/fsw/src/to_hktlm.h"

#include "cop1.h"

/*
** Defines
*/
#define TO_MAX_IP_STRING_SIZE 16

/* Define enable / disable commands */
typedef struct
{
   uint8	ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
   char     cDestIp[TO_MAX_IP_STRING_SIZE];   /**< Destination IP   */	
   uint16   usDestPort;                       /**< Destination PORT */ 
   int32    iFileDesc;       /**< File Descriptor of Port to use.   */
} TO_EnableOutputCmd_t;


typedef struct
{
   uint8	ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} TO_DisableOutputCmd_t;


typedef struct
{
    uint8	ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
    COP1_Clcw_t     clcw;  /**< COP-1 CLCW Data */
} TO_CustomSetOcfCmd_t;


/*************** Telemetry **************/
typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
} TO_InData_t;

typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} TO_OutData_t;


#ifdef __cplusplus
}
#endif

#endif /* _MISSION_TO_TYPES_H_ */

/*==============================================================================
** End of file MISSION_to_types.h
**============================================================================*/
