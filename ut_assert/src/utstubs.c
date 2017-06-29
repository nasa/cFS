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
 * \file utstubs.c
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

#include <stdlib.h>
#include <string.h>

#include "utstubs.h"
#include "utbsp.h"

#define UT_MAX_FUNC_STUBS        64
#define UT_APPNAME_MAX_LEN       80
#define UT_SUBSYS_MAX_LEN        5
#define UT_MODEFLAG_ALLOC_BUF    0x1U

typedef enum
{
    UT_ENTRYTYPE_UNUSED,            /**< Unused/available table entries */
    UT_ENTRYTYPE_COUNTER,           /**< Records a usage count plus most recent return code */
    UT_ENTRYTYPE_FORCE_FAIL,        /**< Always return a designated code from stub */
    UT_ENTRYTYPE_DEFERRED_RC,       /**< Return a designated code from stub after "N" calls */
    UT_ENTRYTYPE_DATA_BUFFER,       /**< Storage for data buffers to simulate read/write or queue ops */
    UT_ENTRYTYPE_CALLBACK_HOOK,     /**< Pointer to a custom callback/hook function */
    UT_ENTRYTYPE_CALLBACK_CONTEXT,  /**< Context data for callback/hook function */
    UT_ENTRYTYPE_COMPAT_SETRTN,     /**< Pointer to a UT_SetRtn table for transitional purposes */
} UT_EntryType_t;

typedef struct
{
    uint32          Count;
    int32           Value;
} UT_RetcodeEntry_t;

typedef struct
{
    uint32          Position;
    uint32          TotalSize;
    uint8           *BasePtr;
} UT_BufferEntry_t;

typedef struct
{
    UT_HookFunc_t   Func;
    void            *CallbackArg;
} UT_CallbackEntry_t;

typedef union
{
    UT_RetcodeEntry_t   Rc;
    UT_BufferEntry_t    Buff;
    UT_CallbackEntry_t  Cb;
    UT_StubContext_t    Context;
    UT_EntryKey_t       KeyMap;
} UT_EntryData_t;

/*
 * Definition of internal stub table structure
 */
typedef struct
{
    UT_EntryType_t  EntryType;
    UT_EntryKey_t   FuncKey;
    uint32          ModeFlags;
    UT_EntryData_t  Data;
} UT_StubTableEntry_t;

static UT_StubTableEntry_t UT_StubTable[UT_MAX_FUNC_STUBS] = {{ 0 }};

/**
 * Helper function to clear an entry in the stub table.
 * This will call "free()" on any dynamically allocated buffers within the entry,
 * then zero out the entire block so it can be re-used.
 */
static void UT_ClearStubEntry(UT_StubTableEntry_t *StubPtr)
{
    /* Be sure to call free() on any malloc'ed buffers before clearing */
    if (StubPtr->EntryType == UT_ENTRYTYPE_DATA_BUFFER &&
            StubPtr->Data.Buff.BasePtr != NULL &&
            (StubPtr->ModeFlags & UT_MODEFLAG_ALLOC_BUF) != 0)
    {
        free(StubPtr->Data.Buff.BasePtr);
    }
    memset(StubPtr, 0, sizeof(*StubPtr));
}

/*
** Functions
*/

UT_StubTableEntry_t *UT_GetStubEntry(UT_EntryKey_t FuncKey, UT_EntryType_t TestMode)
{
    UT_StubTableEntry_t *StubPtr;
    int32 i;

    StubPtr = UT_StubTable;
    i = 0;
    while(1)
    {
        if (i >= UT_MAX_FUNC_STUBS)
        {
            StubPtr = NULL;
            break;
        }
        if (StubPtr->EntryType == TestMode && (FuncKey == 0 || StubPtr->FuncKey == FuncKey))
        {
            break;
        }
        ++StubPtr;
        ++i;
    }

    return (StubPtr);
}

void UT_ResetState(UT_EntryKey_t FuncKey)
{
    UT_StubTableEntry_t *StubPtr;
    int32 i;

    StubPtr = UT_StubTable;
    for (i=0; i < UT_MAX_FUNC_STUBS; ++i)
    {
        if (FuncKey == 0 || StubPtr->FuncKey == FuncKey)
        {
            UT_ClearStubEntry(StubPtr);
        }
        ++StubPtr;
    }
}


void UT_SetDeferredRetcode(UT_EntryKey_t FuncKey, int32 Count, int32 Retcode)
{
    UT_StubTableEntry_t *StubPtr;

    if (Count > 0)
    {
        StubPtr = UT_GetStubEntry(0, UT_ENTRYTYPE_UNUSED);

        if (StubPtr == NULL)
        {
            UtAssert_Abort("Cannot set retcode - UT_MAX_FUNC_STUBS too low?");
        }
        else
        {
            StubPtr->FuncKey = FuncKey;
            StubPtr->EntryType = UT_ENTRYTYPE_DEFERRED_RC;
            StubPtr->Data.Rc.Count = Count;
            StubPtr->Data.Rc.Value = Retcode;
        }
    }
}

void UT_ClearDeferredRetcode(UT_EntryKey_t FuncKey)
{
    UT_StubTableEntry_t *StubPtr;

    while (TRUE)
    {
        StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_DEFERRED_RC);
        if (StubPtr == NULL)
        {
            break;
        }
        UT_ClearStubEntry(StubPtr);
    }
}

osalbool UT_Stub_CheckDeferredRetcode(UT_EntryKey_t FuncKey, int32 *Retcode)
{
    osalbool Result = FALSE;
    UT_StubTableEntry_t *StubPtr;

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_DEFERRED_RC);
    if (StubPtr != NULL)
    {
        if (StubPtr->Data.Rc.Count > 0)
        {
            --StubPtr->Data.Rc.Count;
        }
        if (StubPtr->Data.Rc.Count == 0)
        {
            Result = TRUE;
            *Retcode = StubPtr->Data.Rc.Value;

            /* Once the count has reached zero, void the entry */
            UT_ClearStubEntry(StubPtr);
        }
    }

    return (Result);
}

void UT_SetForceFail(UT_EntryKey_t FuncKey, int32 Value)
{
    UT_StubTableEntry_t *Rc;

    /*
     * First find an existing force fail entry for the function.
     * In case one is already set we do not duplicate (unlike deferred codes)
     */
    Rc = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_FORCE_FAIL);
    if (Rc == NULL)
    {
        /* Creating force fail entry - repeat search and grab any unused slot */
        Rc = UT_GetStubEntry(0, UT_ENTRYTYPE_UNUSED);
    }

    if (Rc != NULL)
    {
        Rc->FuncKey = FuncKey;
        Rc->EntryType = UT_ENTRYTYPE_FORCE_FAIL;
        Rc->Data.Rc.Value = Value;
    }
    else
    {
        UtAssert_Abort("Cannot set retcode - UT_MAX_FUNC_STUBS too low?");
    }
}

void UT_ClearForceFail(UT_EntryKey_t FuncKey)
{
    UT_StubTableEntry_t *StubPtr;

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_FORCE_FAIL);
    if (StubPtr != NULL)
    {
        UT_ClearStubEntry(StubPtr);
    }
}

osalbool UT_GetStubRetcodeAndCount(UT_EntryKey_t FuncKey, int32 *Retcode, int32 *Count)
{
    UT_StubTableEntry_t *StubPtr;
    osalbool Result;

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_COUNTER);
    if (StubPtr != NULL)
    {
        *Count = StubPtr->Data.Rc.Count;
        *Retcode = StubPtr->Data.Rc.Value;
        Result = TRUE;
    }
    else
    {
        Result = FALSE;
    }

    return Result;
}

osalbool UT_Stub_CheckForceFail(UT_EntryKey_t FuncKey, int32 *Value)
{
    osalbool Result = FALSE;
    UT_StubTableEntry_t *StubPtr;

    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_FORCE_FAIL);
    if (StubPtr != NULL)
    {
        /* For "force fail" entries, the count will reflect the number of times it was used */
        ++StubPtr->Data.Rc.Count;
        *Value = StubPtr->Data.Rc.Value;
        Result = TRUE;
    }

    return (Result);
}

void UT_SetDataBuffer(UT_EntryKey_t FuncKey, void *DataBuffer, uint32 BufferSize, osalbool AllocateCopy)
{
    UT_StubTableEntry_t *StubPtr;

    if (DataBuffer != NULL && BufferSize > 0)
    {
        StubPtr = UT_GetStubEntry(0, UT_ENTRYTYPE_UNUSED);

        if (StubPtr == NULL)
        {
            UtAssert_Abort("Cannot set data buffer - UT_MAX_FUNC_STUBS too low?");
        }
        else
        {
            StubPtr->FuncKey = FuncKey;
            StubPtr->EntryType = UT_ENTRYTYPE_DATA_BUFFER;
            if (AllocateCopy)
            {
                StubPtr->Data.Buff.BasePtr = malloc(BufferSize);
                if (StubPtr->Data.Buff.BasePtr == NULL)
                {
                    UtAssert_Abort("Cannot allocate data buffer - malloc() failed!");
                }
                else
                {
                    memcpy(StubPtr->Data.Buff.BasePtr, DataBuffer, BufferSize);
                    StubPtr->ModeFlags |= UT_MODEFLAG_ALLOC_BUF;
                }
            }
            else
            {
                /* Use buffer directly */
                StubPtr->Data.Buff.BasePtr = DataBuffer;
            }
            StubPtr->Data.Buff.TotalSize = BufferSize;
            StubPtr->Data.Buff.Position = 0;
        }
    }
}

uint32 UT_Stub_CopyToLocal(UT_EntryKey_t FuncKey, uint8 *LocalBuffer, uint32 MaxSize)
{
    uint32 ActualCopy;
    UT_StubTableEntry_t *StubPtr;

    ActualCopy = 0;
    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_DATA_BUFFER);

    if (StubPtr != NULL)
    {
        ActualCopy = StubPtr->Data.Buff.TotalSize - StubPtr->Data.Buff.Position;
        if (MaxSize < ActualCopy)
        {
            ActualCopy = MaxSize;
        }

        memcpy(LocalBuffer, &StubPtr->Data.Buff.BasePtr[StubPtr->Data.Buff.Position], ActualCopy);
        StubPtr->Data.Buff.Position += ActualCopy;

        /*
         * Once the buffer is completely copied then drop this entry.
         * The test harness will either refill it or there might be
         * another entry in the table.
         */
        if (StubPtr->Data.Buff.Position >= StubPtr->Data.Buff.TotalSize)
        {
            UT_ClearStubEntry(StubPtr);
        }
    }

    return ActualCopy;
}

uint32 UT_Stub_CopyFromLocal(UT_EntryKey_t FuncKey, const uint8 *LocalBuffer, uint32 MaxSize)
{
    uint32 ActualCopy;
    UT_StubTableEntry_t *StubPtr;

    ActualCopy = 0;
    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_DATA_BUFFER);

    if (StubPtr != NULL)
    {
        ActualCopy = StubPtr->Data.Buff.TotalSize - StubPtr->Data.Buff.Position;
        if (MaxSize < ActualCopy)
        {
            ActualCopy = MaxSize;
        }

        memcpy(&StubPtr->Data.Buff.BasePtr[StubPtr->Data.Buff.Position], LocalBuffer, ActualCopy);
        StubPtr->Data.Buff.Position += ActualCopy;

        /*
         * Once the buffer is completely copied then drop this entry.
         * The test harness will either refill it or there might be
         * another entry in the table.
         */
        if (StubPtr->Data.Buff.Position >= StubPtr->Data.Buff.TotalSize)
        {
            UT_ClearStubEntry(StubPtr);
        }
    }

    return ActualCopy;
}

void UT_SetHookFunction(UT_EntryKey_t FuncKey, UT_HookFunc_t HookFunc, void *UserObj)
{
    UT_StubTableEntry_t *StubPtr;

    /*
     * First find an existing hook entry for the function.
     * In case one is already set we do not duplicate
     */
    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_CALLBACK_HOOK);
    if (StubPtr == NULL && HookFunc != NULL)
    {
        /* Creating force fail entry - repeat search and grab any unused slot */
        StubPtr = UT_GetStubEntry(0, UT_ENTRYTYPE_UNUSED);
        if (StubPtr == NULL)
        {
            UtAssert_Abort("Cannot set hook function - UT_MAX_FUNC_STUBS too low?");
        }
    }

    if (HookFunc == NULL && StubPtr != NULL)
    {
        /* Caller wants to delete the entry */
        UT_ClearStubEntry(StubPtr);
    }
    else if (StubPtr != NULL && HookFunc != NULL)
    {
        /* Caller wants to set the entry */
        StubPtr->FuncKey = FuncKey;
        StubPtr->EntryType = UT_ENTRYTYPE_CALLBACK_HOOK;
        StubPtr->Data.Cb.CallbackArg = UserObj;
        StubPtr->Data.Cb.Func = HookFunc;
    }
}

void UT_Stub_RegisterContext(UT_EntryKey_t FuncKey, const void *Parameter)
{
    UT_StubTableEntry_t *StubPtr;

    /*
     * First find an existing context entry for the function.
     * In case one is already set we do not duplicate
     */
    StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_CALLBACK_CONTEXT);
    if (StubPtr == NULL)
    {
        /* Creating force fail entry - repeat search and grab any unused slot */
        StubPtr = UT_GetStubEntry(0, UT_ENTRYTYPE_UNUSED);
    }

    if (StubPtr == NULL)
    {
        UtAssert_Abort("Cannot set context - UT_MAX_FUNC_STUBS too low?");
    }
    else
    {
        StubPtr->FuncKey = FuncKey;
        StubPtr->EntryType = UT_ENTRYTYPE_CALLBACK_CONTEXT;
        if (StubPtr->Data.Context.ArgCount < UT_STUBCONTEXT_MAXSIZE)
        {
            StubPtr->Data.Context.ArgPtr[StubPtr->Data.Context.ArgCount] = Parameter;
            ++StubPtr->Data.Context.ArgCount;
        }
    }
}

/**
 * Default implementation for a stub function that should be useful for most cases.
 * Checks first for a deferred retcode, then for a constant retcode, and a default if neither is present.
 * Prints a debug level status message to show that the function was called.
 */
int32 UT_DefaultStubImpl(const char *FunctionName, UT_EntryKey_t FuncKey, int32 DefaultRc)
{
   int32 Retcode;
   const char *RetcodeString;
   UT_StubTableEntry_t *StubPtr;
   UT_StubTableEntry_t *ContextTblPtr;
   const UT_StubContext_t *ContextPtr;
   uint32 Counter;

   if (!UT_Stub_CheckDeferredRetcode(FuncKey, &Retcode))
   {
      if (!UT_Stub_CheckForceFail(FuncKey, &Retcode))
      {
         Retcode = DefaultRc;
      }
   }

   if (FunctionName != NULL)
   {
      if (Retcode == DefaultRc)
      {
         RetcodeString = "DEFAULT";
      }
      else
      {
         /* Indicate that this invocation got a non-default return code */
         RetcodeString = "*SPECIAL*";
      }

      UtDebug("%s called (%s,%d)", FunctionName, RetcodeString, (int)Retcode);
   }

   Counter = 0;
   StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_COUNTER);
   if (StubPtr == NULL)
   {
       /* Creating counter entry - repeat search and grab any unused slot */
       StubPtr = UT_GetStubEntry(0, UT_ENTRYTYPE_UNUSED);
   }

   if (StubPtr == NULL)
   {
       UtAssert_Abort("Cannot set counter - UT_MAX_FUNC_STUBS too low?");
   }
   else
   {
       StubPtr->EntryType = UT_ENTRYTYPE_COUNTER;
       StubPtr->FuncKey = FuncKey;
       Counter = StubPtr->Data.Rc.Count;
       ++StubPtr->Data.Rc.Count;
       StubPtr->Data.Rc.Value = Retcode;
   }

   /* Handle a user-requested callback hook.
    * First see if the stub has a registered context.
    */
   ContextTblPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_CALLBACK_CONTEXT);
   if (ContextTblPtr == NULL)
   {
       ContextPtr = NULL;
   }
   else
   {
       ContextPtr = &ContextTblPtr->Data.Context;
   }

   StubPtr = UT_GetStubEntry(FuncKey, UT_ENTRYTYPE_CALLBACK_HOOK);
   if (StubPtr != NULL)
   {
       Retcode = (*StubPtr->Data.Cb.Func)(StubPtr->Data.Cb.CallbackArg, Retcode, Counter, ContextPtr);
   }

   /* Always clear the context entry -- the next call will have a different one */
   if (ContextTblPtr != NULL)
   {
       UT_ClearStubEntry(ContextTblPtr);
   }

   return Retcode;
}

/**
 * Compatibility method for setting a deferred return code.  This is not ideal
 * since it requires instantiating another global object dedicated to this purpose
 * for every individual stub function.
 *
 * The UT_SetDeferredRetcode() replaces this API with one that does not
 * require instantiation of an extra global object.
 */
void UT_Compat_SetCodeMap(const void *ObjPtr, UT_EntryKey_t FuncKey)
{
    UT_StubTableEntry_t *StubPtr;

    StubPtr = UT_GetStubEntry((UT_EntryKey_t)ObjPtr, UT_ENTRYTYPE_COMPAT_SETRTN);
    if (StubPtr == NULL)
    {
        /* Creating force fail entry - repeat search and grab any unused slot */
        StubPtr = UT_GetStubEntry(0, UT_ENTRYTYPE_UNUSED);
    }

    if (StubPtr == NULL)
    {
        UtAssert_Abort("Cannot set compatibility map - UT_MAX_FUNC_STUBS too low?");
    }
    else
    {
        StubPtr->EntryType = UT_ENTRYTYPE_COMPAT_SETRTN;
        StubPtr->FuncKey = (UT_EntryKey_t)ObjPtr;
        StubPtr->Data.KeyMap = FuncKey;
    }
}

/**
 * Compatibility method for setting a deferred return code.  This is not ideal
 * since it requires instantiating another global object dedicated to this purpose
 * for every individual stub function.
 *
 * The UT_SetDeferredRetcode() replaces this API with one that does not
 * require instantiation of an extra global object.
 */
void UT_Compat_SetRtnCode(const void *ObjPtr, int32 Retcode, int32 Count)
{
    UT_StubTableEntry_t *MapPtr;
    UT_StubTableEntry_t *RcPtr;

    MapPtr = UT_GetStubEntry((UT_EntryKey_t)ObjPtr, UT_ENTRYTYPE_COMPAT_SETRTN);
    if (MapPtr != NULL)
    {
        /*
         * First check if an existing entry exists --
         * the old behavior would have overwritten prior values,
         * so this will imitate that
         */
        RcPtr = UT_GetStubEntry(MapPtr->Data.KeyMap, UT_ENTRYTYPE_DEFERRED_RC);
        if (RcPtr != NULL)
        {
            /* update existing entry */
            RcPtr->Data.Rc.Count = Count;
            RcPtr->Data.Rc.Value = Retcode;
        }
        else
        {
            /* add an entry */
            UT_SetDeferredRetcode(MapPtr->Data.KeyMap, Count, Retcode);
        }
    }

}


