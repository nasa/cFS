/*
** File   : osfileapi.c
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software was created at NASAs Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
**
** Author : Nicholas Yanchik
**
** Purpose: This file Contains all of the api calls for manipulating
**            files in a file system for RTEMS 
**
** $Date: 2014/04/23 15:14:17GMT-05:00 $
** $Revision: 1.28 $
** $Log: osfileapi.c  $
** Revision 1.28 2014/04/23 15:14:17GMT-05:00 acudmore 
** Fixed use of O_CREAT flag in OS_open. Dont use it when opening a file as read only
** Revision 1.27 2014/01/16 16:12:15GMT-05:00 acudmore 
** in RTEMS file system calls, check for 0 instead of !ERROR.
** Revision 1.26 2013/07/29 12:03:40GMT-05:00 acudmore 
** Updated OS_open to create a file if it does not exist
** Update OS_ShellOutputToFile to check pointer and return OS_FS_SUCCESS
** Revision 1.25 2013/07/22 15:58:18GMT-05:00 acudmore 
** remove debug printf
** Revision 1.24 2013/04/11 15:25:22GMT-05:00 acudmore 
** Add open file checks to OS_mv, OS_remove, and OS_cp
** Revision 1.23 2012/12/19 13:48:41GMT-05:00 acudmore 
** Updated OS_open and OS_creat to handle access parameters in a consistent way
** Revision 1.22 2012/12/11 13:25:47EST acudmore 
** Added NULL Pointer check to OS_FDGetInfo
** Revision 1.21 2012/11/15 14:18:29EST acudmore 
** removed redundant close in OS_close
** Revision 1.20 2012/11/09 17:09:37EST acudmore 
** Re-wrote OS_mv to use OS_cp and OS_remove
** Revision 1.19 2012/06/14 11:32:50EDT acudmore 
** Fixed a few bugs related to code walkthrough changes:
**   - Max count sem value is now max signed int
**   - typos in table initialization
**   - left an extra semaphore release in
** Revision 1.18 2012/04/13 14:53:56EDT acudmore 
** MMS/RTEMS Code walkthrough changes
** Revision 1.17 2011/12/15 10:50:22EST acudmore 
** Removed file length check from OS_stat
** Revision 1.16 2011/12/05 12:08:34EST acudmore 
** Added O_TRUNC flag to OS_creat open call to handle the case of an existing file.
** Revision 1.15 2011/12/05 12:07:07EST acudmore 
** Completed OS Shell command functionality
** Revision 1.14 2011/06/27 15:52:04EDT acudmore 
** Went over APIs and Documentation for return code consistency.
** Updated documentation, function comments, and return codes as needed.
** Revision 1.13 2011/05/03 14:24:36EDT acudmore 
** - Create semaphore for file system function lock
** - Added function to close file by name
** Revision 1.12 2011/03/23 11:13:02EDT acudmore 
** Added mode parameter to open call in OS_cp. It was causing an error when using the RFS file system.
** Revision 1.11 2010/11/15 11:06:46EST acudmore 
** Added OS_FIleOpenCheck function
** Revision 1.10 2010/11/12 12:41:59EST acudmore 
** updated error codes in comments
** Revision 1.9 2010/11/12 12:01:11EST acudmore 
** replaced copyright character with (c) and added open source notice where needed.
** Revision 1.8 2010/11/10 15:49:25EST acudmore 
** Merged in MMS Fixes to RTEMS port.
** Mostly fixed the left over posix mutex code in this file.
** Revision 1.4 2010/08/09 11:45:36EDT acudmore 
** Fixed semaphore code where RTEMS calls were using the same return code as the "open" and "close" calls.
** Revision 1.3 2010/08/05 12:41:37EDT acudmore 
** - Removed pthread mutex for internal data locking. Replaced with RTEMS semaphore.
** - Implemented OS_cp and OS_mv
** Revision 1.2 2010/03/12 14:04:22EST acudmore 
** upated to OSAL 3.1
** Revision 1.7 2010/02/01 12:40:42EST acudmore 
** Changes to support return code on OS_API_Init
** Revision 1.6 2010/01/25 14:45:14EST acudmore 
** renamed "new" variable to avoid C++ reserved name conflict.
** Revision 1.5 2009/07/16 13:02:23EDT acudmore 
** Fixed warnings found by ARC.
** Revision 1.4 2009/07/14 15:21:55EDT acudmore 
** Updated RTEMS file API to sync with linux version.
** Revision 1.4 2009/07/14 14:26:33EDT acudmore 
** Fixed virtual vs. physical filename errors.
** Created a new config parameter and created OS_TranslatePath.
** OS_TranslatePath will replace OS_NameChange, but OS_NameChange is still included for now.
** Revision 1.3 2009/07/10 14:58:54EDT acudmore 
** Added code to clean up OSAL resources regardless of OS_close error
** Revision 1.2 2009/03/10 10:53:49EDT nyanchik 
** I made the changes to the comments of _OS_check_name_length foir all four OS's
** Revision 1.1 2008/04/21 03:36:04BST ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-OSAL-REPOSITORY/src/os/linux/project.pj
** Revision 1.3 2008/03/28 10:56:05EDT njyanchik 
** I added the code shownin the DCR to OS_rename and OS_mv for all OS's
** Revision 1.2 2008/01/22 15:16:49AST njyanchik 
** I fixed the #define that is being used in the two functions listed in the DCR with the correct ones
** Revision 1.1 2007/10/16 16:14:54EDT apcudmore 
** Initial revision
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <rtems.h>
#include <rtems/shell.h>

#include "common_types.h"
#include "osapi.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

#define ERROR                        -1
#define OS_REDIRECTSTRSIZE           15
#define OS_SHELL_TMP_FILE_EXT        ".out"
#define OS_SHELL_TMP_FILE_EXT_LEN    4 
#define OS_SHELL_CMD_TASK_STACK_SIZE 16384
#define OS_SHELL_CMD_TASK_PRIORITY   250

#define OSAL_TABLE_MUTEX_ATTRIBS \
 (RTEMS_PRIORITY | RTEMS_BINARY_SEMAPHORE | \
  RTEMS_INHERIT_PRIORITY | RTEMS_NO_PRIORITY_CEILING | RTEMS_LOCAL)

/***************************************************************************************
                                 FUNCTION PROTOTYPES
***************************************************************************************/

int32 OS_check_name_length(const char *path);
extern uint32 OS_FindCreator(void);

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

OS_FDTableEntry OS_FDTable[OS_MAX_NUM_OPEN_FILES];
rtems_id        OS_FDTableSem;
rtems_id        OS_VolumeTableSem;
/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/
int32 OS_FS_Init(void)
{
    int               i;
    rtems_status_code rtems_sc;
	
    /* Initialize the file system constructs */
    for (i =0; i < OS_MAX_NUM_OPEN_FILES; i++)
    {
        OS_FDTable[i].OSfd =       -1;
        strcpy(OS_FDTable[i].Path, "\0");
        OS_FDTable[i].User =       0;
        OS_FDTable[i].IsValid =    FALSE;
    }

   /*
   ** Initialize the FS subsystem semaphore
   */
   rtems_sc = rtems_semaphore_create (rtems_build_name ('M', 'U', 'T', 'F'),
                                      1, OSAL_TABLE_MUTEX_ATTRIBS, 0,
                                      &OS_FDTableSem);
   if ( rtems_sc != RTEMS_SUCCESSFUL )
   {
      return(OS_ERROR);
   }

   /*
   ** Initialize the RTEMS system call lock semaphore 
   */
   rtems_sc = rtems_semaphore_create (rtems_build_name ('V', 'O', 'L', 'T'),
                                      1, OSAL_TABLE_MUTEX_ATTRIBS, 0,
                                      &OS_VolumeTableSem);
   if ( rtems_sc != RTEMS_SUCCESSFUL )
   {
      return(OS_ERROR);
   }


   return(OS_SUCCESS);


}
/****************************************************************************************
                                    Filesys API
****************************************************************************************/

/*
** Standard File system API
*/

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
             File Descriptor is returned on success. 
    
---------------------------------------------------------------------------------------*/

int32 OS_creat  (const char *path, int32 access)
{
    int               status;
    rtems_status_code rtems_sc;
    char              local_path[OS_MAX_LOCAL_PATH_LEN];
    int               perm;
    mode_t            mode;
    uint32            PossibleFD;

    /*
    ** Check to see if the path pointer is NULL
    */
    if (path == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }
   
    /*
    ** Check to see if the path is too long
    */
    if (strlen(path) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    /* 
    ** check if the name of the file is too long 
    */
    if (OS_check_name_length(path) != OS_FS_SUCCESS)
    {
        return OS_FS_ERR_NAME_TOO_LONG;
    }

    /*
    ** Check for a valid access mode
    ** For creating a file, OS_READ_ONLY does not make sense
    */
    switch(access)
    {
        case OS_WRITE_ONLY:
            perm = O_WRONLY;
            break;
        case OS_READ_WRITE:
            perm = O_RDWR;
            break;
        default:
            return OS_FS_ERROR;
    }

    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(path, (char *)local_path) != OS_FS_SUCCESS )
    {
        return OS_FS_ERR_PATH_INVALID;
    }
  
    
    /* Check Parameters */
    rtems_sc = rtems_semaphore_obtain (OS_FDTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    for ( PossibleFD = 0; PossibleFD < OS_MAX_NUM_OPEN_FILES; PossibleFD++)
    {
        if( OS_FDTable[PossibleFD].IsValid == FALSE)
        {
            break;
        }
    }

    if (PossibleFD >= OS_MAX_NUM_OPEN_FILES)
    {
        rtems_sc = rtems_semaphore_release (OS_FDTableSem);
        return OS_FS_ERR_NO_FREE_FDS;
    }

    /* 
    ** Mark the table entry as valid so no other 
    ** task can take that ID 
    */
    OS_FDTable[PossibleFD].IsValid =    TRUE;

    rtems_sc = rtems_semaphore_release (OS_FDTableSem);

    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
   
    status =  open(local_path, perm | O_CREAT | O_TRUNC , mode); 

    if (status != ERROR)
    {
        rtems_sc = rtems_semaphore_obtain (OS_FDTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
        /* fill in the table before returning */
        OS_FDTable[PossibleFD].OSfd =       status;
        strncpy(OS_FDTable[PossibleFD].Path, path, OS_MAX_PATH_LEN);
        OS_FDTable[PossibleFD].User =       OS_FindCreator();
        rtems_sc = rtems_semaphore_release (OS_FDTableSem);
        return PossibleFD;
    }
    else
    {
        rtems_sc = rtems_semaphore_obtain (OS_FDTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
        /* Operation failed, so reset to false */
        OS_FDTable[PossibleFD].IsValid = FALSE;
        rtems_sc = rtems_semaphore_release (OS_FDTableSem);
        return OS_FS_ERROR;
    }
 
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
    int               status;
    rtems_status_code rtems_sc;
    char              local_path[OS_MAX_LOCAL_PATH_LEN];
    int               perm;
    uint32            PossibleFD;
    
    /*
    ** Check to see if the path pointer is NULL
    */
    if (path == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }
   
    /*
    ** Check to see if the path is too long
    */
    if (strlen(path) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    /* 
    ** check if the name of the file is too long 
    */
    if (OS_check_name_length(path) != OS_FS_SUCCESS)
    {
        return OS_FS_ERR_NAME_TOO_LONG;
    }
   
    /*
    ** Check for a valid access mode
    */
    switch(access)
    {
        case OS_READ_ONLY:
            perm = O_RDONLY;
            break;
        case OS_WRITE_ONLY:
            perm = O_WRONLY | O_CREAT;
            break;
        case OS_READ_WRITE:
            perm = O_RDWR | O_CREAT;
            break;
        default:
            return OS_FS_ERROR;
    }

    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(path, (char *)local_path) != OS_FS_SUCCESS )
    {
        return OS_FS_ERR_PATH_INVALID;
    }
    
   rtems_sc = rtems_semaphore_obtain (OS_FDTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    for ( PossibleFD = 0; PossibleFD < OS_MAX_NUM_OPEN_FILES; PossibleFD++)
    {
        if( OS_FDTable[PossibleFD].IsValid == FALSE)
        {
            break;
        }
    }

    if (PossibleFD >= OS_MAX_NUM_OPEN_FILES)
    {
        rtems_sc = rtems_semaphore_release (OS_FDTableSem);
        return OS_FS_ERR_NO_FREE_FDS;
    }

    /* 
    ** Mark the table entry as valid so no other 
    ** task can take that ID 
    */
    OS_FDTable[PossibleFD].IsValid = TRUE;

    rtems_sc = rtems_semaphore_release (OS_FDTableSem);

    /* open the file  */
    status =  open(local_path, perm, mode);

    if (status != ERROR)
    {
        rtems_sc = rtems_semaphore_obtain (OS_FDTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
        /* fill in the table before returning */
        OS_FDTable[PossibleFD].OSfd =       status;
        strncpy(OS_FDTable[PossibleFD].Path, path, OS_MAX_PATH_LEN);
        OS_FDTable[PossibleFD].User =       OS_FindCreator();
        rtems_sc = rtems_semaphore_release (OS_FDTableSem);
        
        return PossibleFD;
    }
    else
    {
        rtems_sc = rtems_semaphore_obtain (OS_FDTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
        /* Operation failed, so reset to false */
        OS_FDTable[PossibleFD].IsValid = FALSE;
        rtems_sc = rtems_semaphore_release (OS_FDTableSem);
        return OS_FS_ERROR;
    }
 
} /* end OS_open */


/*--------------------------------------------------------------------------------------
    Name: OS_close
    
    Purpose: Closes a file. 

    Returns: OS_FS_ERROR if file  descriptor could not be closed
             OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             OS_FS_SUCCESS if success
---------------------------------------------------------------------------------------*/

int32 OS_close (int32  filedes)
{
    int               status;
    rtems_status_code rtems_sc;

    /* Make sure the file descriptor is legit before using it */
    if (filedes < 0 || filedes >= OS_MAX_NUM_OPEN_FILES || OS_FDTable[filedes].IsValid == FALSE)
    {
        return OS_FS_ERR_INVALID_FD;
    }
    else
    {    
        status = close ((int) OS_FDTable[filedes].OSfd);
        if (status == ERROR)
        {
            /*
            ** Remove the file from the OSAL list
            ** to free up that slot 
            */
            /* fill in the table before returning */
            rtems_sc = rtems_semaphore_obtain (OS_FDTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
            OS_FDTable[filedes].OSfd =       -1;
            strcpy(OS_FDTable[filedes].Path, "\0");
            OS_FDTable[filedes].User =       0;
            OS_FDTable[filedes].IsValid =    FALSE;
            rtems_sc = rtems_semaphore_release (OS_FDTableSem);

            return OS_FS_ERROR;
        }
        else
        {
            /* fill in the table before returning */
            rtems_sc = rtems_semaphore_obtain (OS_FDTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
            OS_FDTable[filedes].OSfd =       -1;
            strcpy(OS_FDTable[filedes].Path, "\0");
            OS_FDTable[filedes].User =       0;
            OS_FDTable[filedes].IsValid =    FALSE;
            rtems_sc = rtems_semaphore_release (OS_FDTableSem);
            
            return OS_FS_SUCCESS;
        }

    }
    
}/* end OS_close */

/*--------------------------------------------------------------------------------------
    Name: OS_read
    
    Purpose: reads up to nbytes from a file, and puts them into buffer. 
    
    Returns: OS_FS_ERR_INVALID_POINTER if buffer is a null pointer
             OS_FS_ERROR if OS call failed
             OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             number of bytes read if success
---------------------------------------------------------------------------------------*/
int32 OS_read  (int32  filedes, void *buffer, uint32 nbytes)
{
    int32 status;

    if (buffer == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    /* Make sure the file descriptor is legit before using it */
    if (filedes < 0 || filedes >= OS_MAX_NUM_OPEN_FILES || OS_FDTable[filedes].IsValid == FALSE)
    {
        return OS_FS_ERR_INVALID_FD;
    }
    else
    { 
        status = read (OS_FDTable[filedes].OSfd, buffer, nbytes);
        if (status == ERROR)
        {
            return OS_FS_ERROR;
        }
    }

    return status;
    
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

int32 OS_write (int32  filedes, void *buffer, uint32 nbytes)
{
    int32 status;

    if (buffer == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    /* Make sure the file descriptor is legit before using it */
    if (filedes < 0 || filedes >= OS_MAX_NUM_OPEN_FILES || OS_FDTable[filedes].IsValid == FALSE)
    {
        return OS_FS_ERR_INVALID_FD;
    }
    else
    {
        status = write(OS_FDTable[filedes].OSfd, buffer, nbytes );
    
        if (status != ERROR)
        {
            return  status;
        }
        else
        {
            return OS_FS_ERROR;
        }
    }
    
}/* end OS_write */

/*--------------------------------------------------------------------------------------
    Name: OS_chmod

    Notes: This is not going to be implemented because there is no use for this function.
---------------------------------------------------------------------------------------*/

int32 OS_chmod  (const char *path, uint32 access)
{    
    return OS_FS_UNIMPLEMENTED;
} /* end OS_chmod */

/*--------------------------------------------------------------------------------------
    Name: OS_stat
    
    Purpose: returns information about a file or directory in a os_fs_stat structure
    
    Returns: OS_FS_ERR_INVALID_POINTER if path or filestats is NULL
             OS_FS_ERR_PATH_TOO_LONG if the path is too long to be stored locally
*****        OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long to be stored
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERROR id the OS call failed
             OS_FS_SUCCESS if success

    Note: The information returned is in the structure pointed to by filestats         
---------------------------------------------------------------------------------------*/

int32 OS_stat   (const char *path, os_fstat_t  *filestats)
{
    int ret_val;
    char local_path[OS_MAX_LOCAL_PATH_LEN];
  
    /*
    ** Check to see if the file pointers are NULL
    */
    if (path == NULL || filestats == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }
        
    /*
    ** Check to see if the path is too long
    */
    if (strlen(path) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }
   
    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(path, (char *)local_path) != OS_FS_SUCCESS )
    {
        return OS_FS_ERR_PATH_INVALID;
    }

    ret_val = stat( (char*) local_path, filestats);
    if (ret_val == ERROR)
    {
        return OS_FS_ERROR;
    }
    else
    {
        return OS_FS_SUCCESS;
    }
    
} /* end OS_stat */

/*--------------------------------------------------------------------------------------
    Name: OS_lseek

    Purpose: sets the read/write pointer to a specific offset in a specific file. 
             Whence is either OS_SEEK_SET,OS_SEEK_CUR, or OS_SEEK_END

    Returns: the new offset from the beginning of the file
             OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             OS_FS_ERROR if OS call failed
---------------------------------------------------------------------------------------*/

int32 OS_lseek  (int32  filedes, int32 offset, uint32 whence)
{
     off_t status;
     int where;

    /* Make sure the file descriptor is legit before using it */
    if (filedes < 0 || filedes >= OS_MAX_NUM_OPEN_FILES || OS_FDTable[filedes].IsValid == FALSE)
    {
        return OS_FS_ERR_INVALID_FD;
    }
    else
    {
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

    
        status = lseek( OS_FDTable[filedes].OSfd, (off_t) offset, (int) where );

        if ( (int) status != ERROR)
        {
            return (int32) status;
        }
        else
        {
            return OS_FS_ERROR;
        }
    }
 
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
    int  i;
    int  status;
    char local_path[OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check to see if the path pointer is NULL
    */
    if (path == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }
   
    /*
    ** Check to see if the path is too long
    */
    if (strlen(path) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    /* 
    ** check if the name of the file is too long 
    */
    if (OS_check_name_length(path) != OS_FS_SUCCESS)
    {
        return OS_FS_ERR_NAME_TOO_LONG;
    }
  
    /*
    ** Make sure the file is not open by the OSAL before deleting it 
    */
    for ( i =0; i < OS_MAX_NUM_OPEN_FILES; i++)
    {
       if ((OS_FDTable[i].IsValid == TRUE) &&
          (strcmp(OS_FDTable[i].Path, path) == 0))
       {
          return OS_FS_ERROR;
       }
    }
 
    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(path, (char *)local_path) != OS_FS_SUCCESS )
    {
        return OS_FS_ERR_PATH_INVALID;
    }

    /*
    ** Call the system to remove the file
    */
    status = remove (local_path);
    if (status == 0)
    {
        return OS_FS_SUCCESS;
    }
    else
    {
        return OS_FS_ERROR;
    }
    
} /* end OS_remove */

/*--------------------------------------------------------------------------------------
    Name: OS_rename
    
    Purpose: renames a file

    Returns: OS_FS_SUCCESS if the rename works
             OS_FS_ERROR if the file could not be opened or renamed.
             OS_FS_ERR_INVALID_POINTER if old_filename or new_filename are NULL
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERR_PATH_TOO_LONG if the paths given are too long to be stored locally
             OS_FS_ERR_NAME_TOO_LONG if the new name is too long to be stored locally
---------------------------------------------------------------------------------------*/

int32 OS_rename (const char *old_filename, const char *new_filename)
{
    int status,i;
    char old_path[OS_MAX_LOCAL_PATH_LEN];
    char new_path[OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check to see if the path pointers are NULL
    */    
    if (old_filename == NULL || new_filename == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    /*
    ** Check to see if the paths are too long
    */
    if (strlen(old_filename) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }
    
    if (strlen(new_filename) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    /* 
    ** check if the names of the files are too long 
    */
    if (OS_check_name_length(old_filename) != OS_FS_SUCCESS)
    {
        return OS_FS_ERR_NAME_TOO_LONG;
    }

    if (OS_check_name_length(new_filename) != OS_FS_SUCCESS)
    {
        return OS_FS_ERR_NAME_TOO_LONG;
    }

    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(old_filename, (char *)old_path) != OS_FS_SUCCESS )
    {
        return OS_FS_ERR_PATH_INVALID;
    }

    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(new_filename, (char *)new_path) != OS_FS_SUCCESS )
    {
        return OS_FS_ERR_PATH_INVALID;
    }
     
    status = rename (old_path, new_path);
    if (status == 0)
    {
        for ( i =0; i < OS_MAX_NUM_OPEN_FILES; i++) 
        {
            if (strcmp(OS_FDTable[i].Path, old_filename) == 0 &&
                OS_FDTable[i].IsValid == TRUE)
            {
                strncpy (OS_FDTable[i].Path, new_filename, OS_MAX_PATH_LEN);  
            } 
        }
        return OS_FS_SUCCESS;
    }
    else
    {
         return OS_FS_ERROR;
    }
     
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
    int  i;
    int  src_fd;
    int  dest_fd;
    char src_path[OS_MAX_LOCAL_PATH_LEN];
    char dest_path[OS_MAX_LOCAL_PATH_LEN];
    char data_buffer[512];
    int  bytes_read;
    int  bytes_written;

    /*
    ** Check to see if the path pointers are NULL
    */    
    if (src == NULL || dest == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    /*
    ** Check to see if the paths are too long
    */
    if (strlen(src) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }
    
    if (strlen(dest) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }

    /* 
    ** check if the names of the files are too long 
    */
    if (OS_check_name_length(src) != OS_FS_SUCCESS)
    {
        return OS_FS_ERR_NAME_TOO_LONG;
    }

    if (OS_check_name_length(dest) != OS_FS_SUCCESS)
    {
        return OS_FS_ERR_NAME_TOO_LONG;
    }

    /*
    ** Make sure the destintation file is not open by the OSAL before doing the copy 
    ** This may be caught by the host OS open call but it does not hurt to 
    ** be consistent 
    */
    for ( i =0; i < OS_MAX_NUM_OPEN_FILES; i++)
    {
        if ((OS_FDTable[i].IsValid == TRUE) &&
          (strcmp(OS_FDTable[i].Path, dest) == 0))
        {
           return OS_FS_ERROR;
        }
    }

    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(src, (char *)src_path) != OS_FS_SUCCESS )
    {
        return OS_FS_ERR_PATH_INVALID;
    }

    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(dest, (char *)dest_path) != OS_FS_SUCCESS )
    {
        return OS_FS_ERR_PATH_INVALID;
    }

    /*
    ** Do the copy
    */
    if((src_fd = open(src_path, O_RDONLY)) == -1) 
    {
       return OS_FS_ERR_PATH_INVALID;
    }

    if((dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
    {
       close(src_fd);
       return OS_FS_ERR_PATH_INVALID;
    }
 
    while((bytes_read = read(src_fd, data_buffer, sizeof(data_buffer))) > 0)
    {
       bytes_written = write(dest_fd, data_buffer, bytes_read);
       if ( bytes_written < 0 )
       {
          close(src_fd);
          close(dest_fd);
          return OS_FS_ERROR;
       }
    }

    close(src_fd);
    close(dest_fd);

    if ( bytes_read < 0 )
    {
       return OS_FS_ERROR;
    }
    else
    {
       return OS_FS_SUCCESS;
    }
     
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
   int i;
   int32 status;

   /*
   ** Validate the source and destination
   ** These checks may seem redundant because OS_cp and OS_remove also do 
   ** the same checks, but this call needs to abort before doing a copy
   ** in some cases.
   */

   /*
   ** Check to see if the path pointers are NULL
   */
   if (src == NULL || dest == NULL)
   {
        return OS_FS_ERR_INVALID_POINTER;
   }

   /*
   ** Check to see if the paths are too long
   */
   if (strlen(src) >= OS_MAX_PATH_LEN)
   {
       return OS_FS_ERR_PATH_TOO_LONG;
   }

   if (strlen(dest) >= OS_MAX_PATH_LEN)
   {
       return OS_FS_ERR_PATH_TOO_LONG;
   }

   /* 
   ** check if the names of the files are too long 
   */
   if (OS_check_name_length(src) != OS_FS_SUCCESS)
   {
       return OS_FS_ERR_NAME_TOO_LONG;
   }

   if (OS_check_name_length(dest) != OS_FS_SUCCESS)
   {
       return OS_FS_ERR_NAME_TOO_LONG;
   }

   /*
   ** Make sure the source file is not open by the OSAL before doing the move 
   */
   for ( i =0; i < OS_MAX_NUM_OPEN_FILES; i++)
   {
       if ((OS_FDTable[i].IsValid == TRUE) && 
          (strcmp(OS_FDTable[i].Path, src) == 0))
       {
          return OS_FS_ERROR;
       }
   }

   status = OS_cp (src, dest);
   if ( status == OS_FS_SUCCESS )
   {
      status = OS_remove(src);
   }

   return ( status);       
}

/*
** Directory API 
*/
/*--------------------------------------------------------------------------------------
    Name: OS_mkdir

    Purpose: makes a directory specified by path.

    Returns: OS_FS_ERR_INVALID_POINTER if path is NULL
             OS_FS_ERR_PATH_TOO_LONG if the path is too long to be stored locally
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERROR if the OS call fails
             OS_FS_SUCCESS if success

    Note: The access parameter is currently unused.
---------------------------------------------------------------------------------------*/

int32 OS_mkdir (const char *path, uint32 access)
{
   int status;
   mode_t mode;
   char local_path[OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check to see if the path pointer is NULL
    */    
    if (path == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    /*
    ** Check to see if the path is too long
    */      
    if (strlen(path) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }


    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(path, (char *)local_path) != OS_FS_SUCCESS )
    {
        return OS_FS_ERR_PATH_INVALID;
    }
    
    mode = S_IFDIR |S_IRWXU | S_IRWXG | S_IRWXO;
    status = mkdir(local_path, mode);

    if (status == 0)
    {
        return OS_FS_SUCCESS;
    }
    else
    {
        return OS_FS_ERROR;
    }
    
}/* end OS_mkdir */

/*--------------------------------------------------------------------------------------
    Name: OS_opendir

    Purpose: opens a directory for searching

    Returns: NULL if the directory cannot be opened
             Directory pointer if the directory is opened
             
---------------------------------------------------------------------------------------*/

os_dirp_t OS_opendir (const char *path)
{

    os_dirp_t dirdescptr;
    char local_path[OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check to see if the path pointer is NULL
    */        
    if (path == NULL)
    {
        return NULL;
    }

    /*
    ** Check to see if the path is too long
    */      
    if (strlen(path) > OS_MAX_PATH_LEN)
    {
        return NULL;
    }
   
    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(path, (char *)local_path) != OS_FS_SUCCESS )
    {
        return NULL;
    }
   
    dirdescptr = opendir( (char*) local_path);
   
    /*
    ** will return a dirptr or NULL
    */ 
    return dirdescptr;
    
} /* end OS_opendir */

/*--------------------------------------------------------------------------------------
    Name: OS_closedir
    
    Purpose: closes a directory

    Returns: OS_FS_SUCCESS if success
             OS_FS_ERROR if close failed
---------------------------------------------------------------------------------------*/

int32 OS_closedir (os_dirp_t directory)
{ 
    int status;

    if (directory == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    status = closedir(directory);
    if (status == 0)
    {
        return OS_FS_SUCCESS;
    }
    else
    {
        return OS_FS_ERROR;
    }

} /* end OS_closedir */

/*--------------------------------------------------------------------------------------
    Name: OS_readdir

    Purpose: obtains directory entry data for the next file from an open directory

    Returns: a pointer to the next entry for success
             NULL if error or end of directory is reached
---------------------------------------------------------------------------------------*/


os_dirent_t *  OS_readdir (os_dirp_t directory)
{ 
    os_dirent_t *tempptr;

    if (directory == NULL)
    {
        return NULL;
    }

    tempptr = readdir( directory);
   
    /* 
    ** Will return dirptr or NULL
    */ 
    return tempptr;
        
} /* end OS_readdir */

/*--------------------------------------------------------------------------------------
    Name: OS_rewinddir

    Purpose: Rewinds the directory pointer

    Returns: N/A
---------------------------------------------------------------------------------------*/

void  OS_rewinddir (os_dirp_t directory )
{ 
    if (directory != NULL)
    {
       rewinddir( directory);
    }
}
/*--------------------------------------------------------------------------------------
    Name: OS_rmdir
    
    Purpose: removes a directory from  the structure (must be an empty directory)

    Returns: OS_FS_ERR_INVALID_POINTER if path is NULL
             OS_FS_ERR_PATH_INVALID if path cannot be parsed    
             OS_FS_ERR_PATH_TOO_LONG
---------------------------------------------------------------------------------------*/

int32  OS_rmdir (const char *path)
{
    int status;
    char local_path [OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check to see if the path pointer is NULL
    */            
    if (path == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    /*
    ** Check to see if the path is too long
    */      
    if (strlen(path) >= OS_MAX_PATH_LEN)
    {
        return OS_FS_ERR_PATH_TOO_LONG;
    }


    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(path, (char *)local_path) != OS_FS_SUCCESS )
    {
        return OS_FS_ERR_PATH_INVALID;
    }
    
    status = rmdir(local_path);
    
    if (status == 0)
    {
        return OS_FS_SUCCESS;
    }
    else
    {
        return OS_FS_ERROR;
    }
    
}/* end OS_rmdir */

/* --------------------------------------------------------------------------------------
    Name: OS_check_name_length
    
    Purpose: Checks the length of the file name at the end of the path.
    
    Returns: OS_FS_ERROR if path is NULL, path is too long, there is no '/' in the path
             name, the name is too long
             OS_SUCCESS if success
            
    NOTE: This is only an internal function and is not intended for use by the user
 ---------------------------------------------------------------------------------------*/

int32 OS_check_name_length(const char *path)
{
    char* name_ptr;
    char* end_of_path;
    int name_len;
    
    if (path == NULL)
    {
        return OS_FS_ERR_INVALID_POINTER;
    }

    
    if (strlen(path) > OS_MAX_PATH_LEN)
    {
        return OS_FS_ERROR;
    }
    
    /* checks to see if there is a '/' somewhere in the path */
    name_ptr = strrchr(path, '/');
    if (name_ptr == NULL)
    {
        return OS_FS_ERROR;
    }
    
    /* strrchr returns a pointer to the last '/' char, so we advance one char */
    name_ptr = name_ptr + sizeof(char);
    
    /* end_of_path points to the null terminator at the end of the path */
    end_of_path = strrchr(path,'\0');

    /* pointer subraction to see how many characters there are in the name */
    name_len = ((int) end_of_path - (int)name_ptr) / sizeof(char);
    
    if( name_len > OS_MAX_FILE_NAME)
    {
        return OS_FS_ERROR;
    }
    
    return OS_FS_SUCCESS;
    
}/* end OS_check_name_length */
/* --------------------------------------------------------------------------------------
    Name: OS_ShellOutputToFile
    
    Purpose: Takes a shell command in and writes the output of that command to the specified file
    
    Returns: OS_FS_ERROR if the command was not executed properly
             OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
int32 OS_ShellOutputToFile(char* Cmd, int32 OS_fd)
{
    int32              cmdFd;
    int32              tmpFd;
    rtems_status_code  rtemsRc;;
    int32              ReturnCode = OS_FS_SUCCESS;
    int32              fileStatus;
    int32              bytesRead;
    int32              bytesWritten;
    char               readBuffer[256];
    char               outputFileName[OS_MAX_PATH_LEN + OS_SHELL_TMP_FILE_EXT_LEN];
    char               localCmd[OS_MAX_CMD_LEN]; 


    /*
    ** Check parameters
    */
    if (Cmd == NULL)
    {
        return(OS_FS_ERR_INVALID_POINTER);
    }

    /* Make sure the file descriptor is legit before using it */
    if (OS_fd < 0 || OS_fd >= OS_MAX_NUM_OPEN_FILES || OS_FDTable[OS_fd].IsValid == FALSE)
    {
        ReturnCode = OS_FS_ERR_INVALID_FD;
    }
    else
    {
        /* 
        ** Create a file to write the command to (or write over the old one) 
        */
        cmdFd = OS_creat(OS_SHELL_CMD_INPUT_FILE_NAME,OS_READ_WRITE);
        if (cmdFd < OS_FS_SUCCESS)
        {
            ReturnCode = OS_FS_ERROR;
        }
        else
        {
            /*
            ** Write the command to the buffer
            */
            strncpy(localCmd,Cmd,OS_MAX_CMD_LEN);
            strncat(localCmd,"\n",1);

            /*
            ** This function passes in an open file descriptor to write the shell
            **  command output. The RTEMS shell script API expects a filename, not
            **  a file descriptor. So in addition to the temporary file for the shell input,
            **  we need to create a temporary file for the command output, then it has
            **  to be copied to the open file.  
            */
            strncpy(outputFileName,OS_SHELL_CMD_INPUT_FILE_NAME,OS_MAX_PATH_LEN);           
            strncat(outputFileName,OS_SHELL_TMP_FILE_EXT,OS_SHELL_TMP_FILE_EXT_LEN);

            /* 
            ** copy the shell command buffer to the file 
            */
            fileStatus = OS_write(cmdFd, localCmd, strlen(localCmd));
            if ( fileStatus == strlen(localCmd) )
            {
               /*
               ** Close the file
               */
               OS_close(cmdFd);	

               /*
               ** Spawn a task to execute the shell command
               */
               rtemsRc =  rtems_shell_script ( 
                         "RSHL", 
                         OS_SHELL_CMD_TASK_STACK_SIZE, 
                         OS_SHELL_CMD_TASK_PRIORITY, 
                         OS_SHELL_CMD_INPUT_FILE_NAME,
                         outputFileName,
                         FALSE,       /* Do not append output to file */
                         TRUE,        /* Wait for shell task to complete */
                         FALSE        /* Echo output */ 
                        );
                
               /*
               ** Now we have the temporary file with the output 
               */
               if((tmpFd = OS_open(outputFileName, OS_READ_ONLY,0)) == OS_FS_ERROR)
               {
                  ReturnCode = OS_FS_ERROR; 
               }
               else
               { 
                  while((bytesRead = OS_read(tmpFd, readBuffer, 256)) > 0)
                  {
                     bytesWritten = OS_write(OS_fd, readBuffer, bytesRead);
                  }
                  OS_close(tmpFd);
                  /*
                  ** Remove the temporary output file
                  */
                  fileStatus = OS_remove(outputFileName); 
               } 
            }
            else
            {
                 ReturnCode = OS_FS_ERROR;
            }
        } 
        /*
        ** Remove the temporary shell input file
        */
        fileStatus = OS_remove(OS_SHELL_CMD_INPUT_FILE_NAME); 
    }
 
    return ReturnCode;

}/* end OS_ShellOutputToFile */

/* --------------------------------------------------------------------------------------
Name: OS_FDGetInfo
    
Purpose: Copies the information of the given file descriptor into a structure passed in
    
Returns: OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
         OS_FS_SUCCESS if the copying was successfull
 ---------------------------------------------------------------------------------------*/

int32 OS_FDGetInfo (int32 filedes, OS_FDTableEntry *fd_prop)
{

    if (fd_prop == NULL)
    {
        return(OS_FS_ERR_INVALID_POINTER);
    }

    /* Make sure the file descriptor is legit before using it */
    if (filedes < 0 || filedes >= OS_MAX_NUM_OPEN_FILES || OS_FDTable[filedes].IsValid == FALSE)
    {
       (*(fd_prop)).IsValid = FALSE; 
        return OS_FS_ERR_INVALID_FD;
    }
    else
    { 
        *fd_prop = OS_FDTable[filedes];
        return OS_FS_SUCCESS;
    }

}/* end OS_FDGetInfo */

/* --------------------------------------------------------------------------------------
   Name: OS_FileOpenCheck
    
   Purpose: Checks to see if a file is open 
    
   Returns: OS_FS_ERROR if the file is not open 
            OS_FS_SUCCESS if the file is open 
 ---------------------------------------------------------------------------------------*/
int32 OS_FileOpenCheck(char *Filename)
{
    rtems_status_code rtems_sc;
    uint32            i;

    if (Filename == NULL)
    {
        return(OS_FS_ERR_INVALID_POINTER);
    }

    rtems_sc = rtems_semaphore_obtain (OS_FDTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

    for ( i = 0; i < OS_MAX_NUM_OPEN_FILES; i++)
    {
        if ((OS_FDTable[i].IsValid == TRUE) &&  (strcmp(OS_FDTable[i].Path, Filename) == 0))
        {
           rtems_sc = rtems_semaphore_release (OS_FDTableSem);
           return(OS_FS_SUCCESS);
        }
    }/* end for */

    rtems_sc = rtems_semaphore_release (OS_FDTableSem);
    return OS_FS_ERROR;

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
int32 OS_CloseFileByName(char *Filename)
{
    rtems_status_code rtems_sc;
    uint32            i;
    int               status;

    if (Filename == NULL)
    {
        return(OS_FS_ERR_INVALID_POINTER);
    }

    for ( i = 0; i < OS_MAX_NUM_OPEN_FILES; i++)
    {
        if ((OS_FDTable[i].IsValid == TRUE) &&  (strcmp(OS_FDTable[i].Path, Filename) == 0))
        {
           /*
           ** Close the file  
           */
           status = close ((int) OS_FDTable[i].OSfd);

           /*
           ** Next, remove the file from the OSAL list
           ** to free up that slot
           */
           rtems_sc = rtems_semaphore_obtain (OS_FDTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
           OS_FDTable[i].OSfd =       -1;
           strcpy(OS_FDTable[i].Path, "\0");
           OS_FDTable[i].User =       0;
           OS_FDTable[i].IsValid =    FALSE;
           rtems_sc = rtems_semaphore_release (OS_FDTableSem);
           if (status == ERROR)
           {
              return(OS_FS_ERROR);
           }
           else
           {
              return(OS_FS_SUCCESS);
           }
        }

    }/* end for */

    return (OS_FS_ERR_PATH_INVALID);

}/* end OS_CloseFileByName */

/* --------------------------------------------------------------------------------------
   Name: OS_CloseAllFiles

   Purpose: Closes All open files that were opened through the OSAL 

   Returns: OS_FS_ERROR   if one or more file close returned an error
            OS_FS_SUCCESS if the files were all closed without error
 ---------------------------------------------------------------------------------------*/
int32 OS_CloseAllFiles(void)
{
    rtems_status_code rtems_sc;
    uint32            i;
    int32             return_status = OS_FS_SUCCESS;
    int               status;

    for ( i = 0; i < OS_MAX_NUM_OPEN_FILES; i++)
    {
        if ( OS_FDTable[i].IsValid == TRUE )
        {
           /*
           ** Close the file
           */
           status = close ((int) OS_FDTable[i].OSfd);

           /*
           ** Next, remove the file from the OSAL list
           ** to free up that slot
           */
           rtems_sc = rtems_semaphore_obtain (OS_FDTableSem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
           OS_FDTable[i].OSfd =       -1;
           strcpy(OS_FDTable[i].Path, "\0");
           OS_FDTable[i].User =       0;
           OS_FDTable[i].IsValid =    FALSE;
           rtems_sc = rtems_semaphore_release (OS_FDTableSem);
           if (status == ERROR)
           {
              return_status = OS_FS_ERROR;
           }
        }

    }/* end for */

    return (return_status);

}/* end OS_CloseAllFiles */

