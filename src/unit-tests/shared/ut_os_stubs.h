/*================================================================================*
** File:  ut_os_stubs.h
** Owner: Tam Ngo
** Date:  May 2013
**================================================================================*/

#ifndef _UT_OS_STUBS_H_
#define _UT_OS_STUBS_H_

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include <stdarg.h>

#include <stdlib.h>
#include <string.h>

#include "common_types.h"
#include "osapi.h"
#include "osconfig.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/* Only use by developers for debugging; comment out when not debugging */
/*#define OS_LOG_TO_STDOUT_TOO*/

#define UT_OS_LOG_OFF         0
#define UT_OS_LOG_MINIMAL     1
#define UT_OS_LOG_MODERATE    2
#define UT_OS_LOG_EVERYTHING  3

#define UT_OS_PASSED  "PASSED"
#define UT_OS_FAILED  "FAILED"
#define UT_OS_MIR     "_MIR_ "      /* Manual Inspection Required */
#define UT_OS_NA      "__NA__"      /* Not Applicable */
#define UT_OS_UOF     "_UOF_ "      /* Untested OS-call Failure */
#define UT_OS_TSF     "_TSF_ "      /* Test Setup Failure */

#define UT_OS_XS_LIST_LEN   25
#define UT_OS_SM_LIST_LEN   50
#define UT_OS_MD_LIST_LEN  100
#define UT_OS_LG_LIST_LEN  200

#define UT_OS_XS_TEXT_LEN   25
#define UT_OS_SM_TEXT_LEN   75
#define UT_OS_MD_TEXT_LEN  125
#define UT_OS_LG_TEXT_LEN  250

/*--------------------------------------------------------------------------------*/

#define UT_OS_CLEAR_API_INFO_MACRO(VAR1, TSTIDX)   \
    memset(&(VAR1), 0x00, sizeof(UT_OsApiInfo_t)); \
    TSTIDX = 0;

/*--------------------------------------------------------------------------------*/

#define UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(VAR1, APINAME, TSTCNT) \
{                                                                      \
    memset(VAR1.name, '\0', sizeof(VAR1.name));                        \
    strncpy(VAR1.name, APINAME, strlen(APINAME));                      \
    VAR1.tstCnt = TSTCNT;                                              \
}

/*--------------------------------------------------------------------------------*/

#define UT_OS_SET_TEST_RESULT_MACRO(VAR1, TSTIDX, TSTNAME, TSTRESULT)  \
{                                                                      \
    memset(&(VAR1.tests[TSTIDX]), 0x00, sizeof(UT_OsTestDesc_t));      \
    strncpy(VAR1.tests[TSTIDX].name, TSTNAME, strlen(TSTNAME));        \
    strncpy(VAR1.tests[TSTIDX].result, TSTRESULT, strlen(TSTRESULT));  \
    (TSTIDX)++;                                                        \
}

/*--------------------------------------------------------------------------------*/

#define UT_OS_LOG_API_MACRO(VAR1) \
    UT_os_log_api(&VAR1);

/*--------------------------------------------------------------------------------*/

#ifdef OS_USE_EMBEDDED_PRINTF
#define UT_os_printf   OS_printf
#define UT_os_sprintf  OS_sprintf
#else
#define UT_os_printf   printf
#define UT_os_sprintf  sprintf
#endif  /* OS_USE_EMBEDDED_PRINTF */

/*--------------------------------------------------------------------------------*/

#ifdef OS_USE_EMBEDDED_PRINTF
#define UT_OS_LOG_MACRO(aVAR)  UT_os_printf(aVAR);
#else
#ifdef OS_LOG_TO_STDOUT_TOO
#define UT_OS_LOG_MACRO(aVAR)     \
    UT_os_printf(aVAR);           \
    if (g_logInfo.logFD != NULL)  \
        fprintf(g_logInfo.logFD, aVAR);
#else
#define UT_OS_LOG_MACRO(aVAR)    \
    if (g_logInfo.logFD != NULL) \
        fprintf(g_logInfo.logFD, aVAR);
#endif  /* OS_LOG_TO_STDOUT_TOO */
#endif /* OS_USE_EMBEDDED_PRINTF */

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

typedef struct
{
    char  name[UT_OS_SM_TEXT_LEN];
    char  result[UT_OS_SM_TEXT_LEN];
} UT_OsTestDesc_t;

typedef struct
{
    char             name[UT_OS_SM_TEXT_LEN];
    uint32           tstCnt;
    UT_OsTestDesc_t  tests[UT_OS_XS_LIST_LEN];
} UT_OsApiInfo_t;

typedef struct
{
#ifndef OS_USE_EMBEDDED_PRINTF
    FILE*           logFD;
#endif  /* !OS_USE_EMBEDDED_PRINTF */
    uint32          verboseLevel;
    uint32          apiCnt;
    UT_OsApiInfo_t  apis[UT_OS_LG_LIST_LEN];
    uint32          totalTstCnt;
    uint32          nPassed;
    uint32          nFailed;
    uint32          nMir;
    uint32          nNa;
    uint32          nUof;
    uint32          nTsf;
} UT_OsLogInfo_t;

typedef struct
{
    int32  count;
    int32  value;
} UT_OsReturnCode_t;

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function prototypes
**--------------------------------------------------------------------------------*/

void UT_os_setup(const char* logFileName);
void UT_os_teardown(const char* testSuiteName);

void UT_os_log_api(UT_OsApiInfo_t* apiPtr);

void UT_os_set_log_verbose(uint32 verboseLevel);
void UT_os_set_return_code(UT_OsReturnCode_t* ret, int32 count, int32 value);

void UT_os_log_test_summaries(const char* testSuiteName);
void UT_os_log_test_results(const char* testSuiteName);
void UT_os_log_test_result_category(const char* catName, const char* catKey, uint32 nCases);

void UT_os_print_fdtable(const char* outputDesc);
void UT_os_print_volumetable(const char* outputDesc);

/*--------------------------------------------------------------------------------*/

#endif  /* _UT_OS_STUBS_H_ */

/*================================================================================*
** End of File: ut_os_stubs.h
**================================================================================*/
