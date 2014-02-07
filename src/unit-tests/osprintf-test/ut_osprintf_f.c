/*
 * UT_osprintf_f.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];
extern char trunc_buf[];

/*****************************************************************************
 *  Test %f format
 *****************************************************************************/
void UT_osprintf_f(void)
{
    char *test_fmt = "f"; /* Test format character(s) */
    int i;

    struct
    {
        char  *test_num;    /* Test identifier; sequential numbers */
        float test_val;     /* Test value */
        int   max_len;      /* Maximum output string length */
        char  *format;      /* Format string */
        char  *expected;    /* Expected result */
        char  *description; /* Test description */
    } osp_tests[] =
    {
        {"01", 5.230, 6, "%f", "5.230000", "%f, positive value"},
        {"02", 2.1056, 9, "$$$%f$$$", "$$$2.105600$$$", "%f embedded, positive value"},
        {"03", 91827.3, 4, "%3f", "91827.296875", "%f with maximum field size, positive value"},
        {"04", 5.82345, 5, "%.3f", "5.823", "%f with minimum field size, positive value"},
        {"05", 12.6789, 8, "%9.5f", " 12.67890", "%f with minimum and maximum field size, positive value"},
        {"06", 65.5678, 13, "%-20.5f", "65.56780            ", "%f with left-justify, positive value"},
        {"07", 2.7944, 8, "%+f", "+2.794400", "%f with sign, positive value"},
        {"08", -0.6712237, 7, "%f", "-0.671224", "%f, negative value"},
        {"09", -7.1109, 8, "$$$%f$$$", "$$$-7.110900$$$", "%f embedded, negative value"},
        {"10", -918.987, 6, "%3f", "-918.987000", "%f with maximum field size, negative value"},
        {"11", -3.1415, 3, "%.2f", "-3.14", "%f with precision, negative value"},
        {"12", -1.23456, 6, "%9.7f", "-1.2345600", "%f with precision and maximum field size, negative value"},
        {"13", -65.65, 5, "%-8.3f", "-65.650 ", "%f with left-justify, negative value"},
        {"14", 0.0, 4, "%f", "0.000000", "%f, zero value"},
        {"15", 4.0, 6, "%7.0f", "      4", "%f, no fraction, positive value"},
        {"16", -56.0, 6, "%6.0f", "   -56", "%f, no fraction, negative value"},
        {"17", 4887.12, 5, "%010.3f", "004887.120", "%f with zero padding, positive value"},
        {"18", -4887.12, 5, "%010.3f", "-04887.120", "%f with zero padding, negative value"},
        {"19", 0.0, 6, "%06.2f", "000.00", "%f, with zero padding, zero value"},
        {"20", 4.0, 6, "%07.0f", "0000004", "%f, zero padding, no fraction, positive value"},
        {"21", -56.0, 6, "%06.0f", "-00056", "%f, zero padding, no fraction, negative value"},
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
