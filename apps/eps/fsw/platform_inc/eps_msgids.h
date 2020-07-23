/*=======================================================================================
** File Name:  eps_msgids.h
**
** Title:  Message ID Header File for EPS Application
**
** $Author:    Paula Bosca
** $Revision: 1.1 $
** $Date:      2020-06-24
**
** Purpose:  This header file contains declartions and definitions of all EPS's
**           Message IDs.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2020-06-24 | Paula Bosca | Build #: Code Started
**
**=====================================================================================*/

#ifndef _EPS_MSGIDS_H_
#define _EPS_MSGIDS_H_

/**************************
** EPS Command Message IDs
***********************/

#define EPS_CMD_MID            	0x1900 /**< \brief EPS Ground Command Message ID **/
#define EPS_SEND_HK_MID        	0x1901 /**< \brief EPS Send HK Data cmd Message ID **/
#define EPS_WAKEUP_MID        	0x1902 /**< \brief EPS Wakeup cmd Message ID **/
#define EPS_OUT_DATA_MID        0x1903 /**< \brief EPS Output data cmd Message ID **/

/**************************
** EPS Telemetry Message IDs
***********************/
#define EPS_HK_TLM_MID		0x0900 /**< \brief EPS Hk Telemetry Message ID **/


#endif /* _EPS_MSGIDS_H_ */

/*=======================================================================================
** End of file eps_msgids.h
**=====================================================================================*/
