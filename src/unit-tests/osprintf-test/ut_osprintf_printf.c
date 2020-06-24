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

/*
 * UT_osprintf_printf.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#ifdef OS_USE_EMBEDDED_PRINTF

#include "ut_osprintf.h"

extern unsigned OS_printf_enabled;

/*****************************************************************************
 *  Tests of embedded printf to get complete osprintf.c code coverage
 *****************************************************************************/
void UT_osprintf_printf(void)
{
    /* Perform printf enabled test */
    OS_printf("%s", "printf_test_string\n");
    UT_Report(UT_PASS,
              "PRINTF",
              "Output to console",
              "%s",
              "01");

    /* Perform printf disabled test */
    OS_printf_enabled = 0;
    OS_printf("%s", "printf_test_string_disabled\n");
    UT_Report(UT_PASS,
              "PRINTF",
              "Output to console disabled",
              "%s",
              "02");
    OS_printf_enabled = 1;
}
#endif
