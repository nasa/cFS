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
#ifndef _OSAL_OVERRIDE_STAT_H_
#define _OSAL_OVERRIDE_STAT_H_

#include <OCS_stat.h>

/* ----------------------------------------- */
/* mappings for declarations in sys/stat.h */
/* ----------------------------------------- */

#define stat          OCS_stat
#define fchmod        OCS_fchmod
#define chmod         OCS_chmod
#define mkdir         OCS_mkdir


/* ----------------------------------------- */
/* mappings for declarations in sys/statvfs.h */
/* ----------------------------------------- */
#define statvfs     OCS_statvfs
#define statfs      OCS_statvfs



#endif /* _OSAL_OVERRIDE_STAT_H_ */
