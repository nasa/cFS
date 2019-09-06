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

/* --------------------------------------------------------------------------------------
    Name: OS_check_name_length

    Purpose: Checks the length of the file name at the end of the path.

    Returns: OS_FS_ERROR if path is NULL, path is too long, there is no '/' in the path
             name, the name is too long
             OS_SUCCESS if success

    NOTE: This is only an internal function and is not intended for use by the user
 ---------------------------------------------------------------------------------------*/

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

}/* end OS_check_name_length */

/****************************************************************************************
                                  FILE API
 ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_StreamAPI_Init

   Purpose: Init function for OS-independent layer

   Returns: OS_SUCCESS

---------------------------------------------------------------------------------------*/
int32 OS_FileAPI_Init(void)
{
   memset(OS_stream_table, 0, sizeof(OS_stream_table));
   return OS_SUCCESS;
}


/*---------------------------------------------------------------------------------------
   Name: OS_OpenCreate

   Purpose: Local function to implement both "open" and "creat"
            (The difference is a matter of what flags are passed in)

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
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
}

/*--------------------------------------------------------------------------------------
    Name: OS_creat

    Purpose: creates a file specified by const char *path, with read/write
             permissions by access. The file is also automatically opened by the
             create call.

    Returns: OS_FS_ERR_INVALID_POINTER if path is NULL
             OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long
             OS_FS_ERROR if permissions are unknown or OS call fails
             OS_FS_ERR_NO_FREE_FDS if there are no free file descripors left
             OS_FS_SUCCESS if success

---------------------------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------------------------
    Name: OS_open

    Purpose: Opens a file. access parameters are OS_READ_ONLY,OS_WRITE_ONLY, or
             OS_READ_WRITE

    Returns: OS_FS_ERR_INVALID_POINTER if path is NULL
             OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long
             OS_FS_ERROR if permissions are unknown or OS call fails
             OS_FS_ERR_NO_FREE_FDS if there are no free file descriptors left
             a file descriptor if success
---------------------------------------------------------------------------------------*/

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


/*--------------------------------------------------------------------------------------
    Name: OS_close

    Purpose: Closes a file.

    Returns: OS_FS_ERROR if file  descriptor could not be closed
             OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             OS_FS_SUCCESS if success
---------------------------------------------------------------------------------------*/

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

}/* end OS_close */

/*--------------------------------------------------------------------------------------
    Name: OS_TimedRead

    Purpose: reads up to nbytes from a stream, and puts them into buffer.
             Will wait up to the time specified in timeout (OS_PEND = forever)

    Returns: number of bytes read if success, <0 on error
---------------------------------------------------------------------------------------*/
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
}/* end OS_read */

/*--------------------------------------------------------------------------------------
    Name: OS_TimedWrite

    Purpose: writes to a stream up to a maximum of nbytes to the file described in filedes
             Will wait up to the time specified in timeout (OS_PEND = forever)

    Returns: number of bytes written if success, <0 on error
---------------------------------------------------------------------------------------*/
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
}/* end OS_write */

/*--------------------------------------------------------------------------------------
    Name: OS_read

    Purpose: reads up to nbytes from a file, and puts them into buffer.

    Returns: OS_FS_ERR_INVALID_POINTER if buffer is a null pointer
             OS_FS_ERROR if OS call failed
             OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             number of bytes read if success
---------------------------------------------------------------------------------------*/
int32 OS_read  (uint32  filedes, void *buffer, uint32 nbytes)
{
   return OS_TimedRead(filedes, buffer, nbytes, OS_PEND);
}/* end OS_read */

/*--------------------------------------------------------------------------------------
    Name: OS_write

    Purpose: writes to a file. copies up to a maximum of nbtyes of buffer to the file
             described in filedes

    Returns: OS_FS_ERR_INVALID_POINTER if buffer is NULL
             OS_FS_ERROR if OS call failed
             OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             number of bytes written if success
---------------------------------------------------------------------------------------*/

int32 OS_write (uint32  filedes, const void *buffer, uint32 nbytes)
{
    return OS_TimedWrite(filedes, buffer, nbytes, OS_PEND);
}/* end OS_write */


/*--------------------------------------------------------------------------------------
    Name: OS_chmod

    Notes: This is not going to be implemented because there is no use for this function.
---------------------------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------------------------
    Name: OS_stat

    Purpose: returns information about a file or directory in a os_fs_stat structure

    Returns: OS_FS_ERR_INVALID_POINTER if path or filestats is NULL
             OS_FS_ERR_PATH_TOO_LONG if the path is too long to be stored locally
 ****        OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long to be stored
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERROR id the OS call failed
             OS_FS_SUCCESS if success

    Note: The information returned is in the structure pointed to by filestats
---------------------------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------------------------
    Name: OS_lseek

    Purpose: sets the read/write pointer to a specific offset in a specific file.
             Whence is either OS_SEEK_SET,OS_SEEK_CUR, or OS_SEEK_END

    Returns: the new offset from the beginning of the file
             OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             OS_FS_ERROR if OS call failed
---------------------------------------------------------------------------------------*/

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
}/* end OS_lseek */

/*--------------------------------------------------------------------------------------
    Name: OS_remove

    Purpose: removes a given filename from the drive

    Returns: OS_FS_SUCCESS if the driver returns OK
             OS_FS_ERROR if there is no device or the driver returns error
             OS_FS_ERR_INVALID_POINTER if path is NULL
             OS_FS_ERR_PATH_TOO_LONG if path is too long to be stored locally
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERR_NAME_TOO_LONG if the name of the file to remove is too long to be
             stored locally
---------------------------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------------------------
    Name: OS_rename

    Purpose: renames a file

    Returns: OS_FS_SUCCESS if the rename works
             OS_FS_ERROR if the file could not be opened or renamed.
             OS_FS_ERR_INVALID_POINTER if old or new are NULL
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
             OS_FS_ERR_NAME_TOO_LONG if the new name is too long to be stored locally
---------------------------------------------------------------------------------------*/

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

}/*end OS_rename */

/*--------------------------------------------------------------------------------------
    Name: OS_cp

    Purpose: Copies a single file from src to dest

    Returns: OS_FS_SUCCESS if the operation worked
             OS_FS_ERROR if the file could not be accessed
             OS_FS_ERR_INVALID_POINTER if src or dest are NULL
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
             OS_FS_ERR_NAME_TOO_LONG if the dest name is too long to be stored locally

---------------------------------------------------------------------------------------*/

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
        return OS_FS_ERR_INVALID_POINTER;
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

}/*end OS_cp */

/*--------------------------------------------------------------------------------------
    Name: OS_mv

    Purpose: moves a single file from src to dest

    Returns: OS_FS_SUCCESS if the rename works
             OS_FS_ERROR if the file could not be opened or renamed.
             OS_FS_ERR_INVALID_POINTER if src or dest are NULL
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
             OS_FS_ERR_NAME_TOO_LONG if the dest name is too long to be stored locally
---------------------------------------------------------------------------------------*/

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

}/*end OS_mv */



/* --------------------------------------------------------------------------------------
Name: OS_FDGetInfo

Purpose: Copies the information of the given file descriptor into a structure passed in

Returns: OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
         OS_FS_SUCCESS if the copying was successfull
 ---------------------------------------------------------------------------------------*/

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

}/* end OS_FDGetInfo */

/* --------------------------------------------------------------------------------------
   Name: OS_FileOpenCheck

   Purpose: Checks to see if a file is open

   Returns: OS_FS_ERROR if the file is not open
            OS_FS_SUCCESS if the file is open
 ---------------------------------------------------------------------------------------*/
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
}/* end OS_FileOpenCheck */


/* --------------------------------------------------------------------------------------
   Name: OS_CloseFileByName

   Purpose: Allows a file to be closed by name.
            This will only work if the name passed in is the same name used to open
            the file.

   Returns: OS_FS_ERR_PATH_INVALID if the file is not found
            OS_FS_ERROR   if the file close returned an error
            OS_FS_SUCCESS if the file close suceeded
 ---------------------------------------------------------------------------------------*/
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

}/* end OS_CloseFileByName */

/* --------------------------------------------------------------------------------------
   Name: OS_CloseAllFiles

   Purpose: Closes All open files that were opened through the OSAL

   Returns: OS_FS_ERROR   if one or more file close returned an error
            OS_FS_SUCCESS if the files were all closed without error
 ---------------------------------------------------------------------------------------*/
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

}/* end OS_CloseAllFiles */

/* --------------------------------------------------------------------------------------
    Name: OS_ShellOutputToFile

    Purpose: Takes a shell command in and writes the output of that command to the specified file

    Returns: OS_FS_ERROR if the command was not executed properly
             OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
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
}/* end OS_ShellOutputToFile */

