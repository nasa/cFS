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
 * \file     os-impl-queues.c
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"
#include "bsp-impl.h"

#include "os-impl-queues.h"
#include "os-shared-queue.h"
#include "os-shared-idmap.h"

/* Tables where the OS object information is stored */
OS_impl_queue_internal_record_t     OS_impl_queue_table         [OS_MAX_QUEUES];

/****************************************************************************************
                                MESSAGE QUEUE API
 ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_Posix_QueueAPI_Impl_Init

   Purpose: Initialize the Queue data structures

 ----------------------------------------------------------------------------------------*/
int32 OS_Posix_QueueAPI_Impl_Init(void)
{
   memset(OS_impl_queue_table, 0, sizeof(OS_impl_queue_table));

   /*
    * Automatic truncation is dependent on the OSAL_CONFIG_DEBUG_PERMISSIVE_MODE compile-time define - so
    * creating a too-large message queue on a target without OSAL_CONFIG_DEBUG_PERMISSIVE_MODE will fail
    * with an OS error as intended.
    */
#ifdef OSAL_CONFIG_DEBUG_PERMISSIVE_MODE
   /*
    * Use the BSP-provided limit
    */
   POSIX_GlobalVars.TruncateQueueDepth = OS_BSP_Global.MaxQueueDepth;
#else
   /*
    * Initialize this to zero to indicate no limit
    */
   POSIX_GlobalVars.TruncateQueueDepth = 0;
#endif

   return OS_SUCCESS;
} /* end OS_Posix_QueueAPI_Impl_Init */



/*----------------------------------------------------------------
 *
 * Function: OS_QueueCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueCreate_Impl (uint32 queue_id, uint32 flags)
{
   int                     return_code;
   mqd_t                   queueDesc;
   struct mq_attr          queueAttr;
   char                    name[OS_MAX_API_NAME * 2];

   /* set queue attributes */
   memset(&queueAttr, 0, sizeof(queueAttr));
   queueAttr.mq_maxmsg  = OS_queue_table[queue_id].max_depth;
   queueAttr.mq_msgsize = OS_queue_table[queue_id].max_size;

   /*
    * The "TruncateQueueDepth" indicates a soft limit to the size of a queue.
    * If nonzero, anything larger than this will be silently truncated
    * (Supports running applications as non-root)
    */
   if (POSIX_GlobalVars.TruncateQueueDepth > 0 &&
         POSIX_GlobalVars.TruncateQueueDepth < queueAttr.mq_maxmsg)
   {
      queueAttr.mq_maxmsg = POSIX_GlobalVars.TruncateQueueDepth;
   }

    /*
    ** Construct the queue name:
    ** The name will consist of "/<process_id>.queue_name"
    */
   snprintf(name, sizeof(name), "/%d.%s", (int)getpid(), OS_global_queue_table[queue_id].name_entry);

   /*
    ** create message queue
    */
   queueDesc = mq_open(name, O_CREAT | O_RDWR, 0666, &queueAttr);
   if ( queueDesc == (mqd_t)(-1) )
   {
      OS_DEBUG("OS_QueueCreate Error. errno = %d (%s)\n",errno,strerror(errno));
      if( errno == EINVAL )
      {
         OS_DEBUG("Your queue depth may be too large for the\n");
         OS_DEBUG("OS to handle. Please check the msg_max\n");
         OS_DEBUG("parameter located in /proc/sys/fs/mqueue/msg_max\n");
         OS_DEBUG("on your Linux file system and raise it if you\n");
         OS_DEBUG(" need to or run as root\n");
      }
      return_code = OS_ERROR;
   }
   else
   {
      OS_impl_queue_table[queue_id].id = queueDesc;
      return_code = OS_SUCCESS;

      /*
       * Unlink the queue right now --
       * queues have kernel persistence and if we do a lot of restarts (i.e. during debugging)
       * a lot of stale message queues will collect in the system.  It is OK to unlink right now
       * as this only affects the ability of another process to open the same queue, but we do
       * not need that to happen anyway.
       */
      if(mq_unlink(name) != 0)
      {
         OS_DEBUG("OS_QueueDelete Error during mq_unlink(). errno = %d (%s)\n",errno,strerror(errno));
         /* Note - since the queue is already closed, we cannot really handle this failure gracefully,
          * The queue is no longer usable so we can't return an error and go back to the way things were.
          * In this case we need to return OS_SUCCESS so the rest of the entry will be cleaned up.
          */
      }
   }

   return return_code;
} /* end OS_QueueCreate_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_QueueDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueDelete_Impl (uint32 queue_id)
{
   int32     return_code;

   /* Try to delete and unlink the queue */
   if (mq_close(OS_impl_queue_table[queue_id].id) != 0)
   {
      OS_DEBUG("OS_QueueDelete Error during mq_close(). errno = %d (%s)\n",errno,strerror(errno));
      return_code = OS_ERROR;
   }
   else
   {
      return_code = OS_SUCCESS;
   }

    return return_code;
} /* end OS_QueueDelete_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_QueueGet_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueGet_Impl (uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout)
{
   int32 return_code;
   ssize_t sizeCopied;
   struct timespec ts;

   /*
    ** Read the message queue for data
    */
   sizeCopied = -1;
   if (timeout == OS_PEND)
   {
      /*
       ** A signal can interrupt the mq_receive call, so the call has to be done with
       ** a loop
       */
      do
      {
         sizeCopied = mq_receive(OS_impl_queue_table[queue_id].id, data, size, NULL);
      }
      while ( sizeCopied < 0 && errno == EINTR );

   }
   else
   {
      /*
       * NOTE - a prior implementation of OS_CHECK would check the mq_attr for a nonzero depth
       * and then call mq_receive().  This is insufficient since another thread might do the same
       * thing at the same time in which case one thread will read and the other will block.
       *
       * Calling mq_timedreceive with a zero timeout effectively does the same thing in the typical
       * case, but for the case where two threads do a simultaneous read, one will get the message
       * while the other will NOT block (as expected).
       */
      if (timeout == OS_CHECK)
      {
         memset(&ts, 0, sizeof(ts));
      }
      else
      {
         OS_Posix_CompAbsDelayTime( timeout, &ts);
      }

      /*
       ** If the mq_timedreceive call is interrupted by a system call or signal,
       ** call it again.
       */
      do
      {
         sizeCopied = mq_timedreceive(OS_impl_queue_table[queue_id].id, data, size, NULL, &ts);
      }
      while ( timeout != OS_CHECK && sizeCopied < 0 && errno == EINTR );

   } /* END timeout */

   /* Figure out the return code */
   if(sizeCopied == -1)
   {
      *size_copied = 0;

      /* Map the system errno to the most appropriate OSAL return code */
      if (errno == EMSGSIZE)
      {
         return_code = OS_QUEUE_INVALID_SIZE;
      }
      else if (timeout == OS_PEND || errno != ETIMEDOUT)
      {
         /* OS_PEND was supposed to pend forever until a message arrived
          * so something else is wrong.  Otherwise, at this point the only
          * "acceptable" errno is TIMEDOUT for the other cases.
          */
         return_code = OS_ERROR;
      }
      else if (timeout == OS_CHECK)
      {
         return_code = OS_QUEUE_EMPTY;
      }
      else
      {
         return_code = OS_QUEUE_TIMEOUT;
      }
   }
   else
   {
      *size_copied = sizeCopied;
      return_code = OS_SUCCESS;
   }

   return return_code;
} /* end OS_QueueGet_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_QueuePut_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueuePut_Impl (uint32 queue_id, const void *data, uint32 size, uint32 flags)
{
   int32 return_code;
   int result;
   struct timespec ts;

   /*
    * NOTE - using a zero timeout here for the same reason that QueueGet does ---
    * checking the attributes and doing the actual send is non-atomic, and if
    * two threads call QueuePut() at the same time on a nearly-full queue,
    * one could block.
    */
   memset(&ts, 0, sizeof(ts));

   /* send message */
   do
   {
      result = mq_timedsend(OS_impl_queue_table[queue_id].id, data, size, 1, &ts);
   }
   while ( result == -1 && errno == EINTR );

   if(result == 0)
   {
      return_code = OS_SUCCESS;
   }
   else if (errno == ETIMEDOUT)
   {
      return_code = OS_QUEUE_FULL;
   }
   else
   {
      /* Something else went wrong */
      return_code = OS_ERROR;
   }

   return return_code;

} /* end OS_QueuePut_Impl */


