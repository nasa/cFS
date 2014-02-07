/*================================================================================*
** File:  ut_os_stubs.c
** Owner: Tam Ngo
** Date:  May 2013
**================================================================================*/

#include "ut_os_stubs.h"

#include "ut_os_stub_platforms.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/* As defined in osfilesys.c */
extern OS_VolumeInfo_t  OS_VolumeTable[NUM_TABLE_ENTRIES];

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

UT_OsLogInfo_t  g_logInfo;

/*--------------------------------------------------------------------------------*
** Sets up test environment
**--------------------------------------------------------------------------------*/
void UT_os_setup(const char* logFileName)
{
    g_logInfo.nPassed = 0;
    g_logInfo.nFailed = 0;
    g_logInfo.nMir    = 0;
    g_logInfo.nNa     = 0;
    g_logInfo.nUof    = 0;
    g_logInfo.nTsf    = 0;

    g_logInfo.totalTstCnt = 0;

    g_logInfo.apiCnt = 0;
    memset(g_logInfo.apis, 0x00, sizeof(g_logInfo.apis));

    g_logInfo.verboseLevel = 0;

#ifndef OS_USE_EMBEDDED_PRINTF
    g_logInfo.logFD = NULL;
    if (logFileName != NULL)
    {
        g_logInfo.logFD = fopen(logFileName, "w");
        if (g_logInfo.logFD == NULL)
            fprintf(stderr, "Error: Can't open file %s\n", logFileName);
    }
#endif  /* !OS_USE_EMBEDDED_PRINTF */
}

/*--------------------------------------------------------------------------------*
** Tears down test setup
**--------------------------------------------------------------------------------*/
void UT_os_teardown(const char* testSuiteName)
{
    UT_os_log_test_summaries(testSuiteName);
    UT_os_log_test_results(testSuiteName);

#ifndef OS_USE_EMBEDDED_PRINTF
    if (g_logInfo.logFD != NULL)
    {
        fflush(g_logInfo.logFD);
        fclose(g_logInfo.logFD);
        g_logInfo.logFD = NULL;
    }
#endif  /* !OS_USE_EMBEDDED_PRINTF */
}

/*--------------------------------------------------------------------------------*
** Logs an API test descriptions
**--------------------------------------------------------------------------------*/
void UT_os_log_api(UT_OsApiInfo_t* apiPtr)
{
    if (apiPtr != NULL)
    {
        int32 i = 0;
        UT_OsApiInfo_t*  apiInfo  = NULL;
        UT_OsTestDesc_t* testInfo = NULL;
        UT_OsTestDesc_t* testPtr  = NULL;

        apiInfo = &(g_logInfo.apis[g_logInfo.apiCnt]);
        memset(apiInfo, 0x00, sizeof(UT_OsApiInfo_t));

        strncpy(apiInfo->name, apiPtr->name, strlen(apiPtr->name));

        apiInfo->tstCnt = apiPtr->tstCnt;
        g_logInfo.totalTstCnt += apiInfo->tstCnt;

        for (i=0; i < apiInfo->tstCnt; i++)
        {
            testPtr  = &(apiPtr->tests[i]);
            testInfo = &(apiInfo->tests[i]);

            memset(testInfo, 0x00, sizeof(UT_OsTestDesc_t));

            strncpy(testInfo->name,   testPtr->name,   strlen(testPtr->name));
            strncpy(testInfo->result, testPtr->result, strlen(testPtr->result));

            if (strcmp(testInfo->result, UT_OS_PASSED) == 0)
                g_logInfo.nPassed++;
            else if (strcmp(testInfo->result, UT_OS_FAILED) == 0)
                g_logInfo.nFailed++;
            else if (strcmp(testInfo->result, UT_OS_MIR) == 0)
                g_logInfo.nMir++;
            else if (strcmp(testInfo->result, UT_OS_NA) == 0)
                g_logInfo.nNa++;
            else if (strcmp(testInfo->result, UT_OS_UOF) == 0)
                g_logInfo.nUof++;
            else if (strcmp(testInfo->result, UT_OS_TSF) == 0)
            	g_logInfo.nTsf++;
        }

        g_logInfo.apiCnt++;
    }
}

/*--------------------------------------------------------------------------------*
** Set verbose level for logging
**--------------------------------------------------------------------------------*/
void UT_os_set_log_verbose(uint32 vLevel)
{
    switch (vLevel)
    {
        case UT_OS_LOG_OFF:
        case UT_OS_LOG_MINIMAL:
        case UT_OS_LOG_MODERATE:
        case UT_OS_LOG_EVERYTHING:
            g_logInfo.verboseLevel = vLevel;
            break;

        default:
            g_logInfo.verboseLevel = UT_OS_LOG_EVERYTHING;
            break;
    }
}

/*--------------------------------------------------------------------------------*
** Set return code
**--------------------------------------------------------------------------------*/
void UT_os_set_return_code(UT_OsReturnCode_t* ret, int32 count, int32 value)
{
    if (ret != NULL)
    {
        ret->count = count;
        ret->value = value;
    }
}

/*--------------------------------------------------------------------------------*
** Log test summaries
**--------------------------------------------------------------------------------*/
void UT_os_log_test_summaries(const char* testSuiteName)
{
    char* txtPtr = NULL;
    char txtStr[UT_OS_LG_TEXT_LEN];
    const char* tstName = testSuiteName;

    if (tstName == NULL)
        tstName = " ";

    txtPtr = "\n========================================================\n";
    UT_OS_LOG_MACRO(txtPtr)
    memset(txtStr, '\0', sizeof(txtStr));
    UT_os_sprintf(txtStr, "%s TOTAL APIs: %d\n", tstName, (int)g_logInfo.apiCnt);
    UT_OS_LOG_MACRO(txtStr)
    memset(txtStr, '\0', sizeof(txtStr));
    UT_os_sprintf(txtStr, "%s TOTAL TEST CASES: %d\n\n", tstName, (int)g_logInfo.totalTstCnt);
    UT_OS_LOG_MACRO(txtStr)
    memset(txtStr, '\0', sizeof(txtStr));
    UT_os_sprintf(txtStr, "%s PASSED %3d tests.\n", tstName, (int)g_logInfo.nPassed);
    UT_OS_LOG_MACRO(txtStr)
    memset(txtStr, '\0', sizeof(txtStr));
    UT_os_sprintf(txtStr, "%s FAILED %3d tests.\n", tstName, (int)g_logInfo.nFailed);
    UT_OS_LOG_MACRO(txtStr)

    memset(txtStr, '\0', sizeof(txtStr));
    UT_os_sprintf(txtStr, "\n%s contains %2d tests that are untested OS-call-failure.\n",
                          tstName, (int)g_logInfo.nUof);
    UT_OS_LOG_MACRO(txtStr)
    memset(txtStr, '\0', sizeof(txtStr));
    UT_os_sprintf(txtStr, "%s contains %2d tests that are manual-inspection-required.\n",
                          tstName, (int)g_logInfo.nMir);
    UT_OS_LOG_MACRO(txtStr)
    memset(txtStr, '\0', sizeof(txtStr));
    UT_os_sprintf(txtStr, "\n%s contains %2d tests that are not-applicable.\n",
                          tstName, (int)g_logInfo.nNa);
    UT_OS_LOG_MACRO(txtStr)

    if (g_logInfo.nTsf > 0)
    {
        memset(txtStr, '\0', sizeof(txtStr));
        UT_os_sprintf(txtStr, "\n%s contains %2d tests that are test-setup-failure.\n",
                              tstName, (int)g_logInfo.nTsf);
        UT_OS_LOG_MACRO(txtStr)
    }

    txtPtr = "========================================================\n";
    UT_OS_LOG_MACRO(txtPtr)
}

/*--------------------------------------------------------------------------------*
** Print log info
**--------------------------------------------------------------------------------*/
void UT_os_log_test_results(const char* testSuiteName)
{
    int32 i = 0, j = 0;
    char* txtPtr = NULL;
    UT_OsApiInfo_t* apiPtr = NULL;
    char txtStr[UT_OS_LG_TEXT_LEN];
    const char* tstName = testSuiteName;

    if (tstName == NULL)
        tstName = " ";

    if (g_logInfo.verboseLevel >= UT_OS_LOG_MODERATE)
    {
        txtPtr = "\n--------------------------------------------------------\n";
        UT_OS_LOG_MACRO(txtPtr)
        memset(txtStr, '\0', sizeof(txtStr));
        UT_os_sprintf(txtStr, "%s TOTAL APIs: %d\n", tstName, (int)g_logInfo.apiCnt);
        UT_OS_LOG_MACRO(txtStr)
        memset(txtStr, '\0', sizeof(txtStr));
        UT_os_sprintf(txtStr, "%s TOTAL TEST CASES: %d\n", tstName, (int)g_logInfo.totalTstCnt);
        UT_OS_LOG_MACRO(txtStr)
        txtPtr = "--------------------------------------------------------";
        UT_OS_LOG_MACRO(txtPtr)

        for (i=0; i < g_logInfo.apiCnt; i++)
        {
            apiPtr = &(g_logInfo.apis[i]);
            memset(txtStr, '\0', sizeof(txtStr));
            UT_os_sprintf(txtStr, "\n    %s: %d\n", apiPtr->name, (int)apiPtr->tstCnt);
            UT_OS_LOG_MACRO(txtStr)
            for (j=0; j < apiPtr->tstCnt; j++)
            {
                memset(txtStr, '\0', sizeof(txtStr));
                UT_os_sprintf(txtStr, "        %s [%s]\n", apiPtr->tests[j].name,
                                      apiPtr->tests[j].result);
                UT_OS_LOG_MACRO(txtStr)
            }
        }
    }

    UT_os_log_test_result_category("PASSED", UT_OS_PASSED, g_logInfo.nPassed);
    UT_os_log_test_result_category("FAILED", UT_OS_FAILED, g_logInfo.nFailed);
    UT_os_log_test_result_category("MANUAL-INSPECTION-REQUIRED", UT_OS_MIR, g_logInfo.nMir);
    UT_os_log_test_result_category("NOT-APPLICABLE", UT_OS_NA, g_logInfo.nNa);
    UT_os_log_test_result_category("UNTESTED-OS-CALL-FAILURE", UT_OS_UOF, g_logInfo.nUof);

    if (g_logInfo.nTsf > 0)
    {
        UT_os_log_test_result_category("TEST-SETUP-FAILURE", UT_OS_TSF, g_logInfo.nTsf);
    }

    if (g_logInfo.verboseLevel != UT_OS_LOG_OFF)
    {
        txtPtr = "\n\n";
        UT_OS_LOG_MACRO(txtPtr);
    }
}

/*--------------------------------------------------------------------------------*
** Print a test-result category
**--------------------------------------------------------------------------------*/
void UT_os_log_test_result_category(const char* catName, const char* catKey, uint32 nCases)
{
    int32 i = 0, j = 0;
    char* txtPtr = NULL;
    UT_OsApiInfo_t* apiPtr = NULL;
    const char* inCatKey = catKey;
    char txtStr[UT_OS_LG_TEXT_LEN];
    const char* inCatName = catName;

    if (inCatKey == NULL)
        inCatKey = " ";

    if (inCatName == NULL)
        inCatName = " ";

    if (g_logInfo.verboseLevel >= UT_OS_LOG_MODERATE)
    {
        txtPtr = "\n--------------------------------------------------------\n";
        UT_OS_LOG_MACRO(txtPtr)
        memset(txtStr, '\0', sizeof(txtStr));
        UT_os_sprintf(txtStr, "  TOTAL TEST CASES %s -> %d\n", inCatName, (int)nCases);
        UT_OS_LOG_MACRO(txtStr)
        txtPtr = "--------------------------------------------------------\n";
        UT_OS_LOG_MACRO(txtPtr)

        for (i=0; i < g_logInfo.apiCnt; i++)
        {
            apiPtr = &(g_logInfo.apis[i]);
            for (j=0; j < apiPtr->tstCnt; j++)
            {
                if (strcmp(apiPtr->tests[j].result, inCatKey) == 0)
                {
                    memset(txtStr, '\0', sizeof(txtStr));
                    UT_os_sprintf(txtStr, "    %s [ ] %s - %s \n",
                                          inCatKey, apiPtr->name, apiPtr->tests[j].name);
                    UT_OS_LOG_MACRO(txtStr)
                }
            }
        }
    }
}

/*--------------------------------------------------------------------------------*
** Print OS_FDTable[]
**--------------------------------------------------------------------------------*/
void UT_os_print_fdtable(const char* outputDesc)
{
#ifdef UT_VERBOSE
    char* txtPtr = NULL;
    char aLine[UT_OS_LG_TEXT_LEN], sVal[UT_OS_XS_TEXT_LEN];
    uint32 j = 0, col0 = 1, col1 = 8, col2 = 14, col3 = 44, col4 = 52;

    if (outputDesc)
        UT_OS_LOG_MACRO(outputDesc)

    memset(aLine, ' ', sizeof(aLine));
    memcpy(&aLine[col1], "FD", strlen("FD"));
    memcpy(&aLine[col2], "PATH", strlen("PATH"));
    memcpy(&aLine[col3-2], "USERID", strlen("USERID"));
    memcpy(&aLine[col4-2], "VALID?", strlen("VALID?"));
    aLine[col4+strlen("VALID?")] = '\0';

    txtPtr = "\n----------------------------------------------------------\n";
    UT_OS_LOG_MACRO(txtPtr)
    UT_OS_LOG_MACRO(aLine)
    txtPtr = "\n----------------------------------------------------------\n";
    UT_OS_LOG_MACRO(txtPtr)

    txtPtr = "\n";
    for (j=0; j < OS_MAX_NUM_OPEN_FILES; j++)
    {
        memset(aLine, ' ', sizeof(aLine));

        UT_OS_PRINT_FDTABLE_CONTENT_MACRO

        aLine[col4+strlen(sVal)] = '\0';

        UT_OS_LOG_MACRO(aLine)
        UT_OS_LOG_MACRO(txtPtr)
    }

    UT_OS_LOG_MACRO(txtPtr)
#endif  /* UT_VERBOSE */
}

/*--------------------------------------------------------------------------------*
** Print OS_VolumeTable[]
**--------------------------------------------------------------------------------*/
void UT_os_print_volumetable(const char* outputDesc)
{
#ifdef UT_VERBOSE
    char* txtPtr = NULL;
    char aLine[UT_OS_LG_TEXT_LEN], sVal[UT_OS_XS_TEXT_LEN];
    uint32 j = 0, col0 = 1, col1 = 8, col2 = 24, col3 = 56;
    uint32 col4 = 70, col5 = 88, col6 = 96, col7 = 104, col8 = 114;

    if (outputDesc)
        UT_OS_LOG_MACRO(outputDesc)

    memset(aLine, ' ', sizeof(aLine));
    memcpy(&aLine[col1], "DEVNAME", strlen("DEVNAME"));
    memcpy(&aLine[col2], "PHYSDEV", strlen("PHYSDEV"));
    memcpy(&aLine[col3], "VOLNAME", strlen("VOLNAME"));
    memcpy(&aLine[col4], "MOUNTPNT", strlen("MOUNTPNT"));
    memcpy(&aLine[col5-5], "VOLATILE?", strlen("VOLATILE?"));
    memcpy(&aLine[col6-2], "FREE?", strlen("FREE?"));
    memcpy(&aLine[col7-3], "MOUNTED?", strlen("MOUNTED?"));
    memcpy(&aLine[col8-3], "BLOCKSIZE", strlen("BLOCKSIZE"));
    aLine[col8+strlen("BLOCKSIZE")] = '\0';

    txtPtr = "\n--------------------------------------------------------------";
    UT_OS_LOG_MACRO(txtPtr)
    txtPtr = "-------------------------------------------------------------\n";
    UT_OS_LOG_MACRO(txtPtr)
    UT_OS_LOG_MACRO(aLine)
    txtPtr = "\n--------------------------------------------------------------";
    UT_OS_LOG_MACRO(txtPtr)
    txtPtr = "-------------------------------------------------------------\n";
    UT_OS_LOG_MACRO(txtPtr)

    txtPtr = "\n";
    for (j=0; j < NUM_TABLE_ENTRIES; j++)
    {
        memset(aLine, ' ', sizeof(aLine));

        memset(sVal, '\0', sizeof(sVal));
        UT_os_sprintf(sVal, "[%d]", (int)j);
        memcpy(&aLine[col0], sVal, strlen(sVal));
        memcpy(&aLine[col1], OS_VolumeTable[j].DeviceName, strlen(OS_VolumeTable[j].DeviceName));
        memcpy(&aLine[col2], OS_VolumeTable[j].PhysDevName, strlen(OS_VolumeTable[j].PhysDevName));
        memcpy(&aLine[col3], OS_VolumeTable[j].VolumeName, strlen(OS_VolumeTable[j].VolumeName));
        memcpy(&aLine[col4], OS_VolumeTable[j].MountPoint, strlen(OS_VolumeTable[j].MountPoint));
        memset(sVal, '\0', sizeof(sVal));
        UT_os_sprintf(sVal, "%d", (int)OS_VolumeTable[j].VolatileFlag);
        memcpy(&aLine[col5], sVal, strlen(sVal));
        memset(sVal, '\0', sizeof(sVal));
        UT_os_sprintf(sVal, "%d", (int)OS_VolumeTable[j].FreeFlag);
        memcpy(&aLine[col6], sVal, strlen(sVal));
        memset(sVal, '\0', sizeof(sVal));
        UT_os_sprintf(sVal, "%d", (int)OS_VolumeTable[j].IsMounted);
        memcpy(&aLine[col7], sVal, strlen(sVal));
        memset(sVal, '\0', sizeof(sVal));
        UT_os_sprintf(sVal, "%d", (int)OS_VolumeTable[j].BlockSize);
        memcpy(&aLine[col8], sVal, strlen(sVal));

        aLine[col8+strlen(sVal)] = '\0';

        UT_OS_LOG_MACRO(aLine)
        UT_OS_LOG_MACRO(txtPtr)
    }

    UT_OS_LOG_MACRO(txtPtr)
#endif  /* UT_VERBOSE */
}

/*================================================================================*
** End of File: ut_os_stubs.c
**================================================================================*/
