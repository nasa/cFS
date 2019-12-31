/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for stdio.h */
#ifndef _OSAL_OVERRIDE_STDIO_H_
#define _OSAL_OVERRIDE_STDIO_H_

#include <OCS_stdio.h>

/* ----------------------------------------- */
/* mappings for declarations in stdio.h */
/* ----------------------------------------- */

#define FILE           OCS_FILE
#define fclose         OCS_fclose
#define fgets          OCS_fgets
#define fopen          OCS_fopen
#define fputs          OCS_fputs
#define remove         OCS_remove
#define rename         OCS_rename
#define snprintf       OCS_snprintf
#define vsnprintf      OCS_vsnprintf
#define printf(...)    OCS_printf(__VA_ARGS__)
#define putchar        OCS_putchar

#define stdin          OCS_stdin
#define stdout         OCS_stdout
#define stderr         OCS_stderr


#endif /* _OSAL_OVERRIDE_STDIO_H_ */

