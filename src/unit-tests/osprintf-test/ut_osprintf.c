/*
 * ut_osprintf.c
 *
 *  Created on: May 20, 2013
 *      Author: Kevin McCluney
 */

/*
** Includes
*/
#include "ut_osprintf.h"
#ifdef UT_DO_OFFSET
#include "ut_osprintf_offset.h"
#endif

#ifndef OSP_ARINC653
#include <stdio.h>

FILE *UT_logfile;
#endif

/*
** Global variables
*/
unsigned OS_printf_enabled = 1;
int ut_passed;
int ut_failed;
char cMsg[UT_MAX_MESSAGE_LENGTH];
char cNum[UT_MAX_MESSAGE_LENGTH];
char strg_buf[BUF_LEN];
char trunc_buf[BUF_LEN];
char fill_strg[BUF_LEN];

/*
** Functions
*/
#ifdef OSP_ARINC653
int os_printf_main()
#else
int main()
#endif
{
    int i;

#ifndef OSP_ARINC653
    UT_logfile = fopen("ut_osal_osprintf_log.txt", "w");
#endif

#ifdef OS_USE_EMBEDDED_PRINTF
    UT_Text("Using replacement printf functions\n\n");
#else
    UT_Text("Using standard printf functions\n\n");
#endif

    for (i = 0; i < BUF_LEN; i++)
    {
        fill_strg[i] = '*';
    }

    ut_passed = 0;
    ut_failed = 0;

    /* Test sprintf and snprintf for each format */
    UT_osprintf_d();
    UT_osprintf_i();
    UT_osprintf_u();
    UT_osprintf_x();
    UT_osprintf_X();
    UT_osprintf_p();
    UT_osprintf_c();
    UT_osprintf_s();
    UT_osprintf_ld();
    UT_osprintf_li();
    UT_osprintf_lu();
    UT_osprintf_lx();
    UT_osprintf_lX();

#ifdef UT_DO_FLOAT
    UT_osprintf_f();
    UT_osprintf_lf();
    UT_osprintf_misc();
#endif

#ifdef OS_USE_EMBEDDED_PRINTF
    UT_osprintf_printf();
#endif

    UT_ReportFailures();

#ifdef UT_DO_OFFSET
    /* Calculate argument offsets for cFE variadic functions */
    UT_osprintf_CalcOffsets();
#endif

    return 0;
}

/*
** Initialize string buffer to all asterisks
*/
void init_test(void)
{
    memcpy(strg_buf, fill_strg, BUF_LEN);
}

/*
** Compare expected string output to actual string output; return
** pass/fail result
*/
int check_test(char *expected, char *actual)
{
    int result = UT_PASS;

    if (memcmp(expected, actual, strlen(expected) + 1))
    {
        strcpy(cMsg, "  Mismatch: exp = [");
        strcat(cMsg, expected);
        strcat(cMsg, "], act = [");
        strcat(cMsg, actual);
        strcat(cMsg, "]\n");
        UT_Text(cMsg);
        result = UT_FAIL;
    }

    return result;
}

/*
** Output text
*/
void UT_Text(char *out_text)
{
#ifdef OSP_ARINC653
    TUTF_print(out_text);
#else
    fprintf(UT_logfile, "%s", out_text);
    fflush(UT_logfile);
#endif
}

/*
** Output result of a test
*/
void UT_Report(int test, char *fun_name, char *info,
               char *test_num, char *test_seq)
{
    if (test == UT_PASS)
    {
#ifdef UT_SHOW_PASS
        strcpy(cMsg, "PASSED  [");
        strcat(cMsg, fun_name);
        strcat(cMsg, ".");
        strcat(cMsg, test_num);
        strcat(cMsg, ".");
        strcat(cMsg, test_seq);
        strcat(cMsg, "] ");
        strcat(cMsg, info);
        strcat(cMsg, "\n-----\n");
        UT_Text(cMsg);
#endif
        ut_passed++;
    }
    else
    {
        strcpy(cMsg, "FAILED  [");
        strcat(cMsg, fun_name);
        strcat(cMsg, ".");
        strcat(cMsg, test_num);
        strcat(cMsg, ".");
        strcat(cMsg, test_seq);
        strcat(cMsg, "] ");
        strcat(cMsg, info);
        strcat(cMsg, "\n-----\n");
        UT_Text(cMsg);
        ut_failed++;
    }
}

/*
** Convert an integer to its character representation
*/
void UT_itoa(int value, char *string, int radix, int out_len)
{
    char revertedStr[50];
    int revertedLength = 0;
    unsigned int length = 0;
    unsigned int neg = 0;
    unsigned int digit_value = 0;
    unsigned int uvalue;

    if ((radix == 10) && (value < 0))
    {
        neg = 1;
        uvalue = -value;
    }
    else
    {
        uvalue = (unsigned int) value;
    }

    revertedLength = 0;

    while (uvalue >= radix)
    {
        digit_value = uvalue % radix;

        if (digit_value >= 10)
        {
            revertedStr[revertedLength] = digit_value - 10 + 'a';
        }
        else
        {
            revertedStr[revertedLength] = digit_value + '0';
        }

        uvalue /= radix;
        revertedLength++;
    }

    if (uvalue >= 10)
    {
        revertedStr[revertedLength] = uvalue - 10 + 'a';
    }
    else
    {
        revertedStr[revertedLength] = uvalue + '0';
    }

    if (neg == 1)
    {
        string[length] = '-';
        length++;
    }

    if (revertedLength < out_len - 1)
    {
        while (revertedLength < out_len - 1)
        {
            revertedLength++;
            revertedStr[revertedLength] = '0';
        }
    }

    while (revertedLength >= 0)
    {
        string[length] = revertedStr[revertedLength];
        revertedLength--;
        length++;
    }

    string[length] = '\0';
}

/*
** Output summary of test results (# of passed & failed tests)
*/
void UT_ReportFailures(void)
{
    char cNum[10];

    strcpy(cMsg, "\nosprintf PASSED ");
    UT_itoa(ut_passed, cNum, 10, 0);
    strcat(cMsg, cNum);
    strcat(cMsg, " tests.\n");
    strcat(cMsg, "osprintf FAILED ");
    UT_itoa(ut_failed, cNum, 10, 0);
    strcat(cMsg, cNum);
    strcat(cMsg, " tests.\n");
    UT_Text(cMsg);

#ifndef OSP_ARINC653
    printf("\nosprintf PASSED %d tests.\nosprintf FAILED %d tests.\n\n",
            ut_passed, ut_failed);

    /* Ensure everything gets written */
    fflush(stdout);
    fclose(UT_logfile);
#endif
}
