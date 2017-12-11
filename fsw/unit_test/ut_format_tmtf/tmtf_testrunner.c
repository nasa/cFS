
void TMTF_AddTestCase(void);

/*
 * Filename: tmtf_testrunner.c
 *
 * Purpose: This file contains a unit test runner for TM transfer frames.
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
    TMTF_AddTestCase();
    return(UtTest_Run());
}

