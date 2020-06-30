/*=======================================================================================
** File Name:  eps_msg.h
**
** Title:  Message Definition Header File for EPS Application
**
** $Author:    Paula Bosca
** $Revision: 1.1 $
** $Date:      2020-06-24
**
** Purpose:  To define EPS's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-06-24 | Paula Bosca | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _EPS_MSG_H_
#define _EPS_MSG_H_

/*
** Pragmas
*/

/*
** Include Files
*/



/*
** Local Defines
*/

/*
** EPS command codes
*/
#define EPS_NOOP_CC                 0
#define EPS_RESET_CC                1

/*
** Local Structure Declarations
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */

} EPS_HkTlm_t;


#endif /* _EPS_MSG_H_ */

/*=======================================================================================
** End of file eps_msg.h
**=====================================================================================*/
    