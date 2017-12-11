
void TCTF_AddTestCase(void);

/*
 * Filename: tctf_testrunner.c
 *
 * Purpose: This file contains a unit test runner for TC transfer frames.
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
    TCTF_AddTestCase();
    return(UtTest_Run());
}

