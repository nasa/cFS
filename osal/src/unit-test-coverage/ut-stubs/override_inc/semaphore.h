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
#ifndef _OSAL_OVERRIDE_SEMAPHORE_H_
#define _OSAL_OVERRIDE_SEMAPHORE_H_

#include <OCS_semaphore.h>
#include <time.h>

/* ----------------------------------------- */
/* mappings for declarations in semaphore.h */
/* ----------------------------------------- */
#define sem_t           OCS_sem_t
#define sem_destroy     OCS_sem_destroy
#define sem_getvalue    OCS_sem_getvalue
#define sem_init        OCS_sem_init
#define sem_post        OCS_sem_post
#define sem_timedwait   OCS_sem_timedwait
#define sem_wait        OCS_sem_wait


#endif /* _OSAL_OVERRIDE_SEMAPHORE_H_ */
