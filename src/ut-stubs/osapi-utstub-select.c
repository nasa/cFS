/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   osapi-utstub-select.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"


/*****************************************************************************
 *
 * Stub function for OS_SelectSingle()
 *
 *****************************************************************************/
int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs)
{
    int32 return_code;

    UT_Stub_RegisterContext(UT_KEY(OS_SelectSingle), StateFlags);
    return_code = UT_DEFAULT_IMPL(OS_SelectSingle);

    return return_code;
}

/*****************************************************************************
 *
 * Stub function for OS_SelectMultiple()
 *
 *****************************************************************************/
int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs)
{
    int32 return_code;

    UT_Stub_RegisterContext(UT_KEY(OS_SelectMultiple), ReadSet);
    UT_Stub_RegisterContext(UT_KEY(OS_SelectMultiple), WriteSet);
    return_code = UT_DEFAULT_IMPL(OS_SelectMultiple);

    return return_code;
}

/*****************************************************************************
 *
 * Stub function for OS_SelectFdZero()
 *
 *****************************************************************************/
int32 OS_SelectFdZero(OS_FdSet *Set)
{
    int32 return_code;

    return_code = UT_DEFAULT_IMPL(OS_SelectFdZero);

    return return_code;
}

/*****************************************************************************
 *
 * Stub function for OS_SelectFdAdd()
 *
 *****************************************************************************/
int32 OS_SelectFdAdd(OS_FdSet *Set, uint32 objid)
{
    int32 return_code;

    return_code = UT_DEFAULT_IMPL(OS_SelectFdAdd);

    return return_code;
}

/*****************************************************************************
 *
 * Stub function for OS_SelectFdClear()
 *
 *****************************************************************************/
int32 OS_SelectFdClear(OS_FdSet *Set, uint32 objid)
{
   int32 return_code;

   return_code = UT_DEFAULT_IMPL(OS_SelectFdClear);

   return return_code;
}

/*****************************************************************************
 *
 * Stub function for OS_SelectFdIsSet()
 *
 *****************************************************************************/
bool OS_SelectFdIsSet(OS_FdSet *Set, uint32 objid)
{
   int32 return_code;

   return_code = UT_DEFAULT_IMPL(OS_SelectFdIsSet);

   return (return_code == 0);
}

