
void TM_SDLP_AddTestCase(void);

/*
 * Filename: cop1_testrunner.c
 *
 * Purpose: This file contains a unit test runner for the COP-1 procedure.
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
    TM_SDLP_AddTestCase();
    return(UtTest_Run());
}

