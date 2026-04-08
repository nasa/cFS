/*=======================================================================================
** File Name:  sbn_msgids.h
**
** Title:
**
** $Author:    Steve Duran
** $Revision:  $
** $Date:      2013-06-10
**
** Purpose:
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2013-06-10 | Steve Duran | Build #: Code Started
**
**=====================================================================================*/

#ifndef _SBN_MSGIDS_H_
#define _SBN_MSGIDS_H_

/*
** Include Files
*/

#include "global_core_api_base_msgid_values.h" /* defines the base MIDs for each CPU */
#include "global_sbn_topicids.h" /* defines the base MIDs for each CPU */

/*
** Pragmas
*/

/*
** Local Defines
*/
#define SBN_CMD_MID (SAMPLE_CPU2_CMD_MID_BASE + SBN_CMD_TOPICID)

#define SBN_HK_TLM_MID          (SAMPLE_CPU2_TLM_MID_BASE + SBN_HK_TLM_TOPICID)
#define SBN_HKNET_TLM_MID       (SAMPLE_CPU2_TLM_MID_BASE + SBN_HKNET_TLM_TOPICID)
#define SBN_HKPEER_TLM_MID      (SAMPLE_CPU2_TLM_MID_BASE + SBN_HKPEER_TLM_TOPICID)
#define SBN_HKMYSUBS_TLM_MID    (SAMPLE_CPU2_TLM_MID_BASE + SBN_HKMYSUBS_TLM_TOPICID)
#define SBN_HKPEERSUBS_TLM_MID  (SAMPLE_CPU2_TLM_MID_BASE + SBN_HKPEERSUBS_TLM_TOPICID)

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

#endif /* _SBN_MSGIDS_H_ */

/*=======================================================================================
** End of file sbn_msgids.h
**=====================================================================================*/
