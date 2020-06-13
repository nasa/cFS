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

/**
 * \brief Adds a new unit test to the test database.
 *
 * Called by the user to register a new test case with the library.
 *
 * \param Test     Main test function to call.
 * \param Setup    Setup function, called before the test function
 * \param Teardown Cleanup function, called after the test function
 * \param TestName Name of test for logging purposes
 */
void    UtTest_Add(void (*Test)(void), void (*Setup)(void), void (*Teardown)(void), const char *TestName);

/**
 * \brief Early initialization function
 *
 * Reset the global data to a safe state for initial start-up.
 * This should be called before any other API.
 */
void UtTest_EarlyInit(void);


/**
 * \brief Execute all registered tests
 *
 * All test functions that were registered with UtTest_Add will be executed in order.
 */
void UtTest_Run(void);

/*
 * \brief Set up function for UT-Assert based test routines
 *
 * This function must be provided by the user to set up test cases.
 * This should call UtTest_Add() for each test case.
 */
void UtTest_Setup(void);

#endif

