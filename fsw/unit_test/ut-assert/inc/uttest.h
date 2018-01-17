
/*
 * Filename: uttest.h
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose: This file contains functions to implement a standard way to execute unit tests.
 *
 * Design Notes: 
 *    By default the only output that is printed to the console is assert failures
 *    and a summary of the test results after all tests have executed.  To enable additional 
 *    test output define the macro UT_VERBOSE.
 *
 * References:
 *
 */

#ifndef _uttest_
#define	_uttest_

/*
 * Exported Functions
 */

/* Adds a new unit test to the test database. */
void    UtTest_Add(void (*Test)(void), void (*Setup)(void), void (*Teardown)(void), char *TestName);

/* Executes all unit tests contained in the test database.  Once all tests have finished executing 
 * a results summary is printed to the console and the test database is deleted.  This function also 
 * returns a boolean status indicating if any of the tests failed. (TRUE = at least one test failure 
 * has occurred, FALSE = all tests passed) */
int     UtTest_Run(void);

#endif

