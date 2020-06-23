/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * File: uttools.c
 *
 * Purpose: This file contains functions to implement a set of tools for use in unit testing.
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

bool UtMem2BinFile(const void *Memory, const char *Filename, uint32 Length)
{
    FILE   *fp;

    if ((fp = fopen(Filename, "w"))) {
        fwrite(Memory, Length, 1, fp);
        fclose(fp);
        return(true);
    }
    else
    {
        printf("UtMem2BinFile: Error Opening File: %s, %s\n", Filename, strerror(errno));
        UtAssert_True(false, "UtMem2BinFile: Error Opening File");
        return(false);
    }
}

bool UtBinFile2Mem(void *Memory, const char *Filename, uint32 Length)
{
    FILE   *fp;
    bool Success;

    Success = false;
    if ((fp = fopen(Filename, "r")))
    {
        if (fread(Memory, Length, 1, fp) == 1)
        {
            Success = true;
        }
        fclose(fp);
    }
    else
    {
        UtAssert_Failed("UtBinFile2Mem: Error Opening %s: %s", Filename, strerror(errno));
    }

    return(Success);
}

bool UtMem2HexFile(const void *Memory, const char *Filename, uint32 Length)
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
        return(true);
    }
    else {
        printf("UtMem2HexFile: Error Opening File: %s, %s\n", Filename, strerror(errno));
        UtAssert_True(false, "UtMem2HexFile: Error Opening File");
        return(false);
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

bool UtMemCmpValue(const void *Memory, uint8 Value, uint32 Length)
{
    uint32 i;
    const uint8  *Byte_ptr = Memory;

    for (i=0; i < Length; i++) {
        if (Byte_ptr[i] != Value) {
            return(false);
        }
    }
    return (true);
}

bool UtMemCmpCount(const void *Memory, uint32 Length)
{
    uint32 i;
    const uint8  *Byte_ptr = Memory;

    for (i=0; i < Length; i++) {
        if (Byte_ptr[i] != (i & 0xFF)) {
           return(false);
        }
    }
    return (true);
}

bool UtMem2BinFileCmp(const void *Memory, const char *Filename)
{
    FILE   *fp;
    const uint8  *MemByte = Memory;
    int     FileByte;
    bool Success;
    uint32  i;

    Success = true;
    if ((fp = fopen(Filename, "r"))) {

        for (i=0; (FileByte = fgetc(fp)) != EOF; i++) {
            if (MemByte[i] != FileByte) {
                Success = false;
                printf("UtMem2BinFileCmp: Miscompare in file: %s, byte offset: %lu, expected: %u, found: %u\n", Filename, (unsigned long)i, (unsigned int)MemByte[i], (unsigned int)FileByte);
                break;
            }            
        }
        fclose(fp);
    }
    else {
        Success = false;
        printf("UtMem2BinFileCmp: Error Opening File: %s, %s\n", Filename, strerror(errno));
        UtAssert_True(false, "UtMem2BinFileCmp: Error Opening File");
    }

    return(Success);
}
