/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
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

