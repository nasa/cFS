/*=======================================================================================
** File Name:  comms_msgids.h
**
** Title:  Message ID Header File for COMMS Application
**
** $Author:    Paula Bosca
** $Revision: 1.1 $
** $Date:      2020-06-24
**
** Purpose:  This header file contains declartions and definitions of all COMMS's
**           Message IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-06-24 | Paula Bosca | Build #: Code Started
**
**=====================================================================================*/

#ifndef _COMMS_MSGIDS_H_
#define _COMMS_MSGIDS_H_

/**************************
** Comms Command Message IDs
***********************/

#define COMMS_CMD_MID            	0x1910 /**< \brief COMMS Ground Command Message ID **/
#define COMMS_SEND_HK_MID        	0x1911 /**< \brief COMMS Send HK Data cmd Message ID **/
#define COMMS_WAKEUP_MID        	0x1912 /**< \brief COMMS Wakeup cmd Message ID **/
#define COMMS_OUT_DATA_MID        0x1913 /**< \brief COMMS Output data cmd Message ID **/

/**************************
** Comms Telemetry Message IDs
***********************/
#define COMMS_HK_TLM_MID		0x0910 /**< \brief COMMS Hk Telemetry Message ID **/

#endif /* _COMMS_MSGIDS_H_ */

/*=======================================================================================
** End of file comms_msgids.h
**=====================================================================================*/
