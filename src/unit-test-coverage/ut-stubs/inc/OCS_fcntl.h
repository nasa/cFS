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

/* OSAL coverage stub replacement for fcntl.h */
#ifndef _OSAL_STUB_FCNTL_H_
#define _OSAL_STUB_FCNTL_H_

#include <OCS_basetypes.h>

/* ----------------------------------------- */
/* constants normally defined in fcntl.h */
/* ----------------------------------------- */


/*
 * note these constants are used as bitmasks and so
 * they must be single bit flags to work properly.
 *
 * However, they are defined differently from a typical UNIX
 * in order to increase the likelihood that a hard-coded value
 * is caught by UT.
 *
 * These are exposed to apps through fcntl.h and sys/stat.h
 */
#define OCS_S_IXOTH     0x1000
#define OCS_S_IXGRP     0x2000
#define OCS_S_IXUSR     0x4000

#define OCS_S_IROTH     0x0100
#define OCS_S_IRGRP     0x0200
#define OCS_S_IRUSR     0x0400

#define OCS_S_IWOTH     0x0010
#define OCS_S_IWGRP     0x0020
#define OCS_S_IWUSR     0x0040

#define OCS_S_IFDIR     0x0001
#define OCS_S_ISDIR(x)  (((x) & OCS_S_IFDIR) == OCS_S_IFDIR)


#define OCS_O_RDONLY                     0x1501
#define OCS_O_WRONLY                     0x1502
#define OCS_O_RDWR                       0x1503
#define OCS_O_CREAT                      0x1504
#define OCS_O_TRUNC                      0x1508
#define OCS_O_NONBLOCK                   0x1510


#define OCS_F_GETFL                      0x1D01
#define OCS_F_SETFL                      0x1D02
#define OCS_F_GETFD                      0x1D03
#define OCS_F_SETFD                      0x1D04
#define OCS_F_DUPFD                      0x1D05

/* ----------------------------------------- */
/* types normally defined in fcntl.h */
/* ----------------------------------------- */


/* ----------------------------------------- */
/* prototypes normally declared in fcntl.h */
/* ----------------------------------------- */

extern int OCS_fcntl (int fd, int cmd, ...);
extern int OCS_open (const char * file, int oflag, ...);



#endif /* _OSAL_STUB_FCNTL_H_ */

