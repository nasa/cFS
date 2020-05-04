/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     os-shared-queue.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_SHARED_QUEUE_H_
#define INCLUDE_OS_SHARED_QUEUE_H_

#include <os-shared-globaldefs.h>


typedef struct
{
   char      queue_name[OS_MAX_API_NAME];
   uint32    max_size;
   uint32    max_depth;
} OS_queue_internal_record_t;


/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_queue_internal_record_t          OS_queue_table[OS_MAX_QUEUES];



/****************************************************************************************
                 MESSAGE QUEUE API LOW-LEVEL IMPLEMENTATION FUNCTIONS
  ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_QueueAPI_Init

   Purpose: Initialize the OS-independent layer for queues

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_QueueAPI_Init               (void);


/*----------------------------------------------------------------
   Function: OS_QueueCreate_Impl

    Purpose: Prepare/Allocate OS resources for a message queue

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_QueueCreate_Impl           (uint32 queue_id, uint32 flags);

/*----------------------------------------------------------------
   Function: OS_QueueDelete_Impl

    Purpose: Free the OS resources associated with the message queue

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_QueueDelete_Impl           (uint32 queue_id);

/*----------------------------------------------------------------
   Function: OS_QueueGet_Impl

    Purpose: Receive a message on a message queue.
             The calling task will be blocked if no message is immediately available

    Returns: OS_SUCCESS on success, or relevant error code
             OS_QUEUE_TIMEOUT must be returned if the timeout expired and no message was received
             OS_QUEUE_EMPTY must be returned if the queue is empty when polled (OS_CHECK)
             OS_QUEUE_INVALID_SIZE must be returned if the supplied buffer is too small
 ------------------------------------------------------------------*/
int32 OS_QueueGet_Impl              (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout);

/*----------------------------------------------------------------
   Function: OS_QueuePut_Impl

    Purpose: Put a message into a message queue

    Returns: OS_SUCCESS on success, or relevant error code
             OS_QUEUE_FULL must be returned if the queue is full.
 ------------------------------------------------------------------*/
int32 OS_QueuePut_Impl              (uint32 queue_id, const void *data, uint32 size, uint32 flags);

/*----------------------------------------------------------------
   Function: OS_QueueGetInfo_Impl

    Purpose: Obtain OS-specific information about a message queue

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_QueueGetInfo_Impl          (uint32 queue_id, OS_queue_prop_t *queue_prop);


#endif  /* INCLUDE_OS_SHARED_QUEUE_H_ */

