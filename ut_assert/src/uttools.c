/*
**
** File: uttools.c
**
** $Id: uttools.c 1.5 2015/06/16 16:14:05EDT sstrege Exp  $
**
** Copyright 2012-2013 United States Government as represented by the 
** Administrator of the National Aeronautics and Space Administration. 
** All Other Rights Reserved.  
**
** This software was created at NASA's Goddard Space Flight Center.
** This software is governed by the NASA Open Source Agreement and may be 
** used, distributed and modified only pursuant to the terms of that 
** agreement.
**
** Purpose: This file contains functions to implement a set of tools for use in unit testing.
**
** $Log: uttools.c  $
** Revision 1.5 2015/06/16 16:14:05EDT sstrege 
** Added copyright information
** Revision 1.4 2015/05/01 14:54:02EDT czogby 
** Add UtAssert fail where error messages
** Revision 1.3 2015/03/10 15:21:44EDT czogby 
** Add Missing Functionality to UT Assert Library
** --- Added comments ---  czogby [2015/03/31 18:37:45Z]
** No updates were made to this source code in this revision
**
*/

/*
 * Includes
 */

#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "common_types.h"
#include "utassert.h"
#include "utbsp.h"

#define UT_SNPRINTF_BUF_SIZE        256
#define UT_SNPRINTF_MAX_BUFS        10

/*
 * Local types/objects
 */
typedef struct
{
    char Text[UT_SNPRINTF_BUF_SIZE];
} UT_Snprintf_Buffer_t;

/*
 * Function Definitions
 */

osalbool UtMem2BinFile(const void *Memory, const char *Filename, uint32 Length)
{
    FILE   *fp;

    if ((fp = fopen(Filename, "w"))) {
        fwrite(Memory, Length, 1, fp);
        fclose(fp);
        return(TRUE);
    }
    else
    {
        printf("UtMem2BinFile: Error Opening File: %s, %s\n", Filename, strerror(errno));
        UtAssert_True(FALSE, "UtMem2BinFile: Error Opening File");
        return(FALSE);
    }
}

osalbool UtBinFile2Mem(void *Memory, const char *Filename, uint32 Length)
{
    FILE   *fp;
    osalbool Success;

    Success = FALSE;
    if ((fp = fopen(Filename, "r")))
    {
        if (fread(Memory, Length, 1, fp) == 1)
        {
            Success = TRUE;
        }
        fclose(fp);
    }
    else
    {
        UtAssert_Failed("UtBinFile2Mem: Error Opening %s: %s", Filename, strerror(errno));
    }

    return(Success);
}

osalbool UtMem2HexFile(const void *Memory, const char *Filename, uint32 Length)
{
    FILE        *fp;
    uint32       i;
    uint32       j;

    if ((fp = fopen(Filename, "w"))) {

        for (i=0; i < Length; i+=16) {
            fprintf(fp, "   %06lX: ", (unsigned long)i);
            for (j=0; j < 16; j++) {
                if ((i+j) < Length)
                    fprintf(fp, "%02X ", ((uint8 *)Memory)[i+j]);
                else
                    fprintf(fp, "   ");
            }
            fprintf(fp, " ");
            for (j=0; j < 16; j++) {
                if ((i+j) < Length)
                    fprintf(fp, "%c", isprint(((uint8 *)Memory)[i+j]) ? ((uint8 *)Memory)[i+j] : '.');
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
        return(TRUE);
    }
    else {
        printf("UtMem2HexFile: Error Opening File: %s, %s\n", Filename, strerror(errno));
        UtAssert_True(FALSE, "UtMem2HexFile: Error Opening File");
        return(FALSE);
    }
}

void UtMemFill(void *Memory, uint32 Length)
{
    uint32 i;
    uint8  *Byte_ptr = Memory;

    for(i=0; i < Length; i++) {
        Byte_ptr[i] = i;
    }
}

void UtMessage(uint8 MessageType, const char *File, uint32 Line, const char *Spec, ...)
{
    UT_Snprintf_Buffer_t Buf;
    va_list va;

    va_start(va, Spec);
    vsnprintf(Buf.Text, sizeof(Buf.Text), Spec, va);
    va_end(va);

    UT_BSP_DoText(MessageType, Buf.Text);
}

char *UtSprintf(const char *Spec, ...)
{
    va_list         Args;
    static UT_Snprintf_Buffer_t Buf[UT_SNPRINTF_MAX_BUFS];
    static uint32   TextIndex = 0;

    ++TextIndex;
    if (TextIndex >= UT_SNPRINTF_MAX_BUFS) TextIndex = 0;

    va_start(Args, Spec);
    vsnprintf(Buf[TextIndex].Text, sizeof(Buf[TextIndex].Text), Spec, Args);
    va_end(Args);

    return(Buf[TextIndex].Text);
}

void UtPrintx(const void *Memory, uint32 Length)
{
    uint32 i;
    uint32 j;
    const uint8  *Byte_ptr = Memory;
    char OutputLine[50];
    char *OutPtr;

    i=0;
    while (1)
    {
        OutPtr = OutputLine;
        for (j=0; j < 16 && i < Length; j++, i++) {
            sprintf(OutPtr, "%02X  ", Byte_ptr[i]);
            OutPtr += 3;
        }
        UtPrintf("%s\n", OutputLine);
    }
}

osalbool UtMemCmpValue(const void *Memory, uint8 Value, uint32 Length)
{
    uint32 i;
    const uint8  *Byte_ptr = Memory;

    for (i=0; i < Length; i++) {
        if (Byte_ptr[i] != Value) {
            return(FALSE);
        }
    }
    return (TRUE);
}

osalbool UtMemCmpCount(const void *Memory, uint32 Length)
{
    uint32 i;
    const uint8  *Byte_ptr = Memory;

    for (i=0; i < Length; i++) {
        if (Byte_ptr[i] != (i & 0xFF)) {
           return(FALSE);
        }
    }
    return (TRUE);
}

osalbool UtMem2BinFileCmp(const void *Memory, const char *Filename)
{
    FILE   *fp;
    const uint8  *MemByte = Memory;
    int     FileByte;
    osalbool Success;
    uint32  i;

    Success = TRUE;
    if ((fp = fopen(Filename, "r"))) {

        for (i=0; (FileByte = fgetc(fp)) != EOF; i++) {
            if (MemByte[i] != FileByte) {
                Success = FALSE;
                printf("UtMem2BinFileCmp: Miscompare in file: %s, byte offset: %lu, expected: %u, found: %u\n", Filename, (unsigned long)i, (unsigned int)MemByte[i], (unsigned int)FileByte);
                break;
            }            
        }
        fclose(fp);
    }
    else {
        Success = FALSE;
        printf("UtMem2BinFileCmp: Error Opening File: %s, %s\n", Filename, strerror(errno));
        UtAssert_True(FALSE, "UtMem2BinFileCmp: Error Opening File");
    }

    return(Success);
}
