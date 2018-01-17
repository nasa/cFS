/*
 * File: tcTCTF_stubs.c
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose:
 *  Stub out various functions not stubbed out by the UT-Assert code
 *
 */

#include "cfe.h"

#include "tctf_stubs.h"

TCTF_ReturnCodeTable_t     TCTF_ReturnCodeTable[TCTF_MAX_INDEX];

void TCTF_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < TCTF_MAX_INDEX) {
        TCTF_ReturnCodeTable[Index].Value = RtnVal;
        TCTF_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported Index In SetReturnCode Call %u\n", Index);
    }
}


boolean TCTF_UseReturnCode(uint32 Index)
{
    if (TCTF_ReturnCodeTable[Index].Count > 0) {
        TCTF_ReturnCodeTable[Index].Count--;
        if (TCTF_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}

