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

#ifndef INCLUDE_UTASSERT_GLOBAL_H_
#define INCLUDE_UTASSERT_GLOBAL_H_

#include "osapi.h"
#include "utassert.h"
#include "utlist.h"
#include "utbsp.h"
#include "uttest.h"
#include "utstubs.h"

/*
 * Type Definitions
 */

typedef struct
{
    void    (*Test)(void);
    void    (*Setup)(void);
    void    (*Teardown)(void);

    /* Note - the name entry should be long enough to support a GroupName.TestName pattern,
     * hence why it uses double the OS_MAX_API_NAME length */
    char    TestName[OS_MAX_API_NAME*2];
} UtTestDataBaseEntry_t;

typedef struct
{
    UtListHead_t    DataBase;
    uint32          ExecutedCount;
} UtAssert_Global_t;

/*
 * Global Test Data
 */
extern UtAssert_Global_t UtAssert_Global;

#endif /* INCLUDE_UTASSERT_GLOBAL_H_ */

