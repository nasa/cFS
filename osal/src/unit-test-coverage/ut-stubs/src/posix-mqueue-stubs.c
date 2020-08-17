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

