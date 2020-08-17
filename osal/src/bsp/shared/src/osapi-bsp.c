/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file     osapi-bsp.c
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file  contains some of the OS APIs abstraction layer code
 * that is shared/common across all BSP implementations.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * User defined include files
 */
#include "common_types.h"
#include "bsp-impl.h"

/*
 * BSP Global Data Object
 */
OS_BSP_GlobalData_t OS_BSP_Global;

/*
 *********************************************************************************
 *          PUBLIC API (application-callable functions)
 *********************************************************************************
 */

/*----------------------------------------------------------------
   OS_BSP_GetArgC
   See full description in header
 ------------------------------------------------------------------*/
uint32 OS_BSP_GetArgC(void)
{
    return OS_BSP_Global.ArgC;
}

/*----------------------------------------------------------------
   OS_BSP_GetArgV
   See full description in header
 ------------------------------------------------------------------*/
char * const * OS_BSP_GetArgV(void)
{
    return OS_BSP_Global.ArgV;
}

/*----------------------------------------------------------------
   OS_BSP_SetExitCode
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_SetExitCode(int32 code)
{
    OS_BSP_Global.AppStatus = code;
}

