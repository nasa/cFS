/*
 * File: tmtf_stubs.h
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
