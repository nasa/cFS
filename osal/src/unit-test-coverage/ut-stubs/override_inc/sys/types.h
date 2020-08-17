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
#ifndef _OSAL_OVERRIDE_SYS_TYPES_H_
#define _OSAL_OVERRIDE_SYS_TYPES_H_

#include <OCS_sys_types.h>

/* ----------------------------------------- */
/* mappings for declarations in sys/types.h */
/* ----------------------------------------- */
#define ssize_t           OCS_ssize_t
#define off_t             OCS_off_t
#define mode_t            OCS_mode_t
#define pid_t             OCS_pid_t
#define gid_t             OCS_gid_t
#define uid_t             OCS_uid_t

#endif /* _OSAL_OVERRIDE_SYS_TYPES_H_ */
