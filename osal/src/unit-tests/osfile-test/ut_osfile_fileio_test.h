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
** File:  ut_osfile_fileio.h
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

#ifndef _UT_OSFILE_FILEIO_H_
#define _UT_OSFILE_FILEIO_H_

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_support.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

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

void UT_os_createfile_test(void);
void UT_os_openfile_test(void);
void UT_os_closefile_test(void);

void UT_os_readfile_test(void);
void UT_os_writefile_test(void);
void UT_os_lseekfile_test(void);

void UT_os_chmodfile_test(void);
void UT_os_statfile_test(void);

void UT_os_removefile_test(void);
void UT_os_renamefile_test(void);
void UT_os_copyfile_test(void);
void UT_os_movefile_test(void);

void UT_os_outputtofile_test(void);
void UT_os_getfdinfo_test(void);

void UT_os_checkfileopen_test(void);
void UT_os_closeallfiles_test(void);
void UT_os_closefilebyname_test(void);

/*--------------------------------------------------------------------------------*/

#endif  /* _UT_OSFILE_FILEIO_H_ */

/*================================================================================*
** End of File: ut_osfile_fileio.h
**================================================================================*/
