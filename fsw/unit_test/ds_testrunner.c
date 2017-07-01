 /*************************************************************************
 ** File:
 **   $Id: ds_testrunner.c 1.3 2017/01/25 12:29:20EST sstrege Exp  $
 **
 **  Copyright (c) 2007-2014 United States Government as represented by the 
 **  Administrator of the National Aeronautics and Space Administration. 
 **  All Other Rights Reserved.  
 **
 **  This software was created at NASA's Goddard Space Flight Center.
 **  This software is governed by the NASA Open Source Agreement and may be 
 **  used, distributed and modified only pursuant to the terms of that 
 **  agreement.
 **
 ** Purpose: 
 **   This file contains the unit test runner for the DS application.
 **
 ** References:
 **   Flight Software Branch C Coding Standard Version 1.2
 **   CFS Development Standards Document
 **
 ** Notes:
 **
 *************************************************************************/

/*
 * Includes
 */

#include "uttest.h"
#include "ds_app_test.h"
#include "ds_cmds_test.h"
#include "ds_file_test.h"
#include "ds_table_test.h"

/*
 * Function Definitions
 */

int main(void)
{   
    DS_App_Test_AddTestCases();
    DS_Cmds_Test_AddTestCases();
    DS_File_Test_AddTestCases();
    DS_Table_Test_AddTestCases();

    return(UtTest_Run());
} /* end main */


/************************/
/*  End of File Comment */
/************************/
