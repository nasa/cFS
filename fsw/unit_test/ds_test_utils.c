 /*************************************************************************
 ** File:
 **   $Id: ds_test_utils.c 1.3 2017/01/25 12:29:17EST sstrege Exp  $
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
 **   This file contains unit test utilities for the DS application.
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

#include "ds_test_utils.h"
#include "ds_app.h"

extern DS_AppData_t    DS_AppData;

/*
 * Function Definitions
 */

void DS_Test_Setup(void)
{
    /* initialize test environment to default state for every test */

    CFE_PSP_MemSet(&DS_AppData, 0, sizeof(DS_AppData_t));
    
    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();
} /* end DS_Test_Setup */

void DS_Test_TearDown(void)
{
    /* cleanup test environment */
} /* end DS_Test_TearDown */


/************************/
/*  End of File Comment */
/************************/
