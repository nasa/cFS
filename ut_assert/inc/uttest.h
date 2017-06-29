/*
**
** File: uttest.h
**
** $Id: uttest.h 1.3 2015/06/16 16:02:15EDT sstrege Exp  $
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
** $Log: uttest.h  $
** Revision 1.3 2015/06/16 16:02:15EDT sstrege 
** Added copyright information
** Revision 1.2 2015/03/10 15:19:30EDT czogby 
** Add Missing Functionality to UT Assert Library
** --- Added comments ---  czogby [2015/03/31 18:02:49Z]
** No source code changes were made in this revision.
**
** Design Notes: 
**    By default the only output that is printed to the console is assert failures
**    and a summary of the test results after all tests have executed.  To enable additional 
**    test output define the macro UT_VERBOSE.
**
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

