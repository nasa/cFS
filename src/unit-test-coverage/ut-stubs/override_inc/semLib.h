/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for semLib.h */
#ifndef _OSAL_OVERRIDE_SEMLIB_H_
#define _OSAL_OVERRIDE_SEMLIB_H_

#include <OCS_semLib.h>
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in semLib.h */
/* ----------------------------------------- */

#define VX_BINARY_SEMAPHORE(x)     OCS_VX_BINARY_SEMAPHORE(x)
#define VX_COUNTING_SEMAPHORE(x)   OCS_VX_COUNTING_SEMAPHORE(x)
#define VX_MUTEX_SEMAPHORE(x)      OCS_VX_MUTEX_SEMAPHORE(x)

#define SEM_ID                     OCS_SEM_ID
#define SEM_B_STATE                OCS_SEM_B_STATE
#define SEM_EMPTY                  OCS_SEM_EMPTY
#define SEM_FULL                   OCS_SEM_FULL
#define SEM_Q_FIFO                 OCS_SEM_Q_FIFO
#define SEM_Q_PRIORITY             OCS_SEM_Q_PRIORITY
#define SEM_DELETE_SAFE            OCS_SEM_DELETE_SAFE
#define SEM_INVERSION_SAFE         OCS_SEM_INVERSION_SAFE
#define SEM_EVENTSEND_ERR_NOTIFY   OCS_SEM_EVENTSEND_ERR_NOTIFY

#define semBInitialize          OCS_semBInitialize
#define semBCreate              OCS_semBCreate
#define semMInitialize          OCS_semMInitialize
#define semMCreate              OCS_semMCreate
#define semCInitialize          OCS_semCInitialize
#define semCCreate              OCS_semCCreate
#define semDelete               OCS_semDelete
#define semFlush                OCS_semFlush
#define semTake                 OCS_semTake
#define semGive                 OCS_semGive



#endif /* _OSAL_OVERRIDE_SEMLIB_H_ */
