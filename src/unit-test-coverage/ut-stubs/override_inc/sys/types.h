/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for sys/types.h */
#ifndef _OSAL_OVERRIDE_SYS_TYPES_H_
#define _OSAL_OVERRIDE_SYS_TYPES_H_

#include <OCS_sys_types.h>

/* ----------------------------------------- */
/* mappings for declarations in sys/types.h */
/* ----------------------------------------- */
#define ssize_t           OCS_ssize_t
#define off_t             OCS_off_t
#define mode_t            OCS_mode_t
#define pid_t             OCS_pid_t
#define gid_t             OCS_gid_t
#define uid_t             OCS_uid_t

#endif /* _OSAL_OVERRIDE_SYS_TYPES_H_ */
