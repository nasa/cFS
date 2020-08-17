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
 * \file     os-shared-shell.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_SHELL_H_
#define INCLUDE_OS_SHARED_SHELL_H_

#include <os-shared-globaldefs.h>




/****************************************************************************************
                 SHELL API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/


/*----------------------------------------------------------------
   Function: OS_ShellOutputToFile_Impl

    Purpose: Takes a shell command in and writes the output of that command to the specified file

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ShellOutputToFile_Impl(uint32 stream_id, const char* Cmd);

#endif  /* INCLUDE_OS_SHARED_SHELL_H_ */

