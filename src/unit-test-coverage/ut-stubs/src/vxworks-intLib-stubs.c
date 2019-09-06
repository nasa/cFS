/* OSAL coverage stub replacement for intLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/intLib.h>


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



