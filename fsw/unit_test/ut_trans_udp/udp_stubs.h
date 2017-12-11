/*
 * File: rs422_stubs.h
 *
 * Purpose:
 *   Provide stubs for unit testing transfer frames
 *
 * History:
 *   04/16/2015  G. de Carufel
 *    * Created
 */

#ifndef _UDP_STUBS_H_
#define _UDP_STUBS_H_

#include "uttools.h"

typedef enum
{
    UDP_SOCKET_INDEX,
    UDP_SETSOCKOPT_INDEX,
    UDP_INET_ATON_INDEX,
    UDP_BIND_INDEX,
    UDP_CLOSE_INDEX,
    UDP_SELECT_INDEX,
    UDP_RECVFROM_INDEX,
    UDP_SENDTO_INDEX,
    UDP_MAX_INDEX
} UDP_INDEX_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} UDP_ReturnCodeTable_t;


typedef struct
{
   int32 (*socket)(int a, int b, int c);
} UDP_HookTable_t;


void UDP_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void UDP_SetFunctionHook(uint32 Index, void *FunPtr);
boolean UDP_UseReturnCode(uint32 Index);
void UDP_Reset(void);

#endif
