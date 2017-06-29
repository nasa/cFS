/*
 *  Copyright (c) 2004-2015, United States government as represented by the
 *  administrator of the National Aeronautics Space Administration.
 *  All rights reserved. This software was created at NASA Glenn
 *  Research Center pursuant to government contracts.
 *
 *  This is governed by the NASA Open Source Agreement and may be used,
 *  distributed and modified only according to the terms of that agreement.
 */

/**
 * \file utstubs.h
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Types and definitions for the generic Unit Test Stub framework implemented
 * as part of OSAL.
 *
 * Provides a means to implement stub functions WITHOUT needing any specific
 * headers or knowledge about the real implementations of those functions.
 *
 * This is a major key difference between this implementation and the other stub
 * implementations that have existed before this, as all other hook/stub implementations
 * ultimately use the prototypes or are somehow tied to the functions they are stubbing out.
 *
 */


#ifndef _UTSTUBS_H_
#define _UTSTUBS_H_

#include "common_types.h"

/**
 * Using a generic memory address as a key into table -
 * this should allow the function name (with a cast) to be used as the key,
 * but allow allows a fancier hash algorithm if needed.
 * Note - in pedantic mode using a "void *" here triggers a warning
 * if used with a function address, but no warning is generated if using
 * an integer memory address type.
 */
typedef cpuaddr UT_EntryKey_t;

/**
 * Macro to obtain a UT_EntryKey_t value from any function name
 */
#define UT_KEY(Func)         ((UT_EntryKey_t)&Func)


/**
 * Maximum size of a callback hook context list
 *
 * This is the maximum number of function arguments that can be passed to a hook
 */
#define UT_STUBCONTEXT_MAXSIZE      4

/**
 * Structure to hold context data for callback hooks
 */
typedef struct
{
    uint32 ArgCount;
    const void *ArgPtr[UT_STUBCONTEXT_MAXSIZE];
} UT_StubContext_t;

/**
 * Function pointer for user-specified hooks/stub callbacks
 */
typedef int32 (*UT_HookFunc_t)(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context);


/**************************************************************
 * Functions for use within test code implementation
 **************************************************************/

/**
 * Reset the Unit test framework for only the given function
 *
 * Any return code or data buffer records for the given function key
 * are cleared.  The special FuncKey value of "0" matches all entries.
 *
 * \param FuncKey The stub function to reset.  If zero, all functions
 *      are reset.  This is basically equivalent to UT_Init() without
 *      changing the Subsys string.
 */
void UT_ResetState(UT_EntryKey_t FuncKey);

/**
 * Add a deferred return code entry for the given stub function
 *
 * A deferred ("count down") return code for the stub function will be
 * installed.  The specific implementation depends on the stub function,
 * but typically it will return its default code until it is called "Count"
 * times, after which it will return the given Retcode, then return to
 * its default return code again.
 *
 * Multiple deferred entries for a single function are allowed.  These
 * will be done in the order they were added.
 *
 * It is recommended to call UT_ResetState() at the beginning of the test case
 * to ensure that any old entries for the stub functions are cleared out.
 *
 * \param FuncKey The stub function to add the return code to.
 * \param Count   The number of times after which the Retcode should be triggered
 * \param Retcode The code to return after Count calls.
 */
void UT_SetDeferredRetcode(UT_EntryKey_t FuncKey, int32 Count, int32 Retcode);

/**
 * Add a data buffer for a given stub function
 *
 * Some stub functions (i.e. read/write calls) may need to work with actual
 * test data supplied by the test function.  This specifies a data buffer
 * that the read/write call copy to or from.  Its specific usage is dependent
 * on the implementation of the stub function itself.
 *
 * Multiple buffer entries for a single function are allowed.  These
 * will be used in the order they were added.
 *
 * It is recommended to call UT_ResetState() at the beginning of the test case
 * to ensure that any old entries for the stub functions are cleared out.
 *
 * \param FuncKey The stub function to add the data buffer to.
 * \param DataBuffer Pointer to data buffer that should be associated with the stub function
 * \param BufferSize Size of data buffer
 * \param AllocateCopy If TRUE then malloc() will be used to allocate a copy of the buffer
 *      and the supplied DataBuffer will be copied into it via memcpy() before returning.  The
 *      buffer will be freed after the test code reads it or the test state is reset.  If this
 *      is FALSE then the DataBuffer pointer is used directly, and must remain valid for the duration
 *      of the current test case.
 */
void UT_SetDataBuffer(UT_EntryKey_t FuncKey, void *DataBuffer, uint32 BufferSize, osalbool AllocateCopy);

/**
 * Enable or disable the forced failure mode for the given stub function
 *
 * This triggers a constant failure mode from the stub function, if implemented.
 * The stub function will invoke a given failure path as defined by
 * the stub implementation.
 *
 * A count of the number of times the failure mode is invoked will be maintained.
 *
 * \param FuncKey The stub function to add the return code to.
 * \param Value Arbitrary failure mode value (may or may not be used by the stub)
 */
void UT_SetForceFail(UT_EntryKey_t FuncKey, int32 Value);

/**
 * Disable the forced failure mode for the given stub function
 *
 * This undoes the action of UT_SetForceFail()
 *
 * \param FuncKey The stub function entry to clear.
 */
void UT_ClearForceFail(UT_EntryKey_t FuncKey);

/**
 * Set a Hook function for a particular call
 *
 * This triggers a callback to a user-defined function when the stub is invoked.
 * The callback may optionally pass back context data, depending on the stub
 * implementation.
 *
 * \param FuncKey  The stub function to add the hook to.
 * \param HookFunc User defined hook function.  Set NULL to delete/clear an entry.
 * \param UserObj  Arbitrary user data object to pass to the hook function
 */
void UT_SetHookFunction(UT_EntryKey_t FuncKey, UT_HookFunc_t HookFunc, void *UserObj);



/**
 * Get a count for the number of times a forced failure mode was taken
 *
 * More specifically, this counts the number of times UT_Stub_CheckForceFail()
 * was called and returned TRUE since UT_SetForceFail() was called.  The
 * actual meaning of this depends on the stub's implementation.
 *
 * \param FuncKey The stub function to check.
 * \param Count   The number of times the failure mode was invoked
 */
osalbool UT_GetStubRetcodeAndCount(UT_EntryKey_t FuncKey, int32 *Retcode, int32 *Count);


/**************************************************************
 * Functions for use within stub function implementation
 **************************************************************/

/**
 * Check for a deferred return code entry for the given stub function
 *
 * This is a default implementation for deferred retcodes and can be used
 * by stub functions as a common implementation.  If a deferred retcode
 * for the given function is present, this will decrement the associated
 * count.  If the count becomes zero, this function returns TRUE and
 * the Retcode parameter is assigned the originally requested code.
 * Otherwise this function returns FALSE which indicates the default
 * stub implementation should be used.
 *
 * Once the counter reaches zero, this clears the entry so that if a
 * second deferred code is recorded it will be found next.
 *
 * \param FuncKey The stub function to check the return code.
 * \param Retcode Buffer to store deferred return code, if available.
 * \returns TRUE if deferred code is present and counter reached zero
 */
osalbool UT_Stub_CheckDeferredRetcode(UT_EntryKey_t FuncKey, int32 *Retcode);

/**
 * Check for a forced failure mode entry for the given stub function
 *
 * If a UT_SetForceFail() option is in place for the given function this
 * will return TRUE and increment the internal usage counter.
 *
 * \param FuncKey The stub function to check the return code.
 * \param Value Set to the value supplied to UT_SetForceFail()
 * \returns TRUE if force fail mode is active
 */
osalbool UT_Stub_CheckForceFail(UT_EntryKey_t FuncKey, int32 *Value);

/**
 * Copies data from a test-supplied buffer to the local buffer
 *
 * If a UT_SetDataBuffer() option is in place for the given function this
 * will return nonzero and increment the internal usage counter.
 *
 * \param FuncKey The stub function to entry to use.
 * \param LocalBuffer The local (destination) buffer
 * \param MaxSize The maximum size of data to copy
 * \returns The actual size of data copied.  If no data buffer is
 *      supplied by the test harness this will return 0.
 */
uint32 UT_Stub_CopyToLocal(UT_EntryKey_t FuncKey, uint8 *LocalBuffer, uint32 MaxSize);

/**
 * Copies data from a local buffer to the test-supplied buffer
 *
 * If a UT_SetDataBuffer() option is in place for the given function this
 * will return nonzero and increment the internal usage counter.
 *
 * \param FuncKey The stub function to entry to use.
 * \param LocalBuffer The local (source) buffer
 * \param MaxSize The maximum size of data to copy
 * \returns The actual size of data copied.  If no data buffer is
 *      supplied by the test harness this will return 0.
 */
uint32 UT_Stub_CopyFromLocal(UT_EntryKey_t FuncKey, const uint8 *LocalBuffer, uint32 MaxSize);

/**
 * Registers a single context element for the hook callback
 *
 * Stubs may pass up to UT_STUBCONTEXT_MAXSIZE arguments to a user-defined
 * hook function.  These arguments are opaque to the stub function and generally
 * passed as "void *" pointers to the actual stack values.  The user code must
 * then cast them to the right type again.
 *
 * \param FuncKey   The stub function to entry to use.
 * \param Parameter Arbitrary parameter to pass.
 */
void UT_Stub_RegisterContext(UT_EntryKey_t FuncKey, const void *Parameter);

/**
 * Default implementation for a stub function that should be useful for most cases.
 *
 * Checks first for a deferred retcode, then for a constant retcode, and a default (0) if neither is present.
 * Optionally also prints a debug level status message to show that the function was called.
 *
 * \param FunctionName  The printable name of the actual function called, for the debug message.  If
 *    NULL then no debug message will be generated.
 * \param FuncKey       The Key to look up in the table
 */
int32 UT_DefaultStubImpl(const char *FunctionName, UT_EntryKey_t FuncKey, int32 DefaultRc);

/**
 * Macro to simplify usage of the UT_DefaultStubImpl() function
 *
 * The UT_DEFAULT_IMPL can be used as part of a larger implementation
 * where the default items of deferred return codes / force fails are
 * checked first, then additional functionality is added.
 */
#define UT_DEFAULT_IMPL(FuncName)           UT_DefaultStubImpl(#FuncName, UT_KEY(FuncName), 0)

/**
 * Macro to simplify usage of the UT_DefaultStubImpl() function
 *
 * The UT_DEFAULT_IMPL can be used as part of a larger implementation
 * where the default items of deferred return codes / force fails are
 * checked first, then additional functionality is added.
 */
#define UT_DEFAULT_IMPL_RC(FuncName, Rc)    UT_DefaultStubImpl(#FuncName, UT_KEY(FuncName), Rc)

/**
 * Macro to simplify usage of the UT_DefaultStubImpl() function
 *
 * The UT_DEFAULT_STUB can be used as a complete stub implementation
 * when only deferred return codes / force fails are in use.
 */
#define UT_DEFAULT_STUB(FuncName, Args)     int32 FuncName Args { return UT_DEFAULT_IMPL(FuncName); }


/*******************************************************************
 * COMPATIBILITY LAYER DEFINITIONS AND PROTOTYPES
 *******************************************************************
 *
 * The definitions in this section are to support the CFE unit test without
 * requiring a major rewrite of the test code.
 *
 * Much of that code "communicates" with a stub function via the UT_SetRtn_t
 * structure types.  For every stub function there is also a global object (or 2)
 * of this type instantiated that hold the state.
 *
 * This is unnecessary using the stub API defined here, but in order to support
 * code ALREADY WRITTEN for this, some adaption layers are defined here.
 */

/**
 * A structure to interface between the test code and stubs
 * This is intended to be compatible with the old "UT_SetRtn_t" structure
 * used throughout the CFE unit test code.  It can be removed once that
 * code is updated.
 */
typedef struct
{
    int32 count;
    int32 value;
} UT_Compat_SetRtn_t;

/**
 * Maps an arbitrary object pointer to a stub function
 *
 * This just serves to associate some global object to a stub function.
 * Once the association is established, calling UT_Compat_SetRtnCode()
 * on the same global object works as if the UT_SetDeferredRetcode()
 * were called directly using the associated stub function
 *
 * /param ObjPtr    Arbitrary (opaque) object to associate
 * /param FuncKey   Stub function to associate with
 */
void UT_Compat_SetCodeMap(const void *ObjPtr, UT_EntryKey_t FuncKey);


/**
 * Compatibility layer for UT_SetDeferredRetcode() for existing unit tests.
 *
 * Using the associates set up by UT_Compat_SetCodeMap(), this calls
 * UT_SetDeferredRetcode() with the correct function ID.
 *
 * This is an adaption layer for code that expects a separate global object
 * to be the interface between it and the stub function.
 */
void UT_Compat_SetRtnCode(const void *ObjPtr, int32 RetCode, int32 Count);



#endif /* _UTSTUBS_H_ */
