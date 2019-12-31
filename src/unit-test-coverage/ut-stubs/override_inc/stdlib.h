/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for stdlib.h */
#ifndef _OSAL_OVERRIDE_STDLIB_H_
#define _OSAL_OVERRIDE_STDLIB_H_

#include <OCS_stdlib.h>

/* ----------------------------------------- */
/* mappings for declarations in stdlib.h */
/* ----------------------------------------- */

#define EXIT_SUCCESS   OCS_EXIT_SUCCESS
#define EXIT_FAILURE   OCS_EXIT_FAILURE
#define exit           OCS_exit
#define strtoul        OCS_strtoul
#define system         OCS_system
#define malloc         OCS_malloc
#define free           OCS_free


#endif /* _OSAL_OVERRIDE_STDLIB_H_ */
