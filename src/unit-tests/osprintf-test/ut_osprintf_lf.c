/*
 * UT_osprintf_lf.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];
extern char trunc_buf[];

/*****************************************************************************
 *  Test %lf format
 *****************************************************************************/
void UT_osprintf_lf(void)
{
    char *test_fmt = "lf"; /* Test format character(s) */
    int i;

    struct
    {
        char   *test_num;    /* Test identifier; sequential numbers */
        double test_val;     /* Test value */
        int    max_len;      /* Maximum output string length */
        char   *format;      /* Format string */
        char   *expected;    /* Expected result */
        char   *description; /* Test description */
    } osp_tests[] =
    {
        {"01", 9876543.125, 14, "%lf", "9876543.125000", "%lf, positive value"},
        {"02", 4637210.36, 12, "$$$%lf$$$", "$$$4637210.360000$$$", "%lf embedded, positive value"},
        {"03", 9182755.2756, 9, "%3lf", "9182755.275600", "%lf with maximum field size, positive value"},
        {"04", 12554.08, 5, "%.7lf", "12554.0800000", "%lf with precision, positive value"},
        {"05", 123456.2311, 11, "%9.7lf", "123456.2311000", "%lf with precision and maximum field size, positive value"},
        {"06", 9876543.6765, 15, "%-20lf", "9876543.676500      ", "%lf with left-justify, positive value"},
        {"07", 9876543.001, 13, "%+lf", "+9876543.001000", "%lf with sign, positive value"},
        {"08", -9876543.1987, 12, "%lf", "-9876543.198700", "%lf, negative value"},
        {"09", -4637210.871, 14, "$$$%lf$$$", "$$$-4637210.871000$$$", "%lf embedded, negative value"},
        {"10", -9182755.22222, 15, "%3lf", "-9182755.222220", "%lf with maximum field size, negative value"},
        {"11", -3355.6109, 11, "%.5lf", "-3355.61090", "%lf with precision, negative value"},
        {"12", -123456.7, 14, "%15.4lf", "   -123456.7000", "%lf with precision and maximum field size, negative value"},
        {"13", -9876543.64388, 17, "%-20lf", "-9876543.643880     ", "%lf with left-justify, negative value"},
        {"14", 0.0, 4, "%lf", "0.000000", "%lf, zero value"},
        {"15", 123456789.0, 6, "%10.0lf", " 123456789", "%lf, no fraction, positive value"},
        {"16", -987654321.0, 6, "%12.0lf", "  -987654321", "%lf, no fraction, negative value"},
        {"17", 34887.1255667, 5, "%020.4lf", "000000000034887.1256", "%lf with zero padding, positive value"},
        {"18", -34887.1255667, 5, "%020.4lf", "-00000000034887.1256", "%lf with zero padding, negative value"},
        {"19", 0.0, 6, "%09.4lf", "0000.0000", "%lf, with zero padding, zero value"},
        {"20", 467812.3, 6, "%9.0lf", "   467812", "%lf, no fraction, positive value"},
        {"21", -544446.0, 6, "%8.0lf", " -544446", "%lf, no fraction, negative value"},
        {"", 0, 0, "", "", ""} /* End with a null format to terminate list */
    };

    for (i = 0; osp_tests[i].format[0] != '\0'; i++)
    {
        /* Perform sprintf test */
        init_test();
        SPRINTF(strg_buf, osp_tests[i].format, osp_tests[i].test_val);
        UT_Report(check_test(osp_tests[i].expected, strg_buf),
                  "SPRINTF",
                  osp_tests[i].description,
                  test_fmt,
                  osp_tests[i].test_num);

        /* Truncate expected string in preparation for snprintf test */
        strcpy(trunc_buf, osp_tests[i].expected);

        if (strlen(trunc_buf) >= osp_tests[i].max_len)
        {
            trunc_buf[osp_tests[i].max_len - 1] = '\0';
        }

        /* Perform snprintf test */
        init_test();
        SNPRINTF(strg_buf, osp_tests[i].max_len,
                 osp_tests[i].format, osp_tests[i].test_val);
        UT_Report(check_test(trunc_buf, strg_buf),
                  "SNPRINTF",
                  osp_tests[i].description,
                  test_fmt,
                  osp_tests[i].test_num);
    }
}
