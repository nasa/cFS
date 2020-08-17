/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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



