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
** File:  ut_osloader_test_platforms.h
** Owner: Tam Ngo
** Date:  November 2014
**================================================================================*/

#ifndef _UT_OSLOADER_TEST_PLATFORMS_H_
#define _UT_OSLOADER_TEST_PLATFORMS_H_

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_OS_GENERIC_MODULE_DIR        "/utmod/"
#define UT_OS_GENERIC_MODULE_BASENAME   "MODULE"

#define UT_OS_GENERIC_MODULE_NAME1      UT_OS_GENERIC_MODULE_DIR UT_OS_GENERIC_MODULE_BASENAME "0" OS_MODULE_FILE_EXTENSION
#define UT_OS_GENERIC_MODULE_NAME2      UT_OS_GENERIC_MODULE_DIR UT_OS_GENERIC_MODULE_BASENAME "1" OS_MODULE_FILE_EXTENSION

#define UT_OS_GENERIC_MODULE_NAME_TEMPLATE   UT_OS_GENERIC_MODULE_BASENAME "%d"
#define UT_OS_GENERIC_MODULE_FILE_TEMPLATE   UT_OS_GENERIC_MODULE_DIR UT_OS_GENERIC_MODULE_NAME_TEMPLATE OS_MODULE_FILE_EXTENSION

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

/*--------------------------------------------------------------------------------*/

#endif  /* _UT_OSLOADER_TEST_PLATFORMS_H_ */

/*================================================================================*
** End of File: ut_osloader_test_platforms.h
**================================================================================*/
