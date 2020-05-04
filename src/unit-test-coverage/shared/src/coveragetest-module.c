/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     coveragetest-module.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "ut-adaptor-module.h"

#include "os-shared-module.h"

#include <OCS_string.h>

/* A dummy function for the static symbol lookup test.  Not called */
void Test_DummyFunc(void)
{
}

/*
 * A symbol table for the static loader.
 *
 * Note - the symbol name is intentionally not the actual function name here.
 * This way, the test case knows that the answer actually came from the static table
 */
OS_static_symbol_record_t OS_UT_STATIC_SYMBOL_TABLE[] =
{
        { "UT_staticsym", &Test_DummyFunc, "UTS" },
        { NULL }
};

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_ModuleAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_ModuleAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_ModuleAPI_Init();

    UtAssert_True(actual == expected, "OS_ModuleAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}



void Test_OS_ModuleLoad(void)
{
    /*
     * Test Case For:
     * int32 OS_ModuleLoad ( uint32 *module_id, const char *module_name, const char *filename )
     */
    int32 expected = OS_SUCCESS;
    uint32 objid = 0xFFFFFFFF;
    int32 actual = OS_ModuleLoad(&objid, "UT", "File");

    UtAssert_True(actual == expected, "OS_ModuleLoad() (%ld) == OS_SUCCESS", (long)actual);
    actual = UT_GetStubCount(UT_KEY(OS_ModuleLoad_Impl));
    UtAssert_True(actual == 1, "OS_ModuleLoad_Impl() called (%ld) == 1", (long)actual);
    UtAssert_True(objid != 0, "objid (%lu) != 0", (unsigned long)objid);


    /* for a static module, it should also return a valid objid, but should NOT invoke OS_ModuleLoad_Impl */
    actual = OS_ModuleLoad(&objid, "UTS", "File2");
    UtAssert_True(actual == expected, "OS_ModuleLoad() (%ld) == OS_SUCCESS", (long)actual);
    actual = UT_GetStubCount(UT_KEY(OS_ModuleLoad_Impl));
    UtAssert_True(actual == 1, "OS_ModuleLoad_Impl() called (%ld) == 1", (long)actual);
    UtAssert_True(objid != 0, "objid (%lu) != 0", (unsigned long)objid);

    /* error cases */
    actual = OS_ModuleLoad(NULL,NULL,NULL);
    expected = OS_INVALID_POINTER;
    UtAssert_True(actual == expected, "OS_ModuleLoad() (%ld) == OS_INVALID_POINTER", (long)actual);

    UT_SetForceFail(UT_KEY(OCS_strlen), 2 + OS_MAX_API_NAME);
    actual = OS_ModuleLoad(&objid, "UTS", "File2");
    expected = OS_ERR_NAME_TOO_LONG;
    UtAssert_True(actual == expected, "OS_ModuleLoad() (%ld) == OS_ERR_NAME_TOO_LONG", (long)actual);
    UT_ResetState(UT_KEY(OCS_strlen));


    UT_SetForceFail(UT_KEY(OS_TranslatePath), OS_ERROR);
    actual = OS_ModuleLoad(&objid, "UT", "FileBad");
    expected = OS_ERROR;
    UtAssert_True(actual == expected, "OS_ModuleLoad() (%ld) == OS_ERROR", (long)actual);

}

void Test_OS_ModuleUnload(void)
{
    /*
     * Test Case For:
     * int32 OS_ModuleUnload ( uint32 module_id )
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_ModuleUnload(1);

    UtAssert_True(actual == expected, "OS_ModuleDelete() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_SymbolLookup(void)
{
    /*
     * Test Case For:
     * int32 OS_ModuleUnload ( uint32 module_id )
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    cpuaddr testaddr = 0;
    cpuaddr symaddr = 0;

    actual = OS_SymbolLookup(&symaddr,"uttestsym0");
    UtAssert_True(actual == expected, "OS_SymbolLookup(name=%s) (%ld) == OS_SUCCESS",
            "uttestsym0", (long)actual);

    UT_ResetState(UT_KEY(OS_SymbolLookup_Impl));
    UT_SetForceFail(UT_KEY(OS_SymbolLookup_Impl), OS_ERROR);

    /* this lookup should always fail */
    symaddr = 0;
    testaddr = 0;
    actual = OS_SymbolLookup(&symaddr,"uttestsym1");
    expected = OS_ERROR;
    UtAssert_True(actual == expected, "OS_SymbolLookup(name=%s) (%ld) == OS_ERROR",
            "uttestsym1", (long)actual);
    UtAssert_True(symaddr == testaddr, "OS_SymbolLookup(address=%lx) == %lx",
            (unsigned long)symaddr, (unsigned long)testaddr);

    actual = OS_SymbolLookup(NULL, NULL);
    expected = OS_INVALID_POINTER;
    UtAssert_True(actual == expected, "OS_SymbolLookup(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

    /*
     * Look up a symbol that is present in the static symbol table
     */
    actual = OS_SymbolLookup(&symaddr, "UT_staticsym");
    expected = OS_SUCCESS;
    UtAssert_True(actual == expected, "OS_SymbolLookup(UT_staticsym) (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_StaticSymbolLookup(void)
{
    /*
     * Test case for:
     * static int32 OS_ModuleLoad_Static(OS_static_symbol_record_t *StaticSymTable, const char *ModuleName)
     * static int32 OS_SymbolLookup_Static(OS_static_symbol_record_t *StaticSymTable, cpuaddr *SymbolAddress, const char *SymbolName)
     *
     * As these are internal helper functions we must go through a wrapper to invoke them.
     * The ability to get line coverage requires a non-empty lookup table, so one is supplied here.
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    cpuaddr addr;

    /* nominal */
    actual = OS_SymbolLookup_Static(&addr, "UT_staticsym");
    UtAssert_True(actual == expected, "OS_SymbolLookup_Static(name=%s) (%ld) == OS_SUCCESS",
            "Test_Func1", (long)actual);
    UtAssert_True(addr == (cpuaddr)&Test_DummyFunc, "OS_SymbolLookup_Static(address=%lx) == %lx",
            (unsigned long)addr, (unsigned long)&Test_DummyFunc);

    actual = OS_ModuleLoad_Static("UTS");
    UtAssert_True(actual == expected, "OS_ModuleLoad_Static(name=%s) (%ld) == OS_SUCCESS",
            "UT", (long)actual);

    expected = OS_ERROR;
    actual = OS_SymbolLookup_Static(&addr, "Invalid");
    UtAssert_True(actual == expected, "OS_SymbolLookup_Static(name=%s) (%ld) == OS_ERROR",
            "Invalid", (long)actual);

    expected = OS_ERR_NAME_NOT_FOUND;
    actual = OS_ModuleLoad_Static("Invalid");
    UtAssert_True(actual == expected, "OS_ModuleLoad_Static(name=%s) (%ld) == OS_SUCCESS",
            "Invalid", (long)actual);
}


void Test_OS_SymbolTableDump(void)
{
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_SymbolTableDump ( "test", 555 );
    UtAssert_True(actual == expected,
            "OS_SymbolTableDump() (%ld) == OS_SUCCESS",
            (long)actual);


    expected = OS_INVALID_POINTER;
    actual = OS_SymbolTableDump ( NULL, 555 );
    UtAssert_True(actual == expected,
            "OS_SymbolTableDump() (%ld) == OS_INVALID_POINTER",
            (long)actual);

    UT_SetForceFail(UT_KEY(OS_TranslatePath), OS_ERROR);
    expected = OS_ERROR;
    actual = OS_SymbolTableDump ( "test", 555 );
    UtAssert_True(actual == expected,
            "OS_SymbolTableDump() (%ld) == OS_ERROR",
            (long)actual);

}

void Test_OS_ModuleGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_ModuleInfo ( uint32 module_id, OS_module_prop_t *module_prop )
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    OS_module_prop_t module_prop;
    uint32 local_index = 1;
    OS_common_record_t utrec;
    OS_common_record_t *rptr = &utrec;

    memset(&utrec, 0, sizeof(utrec));
    utrec.creator = 111;
    utrec.name_entry = "ABC";
    strncpy(OS_module_table[1].file_name, "DEF", sizeof(OS_module_table[1].file_name));
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &local_index, sizeof(local_index), false);
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &rptr, sizeof(rptr), false);
    actual = OS_ModuleInfo(1, &module_prop);

    UtAssert_True(actual == expected, "OS_ModuleGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(strcmp(module_prop.filename, "DEF") == 0, "module_prop.filename (%s) == DEF",
            module_prop.filename);
    UtAssert_True(strcmp(module_prop.name, "ABC") == 0, "module_prop.name (%s) == ABC",
            module_prop.name);


    actual = OS_ModuleInfo(1, NULL);
    expected = OS_INVALID_POINTER;
    UtAssert_True(actual == expected, "OS_ModuleGetInfo(NULL) (%ld) == OS_INVALID_POINTER", (long)actual);
}



/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);
}

/*
 * Osapi_Test_Teardown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_Test_Teardown(void)
{

}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_ModuleAPI_Init);
    ADD_TEST(OS_ModuleLoad);
    ADD_TEST(OS_ModuleUnload);
    ADD_TEST(OS_SymbolLookup);
    ADD_TEST(OS_ModuleGetInfo);
    ADD_TEST(OS_SymbolTableDump);
    ADD_TEST(OS_StaticSymbolLookup);
}





