 /*************************************************************************
 ** File: ds_test_utils.c
 **
 ** NASA Docket No. GSC-16,126-1, and identified as "Core Flight Software System
 ** (CFS) Data Storage Application Version 2” 
 **
 ** Copyright © 2007-2014 United States Government as represented by the
 ** Administrator of the National Aeronautics and Space Administration. All Rights
 ** Reserved. 
 ** 
 ** Licensed under the Apache License, Version 2.0 (the "License"); 
 ** you may not use this file except in compliance with the License. 
 ** You may obtain a copy of the License at 
 ** http://www.apache.org/licenses/LICENSE-2.0 
 **
 ** Unless required by applicable law or agreed to in writing, software 
 ** distributed under the License is distributed on an "AS IS" BASIS, 
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 ** See the License for the specific language governing permissions and 
 ** limitations under the License.
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
