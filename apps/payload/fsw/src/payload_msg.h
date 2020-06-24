/*=======================================================================================
** File Name:  payload_msg.h
**
** Title:  Message Definition Header File for PAYLOAD Application
**
** $Author:    Paula Bosca
** $Revision: 1.1 $
** $Date:      2020-06-24
**
** Purpose:  To define PAYLOAD's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-06-24 | Paula Bosca | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _PAYLOAD_MSG_H_
#define _PAYLOAD_MSG_H_

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
** PAYLOAD command codes
*/
#define PAYLOAD_NOOP_CC                 0
#define PAYLOAD_RESET_CC                1

/*
** Local Structure Declarations
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */

} PAYLOAD_HkTlm_t;


#endif /* _PAYLOAD_MSG_H_ */

/*=======================================================================================
** End of file payload_msg.h
**=====================================================================================*/
    