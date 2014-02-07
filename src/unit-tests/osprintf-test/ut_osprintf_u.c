/*
 * UT_osprintf_u.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];
extern char trunc_buf[];

/*****************************************************************************
 *  Test %u format
 *****************************************************************************/
void UT_osprintf_u(void)
{
    char *test_fmt = "u"; /* Test format character(s) */
    int i;

    struct
    {
        char *test_num;    /* Test identifier; sequential numbers */
        int  test_val;     /* Test value */
        int  max_len;      /* Maximum output string length */
        char *format;      /* Format string */
        char *expected;    /* Expected result */
        char *description; /* Test description */
    } osp_tests[] =
    {
        {"01", 98765, 5, "%u", "98765", "%u"},
        {"02", 46372, 10, "$$$%u$$$", "$$$46372$$$", "%u embedded"},
        {"03", 91827, 5, "%3u", "91827", "%u with maximum field size"},
        {"04", 33225, 8, "%.10u", "0000033225", "%u with minimum field size"},
        {"05", 12345, 7, "%9.7u", "  0012345", "%u with minimum and maximum field size"},
        {"06", 98765, 18, "%-.20u", "00000000000000098765", "%u with left-justify"},
        {"07", 98765, 5, "%+u", "98765", "%u with sign"},
        {"08", -98765, 8, "%u", "4294868531", "%u, negative value"},
        {"09", 0, 6, "%u", "0", "%u, zero value"},
        {"10", 162534, 5, "%08u", "00162534", "%u with zero padding"},
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
