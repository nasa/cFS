/*
 * File: tctf_stubs.h
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose:
 *   Provide stubs for unit testing transfer frames
 *
 * History:
 *   04/13/2015  A. Asp
 *    * Created
 */

#ifndef _TCTF_STUBS_H_
#define _TCTF_STUBS_H_

#include "uttools.h"

typedef enum
{
    TCTF_MAX_INDEX
} TCTF_INDEX_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} TCTF_ReturnCodeTable_t;


void TCTF_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
boolean TCTF_UseReturnCode(uint32 Index);


#endif
