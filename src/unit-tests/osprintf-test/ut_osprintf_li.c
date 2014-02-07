/*
 * UT_osprintf_li.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];
extern char trunc_buf[];

/*****************************************************************************
 *  Test %li format
 *****************************************************************************/
void UT_osprintf_li(void)
{
    char *test_fmt = "li"; /* Test format character(s) */
    int i;

    struct
    {
        char     *test_num;    /* Test identifier; sequential numbers */
        long int test_val;     /* Test value */
        int      max_len;      /* Maximum output string length */
        char     *format;      /* Format string */
        char     *expected;    /* Expected result */
        char     *description; /* Test description */
    } osp_tests[] =
    {
        {"01", 12798765, 7, "%li", "12798765", "%li, positive value"},
        {"02", 43246372, 9, "$$$%li$$$", "$$$43246372$$$", "%li embedded, positive value"},
        {"03", 63198765, 8, "% li", " 63198765", "%li with space for sign, positive value"},
        {"04", 77691827, 7, "%11li", "   77691827", "%li with minimum field size > number of digits, positive value"},
        {"05", 54691827, 5, "%4li", "54691827", "%li with minimum field size < number of digits, positive value"},
        {"06", 77833225, 8, "%.10li", "0077833225", "%li with precision field size > number of digits, positive value"},
        {"07", 99933225, 5, "%.3li", "99933225", "%li with precision field size < number of digits, positive value"},
        {"08", 12345789, 10, "%12.10li", "  0012345789", "%li with minimum field size > precision field size > number of digits, positive value"},
        {"09", 12345987, 12, "%12.3li", "    12345987", "%li with minimum field size > number of digits > precision field size, positive value"},
        {"10", 12345444, 8, "%4.2li", "12345444", "%li with number of digits > minimum field size > precision field size, positive value"},
        {"11", 12345321, 11, "%10.12li", "000012345321", "%li with precision field size > minimum field size > number of digits, positive value"},
        {"12", 12333345, 7, "%6.12li", "000012333345", "%li with precision field size > number of digits > minimum field size, positive value"},
        {"13", 12345777, 5, "%2.4li", "12345777", "%li with number of digits > precision field size > minimum field size, positive value"},
        {"14", 98765321, 19, "%-.20li", "00000000000098765321", "%li with left-justify and precision field size > number of digits, positive value"},
        {"15", 98765111, 7, "%-.3li", "98765111", "%li with left-justify and precision field size < number of digits, positive value"},
        {"16", 98765222, 10, "%+li", "+98765222", "%li with sign, positive value"},
        {"17", 46372333, 9, "$$$%+li$$$", "$$$+46372333$$$", "%li sign and embedded, positive value"},
        {"18", 91827444, 8, "%+11li", "  +91827444", "%li with sign and minimum field size > number of digits, positive value"},
        {"19", 91827555, 7, "%+4li", "+91827555", "%li with sign and minimum field size < number of digits, positive value"},
        {"20", 33225666, 2, "%+.13li", "+0000033225666", "%li with sign and precision field size > number of digits, positive value"},
        {"21", 33225777, 5, "%+.3li", "+33225777", "%li with sign and precision field size < number of digits, positive value"},
        {"22", 12345888, 9, "%+12.10li", " +0012345888", "%li with sign and minimum field size > precision field size > number of digits, positive value"},
        {"23", 12345999, 8, "%+12.3li", "   +12345999", "%li with sign and minimum field size > number of digits > precision field size, positive value"},
        {"24", 12345000, 9, "%+4.2li", "+12345000", "%li with sign and number of digits > minimum field size > precision field size, positive value"},
        {"25", 12345121, 11, "%+10.12li", "+000012345121", "%li with sign and precision field size > minimum field size > number of digits, positive value"},
        {"26", 12345232, 10, "%+6.12li", "+000012345232", "%li with sign and precision field size > number of digits > minimum field size, positive value"},
        {"27", 12345343, 7, "%+2.4li", "+12345343", "%li with sign and number of digits > precision field size > minimum field size, positive value"},
        {"28", 98765454, 15, "%+-.20li", "+00000000000098765454", "%li with sign and left-justify and precision field size > number of digits, positive value"},
        {"29", 98765565, 8, "%+-.3li", "+98765565", "%li with sign and left-justify and precision field size < number of digits, positive value"},
        {"30", 98765676, 7, "%+li", "+98765676", "%li with sign, positive value"},
        {"31", -98765787, 6, "%li", "-98765787", "%li, negative value"},
        {"32", -46372898, 15, "$$$%li$$$", "$$$-46372898$$$", "%li embedded, negative value"},
        {"33", -98765909, 9, "% li", "-98765909", "%li with space for sign, negative value"},
        {"34", -91827121, 11, "%13li", "    -91827121", "%li with minimum field size > number of digits, negative value"},
        {"35", -91827232, 5, "%4li", "-91827232", "%li with minimum field size < number of digits, negative value"},
        {"36", -33225343, 8, "%.13li", "-0000033225343", "%li with precision field size > number of digits, negative value"},
        {"37", -33225454, 7, "%.3li", "-33225454", "%li with precision field size < number of digits, negative value"},
        {"38", -12345565, 9, "%12.10li", " -0012345565", "%li with minimum field size > precision field size > number of digits, negative value"},
        {"39", -12345676, 8, "%12.4li", "   -12345676", "%li with minimum field size > number of digits > precision field size, negative value"},
        {"40", -12345787, 4, "%4.2li", "-12345787", "%li with number of digits > minimum field size > precision field size, negative value"},
        {"41", -12345898, 10, "%7.12li", "-000012345898", "%li with precision field size > minimum field size > number of digits, negative value"},
        {"42", -12345909, 9, "%3.12li", "-000012345909", "%li with precision field size > number of digits > minimum field size, negative value"},
        {"43", -12345101, 7, "%2.4li", "-12345101", "%li with number of digits > precision field size > minimum field size, negative value"},
        {"44", -98765292, 18, "%-.20li", "-00000000000098765292", "%li with left-justify and precision field size > number of digits, negative value"},
        {"45", -98765383, 7, "%-.3li", "-98765383", "%li with left-justify and precision field size < number of digits, negative value"},
        {"46", -98765474, 8, "%+li", "-98765474", "%li with sign, negative value"},
        {"47", -46372565, 10, "$$$%+li$$$", "$$$-46372565$$$", "%li sign and embedded, negative value"},
        {"48", -91827112, 9, "%+11li", "  -91827112", "%li with sign and minimum field size > number of digits, negative value"},
        {"49", -91827223, 5, "%+4li", "-91827223", "%li with sign and minimum field size < number of digits, negative value"},
        {"50", -33225334, 11, "%+.13li", "-0000033225334", "%li with sign and precision field size > number of digits, negative value"},
        {"51", -33225445, 8, "%+.3li", "-33225445", "%li with sign and precision field size < number of digits, negative value"},
        {"52", -12345556, 12, "%+12.10li", " -0012345556", "%li with sign and minimum field size > precision field size > number of digits, negative value"},
        {"53", -12345667, 10, "%+12.3li", "   -12345667", "%li with sign and minimum field size > number of digits > precision field size, negative value"},
        {"54", -12345778, 6, "%+4.2li", "-12345778", "%li with sign and number of digits > minimum field size > precision field size, negative value"},
        {"55", -12345889, 9, "%+7.12li", "-000012345889", "%li with sign and precision field size > minimum field size > number of digits, negative value"},
        {"56", -12345990, 11, "%+3.12li", "-000012345990", "%li with sign and precision field size > number of digits > minimum field size, negative value"},
        {"57", -12345221, 7, "%+2.4li", "-12345221", "%li with sign and number of digits > precision field size > minimum field size, negative value"},
        {"58", -98765332, 15, "%+-.20li", "-00000000000098765332", "%li with sign and left-justify and precision field size > number of digits, negative value"},
        {"59", -98765443, 7, "%+-.3li", "-98765443", "%li with sign and left-justify and precision field size < number of digits, negative value"},
        {"60", -98765554, 5, "%+li", "-98765554", "%li with sign, negative value"},
        {"61", 0, 6, "%li", "0", "%li, zero value"},
        {"62", 16253409, 5, "%010li", "0016253409", "%li with zero padding, positive value"},
        {"63", -16253409, 6, "%010li", "-016253409", "%li with zero padding, negative value"},
        {"64", 0, 6, "%012li", "000000000000", "%li, with zero padding, zero value"},
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
