/*
 * UT_osprintf_misc.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];

/*****************************************************************************
 *  Test miscellaneous formats & combinations
 *****************************************************************************/
void UT_osprintf_misc(void)
{
    int test_int;
    long test_long;
    unsigned test_unsigned;
    unsigned test_hex;
    float test_float;
    double test_double;
    char test_char;
    char *test_num = "misc";

    /* Test representing percent character using %% */
    init_test();
    SPRINTF(strg_buf, "Show percent character .%%.");
    UT_Report(check_test("Show percent character .%.", strg_buf), "SPRINTF",
              "Represent percent character using %%",
              test_num, "01");

    /* Test representing percent character using %%, truncated */
    init_test();
    SNPRINTF(strg_buf, 11, "Show percent character .%%.");
    UT_Report(check_test("Show perce", strg_buf), "SNPRINTF",
              "Represent percent character using %%, truncated",
              test_num, "01");

    /* Test combination of types */
    init_test();
    test_int = -123;
    test_long = 9876543;
    test_unsigned = 4321;
    test_hex = 0xa1b2;
    test_float = -2.3456;
    test_double = 6543.123456;
    test_char = '$';
    SPRINTF(strg_buf,
            "int = %d  long = %ld  uns = %u  hex = 0x%x  flt = %f  "
              "dbl = %lf  chr = %c",
            test_int, test_long, test_unsigned, test_hex,
            test_float, test_double, test_char);
    UT_Report(check_test("int = -123  long = 9876543  uns = 4321  hex = "
                           "0xa1b2  flt = -2.345600  dbl = 6543.123456  "
                           "chr = $", strg_buf), "SPRINTF",
              "Combination of types",
              test_num, "02");

    /* Test combination of types, truncated */
    init_test();
    test_int = -123;
    test_long = 9876543;
    test_unsigned = 4321;
    test_hex = 0xa1b2;
    test_float = -2.3456;
    test_double = 6543.123456;
    test_char = '$';
    SNPRINTF(strg_buf, 16,
             "int = %d  long = %ld  uns = %u  hex = 0x%x  flt = %f  "
               "dbl = %lf  chr = %c",
             test_int, test_long, test_unsigned, test_hex,
             test_float, test_double, test_char);
    UT_Report(check_test("int = -123  lon", strg_buf), "SNPRINTF",
              "Combination of types, truncated",
              test_num, "02");

    /* Test combination of types 2 */
    init_test();
    test_int = -123;
    test_long = 9876543;
    test_unsigned = 4321;
    test_hex = 0xa1b2;
    test_float = -2.3456;
    test_double = 6543.123456;
    test_char = '$';
    SPRINTF(strg_buf,
            "flt = %f  dbl = %lf  flt = %f",
            test_float, test_double, test_float);
    UT_Report(check_test("flt = -2.345600  dbl = 6543.123456  flt = -2.345600",
                         strg_buf), "SPRINTF",
              "Combination of types 2",
              test_num, "03");

    /* Test combination of types 2, truncated */
    init_test();
    test_int = -123;
    test_long = 9876543;
    test_unsigned = 4321;
    test_hex = 0xa1b2;
    test_float = -2.3456;
    test_double = 6543.123456;
    test_char = '$';
    SNPRINTF(strg_buf, 10,
            "flt = %f  dbl = %lf  flt = %f",
            test_float, test_double, test_float);
    UT_Report(check_test("flt = -2.", strg_buf), "SNPRINTF",
              "Combination of types 2",
              test_num, "03");

    /* Test format string ending with percent character */
    init_test();
    SPRINTF(strg_buf, "End with percent character %");
    UT_Report(check_test("End with percent character ", strg_buf), "SPRINTF",
              "End with percent character",
              test_num, "04");

    /* Test representing percent character using %%, truncated */
    init_test();
    SNPRINTF(strg_buf, 11, "End with percent character %");
    UT_Report(check_test("End with p", strg_buf), "SNPRINTF",
              "End with percent character, truncated",
              test_num, "04");

    /* Test null format string */
    init_test();
    SPRINTF(strg_buf, "");
    UT_Report(check_test("", strg_buf), "SPRINTF",
              "Null format string",
              test_num, "05");

    /* Test representing percent character using %%, truncated */
    init_test();
    SNPRINTF(strg_buf, 5, "");
    UT_Report(check_test("", strg_buf), "SNPRINTF",
              "Null format string",
              test_num, "05");

    /* Test too many decimals in format width/precision modifier */
    init_test();
    SPRINTF(strg_buf, "Too many decimals %13.5.2f");
    UT_Report(check_test("Too many decimals %13.5.2f", strg_buf), "SPRINTF",
              "Invalid format string",
              test_num, "06");

    /* Test too many decimals in format width/precision modifier, truncated */
    init_test();
    SNPRINTF(strg_buf, 23, "Too many decimals %13.5.2f");
    UT_Report(check_test("Too many decimals %13.", strg_buf), "SNPRINTF",
              "Invalid format string",
              test_num, "06");
}
