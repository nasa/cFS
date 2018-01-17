/*
 * File: TMTF_stubs.c
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

#include "tmtf_stubs.h"

TMTF_ReturnCodeTable_t     TMTF_ReturnCodeTable[TMTF_MAX_INDEX];

void TMTF_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < TMTF_MAX_INDEX) {
        TMTF_ReturnCodeTable[Index].Value = RtnVal;
        TMTF_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported Index In SetReturnCode Call %u\n", Index);
    }
}


boolean TMTF_UseReturnCode(uint32 Index)
{
    if (TMTF_ReturnCodeTable[Index].Count > 0) {
        TMTF_ReturnCodeTable[Index].Count--;
        if (TMTF_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}

