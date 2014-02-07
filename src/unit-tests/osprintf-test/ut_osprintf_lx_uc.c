/*
 * UT_osprintf_lX.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];
extern char trunc_buf[];

/*****************************************************************************
 *  Test %lX format
 *****************************************************************************/
void UT_osprintf_lX(void)
{
    char *test_fmt = "lx"; /* Test format character(s) */
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
        {"01", 0x9a8b7c6d, 8, "%lX", "9A8B7C6D", "%lX"},
        {"02", 0xdd46ee21, 8, "$$$%lX$$$", "$$$DD46EE21$$$", "%lX embedded"},
        {"03", 0x9ccc8275, 7, "%3lX", "9CCC8275", "%lX with minimum field size < number of digits"},
        {"04", 0xbee33225, 10, "%.10lX", "00BEE33225", "%lX with precision field size"},
        {"05", 0x123fdb, 7, "%9.7lX", "  0123FDB", "%lX with minimum and precision field size"},
        {"06", 0xabc6543f, 16, "%-.20lX", "000000000000ABC6543F", "%lX with left-justify"},
        {"07", -9876543, 5, "%lX", "FF694BC1", "%lX, negative value"},
        {"08", 0x12b45, 3, "%8lX", "   12B45", "%lX with minimum field size > number of digits"},
        {"09", 0x12b45, 2, "%08lX", "00012B45", "%lX with minimum field size > number of digits and leading zeroes"},
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
