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

/* OSAL coverage stub replacement for sys/mman.h */
#ifndef _OSAL_STUB_SYS_MMAN_H_
#define _OSAL_STUB_SYS_MMAN_H_

#include <OCS_basetypes.h>

/* ----------------------------------------- */
/* constants normally defined in sys/mman.h */
/* ----------------------------------------- */
#define OCS_MAP_SHARED      0x2001
#define OCS_MAP_PRIVATE     0x2002
#define OCS_PROT_EXEC       0x2004
#define OCS_PROT_READ       0x2008
#define OCS_PROT_WRITE      0x2010
#define OCS_PROT_NONE       0x2020
#define OCS_MAP_FIXED       0x2080

/* ----------------------------------------- */
/* types normally defined in sys/mman.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in sys/mman.h */
/* ----------------------------------------- */
void *OCS_mmap(void *addr, size_t length, int prot, int flags,
                  int fd, OCS_off_t offset);
int OCS_munmap(void *addr, size_t length);


#endif /* _OSAL_STUB_SYS_MMAN_H_ */

