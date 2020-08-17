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

/* OSAL coverage stub replacement for errno.h */
#ifndef _OSAL_STUB_ERRNO_H_
#define _OSAL_STUB_ERRNO_H_

#include <OCS_basetypes.h>

/* ----------------------------------------- */
/* constants normally defined in errno.h */
/* ----------------------------------------- */
/* POSIX-specific  errnos */
#define OCS_EINTR                        0x1801
#define OCS_EAGAIN                       0x1802
#define OCS_EINVAL                       0x1803
#define OCS_EMSGSIZE                     0x1804
#define OCS_ETIMEDOUT                    0x1805
#define OCS_ESPIPE                       0x1806


/* ----------------------------------------- */
/* types normally defined in errno.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in errno.h */
/* ----------------------------------------- */

extern int OCS_errno;

#endif /* _OSAL_STUB_ERRNO_H_ */

