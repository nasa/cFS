
void TC_SYNC_AddTestCase(void);

/*
 * Filename: tc_sync_testrunner.c
 *
 * Purpose: This file contains a unit test runner for the TC_SYNC procedure.
 *
 */

/*
 * Includes
 */

#include "uttest.h"

/*
 * Function Definitions
 */

int main(void)
{
    /* Call AddTestSuite or AddTestCase functions here */
    TC_SYNC_AddTestCase();
    return(UtTest_Run());
}

