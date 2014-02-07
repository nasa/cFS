/*
 * UT_osprintf_s.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];
extern char trunc_buf[];

/*****************************************************************************
 *  Test %s format
 *****************************************************************************/
void UT_osprintf_s(void)
{
    char *test_fmt = "s"; /* Test format character(s) */
    int i;

    struct
    {
        char *test_num;    /* Test identifier; sequential numbers */
        char test_val[30]; /* Test value */
        int  max_len;      /* Maximum output string length */
        char *format;      /* Format string */
        char *expected;    /* Expected result */
        char *description; /* Test description */
    } osp_tests[] =
    {
        {"01", "123456789abcd", 7, "%s", "123456789abcd", "%s"},
        {"02", "123456789abcd", 7, "$$$%s$$$", "$$$123456789abcd$$$", "%s embedded"},
        {"03", "123456789abcd", 11, "%20s", "       123456789abcd", "%s with minimum field size"},
        {"04", "123456789abcd", 8, "%.10s", "123456789a", "%s with maximum field size"},
        {"05", "123456789abcd", 7, "%5.7s", "1234567", "%s with minimum and maximum field size"},
        {"06", "123456789abcd", 10, "%-20s", "123456789abcd       ", "%s with left-justify"},
        {"07", "3456789abcd", 7, "%+s", "3456789abcd", "%s with sign"},
        {"08", "", 3, "%s", "", "%s with null string"},
        {"09", "123456789abcd", 11, "%020s", "       123456789abcd", "%s with minimum field size, ignore zero padding"},
        {"", "", 0, "", "", ""} /* End with a null format to terminate list */
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
