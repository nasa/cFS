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
** File:  ut_osloader_symtable_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osloader_symtable_test.h"
#include "ut_osloader_test_platforms.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

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
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Syntax: OS_SymbolLookup
** Purpose: Returns the memory address of a symbol
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERROR if the symbol name is not found
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/

void UT_os_symbol_lookup_test()
{
    int32 res = 0;
    const char*    testDesc;
    cpuaddr        symbol_addr;
    uint32         module_id = 0;

    /*-----------------------------------------------------*/
    testDesc = "API Not implemented";

    res = OS_SymbolLookup(&symbol_addr, "main");
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_symbol_lookup_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-pointer-arg-1";

    res = OS_SymbolLookup(0, "main");
    if ( res == OS_INVALID_POINTER )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg-2";

    res = OS_SymbolLookup(&symbol_addr, 0);
    if ( res == OS_INVALID_POINTER )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Symbol-not-found";

    res = OS_SymbolLookup(&symbol_addr, "ThisSymbolIsNotFound");
    if ( res == OS_ERROR )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    /* Setup */
    res = OS_ModuleLoad(&module_id, "Mod1", UT_OS_GENERIC_MODULE_NAME2);
    if ( res != OS_SUCCESS )
    {
        UT_OS_TEST_RESULT( "#4 Nominal - Module Load failed", UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_SymbolLookup(&symbol_addr, "module1");
        if (res == OS_SUCCESS)
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Reset test environment */
        res = OS_ModuleUnload(module_id);
    }

UT_os_symbol_lookup_test_exit_tag:
    return;
    
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_SymbolTableDump
** Purpose: Dumps the system symbol table to the given filename
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_FS_ERR_PATH_INVALID if the filename is invalid
**          OS_ERROR if there was any problem writing the symbol table to the file
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_symbol_table_dump_test()
{
    int32 res = 0;
    const char* testDesc;

    /*
     * Note that even if the functionality is not implemented,
     * the API still validates the input pointers (not null) and
     * the validity of the file name.
     */

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-pointer-arg";

    res = OS_SymbolTableDump(0,10000);
    if ( res == OS_INVALID_POINTER )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-path";

    res = OS_SymbolTableDump("/this/path/is/invalid.dat", 10000);
    if ( res == OS_FS_ERR_PATH_INVALID )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    res = OS_SymbolTableDump(UT_OS_GENERIC_MODULE_DIR "SymbolFile.dat", 32000);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        /* allowed, not applicable on this system */
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
    }
    else if ( res == OS_SUCCESS )
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    }
    else
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }
}

/*================================================================================*
** End of File: ut_osloader_symtable_test.c
**================================================================================*/
