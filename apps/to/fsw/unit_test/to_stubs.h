/*
 * File: to_stubs.h
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose:
 *   Provide stubs for unit testing TO
 *
 * History:
 *   Feb 2, 2016  G. de Carufel
 *    *
 */

#ifndef _Ut_TO_STUBS_H_
#define _Ut_TO_STUBS_H_

#include "uttools.h"

typedef enum
{
    UT_TO_CUSTOMINIT_INDEX,
    UT_TO_CUSTOMAPPCMDS_INDEX,
    UT_TO_CUSTOMPROCESSDATA_INDEX,
    UT_TO_CUSTOMENABLEOUTPUTCMD_INDEX,
    UT_TO_CUSTOMDISABLEOUTPUTCMD_INDEX,
    UT_TO_CUSTOMFRAMESTART_INDEX,
    UT_TO_CUSTOMFRAMESEND_INDEX,
    UT_TO_MAX_INDEX
} Ut_TO_INDEX_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} Ut_TO_ReturnCodeTable_t;


void Ut_TO_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
boolean Ut_TO_UseReturnCode(uint32 Index);


#endif
