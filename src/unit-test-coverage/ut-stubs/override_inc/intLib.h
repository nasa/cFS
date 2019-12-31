/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for intLib.h */
#ifndef _OSAL_OVERRIDE_INTLIB_H_
#define _OSAL_OVERRIDE_INTLIB_H_

#include <OCS_intLib.h>
#include <vxWorks.h>


/* ----------------------------------------- */
/* mappings for declarations in intLib.h */
/* ----------------------------------------- */


#define intConnect    OCS_intConnect
#define intDisable    OCS_intDisable
#define intEnable     OCS_intEnable
#define intLock       OCS_intLock
#define intUnlock     OCS_intUnlock
#define INUM_TO_IVEC  OCS_INUM_TO_IVEC

#endif /* _OSAL_OVERRIDE_INTLIB_H_ */
