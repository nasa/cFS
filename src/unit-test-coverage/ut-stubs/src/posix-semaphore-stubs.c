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
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_semaphore.h>

int OCS_sem_destroy (OCS_sem_t * sem)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sem_destroy);

    return Status;
}

int OCS_sem_getvalue (OCS_sem_t * sem, int * sval)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sem_getvalue);

    return Status;
}

int OCS_sem_init (OCS_sem_t * sem, int pshared, unsigned int value)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sem_init);

    return Status;
}

int OCS_sem_post (OCS_sem_t * sem)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sem_post);

    return Status;
}

int OCS_sem_timedwait (OCS_sem_t * sem, const struct OCS_timespec * abstime)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sem_timedwait);

    return Status;
}

int OCS_sem_wait (OCS_sem_t * sem)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_sem_wait);

    return Status;
}


