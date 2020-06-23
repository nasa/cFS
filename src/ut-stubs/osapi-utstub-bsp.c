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
 * \file     osapi-utstub-bsp.c
 * \author   joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"

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
    int32 status = UT_DEFAULT_IMPL(OS_BSP_GetArgC);

    return status;
}

/*----------------------------------------------------------------
   OS_BSP_GetArgV
   See full description in header
 ------------------------------------------------------------------*/
char * const * OS_BSP_GetArgV(void)
{
    void *buffer;
    int32 status;

    status = UT_DEFAULT_IMPL(OS_BSP_GetArgV);
    if (status == 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OS_BSP_GetArgV), &buffer, sizeof(buffer)) < sizeof(buffer))
    {
        buffer = NULL;
    }
    return buffer;
}

