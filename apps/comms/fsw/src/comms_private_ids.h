/*=======================================================================================
** File Name:  comms_private_ids.h
**
** Title:  ID Header File for COMMS Application
**
** $Author:    Paula Bosca
** $Revision: 1.1 $
** $Date:      2020-06-24
**
** Purpose:  This header file contains declarations and definitions of COMMS's private IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-06-24 | Paula Bosca | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _COMMS_PRIVATE_IDS_H_
#define _COMMS_PRIVATE_IDS_H_

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
#define COMMS_RESERVED_EID  0

#define COMMS_INF_EID        1
#define COMMS_INIT_INF_EID   2
#define COMMS_ILOAD_INF_EID  3
#define COMMS_CDS_INF_EID    4
#define COMMS_CMD_INF_EID    5

#define COMMS_ERR_EID         51
#define COMMS_INIT_ERR_EID    52
#define COMMS_ILOAD_ERR_EID   53
#define COMMS_CDS_ERR_EID     54
#define COMMS_CMD_ERR_EID     55
#define COMMS_PIPE_ERR_EID    56
#define COMMS_MSGID_ERR_EID   57
#define COMMS_MSGLEN_ERR_EID  58

#define COMMS_EVT_CNT  14

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

#endif /* _COMMS_PRIVATE_IDS_H_ */

/*=======================================================================================
** End of file comms_private_ids.h
**=====================================================================================*/
    