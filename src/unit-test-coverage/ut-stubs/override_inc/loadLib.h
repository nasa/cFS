/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for loadLib.h */
#ifndef _OSAL_OVERRIDE_LOADLIB_H_
#define _OSAL_OVERRIDE_LOADLIB_H_

#include <OCS_loadLib.h>
#include <vxWorks.h>
#include <moduleLib.h>

/* ----------------------------------------- */
/* mappings for declarations in loadLib.h */
/* ----------------------------------------- */

#define loadModule  OCS_loadModule


#endif /* _OSAL_OVERRIDE_LOADLIB_H_ */
