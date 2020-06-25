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

/*================================================================================*
** File:  ut_oscore_select_test.c
** Owner: Chris Knight
** Date:  March 2020
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_select_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_SELECT_FN "/cf/select_test.tmp"

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

char *fsAddrPtr = NULL;
static int32 setup_file(void)
{
    UT_SETUP(OS_mkfs(fsAddrPtr, "/ramdev3", "RAM3", 512, 20));
    UT_SETUP(OS_mount("/ramdev3", "/drive3"));
    return OS_creat("/drive3/select_test.txt", OS_READ_WRITE);
}

static void teardown_file(int32 fd)
{
    OS_close(fd);
    OS_remove("/drive3/select_test.txt");
    OS_unmount("/drive3");
    OS_rmfs("/ramdev3");
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_SelectFdZero, OS_SelectFdAdd, OS_SelectFdClear, OS_SelectFdIsSet
** Purpose: Configure file descriptor set for select
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_select_fd_test(void)
{
    OS_FdSet FdSet;
    int32 fd = setup_file();

    if(OS_SelectFdZero(&FdSet) == OS_ERR_NOT_IMPLEMENTED
        || OS_SelectFdAdd(&FdSet, fd) == OS_ERR_NOT_IMPLEMENTED
        || OS_SelectFdClear(&FdSet, fd) == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssertEx(false, UTASSERT_CASETYPE_NA, __FILE__, __LINE__, "OS_SelectFd...() not implemented");
        goto UT_os_select_fd_test_exit_tag;
    }

    UtAssert_Simple(OS_SelectFdZero(NULL) == OS_INVALID_POINTER);
    UtAssert_Simple(OS_SelectFdAdd(NULL, 0) == OS_INVALID_POINTER);
    UtAssert_Simple(OS_SelectFdClear(NULL, 0) == OS_INVALID_POINTER);
    UtAssert_Simple(OS_SelectFdIsSet(NULL, 0) == false);

    OS_SelectFdZero(&FdSet);
    OS_SelectFdAdd(&FdSet, fd);
    UtAssert_Simple(OS_SelectFdIsSet(&FdSet, fd));

    OS_SelectFdZero(&FdSet);
    OS_SelectFdAdd(&FdSet, fd);
    OS_SelectFdClear(&FdSet, fd);
    UtAssert_Simple(!OS_SelectFdIsSet(&FdSet, fd));

UT_os_select_fd_test_exit_tag:
    teardown_file(fd);
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_SelectSingle(uint32 objid, uint32 *StateFlags, int32 msecs);
** Purpose: Select on a single file descriptor
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_select_single_test(void)
{
    uint32 StateFlags;
    int32 fd = setup_file();
    int32 rc;

    UT_RETVAL(OS_SelectSingle(fd, NULL, 0), OS_INVALID_POINTER, "NULL flags pointer");

    StateFlags = OS_STREAM_STATE_WRITABLE;
    rc = OS_SelectSingle(fd, &StateFlags, 0);
    if(rc == OS_ERR_NOT_IMPLEMENTED || rc == OS_ERR_OPERATION_NOT_SUPPORTED)
    {
        UtAssertEx(false, UTASSERT_CASETYPE_NA, __FILE__, __LINE__, "OS_SelectSingle() not supported");
        goto UT_os_select_single_test_exit_tag;
    }

    UT_RETVAL(rc, OS_SUCCESS, "OS_SelectSingle(fd, OS_STREAM_STATE_WRITABLE, 0)");
    UtAssert_True((StateFlags & OS_STREAM_STATE_WRITABLE) != 0, "StateFlags (0x%x) & OS_STREAM_STATE_WRITABLE", (unsigned int)StateFlags);

    StateFlags = OS_STREAM_STATE_READABLE;
    rc = OS_SelectSingle(fd, &StateFlags, 1);
    UT_RETVAL(rc, OS_SUCCESS, "OS_SelectSingle(fd, OS_STREAM_STATE_READABLE, 0)");
    UtAssert_True((StateFlags & OS_STREAM_STATE_READABLE) != 0, "StateFlags (0x%x) & OS_STREAM_STATE_READABLE", (unsigned int)StateFlags);

UT_os_select_single_test_exit_tag:
    teardown_file(fd);
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs)
** Purpose: Select on a multiple file descriptors
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_select_multi_test(void)
{
    OS_FdSet ReadSet, WriteSet;
    int32 fd = setup_file();
    int32 rc;

    OS_SelectFdZero(&WriteSet);
    OS_SelectFdAdd(&WriteSet, fd);
    rc = OS_SelectMultiple(NULL, &WriteSet, 1);
    if(rc == OS_ERR_NOT_IMPLEMENTED || rc == OS_ERR_OPERATION_NOT_SUPPORTED)
    {
        UtAssertEx(false, UTASSERT_CASETYPE_NA, __FILE__, __LINE__, "OS_SelectMultiple() not supported");
        goto UT_select_multi_test_exit_tag;
    }


    UT_RETVAL(rc, OS_SUCCESS, "OS_SelectMultiple(NULL, &WriteSet, 1)");
    UtAssert_True(OS_SelectFdIsSet(&WriteSet, fd), "OS_SelectFdIsSet(&WriteSet, fd)");

    OS_SelectFdZero(&ReadSet);
    OS_SelectFdAdd(&ReadSet, fd);
    rc = OS_SelectMultiple(&ReadSet, NULL, 1);
    UT_RETVAL(rc, OS_SUCCESS, "OS_SelectMultiple(&ReadSet, NULL, 1)");
    UtAssert_True(OS_SelectFdIsSet(&ReadSet, fd), "!OS_SelectFdIsSet(&ReadSet, fd)");

UT_select_multi_test_exit_tag:
    teardown_file(fd);
}

/*================================================================================*
** End of File: ut_oscore_queue_test.c
**================================================================================*/
