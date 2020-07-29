/******************************************************************************/
/** \file  to_hktlm.h
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
*   \author Guy de Carufel (Odyssey Space Research), NASA, JSC, ER6
*
*   \brief Default HK Telemetry
*
*   \par
*       This header contains the definition of the default HK Telemetry.
*
*   \par Limitations, Assumptions, External Events, and Notes:
*     - Include this file in your MISSION_to_types.hh or define your own.
*     - If a custom HK tlm is required, make sure to include all parameters
*       in this default HK packet in your custom implementation.
*     - The usMsgSubCnt is not reset on a TO_RESET_CC command.
*
*   \par Modification History:
*     - 2015-09-22 | Guy de Carufel | Code Started
*******************************************************************************/
#ifndef _TO_HKTLM_H_
#define _TO_HKTLM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cfe.h"

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
} TO_HkTlm_t;

#ifdef __cplusplus
}
#endif

#endif /* _TO_HKTLM_H_ */

/*==============================================================================
** End of file to_hktlm.h
**============================================================================*/
