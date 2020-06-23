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

/*================================================================================*
** File:  ut_osfilesys_diskio_test.h
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

#ifndef _UT_OSFILESYS_DISKIO_TEST_H_
#define _UT_OSFILESYS_DISKIO_TEST_H_

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_support.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_OS_FILESYS_LIST_LEN  (OS_MAX_FILE_SYSTEMS + 10)

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function prototypes
**--------------------------------------------------------------------------------*/

void UT_os_initfs_test(void);

void UT_os_makefs_test(void);
void UT_os_removefs_test(void);

void UT_os_mount_test(void);
void UT_os_unmount_test(void);

void UT_os_getphysdrivename_test(void);
void UT_os_getfsinfo_test(void);
void UT_os_translatepath_test(void);

void UT_os_checkfs_test(void);

void UT_os_fsblocksfree_test(void);
void UT_os_fsbytesfree_test(void);

/*--------------------------------------------------------------------------------*/

#endif  /* _UT_OSFILESYS_DISKIO_TEST_H_ */

/*================================================================================*
** End of File: ut_osfilesys_diskio_test.h
**================================================================================*/
