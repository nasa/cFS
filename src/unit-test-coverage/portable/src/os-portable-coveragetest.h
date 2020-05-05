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
 * \file     os-portable-coveragetest.h
 * \ingroup  portable
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_PORTABLE_COVERAGETEST_H_
#define INCLUDE_OS_PORTABLE_COVERAGETEST_H_



/**
 * \file     os-vxworks-coveragetest.h
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef _OS_PORTABLE_COVERAGETEST_H_
#define _OS_PORTABLE_COVERAGETEST_H_

/*
 * Includes
 */

#include <utassert.h>
#include <uttest.h>
#include <utstubs.h>


#include "os-shared-globaldefs.h"


#define OSAPI_TEST_FUNCTION_RC(func,args,exp)   \
{                                               \
    int32 rcexp = exp;                          \
    int32 rcact = func args;                    \
    UtAssert_True(rcact == rcexp, "%s%s (%ld) == %s (%ld)", \
        #func, #args, (long)rcact, #exp, (long)rcexp);      \
}

#define ADD_TEST(test) UtTest_Add((Test_ ## test), Osapi_Test_Setup, Osapi_Test_Teardown, #test)

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void);
void Osapi_Test_Teardown(void);


#endif  /* _OS_PORTABLE_COVERAGETEST_H_ */


#endif  /* INCLUDE_OS_PORTABLE_COVERAGETEST_H_ */

