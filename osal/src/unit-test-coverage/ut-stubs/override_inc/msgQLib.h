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
#ifndef _OSAL_OVERRIDE_MSGQLIB_H_
#define _OSAL_OVERRIDE_MSGQLIB_H_

#include <OCS_msgQLib.h>
#include <vxWorks.h>
#include <objLib.h>

/* ----------------------------------------- */
/* mappings for declarations in msgQLib.h */
/* ----------------------------------------- */
#define MSG_Q_FIFO                 OCS_MSG_Q_FIFO
#define MSG_Q_PRIORITY             OCS_MSG_Q_PRIORITY
#define WIND_MSG_Q_OPTION_MASK     OCS_WIND_MSG_Q_OPTION_MASK
#define MSG_PRI_NORMAL             OCS_MSG_PRI_NORMAL
#define MSG_PRI_URGENT             OCS_MSG_PRI_URGENT

#define MSG_Q_ID                   OCS_MSG_Q_ID

#define msgQCreate                 OCS_msgQCreate
#define msgQDelete                 OCS_msgQDelete
#define msgQReceive                OCS_msgQReceive
#define msgQSend                   OCS_msgQSend

#endif /* _OSAL_OVERRIDE_MSGQLIB_H_ */

