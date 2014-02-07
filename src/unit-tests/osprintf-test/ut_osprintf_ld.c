/*
 * UT_osprintf_ld.c
 *
 *  Created on: May 22, 2013
 *      Author: Kevin McCluney
 */

#include "ut_osprintf.h"

extern char strg_buf[];
extern char trunc_buf[];

/*****************************************************************************
 *  Test %ld format
 *****************************************************************************/
void UT_osprintf_ld(void)
{
    char *test_fmt = "ld"; /* Test format character(s) */
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
        {"01", 12798765, 5, "%ld", "12798765", "%ld, positive value"},
        {"02", 43246372, 9, "$$$%ld$$$", "$$$43246372$$$", "%ld embedded, positive value"},
        {"03", 63198765, 9, "% ld", " 63198765", "%ld with space for sign, positive value"},
        {"04", 77691827, 8, "%11ld", "   77691827", "%ld with minimum field size > number of digits, positive value"},
        {"05", 54691827, 5, "%4ld", "54691827", "%ld with minimum field size < number of digits, positive value"},
        {"06", 77833225, 7, "%.10ld", "0077833225", "%ld with precision field size > number of digits, positive value"},
        {"07", 99933225, 6, "%.3ld", "99933225", "%ld with precision field size < number of digits, positive value"},
        {"08", 12345789, 8, "%12.10ld", "  0012345789", "%ld with minimum field size > precision field size > number of digits, positive value"},
        {"09", 12345987, 7, "%12.3ld", "    12345987", "%ld with minimum field size > number of digits > precision field size, positive value"},
        {"10", 12345444, 8, "%4.2ld", "12345444", "%ld with number of digits > minimum field size > precision field size, positive value"},
        {"11", 12345321, 10, "%10.12ld", "000012345321", "%ld with precision field size > minimum field size > number of digits, positive value"},
        {"12", 12333345, 9, "%6.12ld", "000012333345", "%ld with precision field size > number of digits > minimum field size, positive value"},
        {"13", 12345777, 8, "%2.4ld", "12345777", "%ld with number of digits > precision field size > minimum field size, positive value"},
        {"14", 98765321, 15, "%-.20ld", "00000000000098765321", "%ld with left-justify and precision field size > number of digits, positive value"},
        {"15", 98765111, 8, "%-.3ld", "98765111", "%ld with left-justify and precision field size < number of digits, positive value"},
        {"16", 98765222, 8, "%+ld", "+98765222", "%ld with sign, positive value"},
        {"17", 46372333, 7, "$$$%+ld$$$", "$$$+46372333$$$", "%ld sign and embedded, positive value"},
        {"18", 91827444, 6, "%+11ld", "  +91827444", "%ld with sign and minimum field size > number of digits, positive value"},
        {"19", 91827555, 5, "%+4ld", "+91827555", "%ld with sign and minimum field size < number of digits, positive value"},
        {"20", 33225666, 7, "%+.13ld", "+0000033225666", "%ld with sign and precision field size > number of digits, positive value"},
        {"21", 33225777, 8, "%+.3ld", "+33225777", "%ld with sign and precision field size < number of digits, positive value"},
        {"22", 12345888, 9, "%+12.10ld", " +0012345888", "%ld with sign and minimum field size > precision field size > number of digits, positive value"},
        {"23", 12345999, 10, "%+12.3ld", "   +12345999", "%ld with sign and minimum field size > number of digits > precision field size, positive value"},
        {"24", 12345000, 8, "%+4.2ld", "+12345000", "%ld with sign and number of digits > minimum field size > precision field size, positive value"},
        {"25", 12345121, 9, "%+10.12ld", "+000012345121", "%ld with sign and precision field size > minimum field size > number of digits, positive value"},
        {"26", 12345232, 8, "%+6.12ld", "+000012345232", "%ld with sign and precision field size > number of digits > minimum field size, positive value"},
        {"27", 12345343, 6, "%+2.4ld", "+12345343", "%ld with sign and number of digits > precision field size > minimum field size, positive value"},
        {"28", 98765454, 19, "%+-.20ld", "+00000000000098765454", "%ld with sign and left-justify and precision field size > number of digits, positive value"},
        {"29", 98765565, 7, "%+-.3ld", "+98765565", "%ld with sign and left-justify and precision field size < number of digits, positive value"},
        {"30", 98765676, 8, "%+ld", "+98765676", "%ld with sign, positive value"},
        {"31", -98765787, 9, "%ld", "-98765787", "%ld, negative value"},
        {"32", -46372898, 10, "$$$%ld$$$", "$$$-46372898$$$", "%ld embedded, negative value"},
        {"33", -98765909, 9, "% ld", "-98765909", "%ld with space for sign, negative value"},
        {"34", -91827121, 8, "%13ld", "    -91827121", "%ld with minimum field size > number of digits, negative value"},
        {"35", -91827232, 5, "%4ld", "-91827232", "%ld with minimum field size < number of digits, negative value"},
        {"36", -33225343, 8, "%.13ld", "-0000033225343", "%ld with precision field size > number of digits, negative value"},
        {"37", -33225454, 6, "%.3ld", "-33225454", "%ld with precision field size < number of digits, negative value"},
        {"38", -12345565, 7, "%12.10ld", " -0012345565", "%ld with minimum field size > precision field size > number of digits, negative value"},
        {"39", -12345676, 8, "%12.4ld", "   -12345676", "%ld with minimum field size > number of digits > precision field size, negative value"},
        {"40", -12345787, 9, "%4.2ld", "-12345787", "%ld with number of digits > minimum field size > precision field size, negative value"},
        {"41", -12345898, 11, "%7.12ld", "-000012345898", "%ld with precision field size > minimum field size > number of digits, negative value"},
        {"42", -12345909, 10, "%3.12ld", "-000012345909", "%ld with precision field size > number of digits > minimum field size, negative value"},
        {"43", -12345101, 9, "%2.4ld", "-12345101", "%ld with number of digits > precision field size > minimum field size, negative value"},
        {"44", -98765292, 10, "%-.20ld", "-00000000000098765292", "%ld with left-justify and precision field size > number of digits, negative value"},
        {"45", -98765383, 8, "%-.3ld", "-98765383", "%ld with left-justify and precision field size < number of digits, negative value"},
        {"46", -98765474, 9, "%+ld", "-98765474", "%ld with sign, negative value"},
        {"47", -46372565, 8, "$$$%+ld$$$", "$$$-46372565$$$", "%ld sign and embedded, negative value"},
        {"48", -91827112, 7, "%+11ld", "  -91827112", "%ld with sign and minimum field size > number of digits, negative value"},
        {"49", -91827223, 6, "%+4ld", "-91827223", "%ld with sign and minimum field size < number of digits, negative value"},
        {"50", -33225334, 11, "%+.13ld", "-0000033225334", "%ld with sign and precision field size > number of digits, negative value"},
        {"51", -33225445, 9, "%+.3ld", "-33225445", "%ld with sign and precision field size < number of digits, negative value"},
        {"52", -12345556, 11, "%+12.10ld", " -0012345556", "%ld with sign and minimum field size > precision field size > number of digits, negative value"},
        {"53", -12345667, 10, "%+12.3ld", "   -12345667", "%ld with sign and minimum field size > number of digits > precision field size, negative value"},
        {"54", -12345778, 9, "%+4.2ld", "-12345778", "%ld with sign and number of digits > minimum field size > precision field size, negative value"},
        {"55", -12345889, 10, "%+7.12ld", "-000012345889", "%ld with sign and precision field size > minimum field size > number of digits, negative value"},
        {"56", -12345990, 9, "%+3.12ld", "-000012345990", "%ld with sign and precision field size > number of digits > minimum field size, negative value"},
        {"57", -12345221, 8, "%+2.4ld", "-12345221", "%ld with sign and number of digits > precision field size > minimum field size, negative value"},
        {"58", -98765332, 7, "%+-.20ld", "-00000000000098765332", "%ld with sign and left-justify and precision field size > number of digits, negative value"},
        {"59", -98765443, 6, "%+-.3ld", "-98765443", "%ld with sign and left-justify and precision field size < number of digits, negative value"},
        {"60", -98765554, 5, "%+ld", "-98765554", "%ld with sign, negative value"},
        {"61", 0, 6, "%ld", "0", "%ld, zero value"},
        {"62", 16253409, 5, "%010ld", "0016253409", "%ld with zero padding, positive value"},
        {"63", -16253409, 6, "%010ld", "-016253409", "%ld with zero padding, negative value"},
        {"64", 0, 6, "%012ld", "000000000000", "%ld, with zero padding, zero value"},
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
