/*=======================================================================================
** File Name:  comms_msg.h
**
** Title:  Message Definition Header File for COMMS Application
**
** $Author:    Paula Bosca
** $Revision: 1.1 $
** $Date:      2020-06-24
**
** Purpose:  To define COMMS's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-06-24 | Paula Bosca | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _COMMS_MSG_H_
#define _COMMS_MSG_H_

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
** COMMS command codes
*/
#define COMMS_NOOP_CC                 0
#define COMMS_RESET_CC                1

/*
** Local Structure Declarations
*/
typedef struct
{
    uint8              TlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint8              usCmdCnt;
    uint8              usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */

} COMMS_HkTlm_t;


#endif /* _COMMS_MSG_H_ */

/*=======================================================================================
** End of file comms_msg.h
**=====================================================================================*/
    