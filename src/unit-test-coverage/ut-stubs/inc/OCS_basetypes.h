/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub basic data types */
#ifndef _OSAL_STUB_BASETYPES_H_
#define _OSAL_STUB_BASETYPES_H_

/*
 * NOTE: These header files are intentionally _not_ overridden
 * in the replacement/override header directory, so this should
 * pull in the actual (native system) version of these files.
 *
 * It is important to pull in these definitions first before any
 * potential re-mapping (#define) statements are done.
 */

#include <stddef.h>   /* for correct size_t and ptrdiff_t types */
#include <stdint.h>   /* for correct fixed-width integer types */
#include <limits.h>   /* for correct INT_MAX, etc. */
#include <stdbool.h>  /* for correct boolean semantics */


#endif /* _OSAL_STUB_BASETYPES_H_ */

