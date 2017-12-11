
void RS422_AddTestCase(void);

/*
 * Filename: rs422_testrunner.c
 *
 * Purpose: This file contains a unit test runner for RS422 Transport protocol.
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
    RS422_AddTestCase();
    return(UtTest_Run());
}

