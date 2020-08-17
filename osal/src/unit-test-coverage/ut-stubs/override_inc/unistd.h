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

/* OSAL coverage stub replacement for unistd.h */
#ifndef _OSAL_OVERRIDE_UNISTD_H_
#define _OSAL_OVERRIDE_UNISTD_H_

#include <OCS_unistd.h>

/* ----------------------------------------- */
/* mappings for declarations in unistd.h */
/* ----------------------------------------- */

#define SEEK_SET        OCS_SEEK_SET
#define SEEK_CUR        OCS_SEEK_CUR
#define SEEK_END        OCS_SEEK_END
#define STDIN_FILENO    OCS_STDIN_FILENO
#define STDOUT_FILENO   OCS_STDOUT_FILENO
#define STDERR_FILENO   OCS_STDERR_FILENO

#define close             OCS_close
#define getegid           OCS_getegid
#define geteuid           OCS_geteuid
#define gethostid         OCS_gethostid
#define gethostname       OCS_gethostname
#define getpid            OCS_getpid
#define lseek             OCS_lseek
#define read              OCS_read
#define rmdir             OCS_rmdir
#define sysconf           OCS_sysconf
#define write             OCS_write


#endif /* _OSAL_OVERRIDE_UNISTD_H_ */

