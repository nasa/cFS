/*================================================================================*
** File:  ut_oscore_misc_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_misc_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern UT_OsLogInfo_t g_logInfo;

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/* Test code template for testing a single OSAL API with multiple test cases */

#if 0
void UT_os_sample_test()
{
    /* Must declare these variables for each function. They can be renamed.
     * They're referenced in the macros used to track test cases and their results. */
    int32 idx = 0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc = NULL;

    /* Call this once at the beginning of the function to initialize the test variables. */
    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*
     * For each test case,
     *   1. Assign testDesc a brief description of the test
     *   2. Setup the test environment, if necessary
     *   3. Run the test
     *   4. Log result by calling UT_OS_SET_TEST_RESULT_MACRO
     *   4. Reset the test environment, if neccessary
     *
     * NOTE: "Not implemented" is always checked first but not
     *       being included as a test case.
     *       "Nominal" test case is always the last test case.
     *-----------------------------------------------------*/

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    /* TODO: Setup the test environment, if necessary */

    if (OS_xxx() == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_sample_test_exit_tag;
    }

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(NULL,...) == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#2 Name-too-long-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(aVeryLoooooongName) == OS_ERR_NAME_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(...) != OS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)

    /* TODO: Reset the test environment here, if necessary */

UT_os_sample_test_exit_tag:
    /* Call these macros at the very end of the function to close out the test variables
     * and get it added to the global list being tracked. */
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_xxx", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}
#endif

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_API_Init(void)
** Purpose: Initializes the tables that the OS API uses to keep track of information
**          about objects
** Parameters: None
** Returns: OS_ERROR on an unsuccessful inits
**          OS_SUCCESS on a successful inits
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Init-not-call-first condition
**   1) Don't call this routine first
**   2) Call OS_QueueCreate, OS_BinSemCreate, OS_CountSemCreate, OS_MutSemCreate
**   3) Expect the returned value from those routines to be
**       (a) __not__ OS_SUCCESS
*** -----------------------------------------------------
** Test #2: Nominal condition
**   1) Call this routine
**   2) Expect the returned value to be
**       (a) OS_SUCCESS (although results are not directly observable)
**   3) Call OS_QueueCreate, OS_BinSemCreate, OS_CountSemCreate, OS_MutSemCreate
**   4) Expect the returned value from those routines to be
**       (a) OS_SUCCESS
*--------------------------------------------------------------------------------*/
void UT_os_apiinit_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;
    uint32 qId = 0, qDepth = 10, qSize = 4, qFlags = 0;
    uint32 semIds[3], semInitValue = 1, semOptions = 0;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "#1 Init-not-call-first";

    if ((OS_QueueCreate(&qId, "Queue A", qDepth, qSize, qFlags) != OS_SUCCESS) &&
        (OS_BinSemCreate(&semIds[0], "BinSem 1", semInitValue, semOptions) != OS_SUCCESS) &&
        (OS_CountSemCreate(&semIds[1], "CountSem 1", semInitValue, semOptions) != OS_SUCCESS) &&
        (OS_MutSemCreate(&semIds[2], "MutexSem 1", semOptions) != OS_SUCCESS))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    OS_QueueDelete(qId);
    OS_BinSemDelete(semIds[0]);
    OS_CountSemDelete(semIds[1]);
    OS_MutSemDelete(semIds[2]);

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    res = OS_API_Init();
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        testDesc = "API not implemented";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_apiinit_test_exit_tag;
    }
    else if ((res == OS_SUCCESS) &&
             (OS_QueueCreate(&qId, "Queue A", qDepth, qSize, qFlags) == OS_SUCCESS) &&
             (OS_BinSemCreate(&semIds[0], "BinSem 1", semInitValue, semOptions) == OS_SUCCESS) &&
             (OS_CountSemCreate(&semIds[1], "CountSem 1", semInitValue, semOptions) == OS_SUCCESS) &&
             (OS_MutSemCreate(&semIds[2], "MutexSem 1", semOptions) == OS_SUCCESS))
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    }
    else
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

    /* Reset test environment */
    OS_QueueDelete(qId);
    OS_BinSemDelete(semIds[0]);
    OS_CountSemDelete(semIds[1]);
    OS_MutSemDelete(semIds[2]);

UT_os_apiinit_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_API_Init", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: void OS_printf(const char String, ...)
** Purpose: Provides a printing utility similar to printf
** Parameters: String - text portion of the print
**             ellipsis - the other parameters to print
** Returns: Nothing
** -----------------------------------------------------
** Test #1: Nominal condition
**   1) Call OS_printf_enable
**   2) Call this routine to print a text string.
**   3) Visually observe that the text string in #2 did get print.
**--------------------------------------------------------------------------------*/
void UT_os_printf_test()
{
    int32 idx = 0;
    UT_OsApiInfo_t apiInfo;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    OS_printf_enable();
    UT_OS_LOG_MACRO("OS_printf() - #1 Nominal [This is the expected output after API call]\n")

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, "#1 Nominal - Manual inspection required", UT_OS_MIR)

    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_printf", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: void OS_printf_enable(void)
** Purpose: Enables the UART or console output of OS_printf
** Parameters: None
** Returns: Nothing
** Test #1: Nominal condition
**   1) Call OS_printf_disable
**   2) Call OS_printf to print a text string.
**   3) Call this routine
**   4) Call OS_printf to print a text string that's different than #2
**   5) Visually observe that text string in #2 did not print, but text string in #4 did.
**--------------------------------------------------------------------------------*/
void UT_os_printfenable_test()
{
    int32 idx = 0;
    UT_OsApiInfo_t apiInfo;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    OS_printf_disable();

    OS_printf_enable();
    UT_OS_LOG_MACRO("OS_printf_enable() - #1 Nominal [This is the expected output after API call]\n")

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, "#1 Nominal - Manual inspection required", UT_OS_MIR)

    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_printf_enable", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: void OS_printf_disable(void)
** Purpose: Disables the UART or console output of OS_printf
** Parameters: None
** Returns: Nothing
** Test #1: Nominal condition
**   1) Call OS_printf_enable
**   2) Call OS_printf() to print a text string.
**   3) Call this routine
**   4) Call OS_printf() to print a text string that's different than #2
**   5) Visually observe that text string in #2 did get print, but text string in #4 did not.
**--------------------------------------------------------------------------------*/
void UT_os_printfdisable_test()
{
    int32 idx = 0;
    UT_OsApiInfo_t apiInfo;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    OS_printf_enable();
    UT_OS_LOG_MACRO("OS_printf_disable() - #1 Nominal [This is the expected output before API call]\n")

    OS_printf_disable();
    UT_OS_LOG_MACRO("OS_printf_disable() - #1 Nominal [This is NOT the expected output after API call]\n")

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, "#1 Nominal - Manual inspection required", UT_OS_MIR)

    /* Reset test environment */
    OS_printf_enable();
    UT_OS_LOG_MACRO("OS_printf_disable() - #1 Nominal [This is the expected output after test reset]\n")

    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_printf_disable", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_Tick2Micros(void)
** Purpose: Returns the number of microseconds per OS tick
** Parameters: None
** Returns: Microseconds per OS tick
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Nominal condition
**   1) Call this routine
**   2) Expect the returned value to be
**       (a) a value greater than or equal to 0
**--------------------------------------------------------------------------------*/
void UT_os_tick2micros_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    const char* testDesc = NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_Tick2Micros();
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_tick2micros_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Nominal";

    res = OS_Tick2Micros();
    if (res >= 0)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

UT_os_tick2micros_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_Tick2Micros", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_Milli2Ticks(uint32 milli_seconds)
** Purpose: Returns the equivalent number of system clock ticks for the given period
**          of time in milliseconds
** Parameters: milli_seconds - the number of milliseconds to convert to ticks
** Returns: Number of ticks in the given period of milliseconds
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Zero-value-argument condition
**   1) Call this routine with a zero argument
**   2) Expect the returned value to be
**        (a) exactly 0
** -----------------------------------------------------
** Test #2: Nominal condition
**   1) Call this routine with a non-zero argument, T, in milli_seconds
**   2) Expect the returned value to be
**        (a) a value greater than or equal to 0
**--------------------------------------------------------------------------------*/
void UT_os_milli2ticks_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    uint32 timeInMillisecs = 0;
    const char* testDesc = NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_Milli2Ticks(timeInMillisecs);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_milli2ticks_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 zero-value-arg";

    timeInMillisecs = 0;
    res = OS_Milli2Ticks(timeInMillisecs);
    if (res == 0)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    timeInMillisecs = 100;
    res = OS_Milli2Ticks(timeInMillisecs);
    if (res >= 0)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

UT_os_milli2ticks_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_Milli2Ticks", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_GetLocalTime(OS_time_t *time_struct)
** Purpose: Returns the local time of the machine it is on
** Parameters: *time_struct - a pointer to an OS_time_t structure that will hold
**                            the current time in seconds and milliseconds
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null-pointer argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: OS-call-failure condition
**   1) Setup test to cause the OS call to fail inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call this routine with valid argument
**   2) Expect the returned value to be
**        (a) OS_SUCCESS (although results are not directly observable)
**--------------------------------------------------------------------------------*/
void UT_os_getlocaltime_test()
{
    OS_time_t time_struct;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc = NULL;
    char text[UT_OS_LG_TEXT_LEN];
    int32 res = 0, idx = 0, i = 0, j = 0;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_GetLocalTime(NULL);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_getlocaltime_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";
    res = OS_GetLocalTime(NULL);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    res = OS_GetLocalTime(&time_struct);
    if (res == OS_SUCCESS)
    {
    	UT_OS_LOG_MACRO("\n")
        for (i=0; i < 5; i++)
        {
            UT_OS_LOG_MACRO("OS_GetLocalTime() - #3 Nominal ")
            memset(text, '\0', sizeof(text));
            UT_os_sprintf(text, "[Expecting output after API call to increase over time: %d.%d]\n",
                                time_struct.seconds, time_struct.microsecs);
            UT_OS_LOG_MACRO(text)

            for (j=0; j < 10000; j++) { }

            OS_GetLocalTime(&time_struct);
        }

        testDesc = "#3 Nominal - Manual inspection required";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_MIR)
    }
    else
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

UT_os_getlocaltime_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_GetLocalTime", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_SetLocalTime(OS_time_t *time_struct)
** Purpose: Allows user to set the local time of the machine it is on
** Parameters: *time_struct - a pointer to an OS_time_t structure that holds the
**                            target time in seconds and milliseconds
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null-pointer argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: OS-call-failure condition
**   1) Setup the test to cause the OS call to fail inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #3: Nominal condition
**   - Call this routine with valid argument
**   - Expect the returned value to be
**       (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_setlocaltime_test()
{
    OS_time_t time_struct;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc = NULL;
    char text[UT_OS_LG_TEXT_LEN];
    int32 res = 0, idx = 0, i = 0, j = 0;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_SetLocalTime(NULL);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_setlocaltime_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    res = OS_GetLocalTime(NULL);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    res = OS_GetLocalTime(&time_struct);
    if (res == OS_SUCCESS)
    {
    	UT_os_printf("\n");
		for (i=0; i < 5; i++)
		{
			UT_OS_LOG_MACRO("OS_SetLocalTime() - #3 Nominal ")
			memset(text, '\0', sizeof(text));
			UT_os_sprintf(text, "[Expecting output before API call to increase over time: %d.%d]\n",
						        time_struct.seconds, time_struct.microsecs);
            UT_OS_LOG_MACRO(text)

			for (j=0; j < 10000; j++) { }

			OS_GetLocalTime(&time_struct);
		}
    }

    memset(&time_struct, 0x00, sizeof(time_struct));
    time_struct.seconds = 20000;
    time_struct.microsecs = 123;

    res = OS_SetLocalTime(&time_struct);
    if (res == OS_SUCCESS)
    {
    	memset(text, '\0', sizeof(text));
        UT_os_sprintf(text, "OS_SetLocalTime() - #3 Nominal [New time set at %d.%d]\n",
                            time_struct.seconds, time_struct.microsecs);
        UT_OS_LOG_MACRO(text)

        res = OS_GetLocalTime(&time_struct);
        if (res == OS_SUCCESS)
        {
        	UT_os_printf("\n");
			for (i=0; i < 5; i++)
			{
				UT_OS_LOG_MACRO("OS_SetLocalTime() - #3 Nominal ")
		    	memset(text, '\0', sizeof(text));
				UT_os_sprintf(text, "[Expecting output after API call to increase over time: %d.%d]\n",
							        time_struct.seconds, time_struct.microsecs);
				UT_OS_LOG_MACRO(text)

				for (j=0; j < 10000; j++) { }

				OS_GetLocalTime(&time_struct);
			}
        }

        testDesc = "#3 Nominal - Manual inspection required";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_MIR)
    }
    else
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    }

UT_os_setlocaltime_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_SetLocalTime", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: void OS_GetErrorName(int32 error_num, os_err_name_t * err_name)
** Purpose: Returns the string name of the error value
** Parameters: error_num - an error value
**             *err_name - pointer to hold the returned string name
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null-pointer argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Nominal condition
**   - Call this routine with valid argument
**   - Expect the returned value to be
**       (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_geterrorname_test(void)
{
    UT_OsApiInfo_t apiInfo;
    int32 res = 0, idx = 0;
    os_err_name_t errNames[4];
    const char* testDesc = NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_GetErrorName(OS_SUCCESS, &errNames[0]);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_geterrorname_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    res = OS_GetErrorName(OS_ERROR, NULL);
    if (res == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    if ((OS_GetErrorName(OS_ERR_NAME_TOO_LONG, &errNames[0]) == OS_SUCCESS) &&
        (strcmp(errNames[0], "OS_ERR_NAME_TOO_LONG") == 0) &&
        (OS_GetErrorName(OS_ERR_NAME_TAKEN, &errNames[1]) == OS_SUCCESS) &&
        (strcmp(errNames[1], "OS_ERR_NAME_TAKEN") == 0) &&
        (OS_GetErrorName(OS_ERR_NO_FREE_IDS, &errNames[2]) == OS_SUCCESS) &&
        (strcmp(errNames[2], "OS_ERR_NO_FREE_IDS") == 0) &&
        (OS_GetErrorName(12345, &errNames[3]) == OS_ERROR) &&
        (strcmp(errNames[3], "ERROR_UNKNOWN") == 0))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

UT_os_geterrorname_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_GetErrorName", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*================================================================================*
** End of File: ut_oscore_misc_test.c
**================================================================================*/
