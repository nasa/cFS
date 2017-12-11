
void TO_AddTestCase(void);

/*
 * Filename: to_testrunner.c
 *
 * Purpose: This file contains a unit test runner for the CI Application.
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
    TO_AddTestCase();
    return(UtTest_Run());
}

