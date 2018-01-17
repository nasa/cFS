/*
 * File: cop1_stubs.h
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose:
 *   Provide stubs for unit testing COP-1
 *
 * History:
 *   03/16/2015  A. Asp, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _COP1_STUBS_H_
#define _COP1_STUBS_H_

#include "uttools.h"
#include "tctf.h"

typedef enum
{
    COP1_TCTF_GETVCID_INDEX,
    COP1_TCTF_GETBYPASSFLG_INDEX,
    COP1_TCTF_GETCTLCMDFLG_INDEX,
    COP1_TCTF_COPYDATA_INDEX,
    COP1_TCTF_ISVALIDTF_INDEX,
    COP1_MAX_INDEX
} COP1_Index_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} COP1_ReturnCodeTable_t;

typedef struct
{
    uint16  (*TCTF_GetVcId)(TCTF_Hdr_t *);
    uint16  (*TCTF_GetBypassFlag)(TCTF_Hdr_t *);
    uint16  (*TCTF_GetCtlCmdFlag)(TCTF_Hdr_t *);
    void    (*TCTF_CopyData)(uint8 *, TCTF_Hdr_t *, TCTF_ChannelService_t *);
    boolean (*TCTF_IsValidTf)(TCTF_Hdr_t *, TCTF_ChannelService_t *);
} COP1_HookTable_t;


void COP1_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void COP1_SetFunctionHook(uint32 Index, void *FunPtr);
void COP1_Reset(void);


#endif
