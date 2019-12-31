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
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_msgQLib.h>

static OCS_MSG_Q LOCAL_UT_MSGQ = { 0 };

OCS_MSG_Q_ID OCS_msgQCreate(int maxMsgs, int maxMsgLength, int options)
{
    int32 Status;
    OCS_MSG_Q_ID retval;

    Status = UT_DEFAULT_IMPL(OCS_msgQCreate);
    if (Status == 0)
    {
        retval = &LOCAL_UT_MSGQ;
    }
    else
    {
        retval = NULL;
    }

    return retval;
}
OCS_STATUS OCS_msgQDelete(OCS_MSG_Q_ID msgQId)
{
    return (UT_DEFAULT_IMPL(OCS_msgQDelete));
}
int OCS_msgQReceive(OCS_MSG_Q_ID msgQId, char * buffer, OCS_UINT maxNBytes, int timeout)
{
    return (UT_DEFAULT_IMPL(OCS_msgQReceive));
}
OCS_STATUS OCS_msgQSend(OCS_MSG_Q_ID msgQId, char *buffer, OCS_UINT nBytes, int timeout, int priority)
{
    return (UT_DEFAULT_IMPL(OCS_msgQSend));
}


