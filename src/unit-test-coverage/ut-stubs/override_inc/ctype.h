/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for ctype.h */
#ifndef _OSAL_OVERRIDE_CTYPE_H_
#define _OSAL_OVERRIDE_CTYPE_H_

#include <OCS_ctype.h>

/* ----------------------------------------- */
/* mappings for declarations in ctype.h */
/* ----------------------------------------- */

#define isgraph                  OCS_isgraph

#endif /* _OSAL_OVERRIDE_CTYPE_H_ */
