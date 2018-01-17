/*
 * File: rs422_stubs.c
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
#include "udp_stubs.h"

UDP_HookTable_t           UDP_HookTable;
UDP_ReturnCodeTable_t     UDP_ReturnCodeTable[UDP_MAX_INDEX];

void UDP_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < UDP_MAX_INDEX) {
        UDP_ReturnCodeTable[Index].Value = RtnVal;
        UDP_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported Index In SetReturnCode Call %u\n", Index);
    }
}


boolean UDP_UseReturnCode(uint32 Index)
{
    if (UDP_ReturnCodeTable[Index].Count > 0) {
        UDP_ReturnCodeTable[Index].Count--;
        if (UDP_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}


void UDP_SetFunctionHook(uint32 Index, void *FunPtr)
{
    if (Index == UDP_SOCKET_INDEX)      
    { 
        UDP_HookTable.socket = FunPtr; 
    }
    else
    {
        printf("Unsupported UDP Index In SetFunctionHook Call %u\n", Index);
    }
}

void UDP_Reset(void)
{
    memset(&UDP_HookTable, 0, sizeof(UDP_HookTable));
    memset(&UDP_ReturnCodeTable, 0, sizeof(UDP_ReturnCodeTable));
}


int socket(int a, int b, int c)
{
    if (UDP_UseReturnCode(UDP_SOCKET_INDEX))
    {
        return UDP_ReturnCodeTable[UDP_SOCKET_INDEX].Value;
    }
    return 1;
}

int setsockopt(int socket, int level, int option_name, 
               const void *option_value, int option_len)
{
    if (UDP_UseReturnCode(UDP_SETSOCKOPT_INDEX))
    {
        return UDP_ReturnCodeTable[UDP_SETSOCKOPT_INDEX].Value;
    }
    return 0;
}

//Use POSIX version instead
//int inet_aton(const char *cp, struct in_addr *inp)
//{
//    if (UDP_UseReturnCode(UDP_INET_ATON_INDEX))
//    {
//        return UDP_ReturnCodeTable[UDP_INET_ATON_INDEX].Value;
//    }
//    *inp = INADDR_ANY;
//
//    return INADDR_ANY;
//}


int bind(int sockfd, const struct sockaddr *addr, int addrlen)
{
    if (UDP_UseReturnCode(UDP_BIND_INDEX))
    {
        return UDP_ReturnCodeTable[UDP_BIND_INDEX].Value;
    }
    return 0;
}

int close(int fd)
{
    if (UDP_UseReturnCode(UDP_CLOSE_INDEX))
    {
        return UDP_ReturnCodeTable[UDP_CLOSE_INDEX].Value;
    }
    return 0;
}

ssize_t recvfrom(int socket, void *buffer, size_t length,
                 int flags, struct sockaddr *address,
                 int *address_len)
{
    if (UDP_UseReturnCode(UDP_RECVFROM_INDEX))
    {
        return UDP_ReturnCodeTable[UDP_RECVFROM_INDEX].Value;
    }
    return length;
}


int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout)
{
    if (UDP_UseReturnCode(UDP_SELECT_INDEX))
    {
        return UDP_ReturnCodeTable[UDP_SELECT_INDEX].Value;
    }
    return 1;
}

size_t sendto(int socket, const void *message, size_t length,
              int flags, const struct sockaddr *dest_addr,
              int dest_len)
{
    if (UDP_UseReturnCode(UDP_SENDTO_INDEX))
    {
        return UDP_ReturnCodeTable[UDP_SENDTO_INDEX].Value;
    }
    return length;
}
