/*
 *      Copyright (c) 2019, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Goddard
 *      Space Flight Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/* OSAL coverage stub replacement for intLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <OCS_intLib.h>


OCS_STATUS OCS_intConnect(OCS_VOIDFUNCPTR *vector, OCS_VOIDFUNCPTR routine, int parameter)
{
    return (UT_DEFAULT_IMPL(OCS_intConnect));
}
int OCS_intDisable(int level)
{
    return (UT_DEFAULT_IMPL(OCS_intDisable));
}
int OCS_intEnable(int level)
{
    return (UT_DEFAULT_IMPL(OCS_intEnable));
}
int OCS_intLock(void)
{
    return (UT_DEFAULT_IMPL(OCS_intLock));
}
int OCS_intUnlock(int lockKey)
{
    return (UT_DEFAULT_IMPL(OCS_intUnlock));
}

static void OCS_intLib_dummyfunc(void)
{
}

OCS_VOIDFUNCPTR * OCS_INUM_TO_IVEC(unsigned int ui)
{
    int32 Status = UT_DEFAULT_IMPL(OCS_INUM_TO_IVEC);
    OCS_VOIDFUNCPTR *VecTbl;
    OCS_VOIDFUNCPTR DummyVec;
    uint32 VecTblSize;

    if (Status == 0)
    {
        UT_GetDataBuffer(UT_KEY(OCS_INUM_TO_IVEC), (void**)&VecTbl, &VecTblSize, NULL);
        if (VecTbl != NULL && ui < (VecTblSize / sizeof(OCS_VOIDFUNCPTR)))
        {
            VecTbl += ui;
        }
        else
        {
            DummyVec = &OCS_intLib_dummyfunc;
            VecTbl = &DummyVec;
        }
    }
    else
    {
        VecTbl = NULL;
    }

    return VecTbl;
}



