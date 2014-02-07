/*
 * UT_osprintf_lu.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];
extern char trunc_buf[];

/*****************************************************************************
 *  Test %lu format
 *****************************************************************************/
void UT_osprintf_lu(void)
{
    char *test_fmt = "lu"; /* Test format character(s) */
    int i;

    struct
    {
        char *test_num;             /* Test identifier; sequential numbers */
        unsigned long int test_val; /* Test value */
        int  max_len;               /* Maximum output string length */
        char *format;               /* Format string */
        char *expected;             /* Expected result */
        char *description;          /* Test description */
    } osp_tests[] =
    {
        {"01", 9876543, 6, "%lu", "9876543", "%lu, negative value"},
        {"02", 4637210, 12, "$$$%lu$$$", "$$$4637210$$$", "%lu embedded"},
        {"03", 9182755, 6, "%3lu", "9182755", "%lu with maximum field size"},
        {"04", 3322554, 10, "%.10lu", "0003322554", "%lu with minimum field size"},
        {"05", 123456, 8, "%9.7lu", "  0123456", "%lu with minimum and maximum field size"},
        {"06", 9876543, 16, "%-.20lu", "00000000000009876543", "%lu with left-justify"},
        {"07", 9876543, 5, "%+lu", "9876543", "%lu with sign"},
        {"08", -9876543, 8, "%lu", "4285090753", "%lu, negative value"},
        {"09", 0, 6, "%lu", "0", "%lu, zero value"},
        {"10", 162534098, 5, "%011lu", "00162534098", "%lu with zero padding"},
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
