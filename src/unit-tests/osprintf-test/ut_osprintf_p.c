/*
 * UT_osprintf_p.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];
extern char trunc_buf[];

/*****************************************************************************
 *  Test %p format
 *****************************************************************************/
void UT_osprintf_p(void)
{
    char *test_fmt = "p"; /* Test format character(s) */
    int i;

    struct
    {
        char *test_num;    /* Test identifier; sequential numbers */
        long int test_val; /* Test value */
        int  max_len;      /* Maximum output string length */
        char *format;      /* Format string */
        char *expected;    /* Expected result */
        char *description; /* Test description */
    } osp_tests[] =
    {
        {"01", 98765, 5, "%p", "0x181cd", "%p"},
        {"02", 46372, 10, "$$$%p$$$", "$$$0xb524$$$", "%p embedded"},
        {"03", 91827, 5, "%3p", "0x166b3", "%p with minimum field size"},
        {"04", 33225, 11, "%.10p", "0x00000081c9", "%p with precision field size"},
        {"05", 12345, 9, "%9.7p", "0x0003039", "%p with minimum and precision field size"},
        {"06", 98765, 19, "%-.20p", "0x000000000000000181cd", "%p with left-justify"},
        {"07", -98765, 8, "%p", "0xfffe7e33", "%p, negative value"},
        {"08", 4108, 4, "%8p", "  0x100c", "%p with minimum field size > number of digits"},
        {"09", 55220, 6, "%010p", "0x0000d7b4", "%p with minimum field size > number of digits and leading zeroes"},
        {"", 0, 0, "", "", ""} /* End with a null format to terminate list */
    };

    for (i = 0; osp_tests[i].format[0] != '\0'; i++)
    {
        /* Perform sprintf test */
        init_test();
        SPRINTF(strg_buf, osp_tests[i].format, (void *) osp_tests[i].test_val);
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
                 osp_tests[i].format, (void *) osp_tests[i].test_val);
        UT_Report(check_test(trunc_buf, strg_buf),
                  "SNPRINTF",
                  osp_tests[i].description,
                  test_fmt,
                  osp_tests[i].test_num);
    }
}
