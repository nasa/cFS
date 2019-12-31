/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for stdarg.h */
#ifndef _OSAL_OVERRIDE_STDARG_H_
#define _OSAL_OVERRIDE_STDARG_H_

#include <OCS_stdarg.h>

/* ----------------------------------------- */
/* mappings for declarations in stdarg.h */
/* ----------------------------------------- */

#define va_list                 OCS_va_list
#define va_start(ap, last)      OCS_va_start(ap, last)
#define va_end(ap)              OCS_va_end(ap)


#endif /* _OSAL_OVERRIDE_STDARG_H_ */
