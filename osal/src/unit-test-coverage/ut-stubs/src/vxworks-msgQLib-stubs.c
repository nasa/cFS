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


