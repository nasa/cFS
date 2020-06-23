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
 * \file     osapi-errors.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         Contains the code related to error handling.  Currently this
 *         entails conversion of OSAL error codes into printable strings.
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
#include "os-shared-errors.h"

/**
 * Global error name table
 * These are the errors that are defined at the top layer and
 * are intended to have consistent in meaning across all
 * operating systems.
 *
 * The low level implementation can extend this with
 * additional codes if necessary (but they may not be consistent).
 */
static const OS_ErrorTable_Entry_t OS_GLOBAL_ERROR_NAME_TABLE[] =
      {
            { OS_SUCCESS, "OS_SUCCESS" },
            { OS_ERROR, "OS_ERROR" },
            { OS_INVALID_POINTER, "OS_INVALID_POINTER" },
            { OS_ERROR_ADDRESS_MISALIGNED, "OS_ADDRESS_MISALIGNED" },
            { OS_ERROR_TIMEOUT,"OS_ERROR_TIMEOUT" },
            { OS_INVALID_INT_NUM,"OS_INVALID_INT_NUM" },
            { OS_SEM_FAILURE,"OS_SEM_FAILURE" },
            { OS_SEM_TIMEOUT,"OS_SEM_TIMEOUT" },
            { OS_QUEUE_EMPTY,"OS_QUEUE_EMPTY" },
            { OS_QUEUE_FULL,"OS_QUEUE_FULL" },
            { OS_QUEUE_TIMEOUT,"OS_QUEUE_TIMEOUT" },
            { OS_QUEUE_INVALID_SIZE,"OS_QUEUE_INVALID_SIZE" },
            { OS_QUEUE_ID_ERROR,"OS_QUEUE_ID_ERROR" },
            { OS_ERR_NAME_TOO_LONG,"OS_ERR_NAME_TOO_LONG" },
            { OS_ERR_NO_FREE_IDS,"OS_ERR_NO_FREE_IDS" },
            { OS_ERR_NAME_TAKEN,"OS_ERR_NAME_TAKEN" },
            { OS_ERR_INVALID_ID,"OS_ERR_INVALID_ID" },
            { OS_ERR_NAME_NOT_FOUND,"OS_ERR_NAME_NOT_FOUND" },
            { OS_ERR_SEM_NOT_FULL,"OS_ERR_SEM_NOT_FULL" },
            { OS_ERR_INVALID_PRIORITY,"OS_ERR_INVALID_PRIORITY" },
            { OS_ERR_OBJECT_IN_USE,"OS_ERR_OBJECT_IN_USE" },
            { OS_ERR_BAD_ADDRESS,"OS_ERR_BAD_ADDRESS" },
            { OS_ERR_INCORRECT_OBJ_STATE,"OS_ERR_INCORRECT_OBJ_STATE" },
            { OS_ERR_INCORRECT_OBJ_TYPE,"OS_ERR_INCORRECT_OBJ_TYPE" },
            { OS_FS_ERR_PATH_TOO_LONG, "OS_FS_ERR_PATH_TOO_LONG" },
            { OS_FS_ERR_NAME_TOO_LONG, "OS_FS_ERR_NAME_TOO_LONG" },
            { OS_FS_ERR_DRIVE_NOT_CREATED, "OS_FS_ERR_DRIVE_NOT_CREATED" },
            { OS_FS_ERR_DEVICE_NOT_FREE, "OS_FS_ERR_DEVICE_NOT_FREE" },
            { OS_FS_ERR_PATH_INVALID, "OS_FS_ERR_PATH_INVALID" },

            { 0, NULL } /* End of table marker */
      };


/*
 *********************************************************************************
 *          PUBLIC API (application-callable functions)
 *********************************************************************************
 */



/*----------------------------------------------------------------
 *
 * Function: OS_GetErrorName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GetErrorName(int32 error_num, os_err_name_t* err_name)
{
    uint32 return_code;
    const OS_ErrorTable_Entry_t *Error;

    if (err_name == NULL)
    {
        return OS_INVALID_POINTER;
    }

    Error = OS_GLOBAL_ERROR_NAME_TABLE;
    while (Error->Name != NULL && Error->Number != error_num)
    {
       ++Error;
    }

    if (Error->Number != error_num)
    {
       Error = OS_IMPL_ERROR_NAME_TABLE;
       while (Error->Name != NULL && Error->Number != error_num)
       {
          ++Error;
       }
    }

    if (Error->Number == error_num && Error->Name != NULL)
    {
       strncpy(*err_name, Error->Name, OS_ERROR_NAME_LENGTH - 1);
       return_code = OS_SUCCESS;
    }
    else
    {
       snprintf(*err_name, OS_ERROR_NAME_LENGTH, "OS_UNKNOWN(%d)", (int)error_num);
       return_code = OS_ERROR;
    }

    (*err_name)[OS_ERROR_NAME_LENGTH - 1] = '\0';

    return return_code;
} /* end OS_GetErrorName */

