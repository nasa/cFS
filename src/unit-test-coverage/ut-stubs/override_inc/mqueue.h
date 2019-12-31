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
