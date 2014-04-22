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
**          files in a file system for vxworks
**
** $Date: 2014/04/23 15:14:37GMT-05:00 $
** $Revision: 1.22 $
** $Log: osfileapi.c  $
** Revision 1.22 2014/04/23 15:14:37GMT-05:00 acudmore 
** Fixed use of O_CREAT flag in OS_open. Dont use it when opening a file as read only
** Revision 1.21 2013/12/16 12:52:59GMT-05:00 acudmore 
** Removed dupliclate if/then/else block
** Revision 1.20 2013/07/29 12:04:07GMT-05:00 acudmore 
** Updated OS_open to create a file if it does not exist
** Update OS_ShellOutputToFile to check pointer and return OS_FS_SUCCESS
** Revision 1.19 2013/04/11 15:25:32GMT-05:00 acudmore 
** Add open file checks to OS_mv, OS_remove, and OS_cp
** Revision 1.18 2012/12/20 14:04:58GMT-05:00 acudmore 
** Changed OS_mv to do a copy and delete.
**    This is less efficient, but allows cross file system moves to work, which
**    is how the OSAL document says this API works.
** Revision 1.17 2012/12/19 13:49:32EST acudmore 
** Updated OS_open and OS_creat to handle access parameters in a consistent way
** Revision 1.16 2012/12/11 13:25:16EST acudmore 
** Added NULL pointer check to OS_FDGetInfo
** Revision 1.15 2012/11/15 14:19:06EST acudmore 
** Removed redundant close in OS_close
** Revision 1.14 2011/12/05 12:36:19EST acudmore 
** added OS_rewinddir
** Revision 1.13 2011/06/27 15:52:33EDT acudmore 
** Went over APIs and Documentation for return code consistency.
** Updated documentation, function comments, and return codes as needed.
** Revision 1.12 2011/05/03 14:34:52EDT acudmore 
** Added an API to close a file by name
** Revision 1.11 2011/04/05 13:21:09EDT acudmore 
** Updated OS_creat to use open instead of creat and pass in 
** flags that are compatible with an NFS volume
** Revision 1.10 2010/11/15 11:06:58EST acudmore 
** Added OS_FIleOpenCheck function
** Revision 1.9 2010/11/12 12:37:31EST acudmore 
** Updated error return values in comments.
** Revision 1.8 2010/11/12 12:03:09EST acudmore 
** replaced copyright symbol with (c) and added open source notice where needed.
** Revision 1.7 2010/02/01 12:41:21EST acudmore 
** Changes to support return code on OS_API_Init
** Revision 1.6 2010/01/25 14:45:28EST acudmore 
** renamed "new" variable to avoid C++ reserved name conflict.
** Revision 1.5 2009/07/15 16:14:26EDT acudmore 
** changed return statement to remove warning
** Revision 1.4 2009/07/14 16:47:38EDT acudmore 
** Updated vxWorks file system API to use OS_TranslatePath
** Revision 1.3 2009/07/10 15:06:49EDT acudmore 
** Added code to clean up OSAL resources regardless of OS_close error
** Revision 1.2 2009/03/10 10:53:45EDT nyanchik 
** I made the changes to the comments of _OS_check_name_length foir all four OS's
** Revision 1.1 2008/04/21 03:36:12BST ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-OSAL-REPOSITORY/src/os/vxworks6/project.pj
** Revision 1.3 2008/03/28 10:56:02EDT njyanchik 
** I added the code shownin the DCR to OS_rename and OS_mv for all OS's
** Revision 1.2 2008/01/22 15:16:48AST njyanchik 
** I fixed the #define that is being used in the two functions listed in the DCR with the correct ones
** Revision 1.1 2007/10/16 16:15:01EDT apcudmore 
** Initial revision
*/

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "vxWorks.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "ioLib.h"
#include "ramDrv.h"
#include "dosFsLib.h"

#include "dirent.h"
#include "stat.h"

#include "shellLib.h"
#include "taskLib.h"
#include "sysLib.h"
#include "semLib.h"
#include "usrLib.h"

#include "cbioLib.h"
#include "ramDiskCbio.h"

#include "common_types.h"
#include "osapi.h"


/****************************************************************************************
                                     DEFINES
****************************************************************************************/



/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/
OS_FDTableEntry OS_FDTable[OS_MAX_NUM_OPEN_FILES];
SEM_ID  OS_FDTableMutex;

extern uint32 OS_FindCreator(void);
int32 OS_check_name_length(const char *path);
int32 OS_NameChange( char* name);


/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/

int32 OS_FS_Init(void)
{
    int i;
	
    /* Initialize the file system constructs */
    for (i =0; i < OS_MAX_NUM_OPEN_FILES; i++)
    {
        OS_FDTable[i].OSfd =       -1;
        strcpy(OS_FDTable[i].Path, "\0");
        OS_FDTable[i].User =       0;
        OS_FDTable[i].IsValid =    FALSE;
    }
    
    OS_FDTableMutex = semMCreate(SEM_Q_PRIORITY); 
    if ( OS_FDTableMutex == NULL )
	{
	   return(OS_ERROR);
	}
	else
	{
	   return(OS_SUCCESS);
	}

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
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars
             OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long
             OS_FS_ERROR if permissions are unknown or OS call fails
             OS_FS_ERR_NO_FREE_FDS if there are no free file descriptors
             a file descriptor if success
    
---------------------------------------------------------------------------------------*/

int32 OS_creat  (const char *path, int32  access)
{
    int    status;
    char   local_path[OS_MAX_LOCAL_PATH_LEN];
    int    perm;
    mode_t mode;
    uint32 PossibleFD;

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

    semTake(OS_FDTableMutex,WAIT_FOREVER);
    for ( PossibleFD = 0; PossibleFD < OS_MAX_NUM_OPEN_FILES; PossibleFD++)
    {
        if( OS_FDTable[PossibleFD].IsValid == FALSE)
        {
            break;
        }
    }

    if (PossibleFD >= OS_MAX_NUM_OPEN_FILES)
    {
        semGive(OS_FDTableMutex);
        return OS_FS_ERR_NO_FREE_FDS;
    }

    /* Mark the table entry as valid so no other 
     * task can take that ID */
    OS_FDTable[PossibleFD].IsValid =    TRUE; 
    
    semGive(OS_FDTableMutex);

    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    status = open(local_path, (int) perm | O_CREAT | O_TRUNC, mode);

    semTake(OS_FDTableMutex,WAIT_FOREVER);

    if (status != ERROR)
    {
        /* fill in the table before returning */

        OS_FDTable[PossibleFD].OSfd =       status;
        strncpy(OS_FDTable[PossibleFD].Path, path, OS_MAX_PATH_LEN);
        OS_FDTable[PossibleFD].User =       OS_FindCreator();
        semGive(OS_FDTableMutex);

        return PossibleFD;
    }
    else
    {   OS_FDTable[PossibleFD].IsValid = FALSE; 
        semGive(OS_FDTableMutex);
        return OS_FS_ERROR;
    }
} /* end OS_creat */

/*--------------------------------------------------------------------------------------
    Name: OS_open
    
    Purpose: Opens a file. access parameters are OS_READ_ONLY,OS_WRITE_ONLY, or 
             OS_READ_WRITE

    Returns: OS_FS_ERR_INVALID_POINTER if path is NULL
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERR_PATH_TOO_LONG if path exceeds the maximum number of chars
             OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long
             OS_FS_ERROR if permissions are unknown or OS call fails
             OS_FS_ERR_NO_FREE_FDS if there are no free file descriptors
             a file descriptor if success
---------------------------------------------------------------------------------------*/

int32 OS_open   (const char *path,  int32 access,  uint32  mode)
{
    int    status;
    char   local_path[OS_MAX_LOCAL_PATH_LEN];
    uint32 PossibleFD;
    int    perm;

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
    
    semTake(OS_FDTableMutex,WAIT_FOREVER);

    for ( PossibleFD = 0; PossibleFD < OS_MAX_NUM_OPEN_FILES; PossibleFD++)
    {
        if( OS_FDTable[PossibleFD].IsValid == FALSE)
        {
            break;
        }
    }

    if (PossibleFD >= OS_MAX_NUM_OPEN_FILES)
    {
        semGive(OS_FDTableMutex);
        return OS_FS_ERR_NO_FREE_FDS;
    }

    /* Mark the table entry as valid so no other 
     * task can take that ID */
    OS_FDTable[PossibleFD].IsValid =    TRUE; 
    
    semGive(OS_FDTableMutex);  

    /* Open the file */
    status = open(local_path, (int) perm, (int) mode);

    semTake(OS_FDTableMutex,WAIT_FOREVER);

    if (status != ERROR)
    {
        /* fill in the table before returning */

        OS_FDTable[PossibleFD].OSfd =       status;
        strncpy(OS_FDTable[PossibleFD].Path, path, OS_MAX_PATH_LEN);
        OS_FDTable[PossibleFD].User =       OS_FindCreator();
        semGive(OS_FDTableMutex);

        return PossibleFD;
    }
    else
    {   OS_FDTable[PossibleFD].IsValid = FALSE; 
        semGive(OS_FDTableMutex);
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
      int status;

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
            semTake(OS_FDTableMutex,WAIT_FOREVER);
            OS_FDTable[filedes].OSfd =       -1;
            strcpy(OS_FDTable[filedes].Path, "\0");
            OS_FDTable[filedes].User =       0;
            OS_FDTable[filedes].IsValid =    FALSE;
            semGive(OS_FDTableMutex);

            return OS_FS_ERROR;
        }
        else
        {
            /* fill in the table before returning */
            semTake(OS_FDTableMutex,WAIT_FOREVER);
            OS_FDTable[filedes].OSfd =       -1;
            strcpy(OS_FDTable[filedes].Path, "\0");
            OS_FDTable[filedes].User =       0;
            OS_FDTable[filedes].IsValid =    FALSE;
            semGive(OS_FDTableMutex);

            return OS_FS_SUCCESS;
        }

    }    
}/* end OS_close */

/*--------------------------------------------------------------------------------------
    Name: OS_read
    
    Purpose: reads up to nbytes from a file, and puts them into buffer. 
    
    Returns: OS_FS_ERR_INVALID_POINTER if buffer is a null pointer
             OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             OS_FS_ERROR if OS call failed
             number of bytes read if success
---------------------------------------------------------------------------------------*/
int32 OS_read  (int32  filedes, void *buffer, uint32 nbytes)
{
    int32 status;

    if (buffer == NULL)
        return OS_FS_ERR_INVALID_POINTER;

    /* Make sure the file descriptor is legit before using it */
    if (filedes < 0 || filedes >= OS_MAX_NUM_OPEN_FILES || OS_FDTable[filedes].IsValid == FALSE)
    {
        return OS_FS_ERR_INVALID_FD;
    }
    else
    { 
        status = read (OS_FDTable[filedes].OSfd, (char*) buffer, (size_t) nbytes);
 
        if (status == ERROR)
            return OS_FS_ERROR;
    }

    return status;
        
}/* end OS_read */

/*--------------------------------------------------------------------------------------
    Name: OS_write

    Purpose: writes to a file. copies up to a maximum of nbtyes of buffer to the file
             described in filedes

    Returns: OS_FS_ERR_INVALID_POINTER if buffer is NULL
             OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
             OS_FS_ERROR if OS call failed
             number of bytes written if success
---------------------------------------------------------------------------------------*/

int32 OS_write (int32  filedes, void *buffer, uint32 nbytes)
{
    int32 status;

    if (buffer == NULL)
        return OS_FS_ERR_INVALID_POINTER;

    /* Make sure the file descriptor is legit before using it */
    if (filedes < 0 || filedes >= OS_MAX_NUM_OPEN_FILES || OS_FDTable[filedes].IsValid == FALSE)
    {
        return OS_FS_ERR_INVALID_FD;
    }
    else
    {
        status = write(OS_FDTable[filedes].OSfd, buffer, nbytes );
    
        if (status != ERROR)
            return  status;
        else
            return OS_FS_ERROR;
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
             OS_FS_ERR_NAME_TOO_LONG if the name of the file is too long to be stored
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
        return OS_FS_ERR_INVALID_POINTER;
        
    /*
    ** Check to see if the path is too long
    */
    if (strlen(path) >= OS_MAX_PATH_LEN)
        return OS_FS_ERR_PATH_TOO_LONG;

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
     int status;
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

    
        status = lseek( OS_FDTable[filedes].OSfd, (int) offset, (int) where );

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
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_ERR_PATH_TOO_LONG if path is too long to be stored locally
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

    status = remove (local_path);
    if (status == OK)
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
        return OS_FS_ERR_INVALID_POINTER;

    /*
    ** Check to see if the paths are too long
    */
    if (strlen(old_filename) >= OS_MAX_PATH_LEN)
        return OS_FS_ERR_PATH_TOO_LONG;
    
    if (strlen(new_filename) >= OS_MAX_PATH_LEN)
        return OS_FS_ERR_PATH_TOO_LONG;

    /* 
    ** check if the names of the files are too long 
    */
    if (OS_check_name_length(old_filename) != OS_FS_SUCCESS)
        return OS_FS_ERR_NAME_TOO_LONG;

    if (OS_check_name_length(new_filename) != OS_FS_SUCCESS)
        return OS_FS_ERR_NAME_TOO_LONG;

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
    
    if (status != ERROR)
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
    int  status;
    char src_path[OS_MAX_LOCAL_PATH_LEN];
    char dest_path[OS_MAX_LOCAL_PATH_LEN];

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

    status = cp(src_path, dest_path);
     
    if (status != ERROR)
    {
       return OS_FS_SUCCESS;
    }
    else
    {
       return OS_FS_ERROR;
    }
     
}/*end OS_cp */

/*--------------------------------------------------------------------------------------
    Name: OS_mv
    
    Purpose: moves a single file from src to dest

    Returns: OS_FS_SUCCESS if the operation worked
             OS_FS_ERROR if the file could not be accessed
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
 
}/*end OS_mv */

/*
** Directory API 
*/
/*--------------------------------------------------------------------------------------
    Name: OS_mkdir

    Purpose: makes a directory specified by path.

    Returns: OS_FS_ERR_INVALID_POINTER if path is NULL
             OS_FS_ERR_PATH_TOO_LONG if the path is too long to be stored locally
             OS_FS_ERROR if the OS call fails
             OS_FS_ERR_PATH_INVALID if path cannot be parsed
             OS_FS_SUCCESS if success

    Note: The access parameter is currently unused.
---------------------------------------------------------------------------------------*/
int32 OS_mkdir (const char *path, uint32 access)
{
   STATUS status;
      char local_path[OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check to see if the path pointer is NULL
    */    
    if (path == NULL)
        return OS_FS_ERR_INVALID_POINTER;

    /*
    ** Check to see if the path is too long
    */      
    if (strlen(path) >= OS_MAX_PATH_LEN)
        return OS_FS_ERR_PATH_TOO_LONG;

    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(path, (char *)local_path) != OS_FS_SUCCESS )
    {
        return OS_FS_ERR_PATH_INVALID;
    }

    status = mkdir(local_path);

    if (status == OK)
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

    Returns: NULL if path is NULL,path is too long, OS call fails
             a pointer to a directory if success
---------------------------------------------------------------------------------------*/

os_dirp_t OS_opendir (const char *path)
{
    os_dirp_t dirdescptr;
    char local_path[OS_MAX_LOCAL_PATH_LEN];

    /*
    ** Check to see if the path pointer is NULL
    */        
    if (path == NULL)
        return NULL;

    /*
    ** Check to see if the path is too long
    */      
    if (strlen(path) > OS_MAX_PATH_LEN)
        return NULL;
   
    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(path, (char *)local_path) != OS_FS_SUCCESS )
    {
        return NULL;
    }

    dirdescptr = opendir( (char*) local_path);
    
    /* explicitly returns null for clarity */
    if (dirdescptr == NULL)
    {
        return NULL;
    }
    else
    {
        return dirdescptr;
    }
    
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
        return OS_FS_ERR_INVALID_POINTER;

    status = closedir(directory);
    
    if (status == OK)
        return OS_FS_SUCCESS;
    else
        return OS_FS_ERROR;

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

    errno = OK;
    
    if (directory == NULL)
        return NULL;

    tempptr = readdir( directory);
    
    if (tempptr != NULL)
    {
        return tempptr;
    }
    else
    {
        return NULL;
    }

    /* should never reach this point in the code */
    return NULL;
    
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

    Returns: OS_FS_ERR_INVALID_POINTER if path us NULL
             OS_FS_ERR_PATH_TOO_LONG
             OS_FS_ERR_PATH_INVALID if path cannot be parsed             
---------------------------------------------------------------------------------------*/

int32  OS_rmdir (const char *path)
{
    STATUS status;
    char local_path [OS_MAX_LOCAL_PATH_LEN];
 
   /*
    ** Check to see if the path pointer is NULL
    */            
    if (path == NULL)
        return OS_FS_ERR_INVALID_POINTER;

    /*
    ** Check to see if the path is too long
    */      
    if (strlen(path) >= OS_MAX_PATH_LEN)
        return OS_FS_ERR_PATH_TOO_LONG;

    /*
    ** Translate the path
    */
    if ( OS_TranslatePath(path, (char *)local_path) != OS_FS_SUCCESS )
    {
        return OS_FS_ERR_PATH_INVALID;
    }
    
    status = rmdir(local_path);
    
    if (status == OK)
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
    char *name_ptr;
    char *end_of_path;
    int   name_len;

    if (path == NULL)
        return OS_FS_ERROR;

    if (strlen(path) > OS_MAX_PATH_LEN)
        return OS_FS_ERROR;
    
    /* checks to see if there is a '/' somewhere in the path */
    name_ptr = strrchr(path, '/');
    
    if (name_ptr == NULL)
        return OS_FS_ERROR;
    
    /* strrchr returns a pointer to the last '/' char, so we advance one char */
    name_ptr = name_ptr + sizeof(char);
    
    /* end_of_path points to the null terminator at the end of the path */
    end_of_path = strrchr(path,'\0');

    /* pointer subraction to see how many characters there are in the name */
    name_len = ((int) end_of_path - (int)name_ptr) / sizeof(char);
    
    if( name_len > OS_MAX_FILE_NAME)
        return OS_FS_ERROR;

    return OS_FS_SUCCESS;
    
}/* end OS_check_name_length */

/* --------------------------------------------------------------------------------------
Name: OS_ShellOutputToFile
    
Purpose: Takes a shell command in and writes the output of that command to the specified file
    
Returns: OS_SUCCESS if success
         OS_FS_ERR_INVALID_FD if the file descriptor passed in is invalid
         OS_FS_ERROR if Error 
---------------------------------------------------------------------------------------*/
int32 OS_ShellOutputToFile(char* Cmd, int32 OS_fd)
{
    char LocalCmd [OS_MAX_CMD_LEN];
    int32 Result;
    int32 ReturnCode = OS_FS_SUCCESS;
    int32 fdCmd;
    char * shellName;

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
        /* Create a file to write the command to (or write over the old one) */
        fdCmd = OS_creat(OS_SHELL_CMD_INPUT_FILE_NAME,OS_READ_WRITE);

        if (fdCmd < OS_FS_SUCCESS)
        {
            Result = OS_FS_ERROR;
        }

        else
        {
            /* copy the command to the file, and then seek back to the beginning of the file */
        
            strncpy(LocalCmd,Cmd, OS_MAX_CMD_LEN);
            OS_write(fdCmd,Cmd, strlen(LocalCmd));
            OS_lseek(fdCmd,0,OS_SEEK_SET);	

            /* Create a shell task the will run the command in the file, push output to OS_fd */
            Result = shellGenericInit("INTERPRETER=Cmd",0,NULL, &shellName, FALSE, FALSE, OS_FDTable[fdCmd].OSfd,	OS_FDTable[OS_fd].OSfd, OS_FDTable[OS_fd].OSfd);

            /* Wait for the command to terminate */
           do{
              taskDelay(sysClkRateGet());
            }while (taskNameToId(shellName) != ERROR);
    
            /* Close the file descriptor */	
            OS_close(fdCmd);

        } /* else */
        
        if (Result != OK)
        {
             ReturnCode =  OS_FS_ERROR;
        }
        
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
    uint32   i;

    if (Filename == NULL)
    {
        return(OS_FS_ERR_INVALID_POINTER);
    }

    semTake(OS_FDTableMutex,WAIT_FOREVER);

    for ( i = 0; i < OS_MAX_NUM_OPEN_FILES; i++)
    {
        if ((OS_FDTable[i].IsValid == TRUE) &&  (strcmp(OS_FDTable[i].Path, Filename) == 0))
        {
           semGive(OS_FDTableMutex);
           return(OS_FS_SUCCESS);
        }
    }/* end for */

    semGive(OS_FDTableMutex);
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
    uint32            i;
    int               status;

    if (Filename == NULL)
    {
        return(OS_FS_ERR_INVALID_POINTER);
    }

    semTake(OS_FDTableMutex,WAIT_FOREVER);

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
           OS_FDTable[i].OSfd =       -1;
           strcpy(OS_FDTable[i].Path, "\0");
           OS_FDTable[i].User =       0;
           OS_FDTable[i].IsValid =    FALSE;
           semGive(OS_FDTableMutex);

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

    semGive(OS_FDTableMutex);
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
    uint32            i;
    int32             return_status = OS_FS_SUCCESS;
    int               status;
    
    semTake(OS_FDTableMutex,WAIT_FOREVER);

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
           OS_FDTable[i].OSfd =       -1;
           strcpy(OS_FDTable[i].Path, "\0");
           OS_FDTable[i].User =       0;
           OS_FDTable[i].IsValid =    FALSE;
           if (status == ERROR)
           {
              return_status = OS_FS_ERROR;
           }
        }

    }/* end for */

    semGive(OS_FDTableMutex);
    return (return_status);

}/* end OS_CloseAllFiles */

