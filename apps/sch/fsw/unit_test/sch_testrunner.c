 /*************************************************************************
 ** File:
 **   $Id: sch_testrunner.c 1.2 2017/06/21 15:29:03EDT mdeschu Exp  $
 **
 ** Purpose: 
 **   This file contains the unit test runner for the SCH application.
 **
 ** References:
 **   Flight Software Branch C Coding Standard Version 1.2
 **   CFS Development Standards Document
 ** Notes:
 **
 *************************************************************************/

/*
 * Includes
 */

#include "uttest.h"
#include "sch_api_test.h"
#include "sch_app_test.h"
#include "sch_cmds_test.h"
#include "sch_custom_test.h"

/*
 * Function Definitions
 */

int main(void)
{   
    SCH_API_Test_AddTestCases();
    SCH_App_Test_AddTestCases();
    SCH_Cmds_Test_AddTestCases();
    SCH_Custom_Test_AddTestCases();

    return(UtTest_Run());
} /* end main */


/************************/
/*  End of File Comment */
/************************/
