/*=======================================================================================
** File Name:  payload_private_ids.h
**
** Title:  ID Header File for PAYLOAD Application
**
** $Author:    Paula Bosca
** $Revision: 1.1 $
** $Date:      2020-06-24
**
** Purpose:  This header file contains declarations and definitions of PAYLOAD's private IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-06-24 | Paula Bosca | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _PAYLOAD_PRIVATE_IDS_H_
#define _PAYLOAD_PRIVATE_IDS_H_

/*
** Pragmas
*/

/*
** Include Files
*/

/*
** Local Defines
*/

/* Event IDs */
#define PAYLOAD_RESERVED_EID  0

#define PAYLOAD_INF_EID        1
#define PAYLOAD_INIT_INF_EID   2
#define PAYLOAD_ILOAD_INF_EID  3
#define PAYLOAD_CDS_INF_EID    4
#define PAYLOAD_CMD_INF_EID    5

#define PAYLOAD_ERR_EID         51
#define PAYLOAD_INIT_ERR_EID    52
#define PAYLOAD_ILOAD_ERR_EID   53
#define PAYLOAD_CDS_ERR_EID     54
#define PAYLOAD_CMD_ERR_EID     55
#define PAYLOAD_PIPE_ERR_EID    56
#define PAYLOAD_MSGID_ERR_EID   57
#define PAYLOAD_MSGLEN_ERR_EID  58

#define PAYLOAD_EVT_CNT  14

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

#endif /* _PAYLOAD_PRIVATE_IDS_H_ */

/*=======================================================================================
** End of file payload_private_ids.h
**=====================================================================================*/
    