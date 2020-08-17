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
#ifndef _OSAL_OVERRIDE_MQUEUE_H_
#define _OSAL_OVERRIDE_MQUEUE_H_

#include <OCS_mqueue.h>
#include <time.h>

/* ----------------------------------------- */
/* mappings for declarations in mqueue.h */
/* ----------------------------------------- */

#define mqd_t                OCS_mqd_t
#define mq_attr              OCS_mq_attr

#define mq_close             OCS_mq_close
#define mq_open              OCS_mq_open
#define mq_receive           OCS_mq_receive
#define mq_timedreceive      OCS_mq_timedreceive
#define mq_timedsend         OCS_mq_timedsend
#define mq_unlink            OCS_mq_unlink


#endif /* _OSAL_OVERRIDE_MQUEUE_H_ */
