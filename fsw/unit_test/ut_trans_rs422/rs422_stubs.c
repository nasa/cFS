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

#include <termios.h>
#include <string.h>
#include <stdio.h>
#include "cfe.h"
#include "rs422_stubs.h"


/* Global */
struct termios test_term;

RS422_ReturnCodeTable_t     RS422_ReturnCodeTable[RS422_MAX_INDEX];

void RS422_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt)
{
    if (Index < RS422_MAX_INDEX) {
        RS422_ReturnCodeTable[Index].Value = RtnVal;
        RS422_ReturnCodeTable[Index].Count = CallCnt;
    }
    else {
        printf("Unsupported Index In SetReturnCode Call %u\n", Index);
    }
}


boolean RS422_UseReturnCode(uint32 Index)
{
    if (RS422_ReturnCodeTable[Index].Count > 0) {
        RS422_ReturnCodeTable[Index].Count--;
        if (RS422_ReturnCodeTable[Index].Count == 0)
            return(TRUE);
    }

    return(FALSE);
}

//int open(const char *path, int oflags)
//{
//    if (RS422_UseReturnCode(RS422_OPEN_INDEX))
//    {
//        return RS422_ReturnCodeTable[RS422_OPEN_INDEX].Value;
//    }
//
//    return -1;
//}

int tcsetattr(int fd, int flag, const struct termios *termios_p)
{
    memcpy((void *) &test_term, (void *) termios_p, sizeof(struct termios));
    return 0;
}

int tcgetattr(int fd, struct termios *termios_p)
{
    if (RS422_UseReturnCode(RS422_GETATTR_INDEX))
    {
        bzero((void *)&test_term, sizeof(test_term));
        return RS422_ReturnCodeTable[RS422_GETATTR_INDEX].Value;
    }

    memcpy((void *) termios_p, (void *) &test_term, sizeof(struct termios));
    return 0;
}

//int select(int a, fd_set *set, fd_set *except, struct timeval *timeout)
//{
//    if (RS422_UseReturnCode(RS422_SELECT_INDEX))
//    {
//        return RS422_ReturnCodeTable[RS422_SELECT_INDEX].Value;
//    }
//    
//    return 10;
//}

//int read(int fd, void * buffer, size_t nbytes)
//{
//    if (RS422_UseReturnCode(RS422_READ_INDEX))
//    {
//        return RS422_ReturnCodeTable[RS422_READ_INDEX].Value;
//    }
//
//    return nbytes;
//}

//int write(int fd, void * buffer, size_t nbytes)
//{
//    if (RS422_UseReturnCode(RS422_WRITE_INDEX))
//    {
//        return RS422_ReturnCodeTable[RS422_WRITE_INDEX].Value;
//    }
//
//    return nbytes;
//}
