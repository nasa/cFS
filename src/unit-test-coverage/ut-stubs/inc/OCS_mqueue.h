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
#ifndef _OSAL_STUB_MQUEUE_H_
#define _OSAL_STUB_MQUEUE_H_

#include <OCS_basetypes.h>
#include <OCS_sys_types.h>
#include <OCS_time.h>

/* ----------------------------------------- */
/* constants normally defined in mqueue.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in mqueue.h */
/* ----------------------------------------- */

typedef int OCS_mqd_t;

struct OCS_mq_attr
{
    unsigned int mq_maxmsg;
    unsigned int mq_msgsize;
};

/* ----------------------------------------- */
/* prototypes normally declared in mqueue.h */
/* ----------------------------------------- */

extern int OCS_mq_close (OCS_mqd_t mqdes);
extern OCS_mqd_t OCS_mq_open (const char * name, int oflag, ...);
extern OCS_ssize_t OCS_mq_receive (OCS_mqd_t mqdes, char * msg_ptr, size_t msg_len, unsigned int * msg_prio);
extern OCS_ssize_t OCS_mq_timedreceive (OCS_mqd_t mqdes, char * msg_ptr, size_t msg_len, unsigned int * msg_prio, const struct OCS_timespec * abs_timeout);
extern int OCS_mq_timedsend (OCS_mqd_t mqdes, const char * msg_ptr, size_t msg_len, unsigned int msg_prio, const struct OCS_timespec * abs_timeout);
extern int OCS_mq_unlink (const char * name);


#endif /* _OSAL_STUB_MQUEUE_H_ */

