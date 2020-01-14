/*
**
** File: uttest.c
**
** Copyright 2012-2013 United States Government as represented by the 
** Administrator of the National Aeronautics and Space Administration. 
** All Other Rights Reserved.  
**
** This software was created at NASA's Goddard Space Flight Center.
** This software is governed by the NASA Open Source Agreement and may be 
** used, distributed and modified only pursuant to the terms of that 
** agreement.
**
** Purpose: This file contains functions to implement a standard way to execute unit tests.
**
*/

/*
 * Includes
 */

#include "osapi.h"
#include "utassert.h"
#include "utlist.h"
#include "utbsp.h"
#include "uttest.h"
#include "utstubs.h"

/*
 * Type Definitions
 */

typedef struct {
    void    (*Test)(void);
    void    (*Setup)(void);
    void    (*Teardown)(void);
    const char     *TestName;
} UtTestDataBaseEntry_t;

/*
 * Local Data
 */

UtListHead_t    UtTestDataBase;
uint32          UtTestsExecutedCount = 0;

/*
 * Function Definitions
 */

void UtTest_Add(void (*Test)(void), void (*Setup)(void), void (*Teardown)(void), const char *TestName)
{
    UtTestDataBaseEntry_t   UtTestDataBaseEntry;

    UtTestDataBaseEntry.Test = Test;
    UtTestDataBaseEntry.Setup = Setup;
    UtTestDataBaseEntry.Teardown = Teardown;
    UtTestDataBaseEntry.TestName = TestName;
    UtList_Add(&UtTestDataBase, &UtTestDataBaseEntry, sizeof(UtTestDataBaseEntry_t), 0);
}

void OS_Application_Run(void)
{
    uint32                   i;
    UtListNode_t            *UtListNode;
    UtTestDataBaseEntry_t   *UtTestDataBaseEntry;
    
    if (UtTestDataBase.NumberOfEntries > 0) {
        
        UtListNode = UtTestDataBase.First;
        for (i=0; i < UtTestDataBase.NumberOfEntries; i++) {
            
            UtTestDataBaseEntry = UtListNode->Data;

            UtAssert_BeginTest(UtTestDataBaseEntry->TestName);

            UtAssert_SetContext(UTASSERT_CASETYPE_TSF);
            if (UtTestDataBaseEntry->Setup)    { UtTestDataBaseEntry->Setup(); }
            UtAssert_SetContext(UTASSERT_CASETYPE_FAILURE);
            if (UtTestDataBaseEntry->Test)     { UtTestDataBaseEntry->Test(); UtTestsExecutedCount++; }
            UtAssert_SetContext(UTASSERT_CASETYPE_TTF);
            if (UtTestDataBaseEntry->Teardown) { UtTestDataBaseEntry->Teardown(); }

            UtAssert_EndTest();

            UtListNode = UtListNode->Next;
        }
    }

    UtList_Reset(&UtTestDataBase);

    UT_BSP_EndTest(UtAssert_GetCounters());
}

/*
 * Entry point from the BSP.
 * When linking with UT-Assert, the test framework (this library) serves
 * the role of the "application" being executed.
 *
 * There is a separate entry point (UT_Test_Setup) to configure the test cases.
 */
void OS_Application_Startup(void)
{
    /*
     * Reset the test global variables, just in case.
     */
    memset(&UtTestDataBase, 0, sizeof(UtTestDataBase));
    UtTestsExecutedCount = 0;

    UT_BSP_Setup();

    /*
     * Wrap the UtTest_Setup() function in a UT segment called "SETUP"
     * This allows any assert calls to be used and recorded during setup
     */
    UtAssert_BeginTest("SETUP");
    UtTest_Setup();
    UtAssert_EndTest();
}


