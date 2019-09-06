/*
**
** File: uttest.h
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
** Design Notes: 
**    By default the only output that is printed to the console is assert failures
**    and a summary of the test results after all tests have executed.  To enable additional 
**    test output define the macro UT_VERBOSE.
**
*/

#ifndef _uttest_
#define	_uttest_

#include <stdbool.h>

/*
 * Exported Functions
 */

/* Adds a new unit test to the test database. */
void    UtTest_Add(void (*Test)(void), void (*Setup)(void), void (*Teardown)(void), const char *TestName);

/* Executes all unit tests contained in the test database.  Once all tests have finished executing 
 * a results summary is printed to the console and the test database is deleted.  This function also 
 * returns a boolean status indicating if any of the tests failed. (true = at least one test failure 
 * has occurred, false = all tests passed) */
bool     UtTest_Run(void);

#endif

