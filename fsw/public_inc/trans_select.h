/*******************************************************************************
* File: trans_select.c
*
*   Copyright 2017 United States Government as represented by the Administrator
*   of the National Aeronautics and Space Administration.  No copyright is
*   claimed in the United States under Title 17, U.S. Code.
*   All Other Rights Reserved.
*
* Purpose:
*   Provides the functionality to do synchronous I/O multiplexing with 
*   different devices (UDP, RS422) with the use of the POSIX select library.
*
* Notes:
*   1. Use this library if you want to read on multiple devices simultaneously
*   2. For output, select will return the first available device. It will not
*   send over multiple devices, but simply returns the next available device.
*
* History:
*   June 1, 2015  Guy de Carufel
*    * Created
*
******************************************************************************/
#ifndef _IO_TRANS_SELECT_
#define _IO_TRANS_SELECT_

#include "io_lib.h"

/* Select is used to do synchronous I/O multiplexing over multiple devices. */
/* The user is responsible for calling the appropriate device based on the 
 * return of select. Here is the general pattern to use with select:
 *
 * 1. Initialize the Select Device set with IO_TransSelectInitSet()
 * 2. Initialize all devices
 * 3. Add each device to the select fd_set through IO_TransSelectAddFd()
 * 4. Call TransSelectInput() or TransSelectOutput()
 *    Use IO_TRANS_PEND_FOREVER for infinite timeout.
 * 5. Call TransSelectInActiveSet() to verify if fd is the active fd. 
 *
 * X. If a device is closed, remove it from the set with
 * IO_TransSelectRemoveFd()*/

/* All Devices must be set as non-blocking to work with select. */

#define IO_TRANS_SELECT_NO_ERROR                     0
#define IO_TRANS_SELECT_ERROR                       -1
#define IO_TRANS_SELECT_EMPTY_SET_ERR               -2
#define IO_TRANS_SELECT_NULL_SET_ERR                -3
#define IO_TRANS_SELECT_INVALID_TIMEOUT_ERR         -4
#define IO_TRANS_SELECT_BAD_FD_ERR                  -5


/* Structures */
typedef struct {
    int32   iMaxFdNum;       /**< The largest FD number  */
    fd_set  fdSetFull;       /**< The full FD set        */
    fd_set  fdSetActive;     /**< The active FD set      */
} IO_TransSelect_t;

/* Prototypes */
int32 IO_TransSelectClear(IO_TransSelect_t *pSet); 
int32 IO_TransSelectAddFd(IO_TransSelect_t *pSet, int32 fd); 
int32 IO_TransSelectRemoveFd(IO_TransSelect_t *pSet, int32 fd);
int32 IO_TransSelectFdInFull(IO_TransSelect_t *pSet, int32 fd);
int32 IO_TransSelectFdInActive(IO_TransSelect_t *pSet, int32 fd);
int32 IO_TransSelectInput(IO_TransSelect_t *pSet, int32 timeoutUSec);
int32 IO_TransSelectOutput(IO_TransSelect_t *pSet, int32 timeoutUSec);

#endif
