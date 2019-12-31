/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for errno.h */
#ifndef _OSAL_OVERRIDE_ERRNO_H_
#define _OSAL_OVERRIDE_ERRNO_H_

#include <OCS_errno.h>

/* ----------------------------------------- */
/* mappings for declarations in errno.h */
/* ----------------------------------------- */
#define EINTR     OCS_EINTR
#define EAGAIN    OCS_EAGAIN
#define EINVAL    OCS_EINVAL
#define EMSGSIZE  OCS_EMSGSIZE
#define ETIMEDOUT OCS_ETIMEDOUT
#define ESPIPE    OCS_ESPIPE

#define errno       OCS_errno

#endif /* _OSAL_OVERRIDE_ERRNO_H_ */
