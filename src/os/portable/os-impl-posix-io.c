/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   os-impl-posix-io.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains generic calls for manipulating filehandles
 *      in a file system / C library that implements the UNIX-style file API
 *
 *      These generic ops may apply to regular files, sockets, pipes, or
 *      special devices, depending on the OS in use.
 *
 * NOTE: This is a "template" file and not a directly usable source file.
 *       It must be adapted/instantiated from within the OS-specific
 *       implementation on platforms that wish to use this template.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/* handled by includer */

/* some OS libraries (e.g. VxWorks) do not declare the API to be const-correct
 * It can still use this generic implementation but the call to write() must be
 * cast to a void* to avoid a warning.  The includer can define this if needed.
 * If not defined, assume no cast is needed (fine for anything POSIX-compliant).
 */
#ifndef GENERIC_IO_CONST_DATA_CAST
#define GENERIC_IO_CONST_DATA_CAST
#endif

                        
/*----------------------------------------------------------------
 *
 * Function: OS_GenericClose_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GenericClose_Impl(uint32 local_id)
{
   int result;

   result = close (OS_impl_filehandle_table[local_id].fd);
   if (result < 0)
   {
       /*
        * close() can technically fail for various reasons, but
        * there isn't much recourse if this call fails.  Just log
        * the failure for debugging.
        *
        * POSIX also does not specify the state of the filehandle
        * after a close() with an error.
        *
        * At least in  Linux/glibc the filehandle is always closed
        * in the kernel and should not be used again or re-closed.
        */
       OS_DEBUG("close: %s\n",strerror(errno));
   }
   OS_impl_filehandle_table[local_id].fd = -1;
   return OS_FS_SUCCESS;
} /* end OS_GenericClose_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_GenericSeek_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GenericSeek_Impl (uint32 local_id, int32 offset, uint32 whence)
{
   int where;
   int32 result;

   switch(whence)
   {
   case OS_SEEK_SET:
      where = SEEK_SET;
      break;
   case OS_SEEK_CUR:
      where = SEEK_CUR;
      break;
   case OS_SEEK_END:
      where = SEEK_END;
      break;
   default:
      return OS_FS_ERROR;
   }

   result = lseek(OS_impl_filehandle_table[local_id].fd, (off_t)offset, where);
   if (result < 0)
   {
       if (errno == ESPIPE)
       {
           /*
            * this means the user tried to seek on a pipe, socket,
            * or other fifo-like handle that doesn't support seeking.
            *
            * Use a different error code to differentiate from an
            * error involving a bad whence/offset
            */
           result = OS_FS_UNIMPLEMENTED;
       }
       else
       {
           /*
            * Most likely the "whence" and/or "offset" combo was not valid.
            */
           OS_DEBUG("lseek: %s\n",strerror(errno));
           result = OS_FS_ERROR;
       }
   }

   return result;
} /* end OS_GenericSeek_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_GenericRead_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GenericRead_Impl (uint32 local_id, void *buffer, uint32 nbytes, int32 timeout)
{
   int32 return_code;
   int os_result;
   uint32 operation;

   return_code = OS_SUCCESS;

   if (nbytes > 0)
   {
       operation = OS_STREAM_STATE_READABLE;

       /*
        * If filehandle is set with O_NONBLOCK, then must call select() here.
        *
        * The "selectable" field should be set false for those file handles
        * which the underlying OS does not support select() on.
        *
        * Note that a timeout will not work unless selectable is true.
        */
       if (OS_impl_filehandle_table[local_id].selectable)
       {
           return_code = OS_SelectSingle_Impl(local_id, &operation, timeout);
       }

       if (return_code == OS_SUCCESS && (operation & OS_STREAM_STATE_READABLE) != 0)
       {
           os_result = read(OS_impl_filehandle_table[local_id].fd, buffer, nbytes);
           if (os_result < 0)
           {
               OS_DEBUG("read: %s\n",strerror(errno));
               return_code = OS_ERROR;
           }
           else
           {
               return_code = os_result;
           }
       }
   }

   return (return_code);
} /* end OS_GenericRead_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_GenericWrite_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GenericWrite_Impl(uint32 local_id, const void *buffer, uint32 nbytes, int32 timeout)
{
   int32 return_code;
   int os_result;
   uint32 operation;

   return_code = OS_SUCCESS;

   if (nbytes > 0)
   {
       operation = OS_STREAM_STATE_WRITABLE;

       /*
        * If filehandle is set with O_NONBLOCK, then must call select() here.
        *
        * The "selectable" field should be set false for those file handles
        * which the underlying OS does not support select() on.
        *
        * Note that a timeout will not work unless selectable is true.
        */
       if (OS_impl_filehandle_table[local_id].selectable)
       {
           return_code = OS_SelectSingle_Impl(local_id, &operation, timeout);
       }

       if (return_code == OS_SUCCESS && (operation & OS_STREAM_STATE_WRITABLE) != 0)
       {
           /* on some system libraries for which the write() argument is not
            * qualified correctly, it needs to be case to a void* here */
           os_result = write(OS_impl_filehandle_table[local_id].fd,
                  GENERIC_IO_CONST_DATA_CAST buffer, nbytes);
           if (os_result < 0)
           {
               OS_DEBUG("write: %s\n",strerror(errno));
               return_code = OS_ERROR;
           }
           else
           {
               return_code = os_result;
           }
       }
   }

   return (return_code);
} /* end OS_GenericWrite_Impl */

