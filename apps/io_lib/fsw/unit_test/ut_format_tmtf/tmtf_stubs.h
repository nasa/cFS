/*
 * File: tmtf_stubs.h
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
 *   04/26/2015  A. Asp, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _TMTF_STUBS_H_
#define _TMTF_STUBS_H_

#include "uttools.h"

typedef enum
{
    TMTF_MAX_INDEX
} TMTF_INDEX_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} TMTF_ReturnCodeTable_t;


void    TMTF_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
boolean TMTF_UseReturnCode(uint32 Index);


#endif
