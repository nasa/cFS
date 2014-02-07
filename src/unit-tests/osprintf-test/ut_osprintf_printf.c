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
    PRINTF("%s", "printf_test_string\n");
    UT_Report(UT_PASS,
              "PRINTF",
              "Output to console",
              "%s",
              "01");

    /* Perform printf disabled test */
    OS_printf_enabled = 0;
    PRINTF("%s", "printf_test_string_disabled\n");
    UT_Report(UT_PASS,
              "PRINTF",
              "Output to console disabled",
              "%s",
              "02");
    OS_printf_enabled = 1;
}
#endif
