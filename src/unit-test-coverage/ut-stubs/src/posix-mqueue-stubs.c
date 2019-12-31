/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for mqueue.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_mqueue.h>

int OCS_mq_close (OCS_mqd_t mqdes)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_mq_close);

    return Status;
}

OCS_mqd_t OCS_mq_open (const char * name, int oflag, ...)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_mq_open);

    return Status;
}

OCS_ssize_t OCS_mq_receive (OCS_mqd_t mqdes, char * msg_ptr, size_t msg_len, unsigned int * msg_prio)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_mq_receive);

    return Status;
}

OCS_ssize_t OCS_mq_timedreceive (OCS_mqd_t mqdes, char * msg_ptr, size_t msg_len, unsigned int * msg_prio, const struct OCS_timespec * abs_timeout)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_mq_timedreceive);

    return Status;
}

int OCS_mq_timedsend (OCS_mqd_t mqdes, const char * msg_ptr, size_t msg_len, unsigned int msg_prio, const struct OCS_timespec * abs_timeout)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_mq_timedsend);

    return Status;
}

int OCS_mq_unlink (const char * name)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_mq_unlink);

    return Status;
}

