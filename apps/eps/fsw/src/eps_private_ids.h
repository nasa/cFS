/*=======================================================================================
** File Name:  eps_private_ids.h
**
** Title:  ID Header File for EPS Application
**
** $Author:    Paula Bosca
** $Revision: 1.1 $
** $Date:      2020-06-24
**
** Purpose:  This header file contains declarations and definitions of EPS's private IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-06-24 | Paula Bosca | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _EPS_PRIVATE_IDS_H_
#define _EPS_PRIVATE_IDS_H_

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
#define EPS_RESERVED_EID  0

#define EPS_INF_EID        1
#define EPS_INIT_INF_EID   2
#define EPS_ILOAD_INF_EID  3
#define EPS_CDS_INF_EID    4
#define EPS_CMD_INF_EID    5

#define EPS_ERR_EID         51
#define EPS_INIT_ERR_EID    52
#define EPS_ILOAD_ERR_EID   53
#define EPS_CDS_ERR_EID     54
#define EPS_CMD_ERR_EID     55
#define EPS_PIPE_ERR_EID    56
#define EPS_MSGID_ERR_EID   57
#define EPS_MSGLEN_ERR_EID  58

#define EPS_EVT_CNT  14

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

#endif /* _EPS_PRIVATE_IDS_H_ */

/*=======================================================================================
** End of file eps_private_ids.h
**=====================================================================================*/
    