/*
 * UT_osprintf_X.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];
extern char trunc_buf[];

/*****************************************************************************
 *  Test %X format
 *****************************************************************************/
void UT_osprintf_X(void)
{
    char *test_fmt = "x"; /* Test format character(s) */
    int i;

    struct
    {
        char *test_num;        /* Test identifier; sequential numbers */
        unsigned int test_val; /* Test value */
        int  max_len;      /* Maximum output string length */
        char *format;          /* Format string */
        char *expected;        /* Expected result */
        char *description;     /* Test description */
    } osp_tests[] =
    {
        {"01", 0xa8b7, 3, "%X", "A8B7", "%X"},
        {"02", 0xff123, 10, "$$$%X$$$", "$$$FF123$$$", "%X embedded"},
        {"03", 0xd1827, 5, "%3X", "D1827", "%X with minimum field size < number of digits"},
        {"04", 0x3c225, 9, "%.10X", "000003C225", "%X with precision field size"},
        {"05", 0x12b45, 7, "%9.7X", "  0012B45", "%X with minimum and precision field size"},
        {"06", 0xe8a60, 19, "%-.20X", "000000000000000E8A60", "%X with left-justify"},
        {"07", -16, 7, "%X", "FFFFFFF0", "%X, negative value"},
        {"08", 0x12b45, 4, "%8X", "   12B45", "%X with minimum field size > number of digits"},
        {"09", 0x12b45, 5, "%08X", "00012B45", "%X with minimum field size > number of digits and leading zeroes"},
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
