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

/* OSAL coverage stub replacement for stat.h */
#ifndef _OSAL_STUB_STAT_H_
#define _OSAL_STUB_STAT_H_

#include <OCS_basetypes.h>
#include <OCS_sys_types.h>

/* ----------------------------------------- */
/* constants normally defined in sys/stat.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* constants normally defined in sys/statvfs.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/stat.h */
/* ----------------------------------------- */

struct OCS_stat
{
    OCS_mode_t   st_mode;
    OCS_off_t    st_size;
    OCS_time_t   st_mtime;
    OCS_uid_t    st_uid;
    OCS_gid_t    st_gid;
};

/* ----------------------------------------- */
/* types normally defined in sys/statvfs.h */
/* ----------------------------------------- */
struct OCS_statvfs
{
    OCS_off_t f_bsize;
    OCS_off_t f_bfree;
    OCS_off_t f_blocks;
};

/* ----------------------------------------- */
/* prototypes normally declared in sys/stat.h */
/* ----------------------------------------- */

extern int OCS_fchmod (int fd, OCS_mode_t mode);
extern int OCS_chmod (const char *path, OCS_mode_t mode);
extern int OCS_mkdir (const char * path, ...);
extern int OCS_stat (const char * file, struct OCS_stat * buf);


/* ----------------------------------------- */
/* prototypes normally declared in sys/statvfs.h */
/* ----------------------------------------- */

extern int OCS_statvfs (const char * file, struct OCS_statvfs * buf);



#endif /* _OSAL_STUB_STAT_H_ */

