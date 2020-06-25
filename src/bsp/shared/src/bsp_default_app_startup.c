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

/*
 * File:  bsp_app_startup.c
 *
 * Purpose:
 *   Application startup default implementation.
 *
 * NOTE: This is isolated in a separate compilation unit, so that a user
 *   application may directly provide an OS_Application_Startup() implementation
 *   which will override this default.
 */

#include <stdlib.h>
#include "osapi.h"

#include "bsp-impl.h"

/*
 * The default implementation of OS_Application_Startup()
 * just calls the OS_API_Init() provided by OSAL.
 */
void OS_Application_Startup(void)
{
    int32 Status;

    Status = OS_API_Init();
    if (Status != OS_SUCCESS)
    {
        BSP_DEBUG("Error: OS_API_Init() failed with status=%d\n", (int)Status);
        OS_ApplicationExit(Status);
    }
}
