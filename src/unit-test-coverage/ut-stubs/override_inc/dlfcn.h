/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for dlfcn.h */
#ifndef _OSAL_OVERRIDE_DLFCN_H_
#define _OSAL_OVERRIDE_DLFCN_H_

#include <OCS_dlfcn.h>

/* ----------------------------------------- */
/* mappings for declarations in dlfcn.h */
/* ----------------------------------------- */

#define dlclose                    OCS_dlclose
#define dlerror                    OCS_dlerror
#define dlopen                     OCS_dlopen
#define dlsym                      OCS_dlsym


#endif /* _OSAL_OVERRIDE_DLFCN_H_ */
