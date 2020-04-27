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
    OS_mkfs(fsAddrPtr, "/ramdev3", " ", 512, 20);
    OS_mount("/ramdev3", "/drive3");
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

    if(OS_SelectSingle(fd, &StateFlags, 0) == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssertEx(false, UTASSERT_CASETYPE_NA, __FILE__, __LINE__, "OS_SelectSingle() not implemented");
        goto UT_os_select_single_test_exit_tag;
    }

    UtAssert_Simple(OS_SelectSingle(fd, NULL, 0) != OS_SUCCESS);

    StateFlags = OS_STREAM_STATE_WRITABLE;
    UtAssert_Simple(OS_SelectSingle(fd, &StateFlags, 0) == OS_SUCCESS && StateFlags & OS_STREAM_STATE_WRITABLE);

    StateFlags = OS_STREAM_STATE_READABLE;
    UtAssert_Simple(OS_SelectSingle(fd, &StateFlags, 1) == OS_SUCCESS);

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

    if(OS_SelectMultiple(&ReadSet, &WriteSet, 1) == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssertEx(false, UTASSERT_CASETYPE_NA, __FILE__, __LINE__, "OS_SelectMultiple() not implemented");
        goto UT_select_multi_test_exit_tag;
    }

    OS_SelectFdZero(&WriteSet);
    OS_SelectFdAdd(&WriteSet, fd);
    UtAssert_Simple(OS_SelectMultiple(NULL, &WriteSet, 1) == OS_SUCCESS);

    OS_SelectFdZero(&ReadSet);
    OS_SelectFdAdd(&ReadSet, fd);
    UtAssert_Simple(OS_SelectMultiple(&ReadSet, NULL, 1) == OS_SUCCESS);

    OS_SelectFdZero(&ReadSet);
    OS_SelectFdAdd(&ReadSet, fd);
    OS_SelectFdZero(&WriteSet);
    UtAssert_Simple(OS_SelectMultiple(&ReadSet, &WriteSet, 0) == OS_SUCCESS);

UT_select_multi_test_exit_tag:
    teardown_file(fd);
}

/*================================================================================*
** End of File: ut_oscore_queue_test.c
**================================================================================*/
