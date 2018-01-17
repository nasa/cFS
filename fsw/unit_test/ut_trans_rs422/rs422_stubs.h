/*
 * File: rs422_stubs.h
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
 *   04/16/2015  G. de Carufel
 *    * Created
 */

#ifndef _RS422_STUBS_H_
#define _RS422_STUBS_H_

#include "uttools.h"

typedef enum
{
    RS422_GETATTR_INDEX,
    RS422_SELECT_INDEX,
    RS422_READ_INDEX,
    RS422_WRITE_INDEX,
    RS422_MAX_INDEX
} RS422_INDEX_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} RS422_ReturnCodeTable_t;


void RS422_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
boolean RS422_UseReturnCode(uint32 Index);

#endif
