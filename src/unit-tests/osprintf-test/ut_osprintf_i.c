/*
 * UT_osprintf_i.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];
extern char trunc_buf[];

/*****************************************************************************
 *  Test %i format
 *****************************************************************************/
void UT_osprintf_i(void)
{
    char *test_fmt = "i"; /* Test format character(s) */
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
        {"01", 98765, 5, "%i", "98765", "%i, positive value"},
        {"02", 46372, 9, "$$$%i$$$", "$$$46372$$$", "%i embedded, positive value"},
        {"03", 98765, 5, "% i", " 98765", "%i with space for sign, positive value"},
        {"04", 91827, 7, "%8i", "   91827", "%i with minimum field size > number of digits, positive value"},
        {"05", 91827, 2, "%4i", "91827", "%i with minimum field size < number of digits, positive value"},
        {"06", 33225, 7, "%.10i", "0000033225", "%i with precision field size > number of digits, positive value"},
        {"07", 33225, 3, "%.3i", "33225", "%i with precision field size < number of digits, positive value"},
        {"08", 12345, 5, "%9.7i", "  0012345", "%i with minimum field size > precision field size > number of digits, positive value"},
        {"09", 12345, 5, "%9.3i", "    12345", "%i with minimum field size > number of digits > precision field size, positive value"},
        {"10", 12345, 5, "%4.2i", "12345", "%i with number of digits > minimum field size > precision field size, positive value"},
        {"11", 12345, 8, "%7.9i", "000012345", "%i with precision field size > minimum field size > number of digits, positive value"},
        {"12", 12345, 7, "%3.9i", "000012345", "%i with precision field size > number of digits > minimum field size, positive value"},
        {"13", 12345, 4, "%2.4i", "12345", "%i with number of digits > precision field size > minimum field size, positive value"},
        {"14", 98765, 5, "%-.20i", "00000000000000098765", "%i with left-justify and precision field size > number of digits, positive value"},
        {"15", 98765, 5, "%-.3i", "98765", "%i with left-justify and precision field size < number of digits, positive value"},
        {"16", 98765, 5, "%+i", "+98765", "%i with sign, positive value"},
        {"17", 46372, 5, "$$$%+d$$$", "$$$+46372$$$", "%i sign and embedded, positive value"},
        {"18", 91827, 6, "%+8i", "  +91827", "%i with sign and minimum field size > number of digits, positive value"},
        {"19", 91827, 4, "%+4i", "+91827", "%i with sign and minimum field size < number of digits, positive value"},
        {"20", 33225, 8, "%+.10i", "+0000033225", "%i with sign and precision field size > number of digits, positive value"},
        {"21", 33225, 5, "%+.3i", "+33225", "%i with sign and precision field size < number of digits, positive value"},
        {"22", 12345, 5, "%+9.7i", " +0012345", "%i with sign and minimum field size > precision field size > number of digits, positive value"},
        {"23", 12345, 5, "%+9.3i", "   +12345", "%i with sign and minimum field size > number of digits > precision field size, positive value"},
        {"24", 12345, 5, "%+4.2i", "+12345", "%i with sign and number of digits > minimum field size > precision field size, positive value"},
        {"25", 12345, 8, "%+7.9i", "+000012345", "%i with sign and precision field size > minimum field size > number of digits, positive value"},
        {"26", 12345, 7, "%+3.9i", "+000012345", "%i with sign and precision field size > number of digits > minimum field size, positive value"},
        {"27", 12345, 5, "%+2.4i", "+12345", "%i with sign and number of digits > precision field size > minimum field size, positive value"},
        {"28", 98765, 16, "%+-.20i", "+00000000000000098765", "%i with sign and left-justify and precision field size > number of digits, positive value"},
        {"29", 98765, 5, "%+-.3i", "+98765", "%i with sign and left-justify and precision field size < number of digits, positive value"},
        {"30", 98765, 4, "%+i", "+98765", "%i with sign, positive value"},
        {"31", -98765, 6, "%i", "-98765", "%i, negative value"},
        {"32", -46372, 6, "$$$%i$$$", "$$$-46372$$$", "%i embedded, negative value"},
        {"33", -98765, 5, "% i", "-98765", "%i with space for sign, negative value"},
        {"34", -91827, 9, "%10i", "    -91827", "%i with minimum field size > number of digits, negative value"},
        {"35", -91827, 6,  "%4i", "-91827", "%i with minimum field size < number of digits, negative value"},
        {"36", -33225, 9, "%.10i", "-0000033225", "%i with precision field size > number of digits, negative value"},
        {"37", -33225, 5, "%.3i", "-33225", "%i with precision field size < number of digits, negative value"},
        {"38", -12345, 8, "%9.7i", " -0012345", "%i with minimum field size > precision field size > number of digits, negative value"},
        {"39", -12345, 7, "%9.3i", "   -12345", "%i with minimum field size > number of digits > precision field size, negative value"},
        {"40", -12345, 6, "%4.2i", "-12345", "%i with number of digits > minimum field size > precision field size, negative value"},
        {"41", -12345, 7, "%7.9i", "-000012345", "%i with precision field size > minimum field size > number of digits, negative value"},
        {"42", -12345, 8, "%3.9i", "-000012345", "%i with precision field size > number of digits > minimum field size, negative value"},
        {"43", -12345, 5, "%2.4i", "-12345", "%i with number of digits > precision field size > minimum field size, negative value"},
        {"44", -98765, 18, "%-.20i", "-00000000000000098765", "%i with left-justify and precision field size > number of digits, negative value"},
        {"45", -98765, 5, "%-.3i", "-98765", "%i with left-justify and precision field size < number of digits, negative value"},
        {"46", -98765, 6, "%+i", "-98765", "%i with sign, negative value"},
        {"47", -46372, 7, "$$$%+d$$$", "$$$-46372$$$", "%i sign and embedded, negative value"},
        {"48", -91827, 5, "%+8i", "  -91827", "%i with sign and minimum field size > number of digits, negative value"},
        {"49", -91827, 5, "%+4i", "-91827", "%i with sign and minimum field size < number of digits, negative value"},
        {"50", -33225, 7, "%+.10i", "-0000033225", "%i with sign and precision field size > number of digits, negative value"},
        {"51", -33225, 5, "%+.3i", "-33225", "%i with sign and precision field size < number of digits, negative value"},
        {"52", -12345, 7, "%+9.7i", " -0012345", "%i with sign and minimum field size > precision field size > number of digits, negative value"},
        {"53", -12345, 8, "%+9.3i", "   -12345", "%i with sign and minimum field size > number of digits > precision field size, negative value"},
        {"54", -12345, 4, "%+4.2i", "-12345", "%i with sign and number of digits > minimum field size > precision field size, negative value"},
        {"55", -12345, 8, "%+7.9i", "-000012345", "%i with sign and precision field size > minimum field size > number of digits, negative value"},
        {"56", -12345, 7, "%+3.9i", "-000012345", "%i with sign and precision field size > number of digits > minimum field size, negative value"},
        {"57", -12345, 5, "%+2.4i", "-12345", "%i with sign and number of digits > precision field size > minimum field size, negative value"},
        {"58", -98765, 19, "%+-.20i", "-00000000000000098765", "%i with sign and left-justify and precision field size > number of digits, negative value"},
        {"59", -98765, 6, "%+-.3i", "-98765", "%i with sign and left-justify and precision field size < number of digits, negative value"},
        {"60", -98765, 5, "%+i", "-98765", "%i with sign, negative value"},
        {"61", 0, 6, "%i", "0", "%i, zero value"},
        {"62", 162534, 5, "%08i", "00162534", "%i with zero padding, positive value"},
        {"63", -162534, 6, "%08i", "-0162534", "%i with zero padding, negative value"},
        {"64", 0, 6, "%04i", "0000", "%i, with zero padding, zero value"},
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

