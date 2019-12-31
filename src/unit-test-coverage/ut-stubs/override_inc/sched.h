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
#ifndef _OSAL_OVERRIDE_SCHED_H_
#define _OSAL_OVERRIDE_SCHED_H_

#include <OCS_sched.h>

/* ----------------------------------------- */
/* mappings for declarations in sched.h */
/* ----------------------------------------- */

#define SCHED_FIFO              OCS_SCHED_FIFO
#define SCHED_RR                OCS_SCHED_RR

#define sched_param             OCS_sched_param

#define sched_get_priority_max  OCS_sched_get_priority_max
#define sched_get_priority_min  OCS_sched_get_priority_min


#endif /* _OSAL_OVERRIDE_SCHED_H_ */

