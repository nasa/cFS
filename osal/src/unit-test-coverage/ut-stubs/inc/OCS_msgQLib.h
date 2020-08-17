/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

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

