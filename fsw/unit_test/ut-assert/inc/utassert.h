
/*
 * Filename: utassert.h
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose: This file contains a standard set of asserts for use in unit tests.
 *
 * Design Notes: 
 *    - All asserts evaluate a expression as TRUE or FALSE to determine if a unit test has 
 *      passed or failed.  TRUE means the test passed, FALSE means the test failed.
 *    - All asserts return a boolen result to indicate the pass fail status.
 *    - All asserts are implemented as macros to hide the __LINE__ and __FILE__ macros.
 *    - All asserts must call the function UtAssert.
 *
 * References:
 *
 */

#ifndef _utassert_
#define	_utassert_

/*
 * Includes
 */

#include "common_types.h"
#include "uttools.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

/*
 * Macro Definitions
 */

/* Evaluates a expression as either TRUE or FALSE.  TRUE means the test passed, FALSE means the test failed. */
#define     UtAssert_True(Expression, Description) \
                UtAssert(Expression, Description, __FILE__, __LINE__)

/* Evaluates a expression as either TRUE or FALSE.  TRUE means the test passed, FALSE means the test failed. */
#define     UtAssert_Bool(Expression, Description) \
                UtAssert(Expression, Description, __FILE__, __LINE__)

/* Asserts a test failure */
#define     UtAssert_Failed(Description) \
                UtAssert(FALSE, Description, __FILE__, __LINE__)

/* Compares two floating point numbers and determines if they are equal within a specified absolute tolerance. */
#define     UtAssert_DoubleCmpAbs(x, y, Tolerance, Description) \
                UtAssert((fabs((x) - (y)) <= (Tolerance)), Description, __FILE__, __LINE__)

/* Compares two floating point numbers and determines if they are equal within a specified relative tolerance. */
#define     UtAssert_DoubleCmpRel(x, y, Ratio, Description) \
                UtAssert((fabs((x) - (y))/(x) <= (Ratio)), Description, __FILE__, __LINE__)

/* Compares two strings and determines if they are equal. */
#define     UtAssert_StrCmp(String1, String2, Description) \
                UtAssert((strcmp(String1, String2) == 0), Description, __FILE__, __LINE__)

/* Compares at most Length characters of two strings and determines if they are equal. */
#define     UtAssert_StrnCmp(String1, String2, Length, Description) \
                UtAssert((strncmp(String1, String2, Length) == 0), Description, __FILE__, __LINE__)

/* Compares two regions of memory and determines if they are equal. */
#define     UtAssert_MemCmp(Memory1, Memory2, Length, Description) \
                UtAssert((memcmp(Memory1, Memory2, Length) == 0), Description, __FILE__, __LINE__)

/* Compares a region of memory to a static pattern and determines if they are equal.  Note: Use UtMemSet to
 * fill a region of memory with a static pattern. */
#define     UtAssert_MemCmpValue(Memory, Value, Length, Description) \
                UtAssert((UtMemCmpValue(Memory, Value, Length)), Description, __FILE__, __LINE__)

/* Compares a region of memory to a byte count pattern and determines if they are equal.  Note: Use UtMemFill to
 * fill a region of memory with a byte count pattern. */
#define     UtAssert_MemCmpCount(Memory, Length, Description) \
                UtAssert((UtMemCmpCount(Memory, Length)), Description, __FILE__, __LINE__)

/* Compares a region of memory with the contents of a binary file and determines if they are equal.  Note: Use
 * UtMem2BinFile to copy a region of memory to a binary file. */
#define     UtAssert_Mem2BinFileCmp(Memory, Filename, Description) \
                UtAssert((UtMem2BinFileCmp(Memory, Filename)), Description, __FILE__, __LINE__)

/*
 * Exported Functions
 */

/* Returns the number of asserts that have passed. */
uint32      UtAssert_GetPassCount(void);

/* Returns the number of asserts that have failed. */
uint32      UtAssert_GetFailCount(void);

/* Base assert function.  All asserts must call this function. */
boolean     UtAssert(boolean Expression, char *Description, char *File, uint32 Line);

#endif
