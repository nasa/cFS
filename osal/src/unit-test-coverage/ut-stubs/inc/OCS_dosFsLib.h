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

/* OSAL coverage stub replacement for dosFsLib.h */
#ifndef _OSAL_STUB_DOSFSLIB_H_
#define _OSAL_STUB_DOSFSLIB_H_

#include <OCS_basetypes.h>
#include <OCS_vxWorks.h>

/* ----------------------------------------- */
/* constants normally defined in dosFsLib.h */
/* ----------------------------------------- */
#define OCS_DOS_CHK_ONLY        0x00000001
#define OCS_DOS_CHK_REPAIR      0x00000002
#define OCS_DOS_CHK_VERB_0      0x0000ff00 /* verbosity level/flags */
#define OCS_DOS_CHK_VERB_SILENT OCS_DOS_CHK_VERB_0
#define OCS_DOS_OPT_BLANK       0x0002  /* create a clean boot block */

/* ----------------------------------------- */
/* types normally defined in dosFsLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in dosFsLib.h */
/* ----------------------------------------- */
extern OCS_STATUS OCS_dosFsVolFormat(char *path, int opt, OCS_FUNCPTR pPromptFunc);



#endif /* _OSAL_STUB_DOSFSLIB_H_ */

