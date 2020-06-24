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
#ifndef _OSAL_STUB_UNISTD_H_
#define _OSAL_STUB_UNISTD_H_

#include <OCS_basetypes.h>
#include <OCS_sys_types.h>

/* ----------------------------------------- */
/* constants normally defined in unistd.h */
/* ----------------------------------------- */

#define OCS_SEEK_SET        0x1C01
#define OCS_SEEK_CUR        0x1C02
#define OCS_SEEK_END        0x1C03
#define OCS_STDIN_FILENO    0x1C04
#define OCS_STDOUT_FILENO   0x1C05
#define OCS_STDERR_FILENO   0x1C06


/* ----------------------------------------- */
/* types normally defined in unistd.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in unistd.h */
/* ----------------------------------------- */

extern int OCS_close (int fd);
extern OCS_gid_t OCS_getegid (void);
extern OCS_uid_t OCS_geteuid (void);
extern long int OCS_gethostid (void);
extern int OCS_gethostname (char * name, size_t len);
extern OCS_pid_t OCS_getpid (void);
extern OCS_off_t OCS_lseek (int fd, OCS_off_t offset, int whence);
extern OCS_ssize_t OCS_read (int fd, void * buf, size_t nbytes);
extern int OCS_rmdir (const char * path);
extern long int OCS_sysconf (int name);
extern OCS_ssize_t OCS_write (int fd, const void * buf, size_t n);


#endif /* _OSAL_STUB_UNISTD_H_ */

