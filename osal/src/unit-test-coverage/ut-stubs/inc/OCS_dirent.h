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

/* OSAL coverage stub replacement for dirent.h */
#ifndef _OSAL_STUB_DIRENT_H_
#define _OSAL_STUB_DIRENT_H_

#include <OCS_basetypes.h>

/* ----------------------------------------- */
/* constants normally defined in dirent.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in dirent.h */
/* ----------------------------------------- */

typedef struct OCS_DIR           OCS_DIR;

struct OCS_dirent
{
    char d_name[4];
};



/* ----------------------------------------- */
/* prototypes normally declared in dirent.h */
/* ----------------------------------------- */

extern int OCS_closedir (OCS_DIR * dirp);
extern OCS_DIR *OCS_opendir (const char * name);
extern struct OCS_dirent *OCS_readdir (OCS_DIR * dirp);
extern void OCS_rewinddir (OCS_DIR * dirp);


#endif /* _OSAL_STUB_DIRENT_H_ */

