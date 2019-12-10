/* OSAL coverage stub replacement for msgQLib.h */
#ifndef _OSAL_STUB_MSGQLIB_H_
#define _OSAL_STUB_MSGQLIB_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>
#include <OCS_objLib.h>

/* ----------------------------------------- */
/* constants normally defined in msgQLib.h */
/* ----------------------------------------- */
#define OCS_MSG_Q_FIFO              0x0
#define OCS_MSG_Q_PRIORITY          0x1
#define OCS_WIND_MSG_Q_OPTION_MASK  (OCS_MSG_Q_FIFO|OCS_MSG_Q_PRIORITY)

#define OCS_MSG_PRI_NORMAL 0
#define OCS_MSG_PRI_URGENT 1


/* ----------------------------------------- */
/* types normally defined in msgQLib.h */
/* ----------------------------------------- */
typedef struct { int x; } OCS_MSG_Q;
typedef OCS_MSG_Q* OCS_MSG_Q_ID;

/* ----------------------------------------- */
/* prototypes normally declared in msgQLib.h */
/* ----------------------------------------- */

extern OCS_MSG_Q_ID OCS_msgQCreate(int maxMsgs, int maxMsgLength, int options);
extern OCS_STATUS OCS_msgQDelete(OCS_MSG_Q_ID msgQId);
extern int OCS_msgQReceive(OCS_MSG_Q_ID msgQId, char * buffer, OCS_UINT maxNBytes, int timeout);
extern OCS_STATUS OCS_msgQSend(OCS_MSG_Q_ID msgQId, char *buffer, OCS_UINT nBytes, int timeout, int priority);

#endif /* _OSAL_STUB_MSGQLIB_H_ */

