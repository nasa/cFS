/*
 * File: cop1_stubs.c
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose:
 *  Stub out various functions not stubbed out by the UT-Assert code
 *
 * Modification History:
 *   03/16/2015  Alan Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */

#include <string.h>

#include "cfe.h"

#include "cop1_stubs.h"


COP1_HookTable_t           COP1_HookTable;
COP1_ReturnCodeTable_t     COP1_ReturnCodeTable[COP1_MAX_INDEX];


void COP1_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < COP1_MAX_INDEX) {
        COP1_ReturnCodeTable[Index].Value = RtnVal;
        COP1_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported Index In SetReturnCode Call %u\n", Index);
    }
}


boolean COP1_UseReturnCode(uint32 Index)
{
    if (COP1_ReturnCodeTable[Index].Count > 0) {
        COP1_ReturnCodeTable[Index].Count--;
        if (COP1_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}


void COP1_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if      (Index == COP1_TCTF_GETVCID_INDEX)      { COP1_HookTable.TCTF_GetVcId = FunPtr; }
    else if (Index == COP1_TCTF_GETBYPASSFLG_INDEX) { COP1_HookTable.TCTF_GetBypassFlag = FunPtr; }
    else if (Index == COP1_TCTF_GETCTLCMDFLG_INDEX) { COP1_HookTable.TCTF_GetCtlCmdFlag = FunPtr; }
    else if (Index == COP1_TCTF_COPYDATA_INDEX)     { COP1_HookTable.TCTF_CopyData = FunPtr; }
    else
    {
        printf("Unsupported COP1 Index In SetFunctionHook Call %u\n", Index);
    }
}


void COP1_Reset(void)
{
    memset(&COP1_HookTable, 0, sizeof(COP1_HookTable));
    memset(&COP1_ReturnCodeTable, 0, sizeof(COP1_ReturnCodeTable));
}


uint16 TCTF_GetVcId(TCTF_Hdr_t *tfPtr)
{
    /* Check for specified return */
    if (COP1_UseReturnCode(COP1_TCTF_GETVCID_INDEX))
        return COP1_ReturnCodeTable[COP1_TCTF_GETVCID_INDEX].Value;

    return 0xFFFF;
}


uint16 TCTF_GetBypassFlag(TCTF_Hdr_t *tfPtr)
{
    /* Check for specified return */
    if (COP1_UseReturnCode(COP1_TCTF_GETBYPASSFLG_INDEX))
        return COP1_ReturnCodeTable[COP1_TCTF_GETBYPASSFLG_INDEX].Value;

    return 0xFFFF;
}


uint16 TCTF_GetCtlCmdFlag(TCTF_Hdr_t *tfPtr)
{
    /* Check for specified return */
    if (COP1_UseReturnCode(COP1_TCTF_GETCTLCMDFLG_INDEX))
    {
        return COP1_ReturnCodeTable[COP1_TCTF_GETCTLCMDFLG_INDEX].Value;
    }

    return 0xFFFF;
}


uint16 TCTF_CopyData(uint8 *toBuffer, TCTF_Hdr_t *tfPtr, TCTF_ChannelService_t *channelService)
{
    if (COP1_UseReturnCode(COP1_TCTF_COPYDATA_INDEX))
    {
        return (uint16) COP1_ReturnCodeTable[COP1_TCTF_COPYDATA_INDEX].Value;
    }

    /* Check for Function Hook */
    if (COP1_HookTable.TCTF_CopyData)
    {
        COP1_HookTable.TCTF_CopyData(toBuffer, tfPtr, channelService);
    }

    return 0;
}

boolean TCTF_IsValidTf(TCTF_Hdr_t *tfPtr, TCTF_ChannelService_t *channelService)
{
    /* Check for specified return */
    if (COP1_UseReturnCode(COP1_TCTF_ISVALIDTF_INDEX))
    {
        return (boolean)COP1_ReturnCodeTable[COP1_TCTF_ISVALIDTF_INDEX].Value;
    }

    return FALSE;
}

