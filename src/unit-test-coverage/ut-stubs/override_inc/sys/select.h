/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for sys/select.h */
#ifndef _OSAL_OVERRIDE_SYS_SELECT_H_
#define _OSAL_OVERRIDE_SYS_SELECT_H_

#include <OCS_sys_select.h>

/* ----------------------------------------- */
/* constants normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* mappings for declarations in sys/select.h */
/* ----------------------------------------- */

#define fd_set  OCS_fd_set
#define select  OCS_select


#endif /* _OSAL_OVERRIDE_SYS_SELECT_H_ */
