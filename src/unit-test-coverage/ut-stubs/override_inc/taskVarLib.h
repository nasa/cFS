/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for taskVarLib.h */
#ifndef _OSAL_OVERRIDE_TASKVARLIB_H_
#define _OSAL_OVERRIDE_TASKVARLIB_H_

#include <OCS_taskVarLib.h>
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in taskVarLib.h */
/* ----------------------------------------- */

#define taskVarAdd   OCS_taskVarAdd



#endif /* _OSAL_OVERRIDE_TASKVARLIB_H_ */
