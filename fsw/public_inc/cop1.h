/*******************************************************************************
 * File: cop1.h
 *
 * Copyright 2017 United States Government as represented by the Administrator
 * of the National Aeronautics and Space Administration.  No copyright is
 * claimed in the United States under Title 17, U.S. Code.
 * All Other Rights Reserved.
 *
 * Purpose:
 *   Provides the interface functionality for the receiver side of the Communications
 *   Operation Procedure-1 (COP-1), i.e. the Frame Acceptance and Reporting
 *   Mechanism-1 (FARM-1)
 *
 * Reference(s):
 *   - _Communications Operation Procedure-1_, CCSDS 232.1-B-2
 *
 * Notes:
 *   -Setter functions have not been provided for all the fields in the Communications
 *    Link Control Word (CLCW).  The value for the virtual channel ID is set when the
 *    CLCW is initialized.  Most other values are updated internally when processing
 *    a transfer frame.  Values set outside of the COP1 procedure have setters
 *    provided.
 *
 *
 * History:
 *   03/16/2015  Alan Asp, Odyssey Space Research, LLC
 *    * Created
 *
 ******************************************************************************/

#ifndef _COP1_H_
#define _COP1_H_

#include "io_lib.h"
#include "tctf.h"


#define COP1_SLIDING_WINDOW_WIDTH 126

#define COP1_SUCCESS          0
#define COP1_BADINPUT_ERR    -1
#define COP1_INVALID_TF_ERR  -2
#define COP1_FARM1_ERR       -3

typedef struct
{
    uint8 Status;     /* CLCW info and status  */
    uint8 Channel;    /* virtual channel ID    */
    uint8 Flags;      /* flags                 */
    uint8 Report;     /* report value, a.k.a the Next Expected Frame Sequence Number, N(R) */
} COP1_Clcw_t;


int32   COP1_InitClcw(COP1_Clcw_t *clcwPtr, uint16 vcId);
int32   COP1_ProcessFrame(uint8* toBuffer, COP1_Clcw_t *clcwPtr, TCTF_Hdr_t *tfPtr,
                       TCTF_ChannelService_t *channelService);


uint16  COP1_GetClcwCtrlWordType(COP1_Clcw_t *clcwPtr);

uint16  COP1_GetClcwVersion(COP1_Clcw_t *clcwPtr);

void    COP1_SetClcwStatus(COP1_Clcw_t *clcwPtr, uint16 value);
uint16  COP1_GetClcwStatus(COP1_Clcw_t *clcwPtr);

uint16  COP1_GetClcwCopEffect(COP1_Clcw_t *clcwPtr);

uint16  COP1_GetClcwVcId(COP1_Clcw_t *clcwPtr);

void    COP1_SetClcwNoRf(COP1_Clcw_t *clcwPtr, boolean value);
boolean COP1_GetClcwNoRf(COP1_Clcw_t *clcwPtr);

void    COP1_SetClcwNoBitlock(COP1_Clcw_t *clcwPtr, boolean value);
boolean COP1_GetClcwNoBitlock(COP1_Clcw_t *clcwPtr);

boolean COP1_GetClcwLockout(COP1_Clcw_t *clcwPtr);

boolean COP1_GetClcwWait(COP1_Clcw_t *clcwPtr);

boolean COP1_GetClcwRetransmit(COP1_Clcw_t *clcwPtr);

uint16  COP1_GetClcwFarmbCtr(COP1_Clcw_t *clcwPtr);

uint8   COP1_GetClcwReport(COP1_Clcw_t *clcwPtr);


#endif /* _COP1_H_ */
