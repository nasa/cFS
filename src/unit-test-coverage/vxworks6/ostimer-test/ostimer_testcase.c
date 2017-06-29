/*
 * Filename: ostimer_testcase.c
 *
 * Purpose: This file contains unit test cases
 *
 * Modification History:
 *   06/29/2015, A. Brown, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include "osapi.h"

#include "utassert.h"
#include "uttest.h"
#include "utlist.h"

#include "ostimer_stubs.h"

#include "ostimer_copiedstatictypes.h"
#include "ostimer_accessor.h"

/* ------------------ Copied info from unit under test ---------------------- */


/* --------------- Prototypes for non-exported functions -------------------- */


/* -------------------- Special Test Case Variables ------------------------- */



/* ------------------------Utility functions--------------------------------- */

/* OsTimer_ReinitStaticOSTimerTable
 *
 * Reinitialize ostimer.c's static OS_timer_table.
 * Only call this after initStaticPointers();
 */
void OsTimer_ReinitStaticOSTimerTable(void)
{
    unsigned int i, j;
    OS_timer_record_t* tbl = getOSTimeTablePtr();

    for ( i = 0; i < OS_MAX_TIMERS; i++ )
    {
       tbl->free      = TRUE;
       tbl->creator   = 0;
       for ( j = 0; j < OS_MAX_API_NAME; j++ )
       {
           tbl->name[j] = '\0';
       }
       tbl->creator   = 0;
       tbl->start_time = 0;
       tbl->interval_time = 0;
       tbl->accuracy = 0;
       tbl->callback_ptr = (OS_TimerCallback_t)NULL;
       tbl->host_timerid = 0;
       tbl++;
    }
}

/* OsTimer_SetStaticOSTimerTableNotFree
 *
 * Reinitialize ostimer.c's static OS_timer_table so that
 * all indicators are entry->free = FALSE;
 * Only call this after initStaticPointers();
 */
void OsTimer_SetStaticOSTimerTableNotFree(void)
{
    unsigned int i;
    OS_timer_record_t* tbl = getOSTimeTablePtr();

    for ( i = 0; i < OS_MAX_TIMERS; i++ )
    {
       tbl->free = FALSE;
       tbl++;
    }
}

/* OsTimer_countStaticOSTimerTableUsed
 * Counts the entry->free elements in
 * the OS_timer_table are FALSE.
 * Only call this after initStaticPointers();
 */
unsigned int OsTimer_countStaticOSTimerTableUsed(void)
{
    unsigned int i;
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    unsigned int retval = 0;

    for ( i = 0; i < OS_MAX_TIMERS; i++ )
    {
      if (tbl->free == FALSE) {
	retval++;
      }
      tbl++;
    }
    return retval;
}

/* OsTimer_isStaticOSTimerTableFree
 * Checks that all of the entry->free elements in
 * the OS_timer_table are TRUE.
 * Only call this after initStaticPointers();
 */
boolean OsTimer_isStaticOSTimerTableFree(void)
{
  return (OsTimer_countStaticOSTimerTableUsed() == 0);
}

/* ---------------------  Begin test cases  --------------------------------- */

/*******************************************************************************
**
**  OS_TimerAPIInit() Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TimerAPIInit_Success(void)
{
    /* Setup Inputs */
    OsTimer_SetReturnCode(OSTIMER_SEMMCREATE_INDEX, 1, 1); /* success */

    /* Execute Test */
    int32 retval = OS_TimerAPIInit();

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "Checking OS_SUCCESS");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TimerAPIInit_clock_getresFAIL(void)
{
    /* Setup Inputs */
    OsTimer_SetReturnCode(OSTIMER_CLOCKGETRES_INDEX, -1, 1); /* fail */
    OsTimer_SetReturnCode(OSTIMER_SEMMCREATE_INDEX, 1, 1); /* success */

    /* Execute Test */
    int32 retval = OS_TimerAPIInit();

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERROR, "Checking OS_ERROR");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TimerAPIInit_semMCreateFAIL(void)
{
    /* Setup Inputs */
    OsTimer_SetReturnCode(OSTIMER_SEMMCREATE_INDEX, (int32)NULL, 1); /* fail */

    /* Execute Test */
    int32 retval = OS_TimerAPIInit();

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERROR, "Checking OS_ERROR");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}

/*******************************************************************************
**
**  OS_UsecToTimespec() Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_UsecToTimespec_1(void)
{
    /* Setup Inputs */
    uint32 usecs = 10;
    struct timespec ts = {0, 0};

    /* Execute Test */
    OS_UsecToTimespec(usecs, &ts);

    /* Verify Outputs */
    UtAssert_True(ts.tv_sec == 0, "tv_sec == 0");
    UtAssert_True(ts.tv_nsec == (10*1000), "tv_nsec == 10k");
}
/*----------------------------------------------------------------------------*/
void Test_OS_UsecToTimespec_2(void)
{
    /* Setup Inputs */
    uint32 usecs = 10*1000000;
    struct timespec ts = {0, 0};

    /* Execute Test */
    OS_UsecToTimespec(usecs, &ts);

    /* Verify Outputs */
    UtAssert_True(ts.tv_sec == 10, "tv_sec == 10");
    UtAssert_True(ts.tv_nsec == 0, "tv_nsec == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_UsecToTimespec_NullPtrArg(void)
{
    /* Setup Inputs */
    uint32 usecs = 10*1000000;

    /* Execute Test */
    OS_UsecToTimespec(usecs, 0);

    /* Verify Outputs */
    UtAssert_True(TRUE, "segfault avoided!");
}

/*******************************************************************************
**
**  OS_TimespecToUsec() Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TimespecToUsec_1(void)
{
    /* Setup Inputs */
    uint32 usecs = 0;
    struct timespec ts = {0, (10*1000)};

    /* Execute Test */
    OS_TimespecToUsec(ts, &usecs);

    /* Verify Outputs */
    UtAssert_True(usecs == 10, "usecs == 10");
}
/*----------------------------------------------------------------------------*/
/* The same test as 2b, with the output unset before the call.  This shouldn't
 * have an effect on behavior, but it does.  Fails before issue 62 is resolved,
 * passes after.
 * See https://babelfish.arc.nasa.gov/trac/cfs_osal/ticket/62 */
void Test_OS_TimespecToUsec_2(void)
{
    /* Setup Inputs */
    uint32 usecs = 0;
    uint32 max_secs_in_usec = getMaxSecsInUSec();
    struct timespec ts = {max_secs_in_usec+1U, 0};

    /* Execute Test */
    OS_TimespecToUsec(ts, &usecs);

    /* Verify Outputs */
    /* NOTE THE TRUNCATION - we lost a second */
    UtAssert_True(usecs == ((getMaxSecsInUSec()) * 1000000), "usecs == max");
}
/*----------------------------------------------------------------------------*/
/* The same test as 2, with the output set before the call.
 * Passes before and after issue 62 fix.
 * See https://babelfish.arc.nasa.gov/trac/cfs_osal/ticket/62 */
void Test_OS_TimespecToUsec_2b(void)
{
    /* Setup Inputs */
    uint32 usecs = 1000000; /* set the output to get past the if-test */
    struct timespec ts = {getMaxSecsInUSec()+1, 0};

    /* Execute Test */
    OS_TimespecToUsec(ts, &usecs);

    /* Verify Outputs */
    /* NOTE THE TRUNCATION - we lost a second */
    UtAssert_True(usecs == ((getMaxSecsInUSec()) * 1000000), "usecs == max");
}
/*----------------------------------------------------------------------------*/
/* The same test as 3b, with the output unset before the call.  This shouldn't
 * have an effect on behavior, but it does.  Fails before issue 62 is resolved,
 * passes after.
 * See https://babelfish.arc.nasa.gov/trac/cfs_osal/ticket/62 */
void Test_OS_TimespecToUsec_3(void)
{
    /* Setup Inputs */
    uint32 usecs = 0;
    struct timespec ts = {10, 0};

    /* Execute Test */
    OS_TimespecToUsec(ts, &usecs);

    /* Verify Outputs */
    UtAssert_True(usecs == (10*1000000), "usecs == 10million");
}
/*----------------------------------------------------------------------------*/
/* The same test as 3, with the output set before the call.
 * Passes before and after issue 62 fix.
 * See https://babelfish.arc.nasa.gov/trac/cfs_osal/ticket/62 */
void Test_OS_TimespecToUsec_3b(void)
{
    /* Setup Inputs */
    uint32 usecs = 1000000; /* set the output to get past the if-test */
    struct timespec ts = {10, 0};

    /* Execute Test */
    OS_TimespecToUsec(ts, &usecs);

    /* Verify Outputs */
    UtAssert_True(usecs == (10*1000000), "usecs == 10million");
}

/*----------------------------------------------------------------------------*/
/* A max-range check for a signed int value of usecs.
 * See https://babelfish.arc.nasa.gov/trac/cfs_osal/ticket/62 */
void Test_OS_TimespecToUsec_4(void)
{
    /* Setup Inputs */
    uint32 usecs_in = 2147483647;
    struct timespec ts = {0, 0};
    uint32 usecs_out = 0;

    /* Execute Test */
    OS_UsecToTimespec(usecs_in, &ts);
    OS_TimespecToUsec(ts, &usecs_out);

    /* Verify Outputs */
    UtAssert_True(usecs_in == usecs_out, "usecs_in == usecs_out");
    UtAssert_True(ts.tv_sec >= 0, "ts.tv_sec >= 0");
    UtAssert_True(ts.tv_nsec >= 0, "ts.tv_nsec >= 0");
}
/*----------------------------------------------------------------------------*/
/* A max-range check to confirm where truncation begins.
 * See https://babelfish.arc.nasa.gov/trac/cfs_osal/ticket/62 */
void Test_OS_TimespecToUsec_4b(void)
{
    /* Setup Inputs */
    uint32 usecs_in = 4294000000U; /* truncation begins */
    struct timespec ts = {0, 0};
    uint32 usecs_out = 0;

    /* Execute Test */
    OS_UsecToTimespec(usecs_in, &ts);
    OS_TimespecToUsec(ts, &usecs_out);

    /* Verify Outputs */
    UtAssert_True(usecs_in != usecs_out, "usecs_in != usecs_out");
    UtAssert_True(ts.tv_sec >= 0, "ts.tv_sec >= 0");
    UtAssert_True(ts.tv_nsec >= 0, "ts.tv_nsec >= 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimespecToUsec_NullPtrArg(void)
{
    /* Setup Inputs */
    struct timespec ts = {0, (10*1000)};

    /* Execute Test */
    OS_TimespecToUsec(ts, 0);

    /* Verify Outputs */
    UtAssert_True(TRUE, "segfault avoided!");
}
/*******************************************************************************
**
**  OS_TimerCreate() Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TimerCreate(void) /* Nominal behavior test */
{
    /* Setup Inputs */
    uint32 id = 0;
    uint32 *timer_id = &id;
    const char name[] = "test name";
    uint32 ca = 1000;
    uint32 *clock_accuracy = &ca;
    OS_TimerCallback_t  callback_ptr = (OS_TimerCallback_t)1;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Execute Test */
    retval = OS_TimerCreate(timer_id, &name[0], clock_accuracy, callback_ptr);

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == 1, "OsTimer_countStaticOSTimerTableUsed() == 1");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerCreate_NullArg1(void)
{
    /* Setup Inputs */
    const char name[] = "test name";
    uint32 ca = 1000;
    uint32 *clock_accuracy = &ca;
    OS_TimerCallback_t  callback_ptr = (OS_TimerCallback_t)1;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Execute Test */
    retval = OS_TimerCreate(NULL, &name[0], clock_accuracy, callback_ptr);

    /* Verify Outputs */
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerCreate_NullArg2(void)
{
    /* Setup Inputs */
    uint32 id = 0;
    uint32 *timer_id = &id;
    uint32 ca = 1000;
    uint32 *clock_accuracy = &ca;
    OS_TimerCallback_t  callback_ptr = (OS_TimerCallback_t)1;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Execute Test */
    retval = OS_TimerCreate(timer_id, NULL, clock_accuracy, callback_ptr);

    /* Verify Outputs */
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerCreate_NullArg3(void)
{
    /* Setup Inputs */
    uint32 id = 0;
    uint32 *timer_id = &id;
    const char name[] = "test name";
    OS_TimerCallback_t  callback_ptr = (OS_TimerCallback_t)1;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Execute Test */
    retval = OS_TimerCreate(timer_id, &name[0], NULL, callback_ptr);

    /* Verify Outputs */
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerCreate_NullArg4(void)
{
    /* Setup Inputs */
    uint32 id = 0;
    uint32 *timer_id = &id;
    const char name[] = "test name";
    uint32 ca = 1000;
    uint32 *clock_accuracy = &ca;
    OS_TimerCallback_t  callback_ptr = NULL;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Execute Test */
    retval = OS_TimerCreate(timer_id, &name[0], clock_accuracy, callback_ptr);

    /* Verify Outputs */
    UtAssert_True(retval == OS_TIMER_ERR_INVALID_ARGS, "retval == OS_TIMER_ERR_INVALID_ARGS");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerCreate_NameTooLong(void)
{
    /* Setup Inputs */
    uint32 id = 0;
    uint32 *timer_id = &id;
    const char name[] = "test nameTOOOOOOOOOLOOOOOOOOONGGGGGGGGG!!!!!!!";
    uint32 ca = 1000;
    uint32 *clock_accuracy = &ca;
    OS_TimerCallback_t  callback_ptr = (OS_TimerCallback_t)1;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Execute Test */
    retval = OS_TimerCreate(timer_id, &name[0], clock_accuracy, callback_ptr);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_NAME_TOO_LONG, "retval == OS_ERR_NAME_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerCreate_NameMaxLength(void)
{
    /* Setup Inputs */
    uint32 id = 0;
    uint32 *timer_id = &id;
    /* 20 chars including \0, OS_MAX_API_NAME, should be successful */
    const char name[20] = "0123456789012345678\0";
    uint32 ca = 1000;
    uint32 *clock_accuracy = &ca;
    OS_TimerCallback_t  callback_ptr = (OS_TimerCallback_t)1;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Execute Test */
    retval = OS_TimerCreate(timer_id, &name[0], clock_accuracy, callback_ptr);

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == 1, "OsTimer_countStaticOSTimerTableUsed() == 1");

    OS_timer_record_t* tbl = getOSTimeTablePtr();
    UtAssert_MemCmp(name, tbl->name, OS_MAX_API_NAME, "Checking Name Copy In Table");
    UtAssert_True(tbl->name[OS_MAX_API_NAME-1] == 0, "timer name end with NULL char");
    UtAssert_True(strlen(tbl->name) <= OS_MAX_API_NAME, "Time name length");
}
/*----------------------------------------------------------------------------*/
/* See https://babelfish.arc.nasa.gov/trac/cfs_osal/ticket/65
 * This test has failures before the issue #65 fix, but should pass after. */
void Test_OS_TimerCreate_NameMaxLengthPlus1(void)
{
    /* Setup Inputs */
    uint32 id = 0;
    uint32 *timer_id = &id;
    const char name[21] = "01234567890123456789\0"; /* 21 chars with, OS_MAX_API_NAME+1 */
    uint32 ca = 1000;
    uint32 *clock_accuracy = &ca;
    OS_TimerCallback_t  callback_ptr = (OS_TimerCallback_t)1;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Execute Test */
    retval = OS_TimerCreate(timer_id, &name[0], clock_accuracy, callback_ptr);

    /* Verify Outputs */
    /* The given arg is too long with its terminating null. */
    UtAssert_True(retval == OS_ERR_NAME_TOO_LONG, "retval == OS_ERR_NAME_TOO_LONG");

    /* Always, :-) */
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    /* The too-long arg should not require a semaphore lock. */
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
    /* If failure, then no timer should be crated: */
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerCreate_FullTable(void)
{
    /* Setup Inputs */
    uint32 id = 0;
    uint32 *timer_id = &id;
    const char name[] = "test name";
    uint32 ca = 1000;
    uint32 *clock_accuracy = &ca;
    OS_TimerCallback_t  callback_ptr = (OS_TimerCallback_t)1;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Mark everything in the table as not free. */
    OsTimer_SetStaticOSTimerTableNotFree();

    /* Execute Test */
    retval = OS_TimerCreate(timer_id, &name[0], clock_accuracy, callback_ptr);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_NO_FREE_IDS, "retval == OS_ERR_NO_FREE_IDS");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == OS_MAX_TIMERS, "OsTimer_countStaticOSTimerTableUsed() == OS_MAX_TIMERS");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerCreate_NameNotTaken(void)
{
    /* Setup Inputs */
    uint32 id = 0;
    uint32 *timer_id = &id;
    const char name[] = "test name";
    uint32 ca = 1000;
    uint32 *clock_accuracy = &ca;
    OS_TimerCallback_t  callback_ptr = (OS_TimerCallback_t)1;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Mark the first element as not free and give it a name. */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;
    strncpy(tbl->name, "BOGUSname", OS_MAX_API_NAME);

    /* Execute Test */
    retval = OS_TimerCreate(timer_id, &name[0], clock_accuracy, callback_ptr);

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == 2, "OsTimer_countStaticOSTimerTableUsed() == 2");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerCreate_NameTaken(void)
{
    /* Setup Inputs */
    uint32 id = 0;
    uint32 *timer_id = &id;
    const char name[] = "test name";
    uint32 ca = 1000;
    uint32 *clock_accuracy = &ca;
    OS_TimerCallback_t  callback_ptr = (OS_TimerCallback_t)1;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Mark the first element as not free and give it a name. */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;
    strncpy(tbl->name, "test name", OS_MAX_API_NAME);

    /* Execute Test */
    retval = OS_TimerCreate(timer_id, &name[0], clock_accuracy, callback_ptr);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_NAME_TAKEN, "retval == OS_ERR_NAME_TAKEN");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == 1, "OsTimer_countStaticOSTimerTableUsed() == 1");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerCreate_timercreateFAIL(void)
{
    /* Setup Inputs */
    uint32 id = 0;
    uint32 *timer_id = &id;
    const char name[] = "test name";
    uint32 ca = 1000;
    uint32 *clock_accuracy = &ca;
    OS_TimerCallback_t  callback_ptr = (OS_TimerCallback_t)1;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* make create_timer() fail */
    OsTimer_SetReturnCode(OSTIMER_TIMER_CREATE_INDEX, -1, 1);

    /* Execute Test */
    retval = OS_TimerCreate(timer_id, &name[0], clock_accuracy, callback_ptr);

    /* Verify Outputs */
    UtAssert_True(retval == OS_TIMER_ERR_UNAVAILABLE, "retval == OS_TIMER_ERR_UNAVAILABLE");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}
/*----------------------------------------------------------------------------*/
/* See: https://babelfish.arc.nasa.gov/trac/cfs_osal/ticket/66
 * Test fails before this ticket's change, passes afterwards. */
void Test_OS_TimerCreate_timerconnectFAIL(void)
{
    /* Setup Inputs */
    uint32 id = 0;
    uint32 *timer_id = &id;
    const char name[] = "test name";
    uint32 ca = 1000;
    uint32 *clock_accuracy = &ca;
    OS_TimerCallback_t  callback_ptr = (OS_TimerCallback_t)1;
    int32 retval = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* make create_timer() fail */
    OsTimer_SetReturnCode(OSTIMER_TIMER_CONNECT_INDEX, -1, 1);

    /* Execute Test */
    retval = OS_TimerCreate(timer_id, &name[0], clock_accuracy, callback_ptr);

    /* Verify Outputs */
    UtAssert_True(retval == OS_TIMER_ERR_UNAVAILABLE, "retval == OS_TIMER_ERR_UNAVAILABLE");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}

/*******************************************************************************
**
**  OS_TimerSet() Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TimerSet(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0; /* table index [0]*/
    uint32 start_time = 0;
    uint32 interval_time = 1;
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;

    /* Set hooks */
    OsTimer_SetReturnCode(OSTIMER_TIMER_SETTIME_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_TimerSet(timer_id, start_time, interval_time);

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == 1, "OsTimer_countStaticOSTimerTableUsed() == 1");
    UtAssert_True(tbl->start_time == 0, "tbl->start_time == 0");
    UtAssert_True(tbl->interval_time > 0, "tbl->interval_time > 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerSet_BadArg1(void)
{
    /* Setup Inputs */
    uint32 timer_id = OS_MAX_TIMERS;
    uint32 start_time = 0;
    uint32 interval_time = 1;
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;

    /* Set hooks */
    OsTimer_SetReturnCode(OSTIMER_TIMER_SETTIME_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_TimerSet(timer_id, start_time, interval_time);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_INVALID_ID, "retval == OS_ERR_INVALID_ID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == 1, "OsTimer_countStaticOSTimerTableUsed() == 1");
    UtAssert_True(tbl->start_time == 0, "tbl->start_time == 0");
    UtAssert_True(tbl->interval_time == 0, "tbl->interval_time == 0");
}

/*----------------------------------------------------------------------------*/
void Test_OS_TimerSet_FreeEntry(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0;
    uint32 start_time = 0;
    uint32 interval_time = 1;
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) free"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = TRUE;

    /* Set hooks */
    OsTimer_SetReturnCode(OSTIMER_TIMER_SETTIME_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_TimerSet(timer_id, start_time, interval_time);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_INVALID_ID, "retval == OS_ERR_INVALID_ID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
    UtAssert_True(tbl->start_time == 0, "tbl->start_time == 0");
    UtAssert_True(tbl->interval_time == 0, "tbl->interval_time == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerSet_NonZeroStart(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0; /* table index [0]*/
    uint32 start_time = 1;
    uint32 interval_time = 1;
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;

    /* Set hooks */
    OsTimer_SetReturnCode(OSTIMER_TIMER_SETTIME_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_TimerSet(timer_id, start_time, interval_time);

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == 1, "OsTimer_countStaticOSTimerTableUsed() == 1");
    UtAssert_True(tbl->start_time == 1, "tbl->start_time == 1");
    UtAssert_True(tbl->interval_time > 0, "tbl->interval_time > 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerSet_StartLTAccuracy(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0; /* table index [0]*/
    uint32 start_time = 1;
    uint32 interval_time = 1;
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;

    setOSClockAccuracy(2); /* .vs a start_time==1 */

    /* Set hooks */
    OsTimer_SetReturnCode(OSTIMER_TIMER_SETTIME_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_TimerSet(timer_id, start_time, interval_time);

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == 1, "OsTimer_countStaticOSTimerTableUsed() == 1");
    UtAssert_True(tbl->start_time == 2, "tbl->start_time == 2");
    UtAssert_True(tbl->interval_time > 0, "tbl->interval_time > 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerSet_IntervalLTAccuracy(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0; /* table index [0]*/
    uint32 start_time = 10;
    uint32 interval_time = 1;
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;

    setOSClockAccuracy(2); /* .vs a interval_time==1 */

    /* Set hooks */
    OsTimer_SetReturnCode(OSTIMER_TIMER_SETTIME_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_TimerSet(timer_id, start_time, interval_time);

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == 1, "OsTimer_countStaticOSTimerTableUsed() == 1");
    UtAssert_True(tbl->start_time == start_time, "tbl->start_time == start_time");
    UtAssert_True(tbl->interval_time == 2, "tbl->interval_time > 2");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerSet_ZeroInterval(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0; /* table index [0]*/
    uint32 start_time = 0;
    uint32 interval_time = 0;
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;

    setOSClockAccuracy(2);

    /* Set hooks */
    OsTimer_SetReturnCode(OSTIMER_TIMER_SETTIME_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_TimerSet(timer_id, start_time, interval_time);

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == 1, "OsTimer_countStaticOSTimerTableUsed() == 1");
    UtAssert_True(tbl->start_time == 0, "tbl->start_time == 0");
    UtAssert_True(tbl->interval_time == 0, "tbl->interval_time == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerSet_timersettimeFAIL(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0; /* table index [0]*/
    uint32 start_time = 0;
    uint32 interval_time = 1;
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;

    /* Set hooks */
    OsTimer_SetReturnCode(OSTIMER_TIMER_SETTIME_INDEX, -1, 1);

    /* Execute Test */
    retval = OS_TimerSet(timer_id, start_time, interval_time);

    /* Verify Outputs */
    UtAssert_True(retval == OS_TIMER_ERR_INTERNAL, "retval == OS_TIMER_ERR_INTERNAL");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == 1, "OsTimer_countStaticOSTimerTableUsed() == 1");
    UtAssert_True(tbl->start_time == 0, "tbl->start_time == 0");
    UtAssert_True(tbl->interval_time > 0, "tbl->interval_time > 0");
}


/*******************************************************************************
**
**  OS_TimerDelete() Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TimerDelete(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0; /* table index [0]*/
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;

    /* Set hooks */
    OsTimer_SetReturnCode(OSTIMER_TIMER_DELETE_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_TimerDelete(timer_id);

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerDelete_BadArg(void)
{
    /* Setup Inputs */
    uint32 timer_id = OS_MAX_TIMERS;
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;

    /* Set hooks */
    OsTimer_SetReturnCode(OSTIMER_TIMER_DELETE_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_TimerDelete(timer_id);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_INVALID_ID, "retval == OS_ERR_INVALID_ID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(OsTimer_countStaticOSTimerTableUsed() == 1, "OsTimer_countStaticOSTimerTableUsed() == 1");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerDelete_FreeEntry(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0;
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) free"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = TRUE;

    /* Set hooks */
    OsTimer_SetReturnCode(OSTIMER_TIMER_DELETE_INDEX, 1, 1);

    /* Execute Test */
    retval = OS_TimerDelete(timer_id);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_INVALID_ID, "retval == OS_ERR_INVALID_ID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerDelete_timerdeleteFAIL(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0; /* table index [0]*/
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;

    /* Set hooks */
    OsTimer_SetReturnCode(OSTIMER_TIMER_DELETE_INDEX, -1, 1);

    /* Execute Test */
    retval = OS_TimerDelete(timer_id);

    /* Verify Outputs */
    UtAssert_True(retval == OS_TIMER_ERR_INTERNAL, "retval == OS_TIMER_ERR_INTERNAL");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(OsTimer_isStaticOSTimerTableFree(), "OsTimer_isStaticOSTimerTableFree()");
}

/*******************************************************************************
**
**  OS_TimerGetIdByName() Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TimerGetIdByName(void)
{
    /* Setup Inputs */
    uint32 timer_id = 99;
    const char name[] = "test name";
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;
    /* Give it the expected name */
    strncpy(&(tbl->name[0]), &(name[0]), OS_MAX_API_NAME);

    /* Set hooks */

    /* Execute Test */
    retval = OS_TimerGetIdByName(&timer_id, name);

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(timer_id == 0, "timer_id == 0");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerGetIdByName_NullArg1(void)
{
    /* Setup Inputs */
    uint32 timer_id = 99;
    const char name[] = "test name";
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;
    /* Give it the expected name */
    strncpy(&(tbl->name[0]), &(name[0]), OS_MAX_API_NAME);

    /* Set hooks */

    /* Execute Test */
    retval = OS_TimerGetIdByName(NULL, name);

    /* Verify Outputs */
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");
    UtAssert_True(timer_id == 99, "timer_id == 99");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerGetIdByName_NullArg2(void)
{
    /* Setup Inputs */
    uint32 timer_id = 99;
    const char name[] = "test name";
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;
    /* Give it the expected name */
    strncpy(&(tbl->name[0]), &(name[0]), OS_MAX_API_NAME);

    /* Set hooks */

    /* Execute Test */
    retval = OS_TimerGetIdByName(&timer_id, NULL);

    /* Verify Outputs */
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");
    UtAssert_True(timer_id == 99, "timer_id == 99");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerGetIdByName_NameMaxLength(void)
{
    /* Setup Inputs */
    uint32 timer_id = 99;
    const char name[20] = "0123456789012345678\0"; /* 20 chars, OS_MAX_API_NAME */
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;
    /* Give it the expected name */
    strncpy(&(tbl->name[0]), &(name[0]), OS_MAX_API_NAME);

    /* Set hooks */

    /* Execute Test */
    retval = OS_TimerGetIdByName(&timer_id, name);

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(timer_id == 0, "timer_id == 0");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
}
/*----------------------------------------------------------------------------*/
/* See https://babelfish.arc.nasa.gov/trac/cfs_osal/ticket/65
 * This test has failures before the issue #65 fix, but should pass after. */
void Test_OS_TimerGetIdByName_NameMaxLengthPlus1(void)
{
    /* Setup Inputs */
    uint32 timer_id = 99;
    const char name[21] = "01234567890123456789\0"; /* 21 chars, OS_MAX_API_NAME+1 */
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;
    /* Give it the expected name */
    strncpy(&(tbl->name[0]), &(name[0]), OS_MAX_API_NAME); /* shorter than above */
    tbl->name[OS_MAX_API_NAME-1] = 0; /* Properly null-terminate it. */

    /* Set hooks */

    /* Execute Test */
    retval = OS_TimerGetIdByName(&timer_id, name);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_NAME_TOO_LONG, "retval == OS_ERR_NAME_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerGetIdByName_NameTooLong(void)
{
    /* Setup Inputs */
    uint32 timer_id = 99;
    const char name[] = "REEEEEAAAAALLLLLLYYYYYYYTOOOOOOOOLLOOOONNNNGGGG";
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;
    /* Give it the expected name */
    strncpy(&(tbl->name[0]), &(name[0]), OS_MAX_API_NAME); /* shorter than above */
    tbl->name[OS_MAX_API_NAME-1] = 0; /* Properly null-terminate it. */

    /* Set hooks */

    /* Execute Test */
    retval = OS_TimerGetIdByName(&timer_id, name);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_NAME_TOO_LONG, "retval == OS_ERR_NAME_TOO_LONG");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerGetIdByName_FreeEntry(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0;
    const char name[] = "test name";
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set hooks */

    /* Execute Test */
    retval = OS_TimerGetIdByName(&timer_id, name);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_NAME_NOT_FOUND, "retval == OS_ERR_NAME_NOT_FOUND");
    UtAssert_True(timer_id == 0, "timer_id == 0");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerGetIdByName_NameMismatch(void)
{
    /* Setup Inputs */
    uint32 timer_id = 99;
    const char name[] = "test name";
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;
    /* Give it the expected name */
    strncpy(&(tbl->name[0]), "fake name", OS_MAX_API_NAME);

    /* Set hooks */

    /* Execute Test */
    retval = OS_TimerGetIdByName(&timer_id, name);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_NAME_NOT_FOUND, "retval == OS_ERR_NAME_NOT_FOUND");
    UtAssert_True(timer_id == 99, "timer_id == 99");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
}
/*******************************************************************************
**
**  OS_TimerGetInfo() Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TimerGetInfo(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0;
    const char name[] = "test name";
    int32 retval = OS_ERROR;
    OS_timer_prop_t timer_prop;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;
    /* Give it the expected name */
    strncpy(&(tbl->name[0]), &(name[0]), OS_MAX_API_NAME);
    tbl->creator = 5;
    tbl->start_time = 6;
    tbl->interval_time = 4;
    tbl->accuracy = 2;

    /* Set hooks */

    /* Execute Test */
    retval = OS_TimerGetInfo(timer_id, &timer_prop);

    /* Verify Outputs */
    UtAssert_True(retval == OS_SUCCESS, "retval == OS_SUCCESS");
    UtAssert_True(timer_prop.creator == 5, "timer_prop.creator == 5");
    UtAssert_True(timer_prop.start_time == 6, "timer_prop.start_time == 6");
    UtAssert_True(timer_prop.interval_time == 4, "timer_prop.interval_time == 4");
    UtAssert_True(timer_prop.accuracy == 2, "timer_prop.accuracy == 2");
    UtAssert_True(strcmp (&(timer_prop.name[0]),name) == 0, "timer_prop.name check");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerGetInfo_BadArg1(void)
{
    /* Setup Inputs */
    uint32 timer_id = OS_MAX_TIMERS;
    int32 retval = OS_ERROR;
    OS_timer_prop_t timer_prop;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set hooks */

    /* Execute Test */
    retval = OS_TimerGetInfo(timer_id, &timer_prop);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_INVALID_ID, "retval == OS_ERR_INVALID_ID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerGetInfo_BadArg2(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0;
    int32 retval = OS_ERROR;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) use"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = FALSE;

    /* Set hooks */

    /* Execute Test */
    retval = OS_TimerGetInfo(timer_id, NULL);

    /* Verify Outputs */
    UtAssert_True(retval == OS_INVALID_POINTER, "retval == OS_INVALID_POINTER");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 0, "nSemTake == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerGetInfo_FreeEntry(void)
{
    /* Setup Inputs */
    uint32 timer_id = 0;
    int32 retval = OS_ERROR;
    OS_timer_prop_t timer_prop;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set a timer in the table to "(fake) not used"
     * index [0] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl->free = TRUE;

    /* Set hooks */

    /* Execute Test */
    retval = OS_TimerGetInfo(timer_id, &timer_prop);

    /* Verify Outputs */
    UtAssert_True(retval == OS_ERR_INVALID_ID, "retval == OS_ERR_INVALID_ID");
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
}

/*******************************************************************************
**
**   OS_TimerSignalHandler() Tests
**
*******************************************************************************/
/*----------------------------------------------------------------------------*/
void Test_OS_TimerSignalHandler(void)
{
    /* Setup Inputs */
    uint32 timer_id = 1;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set hooks */

    /* Set a timer in the table to "(fake) USED"
     * index [1] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl++;
    tbl->free = FALSE;
    tbl->callback_ptr = (*FakeTimerCallback);
    tbl->host_timerid = timer_id;

    /* Execute Test */
    Pvt_OS_TimerSignalHandler(timer_id);

    /* Verify Outputs */
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(GetFakeTimerCallbackCount() == 1, "GetFakeTimerCallbackCount() == 1");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerSignalHandler_Empty(void)
{
    /* Setup Inputs */
    uint32 timer_id = 99;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set hooks */

    /* Execute Test */
    Pvt_OS_TimerSignalHandler(timer_id);

    /* Verify Outputs */
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(GetFakeTimerCallbackCount() == 0, "GetFakeTimerCallbackCount() == 0");
}
/*----------------------------------------------------------------------------*/
void Test_OS_TimerSignalHandler_Mismatch(void)
{
    /* Setup Inputs */
    uint32 timer_id = 1;

    /* Must init the static items in ostimer.c. */
    OS_TimerAPIInit();

    /* Set hooks */

    /* Set a timer in the table to "(fake) USED"
     * index [1] */
    OS_timer_record_t* tbl = getOSTimeTablePtr();
    tbl++;
    tbl->free = FALSE;
    tbl->callback_ptr = (*FakeTimerCallback);
    tbl->host_timerid = 99; /* mismatch */

    /* Execute Test */
    Pvt_OS_TimerSignalHandler(timer_id);

    /* Verify Outputs */
    UtAssert_True(getNSemTake() == getNSemGive(), "nSemTake == nSemGive");
    UtAssert_True(getNSemTake() == 1, "nSemTake == 1");
    UtAssert_True(GetFakeTimerCallbackCount() == 0, "GetFakeTimerCallbackCount() == 0");
}
/* ------------------- End of test cases --------------------------------------*/


/*
 * OsTimer_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void OsTimer_Setup(void)
{
  OsTimer_Reset(); /* the hooks and return tables in ostimer_stubs.c */
  ResetFakeTimerCallback(); /* for callback testing in ostimer_stubs.c */

  initStaticPointers(); /* grab the pointers to the static items */
  OsTimer_ReinitStaticOSTimerTable(); /* reinit the static data */
}

/*
 * OsTimer_TearDown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void OsTimer_TearDown(void)
{
  initStaticPointers(); /* grab the pointers to the static items */
  OsTimer_ReinitStaticOSTimerTable(); /* reinit the static data */
}


#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

/* OsTimer_AddTestCase
 *
 * Purpose:
 *   Registers the test cases to execute with the unit test tool
 */
void OS_Application_Startup(void)
{
    /* OS_NetworkGetID */
    ADD_TEST(Test_OS_TimerAPIInit_Success,                OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerAPIInit_clock_getresFAIL,       OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerAPIInit_semMCreateFAIL,         OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_UsecToTimespec_1,                    NULL, NULL);
    ADD_TEST(Test_OS_UsecToTimespec_2,                    NULL, NULL);
    ADD_TEST(Test_OS_UsecToTimespec_NullPtrArg,           NULL, NULL);
    ADD_TEST(Test_OS_TimespecToUsec_1,                    NULL, NULL);
    ADD_TEST(Test_OS_TimespecToUsec_2,                    NULL, NULL);
    ADD_TEST(Test_OS_TimespecToUsec_2b,                   NULL, NULL);
    ADD_TEST(Test_OS_TimespecToUsec_3,                    NULL, NULL);
    ADD_TEST(Test_OS_TimespecToUsec_3b,                   NULL, NULL);
    ADD_TEST(Test_OS_TimespecToUsec_4,                    NULL, NULL);
    ADD_TEST(Test_OS_TimespecToUsec_4b,                   NULL, NULL);
    ADD_TEST(Test_OS_TimespecToUsec_NullPtrArg,           NULL, NULL);
    ADD_TEST(Test_OS_TimerCreate,                         OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerCreate_NullArg1,                OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerCreate_NullArg2,                OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerCreate_NullArg3,                OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerCreate_NullArg4,                OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerCreate_NameTooLong,             OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerCreate_NameMaxLength,           OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerCreate_NameMaxLengthPlus1,      OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerCreate_FullTable,               OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerCreate_NameNotTaken,            OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerCreate_NameTaken,               OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerCreate_timercreateFAIL,         OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerCreate_timerconnectFAIL,        OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerSet,                            OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerSet_BadArg1,                    OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerSet_FreeEntry,                  OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerSet_NonZeroStart,               OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerSet_StartLTAccuracy,            OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerSet_IntervalLTAccuracy,         OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerSet_ZeroInterval,               OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerSet_timersettimeFAIL,           OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerDelete,                         OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerDelete_BadArg,                  OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerDelete_FreeEntry,               OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerDelete_timerdeleteFAIL,         OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerGetIdByName,                    OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerGetIdByName_NullArg1,           OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerGetIdByName_NullArg2,           OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerGetIdByName_NameMaxLength,      OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerGetIdByName_NameMaxLengthPlus1, OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerGetIdByName_NameTooLong,        OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerGetInfo,                        OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerGetInfo_BadArg1,                OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerGetInfo_BadArg2,                OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerGetInfo_FreeEntry,              OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerSignalHandler,                  OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerSignalHandler_Empty,            OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerSignalHandler_Mismatch,         OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerGetIdByName_FreeEntry,          OsTimer_Setup, OsTimer_TearDown);
    ADD_TEST(Test_OS_TimerGetIdByName_NameMismatch,       OsTimer_Setup, OsTimer_TearDown);
}
