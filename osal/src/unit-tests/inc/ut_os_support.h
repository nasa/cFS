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

/*================================================================================*
** File:  ut_os_support.h
** Owner: Tam Ngo
** Date:  May 2013
**================================================================================*/

#ifndef _UT_OS_SUPPORT_H_
#define _UT_OS_SUPPORT_H_

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utassert.h"
#include "uttest.h"
#include "osapi.h"


/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*
 * Buffers to hold names of various objects
 *
 * These are sized somewhat larger than the osconfig.h specification,
 * so that test cases may create names that exceed the allowed length
 */
#define UT_OS_NAME_BUFF_SIZE        (OS_MAX_API_NAME + 10)
#define UT_OS_FILE_BUFF_SIZE        (OS_MAX_FILE_NAME + 10)
#define UT_OS_PATH_BUFF_SIZE        (OS_MAX_PATH_LEN + 10)
#define UT_OS_PHYS_NAME_BUFF_SIZE   (OS_FS_PHYS_NAME_LEN + 10)
#define UT_OS_LOCAL_PATH_BUFF_SIZE  (OS_MAX_LOCAL_PATH_LEN + 10)

/*
 * Generic buffer for I/O operations
 */
#define UT_OS_IO_BUFF_SIZE          128

static inline bool UtOsalRetVal(int32 Fn, int32 Exp, UtAssert_CaseType_t casetype, const char *File, uint32 Line, const char *FnTxt, const char *ExpTxt, const char *Msg)
{
    return UtAssertEx(Fn == Exp, casetype, File, Line, "%s (%d) == %s (%d): %s", FnTxt, (int)Fn, ExpTxt, (int)Exp, Msg);
}

/* Only report errors */
static inline bool UtOsalCheck(int32 Fn, int32 Exp, UtAssert_CaseType_t casetype, const char *File, uint32 Line, const char *FnTxt, const char *ExpTxt)
{
    return Fn == Exp ? true : 
        UtAssertEx(Fn == Exp, casetype, File, Line, "%s (%d) == %s (%d)", FnTxt, (int)Fn, ExpTxt, (int)Exp);
}

static inline bool UtOsalImplemented(int32 Fn, const char *File, uint32 Line)
{
    if (Fn == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssertEx(false, UTASSERT_CASETYPE_NA, File, Line, "API not implemented");
        return false;
    }

    return true;
}


#define UT_NOMINAL(Fn) UtOsalRetVal(Fn, OS_SUCCESS, UTASSERT_CASETYPE_FAILURE, __FILE__, __LINE__, #Fn, "OS_SUCCESS", "Nominal")
#define UT_RETVAL(Fn, Exp, Msg) UtOsalRetVal(Fn, Exp, UTASSERT_CASETYPE_FAILURE, __FILE__, __LINE__, #Fn, #Exp, Msg)
#define UT_SETUP(Fn) UtOsalCheck(Fn, OS_SUCCESS, UTASSERT_CASETYPE_TSF, __FILE__, __LINE__, #Fn, "OS_SUCCESS")
#define UT_TEARDOWN(Fn) UtOsalCheck(Fn, OS_SUCCESS, UTASSERT_CASETYPE_TTF, __FILE__, __LINE__, #Fn, "OS_SUCCESS")
#define UT_IMPL(Fn) UtOsalImplemented(Fn, __FILE__, __LINE__)

/*--------------------------------------------------------------------------------*/

#define UT_OS_TEST_RESULT(descStr, caseType)  \
    UtAssertEx(false, caseType, __FILE__, __LINE__, "%s", descStr)

/*--------------------------------------------------------------------------------*/

#define UT_os_sprintf(buf,...)  \
    do { int x = snprintf(buf,sizeof(buf),__VA_ARGS__); if (x > 0 && x < sizeof(buf)) buf[x] = 0; } while (0)

/*--------------------------------------------------------------------------------*/

#define UT_OS_LOG(...)    \
    UtAssert_Message(UTASSERT_CASETYPE_INFO,__FILE__,__LINE__,__VA_ARGS__);

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function prototypes
**--------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------*/

#endif  /* _UT_OS_SUPPORT_H_ */

/*================================================================================*
** End of File: ut_os_support.h
**================================================================================*/
