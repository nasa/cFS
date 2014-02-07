/*
 * UT_osprintf_c.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];
extern char trunc_buf[];

/*****************************************************************************
 *  Test %c format
 *****************************************************************************/
void UT_osprintf_c(void)
{
    char *test_fmt = "c"; /* Test format character(s) */
    int i;

    struct
    {
        char *test_num;    /* Test identifier; sequential numbers */
        char test_val;     /* Test value */
        int  max_len;      /* Maximum output string length */
        char *format;      /* Format string */
        char *expected;    /* Expected result */
        char *description; /* Test description */
    } osp_tests[] =
    {
        {"01", 'k', 1, "%c", "k", "%c"},
        {"02", 'w', 5, "$$$%c$$$", "$$$w$$$", "%c embedded"},
        {"03", '?', 19, "%20c", "                   ?", "%c with minimum field size"},
        {"04", 'Q', 2, "%.10c", "Q", "%c with maximum field size"},
        {"05", '>', 5, "%7.9c", "      >", "%c with minimum and maximum field size"},
        {"06", '#', 17, "%-20c", "#                   ", "%c with left-justify"},
        {"07", 'H', 2, "%+c", "H", "%c with sign"},
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
