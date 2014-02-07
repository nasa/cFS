/*
 * ut_osprintf.h
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#ifndef UT_OSPRINTF_H_
#define UT_OSPRINTF_H_

/*
** Includes
*/
#include <string.h>
#include <stdlib.h>
#include "osprintf.h"

/* printf Replacement Unit Test Compiler Flags
    - Define UT_SHOW_PASS in order to display passing tests; if undefined then
      only the failed tests are displayed.  In either case the total number of
      tests passes and failed are displayed
    - Define UT_DO_FLOAT to test floating point printf's
    - Define UT_DO_OFFSET when compiling on the GHS target to perform tests to
      determine variable parameter offset values for the cFE variadic functions
    - Define OSP_GHS to compile on the GHS platform
   osprintf Compiler Flags
    - Define OS_USE_EMBEDDED_PRINTF to test the replacement printf functions;
      undefine it to test the standard printf functions
    - Define OSP_ARINC653 if using embedded (replacement) printf functions
      and compiling on the GHS target using cFE modified for ARINC653
*/

#ifdef OS_USE_EMBEDDED_PRINTF
#ifndef OSP_GHS
int putchar(int);
#endif
#else
#include <stdio.h>
#endif

/*
** Macro Definitions
*/
#define BUF_LEN 200
#define UT_MAX_MESSAGE_LENGTH 300
#define UT_TRUE 1
#define UT_FALSE 0
#define UT_PASS 0
#define UT_FAIL 1

/*
** Function prototypes
*/
void init_test(void);
int check_test(char *expected, char *actual);

void UT_osprintf_d(void);
void UT_osprintf_i(void);
void UT_osprintf_u(void);
void UT_osprintf_x(void);
void UT_osprintf_X(void);
void UT_osprintf_p(void);
void UT_osprintf_c(void);
void UT_osprintf_s(void);
void UT_osprintf_ld(void);
void UT_osprintf_li(void);
void UT_osprintf_lu(void);
void UT_osprintf_lx(void);
void UT_osprintf_lX(void);

#ifdef UT_DO_FLOAT
void UT_osprintf_f(void);
void UT_osprintf_lf(void);
void UT_osprintf_misc(void);
#endif

#ifdef OS_USE_EMBEDDED_PRINTF
void UT_osprintf_printf(void);
#endif

void UT_Text(char *out_text);
void UT_Report(int test, char *fun_name, char *info,
               char *test_num, char *test_seq);
void UT_itoa(int value, char *string, int radix, int out_len);
void UT_ReportFailures(void);

#endif /* UT_OSPRINTF_H_ */
