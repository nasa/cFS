/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* OSAL coverage stub replacement for semaphore.h */
#ifndef _OSAL_STUB_SEMAPHORE_H_
#define _OSAL_STUB_SEMAPHORE_H_

#include <OCS_basetypes.h>
#include <OCS_time.h>

/* ----------------------------------------- */
/* constants normally defined in semaphore.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in semaphore.h */
/* ----------------------------------------- */

typedef struct { int s; }    OCS_sem_t;

/* ----------------------------------------- */
/* prototypes normally declared in semaphore.h */
/* ----------------------------------------- */

extern int OCS_sem_destroy (OCS_sem_t * sem);
extern int OCS_sem_getvalue (OCS_sem_t * sem, int * sval);
extern int OCS_sem_init (OCS_sem_t * sem, int pshared, unsigned int value);
extern int OCS_sem_post (OCS_sem_t * sem);
extern int OCS_sem_timedwait (OCS_sem_t * sem, const struct OCS_timespec * abstime);
extern int OCS_sem_wait (OCS_sem_t * sem);


#endif /* _OSAL_STUB_SEMAPHORE_H_ */

