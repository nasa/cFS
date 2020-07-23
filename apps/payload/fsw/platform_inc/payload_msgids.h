/*=======================================================================================
** File Name:  payload_msgids.h
**
** Title:  Message ID Header File for PAYLOAD Application
**
** $Author:    Paula Bosca
** $Revision: 1.1 $
** $Date:      2020-06-24
**
** Purpose:  This header file contains declartions and definitions of all PAYLOAD's
**           Message IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-06-24 | Paula Bosca | Build #: Code Started
**
**=====================================================================================*/

#ifndef _PAYLOAD_MSGIDS_H_
#define _PAYLOAD_MSGIDS_H_

/**************************
** Payload Command Message IDs
***********************/

#define PAYLOAD_CMD_MID            	0x1920 /**< \brief Payload Ground Command Message ID **/
#define PAYLOAD_SEND_HK_MID        	0x1921 /**< \brief Payload Send HK Data cmd Message ID **/
#define PAYLOAD_WAKEUP_MID        	0x1922 /**< \brief Payload Wakeup cmd Message ID **/
#define PAYLOAD_OUT_DATA_MID        0x1923 /**< \brief Payload Output data cmd Message ID **/

/**************************
** Payload Telemtry Message IDs
***********************/

#define PAYLOAD_HK_TLM_MID		0x0920 /**< \brief EPS Hk Telemetry Message ID **/

#endif /* _PAYLOAD_MSGIDS_H_ */

/*=======================================================================================
** End of file payload_msgids.h
**=====================================================================================*/
