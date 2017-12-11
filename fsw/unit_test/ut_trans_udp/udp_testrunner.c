
void UDP_AddTestCase(void);

/*
 * Filename: udp_testrunner.c
 *
 * Purpose: This file contains a unit test runner for UDP Transport protocol.
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
    UDP_AddTestCase();
    return(UtTest_Run());
}

