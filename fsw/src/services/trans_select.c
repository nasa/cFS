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
* Reference:
*   http://linux.die.net/man/2/select
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

#include <errno.h>
#include <string.h>

#include "cfe_psp.h"
#include "trans_select.h"

/*******************************************************************************
                          Local Private Functions 
*******************************************************************************/
/** Local functions */
static int32 IO_TransSelectCheckArgs(IO_TransSelect_t *pSet, int32 fd, 
                                     int32 event)
{
    if (pSet == NULL)
    {
        if (event)
        {
            CFE_EVS_SendEvent(IO_LIB_TRANS_SELECT_EID, CFE_EVS_ERROR,
                              "IO_TransSelect Error: NULL Set input.");
        }
        return IO_TRANS_SELECT_NULL_SET_ERR;
    }

    if (fd < 0)
    {
        if (event)
        {
            CFE_EVS_SendEvent(IO_LIB_TRANS_SELECT_EID, CFE_EVS_ERROR,
                              "IO_TransSelect Error: Bad FD input.");
        }
        return IO_TRANS_SELECT_BAD_FD_ERR;
    }
    return IO_TRANS_SELECT_NO_ERROR;
}

static int32 IO_TransSelectCheckIOArgs(IO_TransSelect_t *pSet,int32 timeoutUSec)
{
    if (pSet == NULL)
    {
        return IO_TRANS_SELECT_NULL_SET_ERR;
    }
    if (pSet->iMaxFdNum < 0)
    {
        return IO_TRANS_SELECT_EMPTY_SET_ERR;
    }
    if (timeoutUSec < 0 && timeoutUSec != IO_TRANS_PEND_FOREVER)
    {
        return IO_TRANS_SELECT_INVALID_TIMEOUT_ERR;
    }
    
    return IO_TRANS_SELECT_NO_ERROR;
}


/*******************************************************************************
                                API Functions 
*******************************************************************************/

/** Initialize the set */
int32 IO_TransSelectClear(IO_TransSelect_t *pSet) 
{
    int32 status = IO_TransSelectCheckArgs(pSet, 0, TRUE);
    if (status < 0)
    {
        return status;
    }

    pSet->iMaxFdNum = -1;
    FD_ZERO(&pSet->fdSetFull);
    FD_ZERO(&pSet->fdSetActive);
   
    return status;
}


/** Adds the specified fd from the set. */
int32 IO_TransSelectAddFd(IO_TransSelect_t *pSet, int32 fd) 
{
    int32 status = IO_TransSelectCheckArgs(pSet, fd, TRUE);
    if (status < 0)
    {
        return status;
    }
    
    if (fd > pSet->iMaxFdNum)
    {
        pSet->iMaxFdNum = fd;
    }

    FD_SET((int) fd, &pSet->fdSetFull);
    
    return status;
}


/** Removes the specifies fd from the set. */
int32 IO_TransSelectRemoveFd(IO_TransSelect_t *pSet, int32 fd)
{
    uint32 ii = 0;
    
    int32 status = IO_TransSelectCheckArgs(pSet, fd, TRUE);
    if (status < 0)
    {
        return status;
    }

    FD_CLR((int) fd, &pSet->fdSetFull);
                       
    /* If fd was the previous max, look for the new max */
    if (pSet->iMaxFdNum == fd)
    {
        pSet->iMaxFdNum = -1;
        for (ii = 0; ii < fd; ++ii)
        {
            status = IO_TransSelectFdInFull(pSet, ii);
            if(status == 1)
            {
                status = IO_TRANS_SELECT_NO_ERROR;
                pSet->iMaxFdNum = ii;
            }
            else if (status < 0)
            {
                break;
            }
        }
    }

    return status;
}

/** Returns 1 (True) if the fd is in the full set. */
int32 IO_TransSelectFdInFull(IO_TransSelect_t *pSet, int32 fd)
{
    int32 check = IO_TransSelectCheckArgs(pSet, fd, FALSE);
    if (check < 0)
    {
        return check;
    }
    
    return (int32) FD_ISSET((int) fd, &pSet->fdSetFull);
}


/** Returns 1 (True) if the fd is in the active set. */
int32 IO_TransSelectFdInActive(IO_TransSelect_t *pSet, int32 fd)
{
    int32 check = IO_TransSelectCheckArgs(pSet, fd, FALSE);
    if (check < 0)
    {
        return check;
    }
    
    return (int32) FD_ISSET((int) fd, &pSet->fdSetActive);
}


/** Select Input from fd in set which has available data. */
int32 IO_TransSelectInput(IO_TransSelect_t *pSet, int32 timeoutUSec)
{
    struct timeval timeout;
    int32 size = 0;

    int32 check = IO_TransSelectCheckIOArgs(pSet, timeoutUSec);
    if (check < 0)
    {
        return check;
    }

    /* Set the active set as full set. */
    CFE_PSP_MemCpy((void *) &pSet->fdSetActive, (void *) &pSet->fdSetFull, 
               sizeof(fd_set));

    /* Wait on devices until one is ready. */
    if (timeoutUSec == IO_TRANS_PEND_FOREVER)
    {
        size = select(pSet->iMaxFdNum + 1, &pSet->fdSetActive, NULL,NULL,NULL);
    }
    else
    {
        timeout.tv_sec  = timeoutUSec / 1000000;
        timeout.tv_usec = timeoutUSec % 1000000; 
        size = select(pSet->iMaxFdNum + 1, &pSet->fdSetActive, NULL, NULL, 
                      &timeout);
    }
    
    return size;
}


/** Select output from available fd in set that would not block on write. */
int32 IO_TransSelectOutput(IO_TransSelect_t *pSet, int32 timeoutUSec)
{
    struct timeval timeout;
    int32 size = 0;
    
    int32 check = IO_TransSelectCheckIOArgs(pSet, timeoutUSec);
    if (check < 0)
    {
        return check;
    }
    
    /* Set the active set as full set. */
    CFE_PSP_MemCpy((void *) &pSet->fdSetActive, (void *) &pSet->fdSetFull, 
               sizeof(fd_set));

    /* Wait on devices until one is ready. */
    if (timeoutUSec == IO_TRANS_PEND_FOREVER)
    {
        size = select(pSet->iMaxFdNum + 1, NULL, &pSet->fdSetActive, NULL,NULL);
    }
    else
    {
        timeout.tv_sec  = timeoutUSec / 1000000;
        timeout.tv_usec = timeoutUSec % 1000000; 
        size = select(pSet->iMaxFdNum + 1, NULL, &pSet->fdSetActive, NULL, 
                      &timeout);
    }
    
    return size;
}
