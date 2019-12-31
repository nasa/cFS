/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for sched.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_sched.h>


int OCS_sched_get_priority_max(int policy)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OCS_sched_get_priority_max,9);

    return Status;
}


int OCS_sched_get_priority_min(int policy)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OCS_sched_get_priority_min,1);

    return Status;
}



