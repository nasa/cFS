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
 * File: uttools.h
 *
 * Purpose: This file contains functions to implement a set of tools for use in unit testing.
 */

#ifndef _uttools_
#define	_uttools_

/*
 * Includes
 */

#include "common_types.h"
#include "utassert.h"

/*
 * Exported Functions
 */

/* Copies a region of memory to a binary file.  This file can be reloaded by calling UtBinFile2Mem or it can be
 * used to verify test results by calling UtMem2BinFileCmp. */
bool             UtMem2BinFile(const void *Memory, const char *Filename, uint32 Length);

/* Copies a binary file to a region of memory. */
bool             UtBinFile2Mem(void *Memory, const char *Filename, uint32 Length);

/* Copies a region of memory to a hex file */
bool             UtMem2HexFile(const void *Memory, const char *Filename, uint32 Length);

/* Fills a region of memory with a byte count pattern. */
void                UtMemFill(void *Memory, uint32 Length);

/* Just like the standard sprintf except it returns a pointer to the result string.  The result string
 * cannot be larger than 256 bytes.  */
char               *UtSprintf(const char *Spec, ...);

/* Calls UtPrintf to print a range of memory as hex bytes. */
void                UtPrintx(const void *Memory, uint32 Length);

/* Compares a region of memory to a static pattern and determines if they are equal.  Note: Use UtMemSet to
 * fill a region of memory with a static pattern. */
bool             UtMemCmpValue(const void *Memory, uint8 Value, uint32 Length);

/* Compares a region of memory to a byte count pattern and determines if they are equal.  Note: Use UtMemFill to
 * fill a region of memory with a byte count pattern. */
bool             UtMemCmpCount(const void *Memory, uint32 Length);

/* Compares a region of memory with the contents of a binary file and determines if they are equal.  Note: Use
 * UtMem2BinFile to copy a region of memory to a binary file. */
bool             UtMem2BinFileCmp(const void *Memory, const char *Filename);


/* Macros to implement simple printf-like functions for unit testing */
#define UtPrintf(...)   UtAssert_Message(UTASSERT_CASETYPE_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define UtDebug(...)    UtAssert_Message(UTASSERT_CASETYPE_DEBUG, __FILE__, __LINE__, __VA_ARGS__)



#endif

