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
 * \file   os-impl-posix-dirs.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file Contains all of the api calls for manipulating files
 *      in a file system / C library that implements the UNIX-style file API
 *
 * NOTE: This is a "template" file and not a directly usable source file.
 *       It must be adapted/instantiated from within the OS-specific
 *       implementation on platforms that wish to use this template.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/* Handled by includer */

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/***************************************************************************************
                                 FUNCTION PROTOTYPES
 **************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

                        
/*----------------------------------------------------------------
 *
 * Function: OS_DirCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirCreate_Impl(const char *local_path, uint32 access)
{
   struct stat st;
   int32 return_code;

   if ( mkdir(local_path, S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO) < 0 )
   {
      return_code = OS_FS_ERROR;

      if (errno == EEXIST)
      {
         /* it exists, but not necessarily a directory */
         if ( stat(local_path, &st) == 0  && S_ISDIR(st.st_mode) )
         {
            return_code = OS_FS_SUCCESS;
         }
      }
   }
   else
   {
      return_code = OS_FS_SUCCESS;
   }

   return return_code;
} /* end OS_DirCreate_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_DirOpen_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirOpen_Impl(uint32 local_id, const char *local_path)
{
   OS_impl_dir_table[local_id] = opendir(local_path);
   if (OS_impl_dir_table[local_id] == NULL)
   {
      return OS_FS_ERROR;
   }
   return OS_FS_SUCCESS;
} /* end OS_DirOpen_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_DirClose_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirClose_Impl(uint32 local_id)
{
   closedir(OS_impl_dir_table[local_id]);
   OS_impl_dir_table[local_id] = NULL;
   return OS_FS_SUCCESS;
} /* end OS_DirClose_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_DirRead_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirRead_Impl(uint32 local_id, os_dirent_t *dirent)
{
   struct dirent *de;

   /* NOTE - the readdir() call is non-reentrant ....
    * However, this is performed while the global dir table lock is taken.
    * Therefore this ensures that only one such call can occur at any given time.
    *
    * Static analysis tools may warn about this because they do not know
    * this function is externally serialized via the global lock.
    */
   /* cppcheck-suppress readdirCalled */
   /* cppcheck-suppress nonreentrantFunctionsreaddir */
   de = readdir(OS_impl_dir_table[local_id]);
   if (de == NULL)
   {
      return OS_FS_ERROR;
   }

   strncpy(dirent->FileName, de->d_name, OS_MAX_PATH_LEN - 1);
   dirent->FileName[OS_MAX_PATH_LEN - 1] = 0;

   return OS_FS_SUCCESS;
} /* end OS_DirRead_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_DirRewind_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirRewind_Impl(uint32 local_id)
{
   rewinddir(OS_impl_dir_table[local_id]);
   return OS_FS_SUCCESS;
} /* end OS_DirRewind_Impl */
                        
/*----------------------------------------------------------------
 *
 * Function: OS_DirRemove_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirRemove_Impl(const char *local_path)
{
   if ( rmdir(local_path) < 0 )
   {
      return OS_FS_ERROR;
   }

   return OS_FS_SUCCESS;
} /* end OS_DirRemove_Impl */
