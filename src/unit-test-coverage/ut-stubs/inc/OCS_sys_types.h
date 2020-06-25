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

/* OSAL coverage stub replacement for sys/types.h */
#ifndef _OSAL_STUB_SYS_TYPES_H_
#define _OSAL_STUB_SYS_TYPES_H_

#include <OCS_basetypes.h>

/* ----------------------------------------- */
/* constants normally defined in sys/types.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/types.h */
/* ----------------------------------------- */
typedef ptrdiff_t       OCS_ssize_t;
typedef size_t          OCS_off_t;
typedef unsigned int    OCS_mode_t;
typedef long            OCS_time_t;
typedef int             OCS_pid_t;
typedef int             OCS_gid_t;
typedef int             OCS_uid_t;

/* ----------------------------------------- */
/* prototypes normally declared in sys/types.h */
/* ----------------------------------------- */



#endif /* _OSAL_STUB_SYS_TYPES_H_ */

