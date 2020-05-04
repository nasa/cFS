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
 * \file     ut-adaptor-module.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_MODULE_H_
#define INCLUDE_UT_ADAPTOR_MODULE_H_

#include <common_types.h>
#include <osapi-os-loader.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

/**
 * Purges all state tables and resets back to initial conditions
 * Helps avoid cross-test dependencies
 */
void Osapi_Internal_ResetState(void);

/* A dummy function for the static symbol lookup test.  Not called */
void Test_DummyFunc(void);

int32 Osapi_Call_SymbolLookup_Static(cpuaddr *SymbolAddress, const char *SymbolName);
int32 Osapi_Call_ModuleLoad_Static(const char *ModuleName);

#endif  /* INCLUDE_UT_ADAPTOR_MODULE_H_ */

