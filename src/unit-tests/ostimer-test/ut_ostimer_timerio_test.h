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
** File:  ut_ostimer_timerio_test.h
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

#ifndef _UT_OSTIMER_TIMERIO_TEST_H_
#define _UT_OSTIMER_TIMERIO_TEST_H_

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_support.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_OS_TIMER_LIST_LEN  (OS_MAX_TIMERS + 10)

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

void UT_os_timerinit_test(void);
void UT_os_timercreate_test(void);
void UT_os_timerdelete_test(void);
void UT_os_timerset_test(void);
void UT_os_timergetidbyname_test(void);
void UT_os_timergetinfo_test(void);

/*--------------------------------------------------------------------------------*/

#endif  /* _UT_OSTIMER_TIMERIO_TEST_H_ */

/*================================================================================*
** End of File: ut_ostimer_timerio_test.h
**================================================================================*/
