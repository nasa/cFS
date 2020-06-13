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

#include "utglobal.h"

/*
 * Global state instance
 */
UtAssert_Global_t UtAssert_Global;

/*
 * Function Definitions
 */

void UtTest_Add(void (*Test)(void), void (*Setup)(void), void (*Teardown)(void), const char *TestName)
{
    UtTestDataBaseEntry_t   UtTestDataBaseEntry;

    memset(&UtTestDataBaseEntry, 0, sizeof(UtTestDataBaseEntry));
    UtTestDataBaseEntry.Test = Test;
    UtTestDataBaseEntry.Setup = Setup;
    UtTestDataBaseEntry.Teardown = Teardown;
    strncpy(UtTestDataBaseEntry.TestName, TestName, sizeof(UtTestDataBaseEntry.TestName)-1);
    UtList_Add(&UtAssert_Global.DataBase, &UtTestDataBaseEntry, sizeof(UtTestDataBaseEntry_t), 0);
}

void UtTest_Run(void)
{
    uint32                   i;
    UtListNode_t            *UtListNode;
    UtTestDataBaseEntry_t   *UtTestDataBaseEntry;
    
    if (UtAssert_Global.DataBase.NumberOfEntries > 0) {
        
        UtListNode = UtAssert_Global.DataBase.First;
        for (i=0; i < UtAssert_Global.DataBase.NumberOfEntries; i++) {
            
            UtTestDataBaseEntry = UtListNode->Data;

            UtAssert_BeginTest(UtTestDataBaseEntry->TestName);

            UtAssert_SetContext(UTASSERT_CASETYPE_TSF);
            if (UtTestDataBaseEntry->Setup)    { UtTestDataBaseEntry->Setup(); }
            UtAssert_SetContext(UTASSERT_CASETYPE_FAILURE);
            if (UtTestDataBaseEntry->Test)     { UtTestDataBaseEntry->Test(); UtAssert_Global.ExecutedCount++; }
            UtAssert_SetContext(UTASSERT_CASETYPE_TTF);
            if (UtTestDataBaseEntry->Teardown) { UtTestDataBaseEntry->Teardown(); }

            UtAssert_EndTest();

            UtListNode = UtListNode->Next;
        }
    }

    UtList_Reset(&UtAssert_Global.DataBase);

    UT_BSP_EndTest(UtAssert_GetCounters());
}

void UtTest_EarlyInit(void)
{
    /*
     * Reset the test global variables, just in case.
     */
    memset(&UtAssert_Global, 0, sizeof(UtAssert_Global));
}


