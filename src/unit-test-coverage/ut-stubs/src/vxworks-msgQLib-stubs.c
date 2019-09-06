/* OSAL coverage stub replacement for msgQLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/msgQLib.h>

static int LOCAL_UT_MSGQ;

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


