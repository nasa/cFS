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
 * \file   osapi-file.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * User defined include files
 */
#include "common_types.h"
#include "os-impl.h"


/*
 * Sanity checks on the user-supplied configuration
 * The relevent OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_NUM_OPEN_FILES) || (OS_MAX_NUM_OPEN_FILES <= 0)
#error "osconfig.h must define OS_MAX_NUM_OPEN_FILES to a valid value"
#endif


/*
 * Global data for the API
 */
enum
{
   LOCAL_NUM_OBJECTS = OS_MAX_NUM_OPEN_FILES,
   LOCAL_OBJID_TYPE = OS_OBJECT_TYPE_OS_STREAM
};

OS_stream_internal_record_t         OS_stream_table[OS_MAX_NUM_OPEN_FILES];

                        
/*----------------------------------------------------------------
 *
 * Function: OS_check_name_length
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Validates that the path length is within spec and
 *           contains at least one directory separator (/) char.
 *
 *-----------------------------------------------------------------*/
static int32 OS_check_name_length(const char *path)
{
    char* name_ptr;

    if (path == NULL)
        return OS_FS_ERR_INVALID_POINTER;

    if (strlen(path) > OS_MAX_PATH_LEN)
        return OS_FS_ERR_PATH_TOO_LONG;

    /* checks to see if there is a '/' somewhere in the path */
    name_ptr = strrchr(path, '/');
    if (name_ptr == NULL)
        return OS_FS_ERROR;

    /* strrchr returns a pointer to the last '/' char, so we advance one char */
    name_ptr = name_ptr + 1;

    if( strlen(name_ptr) > OS_MAX_FILE_NAME)
        return OS_FS_ERR_NAME_TOO_LONG;

    return OS_FS_SUCCESS;

} /* end OS_check_name_length */

/****************************************************************************************
                                  FILE API
 ***************************************************************************************/

                        
/*----------------------------------------------------------------
 *
 * Function: OS_FileAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_FileAPI_Init(void)
{
   memset(OS_stream_table, 0, sizeof(OS_stream_table));
   return OS_SUCCESS;
} /* end OS_FileAPI_Init */


/*----------------------------------------------------------------
 *
 * Function: OS_OpenCreate
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Implements both "open" and "creat" file operations
 *           (The difference is a matter of what flags are passed in)
 *
 *-----------------------------------------------------------------*/
static int32 OS_OpenCreate(uint32 *filedes, const char *path, int32 flags, int32 access)
{
   int32 return_code;
   uint32 local_id;
   OS_common_record_t *record;
   char   local_path[OS_MAX_LOCAL_PATH_LEN];

   /*
   ** check if the name of the file is too long
   */
   return_code = OS_check_name_length(path);
   if (return_code == OS_FS_SUCCESS)
   {
      /*
      ** Translate the path
      */
      return_code = OS_TranslatePath(path, (char *)local_path);
   }

   if (return_code == OS_FS_SUCCESS)
   {
      /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
      return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, NULL, &local_id, &record);
      if(return_code == OS_SUCCESS)
      {
         /* Save all the data to our own internal table */
         memset(&OS_stream_table[local_id], 0, sizeof(OS_stream_internal_record_t));
         strcpy(OS_stream_table[local_id].stream_name, path);
         record->name_entry = OS_stream_table[local_id].stream_name;

         /* Now call the OS-specific implementation.  */
         return_code = OS_FileOpen_Impl(local_id, local_path, flags, access);

         /* Check result, finalize record, and unlock global table. */
         return_code = OS_ObjectIdFinalizeNew(return_code, record, filedes);
      }
   }

   return return_code;
} /* end OS_OpenCreate */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_creat
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_creat  (const char *path, int32  access)
{
   uint32 filedes;
   int32 return_code;

   /*
   ** Check for a valid access mode
   */
   switch(access)
   {
       case OS_WRITE_ONLY:
       case OS_READ_WRITE:
           break;
       case OS_READ_ONLY:
       default:
          /* Read only does not make sense for creat() */
          return OS_FS_ERROR;
   }


   return_code = OS_OpenCreate(&filedes, path, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, access);
   if (return_code == OS_FS_SUCCESS)
   {
      return_code = (int32)filedes;
   }

   return return_code;
} /* end OS_creat */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_open
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_open   (const char *path,  int32 access,  uint32  mode)
{
   uint32 filedes;
   int32 return_code;

   /*
   ** Check for a valid access mode
   */
   switch(access)
   {
       case OS_WRITE_ONLY:
       case OS_READ_WRITE:
       case OS_READ_ONLY:
           break;
       default:
          return OS_FS_ERROR;
   }


   return_code = OS_OpenCreate(&filedes, path, OS_FILE_FLAG_NONE, access);
   if (return_code == OS_FS_SUCCESS)
   {
      return_code = (int32)filedes;
   }

   return return_code;
} /* end OS_open */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_close
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_close (uint32 filedes)
{
    OS_common_record_t *record;
    uint32 local_id;
    int32 return_code;

    /* Make sure the file descriptor is legit before using it */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, filedes, &local_id, &record);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_GenericClose_Impl(local_id);

        /* Free the entry in the master table now while still locked */
        if (return_code == OS_SUCCESS)
        {
            /* Only need to clear the ID as zero is the "unused" flag */
            record->active_id = 0;
        }

        OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
    }

    return return_code;

} /* end OS_close */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimedRead
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimedRead(uint32  filedes, void *buffer, uint32 nbytes, int32 timeout)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Check Parameters */
   if (buffer == NULL || nbytes == 0)
   {
      return OS_INVALID_POINTER;
   }

   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, LOCAL_OBJID_TYPE, filedes, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      return_code = OS_GenericRead_Impl (local_id, buffer, nbytes, timeout);
      OS_ObjectIdRefcountDecr(record);
   }

   return return_code;
} /* end OS_TimedRead */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_TimedWrite
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimedWrite(uint32  filedes, const void *buffer, uint32 nbytes, int32 timeout)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Check Parameters */
   if (buffer == NULL || nbytes == 0)
   {
      return OS_INVALID_POINTER;
   }

   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, LOCAL_OBJID_TYPE, filedes, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      return_code = OS_GenericWrite_Impl (local_id, buffer, nbytes, timeout);
      OS_ObjectIdRefcountDecr(record);
   }

   return return_code;
} /* end OS_TimedWrite */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_read
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_read  (uint32  filedes, void *buffer, uint32 nbytes)
{
   return OS_TimedRead(filedes, buffer, nbytes, OS_PEND);
} /* end OS_read */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_write
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_write (uint32  filedes, const void *buffer, uint32 nbytes)
{
    return OS_TimedWrite(filedes, buffer, nbytes, OS_PEND);
} /* end OS_write */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_chmod
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_chmod  (const char *path, uint32 access)
{
    char local_path[OS_MAX_LOCAL_PATH_LEN];
    int32 return_code;

    return_code = OS_TranslatePath(path, local_path);
    if (return_code == OS_FS_SUCCESS)
    {
       return_code = OS_FileChmod_Impl(local_path, access);
    }

    return return_code;

} /* end OS_chmod */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_stat
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_stat   (const char *path, os_fstat_t *filestats)
{
   int32 return_code;
   char local_path[OS_MAX_LOCAL_PATH_LEN];

   if (filestats == NULL)
   {
      return OS_INVALID_POINTER;
   }

   memset(filestats, 0, sizeof(*filestats));

   return_code = OS_TranslatePath(path, local_path);
   if (return_code == OS_FS_SUCCESS)
   {
      return_code = OS_FileStat_Impl(local_path, filestats);
   }

   return return_code;
} /* end OS_stat */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_lseek
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_lseek  (uint32  filedes, int32 offset, uint32 whence)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Make sure the file descriptor is legit before using it */
   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, LOCAL_OBJID_TYPE, filedes, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      return_code = OS_GenericSeek_Impl (local_id, offset, whence);
      OS_ObjectIdRefcountDecr(record);
   }

   return return_code;
} /* end OS_lseek */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_remove
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_remove (const char *path)
{
   int32 return_code;
   char local_path[OS_MAX_LOCAL_PATH_LEN];

   return_code = OS_check_name_length(path);
   if (return_code == OS_FS_SUCCESS)
   {
      return_code = OS_TranslatePath(path, local_path);
      if (return_code == OS_FS_SUCCESS)
      {
         return_code = OS_FileRemove_Impl(local_path);
      }
   }

   return return_code;

} /* end OS_remove */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_rename
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_rename (const char *old, const char *new)
{
   int i;
   int32 return_code;
   char old_path[OS_MAX_LOCAL_PATH_LEN];
   char new_path[OS_MAX_LOCAL_PATH_LEN];

   return_code = OS_check_name_length(old);
   if (return_code == OS_FS_SUCCESS)
   {
      return_code = OS_check_name_length(new);
   }
   if (return_code == OS_FS_SUCCESS)
   {
      return_code = OS_TranslatePath(old, old_path);
   }
   if (return_code == OS_FS_SUCCESS)
   {
      return_code = OS_TranslatePath(new, new_path);
   }

   if (return_code == OS_FS_SUCCESS)
   {
      return_code = OS_FileRename_Impl(old_path, new_path);
   }

   if (return_code == OS_FS_SUCCESS)
   {
      OS_Lock_Global_Impl(LOCAL_OBJID_TYPE);
      for ( i =0; i < OS_MAX_NUM_OPEN_FILES; i++)
      {
         if (OS_global_stream_table[i].active_id != 0 &&
               OS_stream_table[i].socket_domain == OS_SocketDomain_INVALID &&
               strcmp(OS_stream_table[i].stream_name, old) == 0)
         {
            strcpy (OS_stream_table[i].stream_name, new);
         }
      }
      OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
    }

    return return_code;

} /* end OS_rename */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_cp
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_cp (const char *src, const char *dest)
{
    int32 return_code;
    int32 rd_size;
    int32 wr_size;
    int32 wr_total;
    int32 file1;
    int32 file2;
    uint8 copyblock[512];

    if (src == NULL || dest == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return_code = OS_FS_SUCCESS;
    file2 = -1;
    file1 = OS_open(src, OS_READ_ONLY, 0);
    if (file1 < 0)
    {
       return_code = file1;
    }
    else
    {
       file2 = OS_creat(dest, OS_WRITE_ONLY);
       if (file2 < 0)
       {
          return_code = file2;
       }
    }

    while (return_code == OS_FS_SUCCESS)
    {
       rd_size = OS_read((uint32)file1, copyblock, sizeof(copyblock));
       if (rd_size < 0)
       {
          return_code = rd_size;
          break;
       }
       if (rd_size == 0)
       {
          break;
       }
       wr_total = 0;
       while (wr_total < rd_size)
       {
          wr_size = OS_write((uint32)file2, &copyblock[wr_total], rd_size - wr_total);
          if (wr_size < 0)
          {
             return_code = wr_size;
             break;
          }
          wr_total += wr_size;
       }
    }

    if (file1 >= 0)
    {
       OS_close(file1);
    }
    if (file2 >= 0)
    {
       OS_close(file2);
    }

    return return_code;

} /* end OS_cp */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_mv
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_mv (const char *src, const char *dest)
{
   int32 return_code;

   /* First try rename - this only works if it is on the same filesystem */
   return_code = OS_rename(src, dest);
   if (return_code != OS_FS_SUCCESS)
   {
      return_code = OS_cp(src, dest);
      if (return_code == OS_FS_SUCCESS)
      {
         OS_remove(src);
      }
   }

   return (return_code);

} /* end OS_mv */



                        
/*----------------------------------------------------------------
 *
 * Function: OS_FDGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FDGetInfo (uint32 filedes, OS_file_prop_t *fd_prop)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Check parameters */
   if (fd_prop == NULL)
   {
       return(OS_FS_ERR_INVALID_POINTER);
   }

   memset(fd_prop,0,sizeof(OS_file_prop_t));

   /* Check Parameters */
   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, filedes, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      strncpy(fd_prop->Path, record->name_entry, OS_MAX_PATH_LEN - 1);
      fd_prop->User = record->creator;
      fd_prop->IsValid = true;
      OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);
   }

   return return_code;

} /* end OS_FDGetInfo */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_FileOpenCheck
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileOpenCheck(const char *Filename)
{
   int32 return_code;
   uint32  i;

   if (Filename == NULL)
   {
      return(OS_FS_ERR_INVALID_POINTER);
   }

   return_code = OS_FS_ERROR;

   OS_Lock_Global_Impl(LOCAL_OBJID_TYPE);

   for ( i = 0; i < OS_MAX_NUM_OPEN_FILES; i++)
   {
      if (OS_global_stream_table[i].active_id != 0 &&
            OS_stream_table[i].socket_domain == OS_SocketDomain_INVALID &&
            (strcmp(OS_stream_table[i].stream_name, Filename) == 0))
      {
         return_code = OS_FS_SUCCESS;
         break;
      }
   }/* end for */

   OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);

   return return_code;
} /* end OS_FileOpenCheck */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_CloseFileByName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CloseFileByName(const char *Filename)
{
   int32 return_code;
   int32 close_code;
   uint32  i;

   if (Filename == NULL)
   {
      return(OS_FS_ERR_INVALID_POINTER);
   }

   return_code = OS_FS_ERR_PATH_INVALID;

   OS_Lock_Global_Impl(LOCAL_OBJID_TYPE);

   for ( i = 0; i < OS_MAX_NUM_OPEN_FILES; i++)
   {
      if (OS_global_stream_table[i].active_id != 0 &&
            OS_stream_table[i].socket_domain == OS_SocketDomain_INVALID &&
            (strcmp(OS_stream_table[i].stream_name, Filename) == 0))
      {
         close_code = OS_GenericClose_Impl(i);
         if (close_code == OS_FS_SUCCESS)
         {
             OS_global_stream_table[i].active_id = 0;
         }
         if (return_code == OS_FS_ERR_PATH_INVALID || close_code != OS_FS_SUCCESS)
         {
            return_code = close_code;
         }
      }
   }/* end for */

   OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);

   return (return_code);

} /* end OS_CloseFileByName */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_CloseAllFiles
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CloseAllFiles(void)
{
   int32 return_code;
   int32 close_code;
   uint32  i;

   return_code = OS_FS_SUCCESS;

   OS_Lock_Global_Impl(LOCAL_OBJID_TYPE);

   for ( i = 0; i < OS_MAX_NUM_OPEN_FILES; i++)
   {
      if (OS_global_stream_table[i].active_id != 0)
      {
         close_code = OS_GenericClose_Impl(i);
         if (close_code == OS_FS_SUCCESS)
         {
             OS_global_stream_table[i].active_id = 0;
         }
         if (close_code != OS_FS_SUCCESS)
         {
            return_code = close_code;
         }
      }
   }/* end for */

   OS_Unlock_Global_Impl(LOCAL_OBJID_TYPE);

   return (return_code);

} /* end OS_CloseAllFiles */

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ShellOutputToFile
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ShellOutputToFile(const char* Cmd, uint32 filedes)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Check Parameters */
   if (Cmd == NULL)
   {
       return OS_FS_ERR_INVALID_POINTER;
   }

   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, LOCAL_OBJID_TYPE, filedes, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      return_code = OS_ShellOutputToFile_Impl(local_id, Cmd);
      OS_ObjectIdRefcountDecr(record);
   }

   return return_code;
} /* end OS_ShellOutputToFile */

