/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for semaphore.h */
#ifndef _OSAL_OVERRIDE_SEMAPHORE_H_
#define _OSAL_OVERRIDE_SEMAPHORE_H_

#include <OCS_semaphore.h>
#include <time.h>

/* ----------------------------------------- */
/* mappings for declarations in semaphore.h */
/* ----------------------------------------- */
#define sem_t           OCS_sem_t
#define sem_destroy     OCS_sem_destroy
#define sem_getvalue    OCS_sem_getvalue
#define sem_init        OCS_sem_init
#define sem_post        OCS_sem_post
#define sem_timedwait   OCS_sem_timedwait
#define sem_wait        OCS_sem_wait


#endif /* _OSAL_OVERRIDE_SEMAPHORE_H_ */
