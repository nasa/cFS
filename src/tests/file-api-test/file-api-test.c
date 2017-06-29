#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* OS Constructs */
void TestMkfsMount(void);
void TestCreatRemove(void);
void TestOpenClose(void);
void TestReadWriteLseek(void);
void TestMkRmDirFreeBytes(void);
void TestOpenReadCloseDir(void);
void TestRename(void);
void TestStat(void);
void TestOpenFileAPI(void);
void TestUnmountRemount(void);

os_fs_err_name_t errname;

/* *************************************** MAIN ************************************** */

void OS_Application_Startup(void)
{
    errname[0] = 0;

    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /*
     * Register the test setup and check routines in UT assert
     *
     * It is done this way so that the output is logically grouped,
     * otherwise the entire thing would be lumped together
     * as a single test case.
     */
    UtTest_Add(TestMkfsMount, NULL, NULL, "TestMkfsMount");
    UtTest_Add(TestCreatRemove, NULL, NULL, "TestCreatRemove");
    UtTest_Add(TestOpenClose, NULL, NULL, "TestOpenClose");
    UtTest_Add(TestReadWriteLseek, NULL, NULL, "TestReadWriteLseek");
    UtTest_Add(TestMkRmDirFreeBytes, NULL, NULL, "TestMkRmDirFreeBytes");
    UtTest_Add(TestOpenReadCloseDir, NULL, NULL, "TestOpenReadCloseDir");
    UtTest_Add(TestStat, NULL, NULL, "TestStat");
    UtTest_Add(TestOpenFileAPI, NULL, NULL, "TestOpenFileAPI");
    UtTest_Add(TestUnmountRemount, NULL, NULL, "TestUnmountRemount");
    UtTest_Add(TestRename, NULL, NULL, "TestRename");
}

void TestMkfsMount(void)
{
    int status;

    /* Make the file system */
    status = OS_mkfs(0,"/ramdev0","RAM",512,200);
    UtAssert_True(status == OS_SUCCESS, "status after mkfs = %d",(int)status);

    status = OS_mount("/ramdev0","/drive0");
    UtAssert_True(status == OS_SUCCESS, "status after mount = %d",(int)status);

#if 0
    status = OS_mkfs(0,"/eedev1","RAM",512,200);
    UtAssert_True(status == OS_SUCCESS, "status after mkfs = %d",(int)status);
#endif
}

void TestUnmountRemount(void)
{
    int status;

    /*
    ** try unmounting the drive, and then remounting it with a different name 
    */
    status = OS_unmount("/drive0");
    UtAssert_True(status >= OS_SUCCESS, "status after unmount = %d",(int)status);

    status = OS_mount("/ramdev0","/drive1");
    UtAssert_True(status == OS_SUCCESS, "status after reunmount = %d",(int)status);
}

/*---------------------------------------------------------------------------------------
 * Name Test CreatRemove()
 * This function tests the basic create / remove capabilities of the filesystem. It tries
 *  to create files of reasonable length, OS_MAX_FILE_NAME length, and OS_MAX_FILE_NAME+1
 *  length.
 *
 *  The function then tries to remove the files created above, as well as remove a file
 *  that was never on the drive to begin with.
---------------------------------------------------------------------------------------*/

void TestCreatRemove(void)
{
    char filename [OS_MAX_PATH_LEN];
    char maxfilename[OS_MAX_PATH_LEN];
    char longfilename [OS_MAX_PATH_LEN + 10];
    int  status;
 
    strncpy(filename,"/drive0/test11chars", sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = 0;

    /* make the two really long filenames */
    
    /* 1 '/' and 40 'm' */
    strncpy(maxfilename,"/drive0/mmmmmmmmmmmmmmmmmmmm", sizeof(maxfilename) - 1);
    maxfilename[sizeof(maxfilename) - 1] = 0;

    /* 1 '/' and 41 'l' */
    strncpy(longfilename,"/drive0/lllllllllllllllllllllllllllllllllllllllll", sizeof(longfilename) - 1);
    longfilename[sizeof(longfilename) - 1] = 0;
    
    /* create a file of reasonable length (but over 8 chars) */
    status = OS_creat(filename,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat = %d",(int)status);

    /* close the first file */
    status = OS_close(status);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d",(int)status);

    /* create a file of OS_max_file_name size */
    status = OS_creat(maxfilename,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat = %d",(int)status);

    /* close the second file */
    status = OS_close(status);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d",(int)status);

    /* try removing the file from the drive */
    status = OS_remove(filename);
    UtAssert_True(status == OS_SUCCESS, "status after remove = %d",(int)status);
    
    /* try removing the file from the drive */
    status = OS_remove(maxfilename);
    UtAssert_True(status == OS_SUCCESS, "status after remove = %d",(int)status);

    /* try removing the file from the drive. Should Fail */
    status = OS_remove(longfilename);
    UtAssert_True(status < OS_SUCCESS, "status after remove = %d",(int)status);

    /* try removing the file from the drive. Should Fail */
    status = OS_remove("/drive0/FileNotOnDrive");
    UtAssert_True(status < OS_SUCCESS, "status after remove = %d",(int)status);

    /* Similar to previous but with a bad mount point (gives different error code) */
    status = OS_remove("/FileNotOnDrive");
    UtAssert_True(status == OS_FS_ERR_PATH_INVALID, "status after remove = %d",(int)status);
}

/*---------------------------------------------------------------------------------------
 *  Name: TestOpenClose
 * This functions tests the basic functionality of OS_open and OS_close.
---------------------------------------------------------------------------------------*/
void TestOpenClose(void)
{
    char filename [OS_MAX_PATH_LEN];
    int status;
    int fd;

    strncpy(filename,"/drive0/Filename1", sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = 0;

    /* create a file of reasonable length (but over 8 chars) */
    status = OS_creat(filename,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat = %d",(int)status);
        
    fd = status;
   
    /*
    ** try to close the file
    */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d",(int)status);

    /*  reopen the file */
    status = OS_open(filename,OS_READ_WRITE,0644);
    UtAssert_True(status >= OS_SUCCESS, "status after reopen = %d",(int)status);

    fd = status;
   
    /*
    ** try to close the file again
    */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d",(int)status);
   
    /*
    ** try to close the file again. Should Fail
    */
    status = OS_close(fd);
    UtAssert_True(status != OS_SUCCESS, "status after close = %d",(int)status);
    
    /*try to close a file not on the system. Should Fail */
    status = OS_close(43);
    UtAssert_True(status != OS_SUCCESS, "status after close = %d",(int)status);

    /*  open a file that was never in the system */
    status = OS_open("/drive0/FileNotHere",OS_READ_ONLY,0644);
    UtAssert_True(status < OS_SUCCESS, "status after open = %d",(int)status);

    /* try removing the file from the drive  to end the function */
    status = OS_remove(filename);
    UtAssert_True(status == OS_SUCCESS, "status after remove = %d",(int)status);
}
/*---------------------------------------------------------------------------------------
 * Name TestReadWriteLseek
---------------------------------------------------------------------------------------*/

void TestReadWriteLseek(void)
{
    char filename [OS_MAX_PATH_LEN];
    char buffer [30];
    char copyofbuffer[30];
    char seekbuffer[30];
    char newbuffer[30];
    int offset;
    int size;
    int status;
    int fd;

    strncpy(filename,"/drive0/Filename1", sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = 0;

    strcpy(buffer,"ValueToWriteInTheFile");
    strcpy(copyofbuffer,buffer); /* hold a copy of the buffer */
    offset = 12;

    /* start copying offset chars in buffer into seekbuffer */
    strcpy(seekbuffer, &buffer[offset]);
    
    /* create a file of reasonable length (but over 8 chars) */
    
    /* Open In R/W mode */
    status = OS_creat(filename,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat = %d",(int)status);
    
    fd = status;
    size = strlen(buffer);
    
    /* test write portion of R/W mode */
    status = OS_write(fd, (void*)buffer, size);
    UtAssert_True(status == size, "status after write = %d size = %d",(int)status, (int)size);
    
    strcpy(buffer,"");

    /* lseek back to the beginning of the file */
    status = OS_lseek(fd,0, 0);
    UtAssert_True(status >= OS_SUCCESS, "status after lseek = %d",(int)status);
    
    /*Read what we wrote to the file */
    status = OS_read(fd,(void*)buffer, size);
    UtAssert_True(status == size, "status after read = %d size = %d",(int)status,(int)size);
    if (status >= OS_SUCCESS)
    {
        UtAssert_True(strcmp(buffer,copyofbuffer) == 0,
                "Read: %s, Written: %s",buffer,copyofbuffer);
    }

    /* close the file */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d",(int)status);

    /*  open a file again, but only in READ mode */
    status = OS_open(filename,OS_READ_ONLY,0644);
    UtAssert_True(status >= OS_SUCCESS, "status after reopen = %d",(int)status);

    fd = status;
    /* test write in READ ONLY mode */
    status = OS_write(fd, (void*)buffer, size);
    UtAssert_True(status < OS_SUCCESS, "status after write = %d",(int)status);

    /* try to read in READ ONLY MODE */
    
    status = OS_read(fd,(void*)buffer, size);
    UtAssert_True(status == size, "status after read = %d size = %d",(int)status,(int)size);
    if (status >= OS_SUCCESS)
    {
        UtAssert_True(strcmp(buffer,copyofbuffer) == 0,
                "Read: %s, Written: %s",buffer,copyofbuffer);
    }

    /* seek to a spot in the middle of the file to test lseek */
    status = OS_lseek(fd,offset,0);
    UtAssert_True(status >= OS_SUCCESS, "status after lseek = %d",(int)status);

    /* now try to read out only the last chars of the file */
    
    status = OS_read(fd,(void*)newbuffer, (size - offset));
    UtAssert_True(status == (size - offset), "status after read = %d size = %d",(int)status,(int)(size - offset));
    if (status >= OS_SUCCESS)
    {
        UtAssert_True(strncmp(newbuffer,seekbuffer, size - offset) == 0,
                "Read: %s, Written: %s",newbuffer,seekbuffer);
    }

    /* close the file */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d",(int)status);

    /*  open a file again, but only in WRITE mode */
    status = OS_open(filename,OS_WRITE_ONLY,0644);
    UtAssert_True(status >= OS_SUCCESS, "status after reopen = %d",(int)status);

    fd = status;

    /* test write in WRITE ONLY mode */
    status = OS_write(fd, (void*)buffer, size);
    UtAssert_True(status == size, "status after write = %d size = %d",(int)status, (int)size);

    /* try to read in WRITE ONLY MODE */
    status = OS_read(fd,(void*)buffer, size);
    UtAssert_True(status < OS_SUCCESS, "status after read = %d",(int)status);

    /* seek to a spot in the middle of the file to test lseek */
    status = OS_lseek(fd,offset,0);
    UtAssert_True(status >= OS_SUCCESS, "status after lseek = %d",(int)status);

    /* now try to read out only the last chars of the file */
    status = OS_read(fd,(void*)newbuffer, (size - offset));
    UtAssert_True(status < OS_SUCCESS, "status after read = %d",(int)status);

    /* close the file */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d",(int)status);

    /* try removing the file from the drive */
    status = OS_remove(filename);
    UtAssert_True(status == OS_SUCCESS, "status after remove = %d",(int)status);
}

/*---------------------------------------------------------------------------------------
 *  Name TestMkRmDir()
---------------------------------------------------------------------------------------*/
void TestMkRmDirFreeBytes(void)
{
    int status;
    char filename1[OS_MAX_PATH_LEN];
    char filename2[OS_MAX_PATH_LEN];
    char dir1 [OS_MAX_PATH_LEN];
    char dir2 [OS_MAX_PATH_LEN];
    char buffer1 [OS_MAX_PATH_LEN];
    char buffer2 [OS_MAX_PATH_LEN];
    char copybuffer1 [OS_MAX_PATH_LEN];
    char copybuffer2 [OS_MAX_PATH_LEN];
    int fd1;
    int fd2;
    int size;

    /* make the directory names for testing, as well as the filenames and the buffers
     * to put in the files */
    strcpy(dir1,"/drive0/DIRECTORY_ONE");
    strcpy(dir2,"/drive0/directory_two");
    strcpy(filename1,"/drive0/DIRECTORY_ONE/MyFile1");
    strcpy(filename2,"/drive0/directory_two/MyFile2");
    strcpy(buffer1,"111111111111");
    strcpy(copybuffer1,buffer1);
    strcpy(buffer2,"222222222222");
    strcpy(copybuffer2,buffer2);
   
    status = OS_fsBlocksFree("/drive0");
    UtAssert_True(status >= OS_SUCCESS, "Checking Free Blocks: %d",(int)status);

    /* make the two directories */
    status = OS_mkdir(dir1,0);
    UtAssert_True(status == OS_SUCCESS, "status after mkdir 1 = %d",(int)status);
           
    status = OS_mkdir(dir2,0);
    UtAssert_True(status == OS_SUCCESS, "status after mkdir 2 = %d",(int)status);
    
    /* now create two files in the two directories (1 file per directory) */

    status = OS_creat(filename1,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 1 = %d",(int)status);
    
    fd1 = status;

    status = OS_creat(filename2,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 2 = %d",(int)status);
    
    fd2 = status;

    /* write the propper buffers into each of the files */
    size = strlen(buffer1);
    status = OS_write(fd1, buffer1, size);
    UtAssert_True(status == size, "status after write 1 = %d size = %d",(int)status, (int)size);
    
    size = strlen(buffer2);
    status = OS_write(fd2, buffer2, size);
    UtAssert_True(status == size, "status after write 2 = %d size = %d",(int)status, (int)size);

    /* lseek back to the beginning of the file */
    status = OS_lseek(fd1,0, 0);
    UtAssert_True(status >= OS_SUCCESS, "status after lseek 1 = %d",(int)status);
    
    status = OS_lseek(fd2,0, 0);
    UtAssert_True(status >= OS_SUCCESS, "status after lseek 2 = %d",(int)status);

    memset(buffer1, 0, sizeof(buffer1));
    memset(buffer2, 0, sizeof(buffer2));
    status = OS_fsBlocksFree("/drive0");
    UtAssert_True(status >= OS_SUCCESS, "Checking Free Blocks: %d",(int)status);

    /* read back out of the files what we wrote into them */
    size = strlen(copybuffer1);
    status = OS_read(fd1,(void*)buffer1, size);
    UtAssert_True(status == size, "status after read 1 = %d size = %d",(int)status, (int)size);
    if (status >= OS_SUCCESS)
    {
        UtAssert_True(strncmp(buffer1,copybuffer1,size) == 0,
                "Read: %s, Written: %s",buffer1,copybuffer1);
    }

    size = strlen(copybuffer2);
    status = OS_read(fd2,(void*)buffer2, size);
    UtAssert_True(status == size, "status after read 2 = %d size = %d",(int)status, (int)size);
    if (status >= OS_SUCCESS)
    {
        UtAssert_True(strncmp(buffer2,copybuffer2,size) == 0,
                "Read: %s, Written: %s",buffer1,copybuffer1);
    }
    
    /* close the files */
    status = OS_close(fd1);
    UtAssert_True(status == OS_SUCCESS, "status after close 1 = %d",(int)status);

    status = OS_close(fd2);
    UtAssert_True(status == OS_SUCCESS, "status after close 2 = %d",(int)status);

    /* try removing the file from the drive */
    status = OS_remove(filename1);
    UtAssert_True(status == OS_SUCCESS, "status after remove 1 = %d",(int)status);

    status = OS_remove(filename2);
    UtAssert_True(status == OS_SUCCESS, "status after remove 2 = %d",(int)status);

    status = OS_rmdir(dir1);
    UtAssert_True(status == OS_SUCCESS, "status after rmdir 1 = %d",(int)status);

    status = OS_rmdir(dir2);
    UtAssert_True(status == OS_SUCCESS, "status after rmdir 2 = %d",(int)status);

    status = OS_fsBlocksFree("/drive0");
    UtAssert_True(status >= OS_SUCCESS, "Checking Free Blocks: %d",(int)status);
}
/*---------------------------------------------------------------------------------------
 * Name TestOpenReadCloseDir();
---------------------------------------------------------------------------------------*/
void TestOpenReadCloseDir(void)
{
    int status;
    char filename1[OS_MAX_PATH_LEN];
    char filename2[OS_MAX_PATH_LEN];
    char dir0 [OS_MAX_PATH_LEN];
    char dir1 [OS_MAX_PATH_LEN];
    char dir2 [OS_MAX_PATH_LEN];
    char buffer1 [OS_MAX_PATH_LEN];
    char buffer2 [OS_MAX_PATH_LEN];
    int size;
    int fd1;
    int fd2;
    os_dirp_t     dirp0;
    os_dirp_t     dirp1;
    os_dirp_t     dirp2;
    os_dirent_t   *dirent_ptr0;
    os_dirent_t   *dirent_ptr1;
    os_dirent_t   *dirent_ptr2;
    
    /* make the directory names for testing, as well as the filenames and the buffers
     * to put in the files */
    strcpy(dir0,"/drive0");
    strcpy(dir1,"/drive0/DIRECTORY_ONE");
    strcpy(dir2,"/drive0/directory_two");
    strcpy(filename1,"/drive0/DIRECTORY_ONE/MyFile1");
    strcpy(filename2,"/drive0/directory_two/MyFile2");
    strcpy(buffer1,"111111111111");
    strcpy(buffer2,"222222222222");

    /* make the two directories */
    
    status = OS_mkdir(dir1,0);
    UtAssert_True(status == OS_SUCCESS, "status after mkdir 1 = %d",(int)status);
           
    status = OS_mkdir(dir2,0);
    UtAssert_True(status == OS_SUCCESS, "status after mkdir 2 = %d",(int)status);
    
    /* now create two files in the two directories (1 file per directory) */

    status = OS_creat(filename1,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 1 = %d",(int)status);
    
    fd1 = status;

    status = OS_creat(filename2,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 2 = %d",(int)status);
    
    fd2 = status;

    /* write the propper buffers into each of the files */
    
    size = strlen(buffer1);
    status = OS_write(fd1, buffer1, size);
    UtAssert_True(status == size, "status after write 1 = %d size = %d",(int)status, (int)size);
    
    size = strlen(buffer2);
    status = OS_write(fd2, buffer2, size);
    UtAssert_True(status == size, "status after write 2 = %d size = %d",(int)status, (int)size);

    /* close the files */

    status = OS_close(fd1);
    UtAssert_True(status == OS_SUCCESS, "status after close 1 = %d",(int)status);

    status = OS_close(fd2);
    UtAssert_True(status == OS_SUCCESS, "status after close 2 = %d",(int)status);

    /* Now both of the directories are there and have files in them. Also both files
     have stuff written in them */
    
    
    /* try to open the base directory "/" */

    dirp0 = OS_opendir(dir0);
    UtAssert_True(dirp0 != NULL, "OS_opendir not null");

    /* try to read the two folders that are in the "/" */
    /* Have to make it a loop to see if we can find the directories in question */

    dirent_ptr0 = OS_readdir (dirp0);
    while (dirent_ptr0 != NULL)
    {
        if ( strncmp(dirent_ptr0 -> d_name,"DIRECTORY_ONE",strlen("DIRECTORY_ONE")) == 0)
        {
            break;
        }
        else
            dirent_ptr0 = OS_readdir(dirp0);
    }

    UtAssert_True(dirent_ptr0 != NULL, "DIRECTORY_ONE found");

    status = OS_closedir(dirp0);
    UtAssert_True(status >= OS_SUCCESS, "OS_closedir Rc=%d",(int)status);

    dirp0 = OS_opendir(dir0);
    UtAssert_True(dirp0 != NULL, "OS_opendir not null");

    dirent_ptr0 = OS_readdir (dirp0);
    while (dirent_ptr0 != NULL)
    {
        if ( strncmp(dirent_ptr0 -> d_name,"directory_two",strlen("directory_two")) == 0)
        {
            break;
        }
        else
            dirent_ptr0 = OS_readdir(dirp0);
    }

    UtAssert_True(dirent_ptr0 != NULL, "directory_two found");

    status = OS_closedir(dirp0);
    UtAssert_True(status >= OS_SUCCESS, "OS_closedir Rc=%d",(int)status);

    
    /* try and read a nonexisitant 3rd directory */ 
        
    dirp0 = OS_opendir(dir0);
    UtAssert_True(dirp0 != NULL, "OS_opendir not null");

    dirent_ptr0 = OS_readdir (dirp0);
    UtAssert_True(dirent_ptr0 != NULL, "OS_readdir not null");
    while (dirent_ptr0 != NULL)
    {
        dirent_ptr0 = OS_readdir(dirp0);
    }

    status = OS_closedir(dirp0);
    UtAssert_True(status >= OS_SUCCESS, "OS_closedir Rc=%d",(int)status);

    /* Now test the open/ read close for one of the sub directories */
    
    dirp1 = OS_opendir(dir1);
    UtAssert_True(dirp1 != NULL, "OS_opendir not null");

    /* try to read the next file within the first directory "MyFile1" */
    dirent_ptr1 = OS_readdir (dirp1);
    while (dirent_ptr1 != NULL)
    {
        if ( strncmp(dirent_ptr1 -> d_name,"MyFile1",strlen("MyFile1")) == 0)
        {
            break;
        }
        else
            dirent_ptr1 = OS_readdir(dirp1);
    }

    UtAssert_True(dirent_ptr1 != NULL, "MyFile1 found");

    /* Close the file */

    status = OS_closedir(dirp1);
    UtAssert_True(status >= OS_SUCCESS, "OS_closedir Rc=%d",(int)status);

    dirp2 = OS_opendir(dir2);
    UtAssert_True(dirp2 != NULL, "OS_opendir not null");

    /* try to read the next file within the first directory "MyFile2" */

    dirent_ptr2 = OS_readdir (dirp2);
    while (dirent_ptr2 != NULL)
    {
        if ( strncmp(dirent_ptr2 -> d_name,"MyFile2",strlen("MyFile2")) == 0)
        {
            break;
        }
        else
            dirent_ptr2 = OS_readdir(dirp2);
    }

    UtAssert_True(dirent_ptr2 != NULL, "MyFile2 found");

    /*Close the file */

    status = OS_closedir(dirp2);
    UtAssert_True(status >= OS_SUCCESS, "OS_closedir Rc=%d",(int)status);

/* remove the files */
    status = OS_remove(filename1);
    UtAssert_True(status == OS_SUCCESS, "status after remove 1 = %d",(int)status);

    status = OS_remove(filename2);
    UtAssert_True(status == OS_SUCCESS, "status after remove 2 = %d",(int)status);

/* remove the directories */
    status = OS_rmdir(dir1);
    UtAssert_True(status == OS_SUCCESS, "status after rmdir 1 = %d",(int)status);

    status = OS_rmdir(dir2);
    UtAssert_True(status == OS_SUCCESS, "status after rmdir 2 = %d",(int)status);
}

/*---------------------------------------------------------------------------------------
 * Name TestRename
---------------------------------------------------------------------------------------*/
void TestRename(void)
{
    int status;
    char filename1[OS_MAX_PATH_LEN];
    char dir0 [OS_MAX_PATH_LEN];
    char dir1 [OS_MAX_PATH_LEN];

    char buffer1 [OS_MAX_PATH_LEN];
    char copybuffer1 [OS_MAX_PATH_LEN];

    char newdir1 [OS_MAX_PATH_LEN];
    char midname1 [OS_MAX_PATH_LEN];
    char newfilename1 [OS_MAX_PATH_LEN];
    
    int fd1;
    int size;
    
    /* make the directory names for testing, as well as the filenames and the buffers
     * to put in the files */
    strcpy(dir0,"/drive1/");
    strcpy(dir1,"/drive1/DIRECTORY_ONE");
    strcpy(filename1,"/drive1/DIRECTORY_ONE/MyFile1");
    strcpy(buffer1,"111111111111");
    strcpy(copybuffer1,buffer1);

    strcpy(newdir1,"/drive1/NEW_DIR_ONE");
    strcpy(newfilename1,"/drive1/NEW_DIR_ONE/NEW_myfile1");
    
    strcpy(midname1,"/drive1/NEW_DIR_ONE/MyFile1");
    
    /* make the directory */
    
    status = OS_mkdir(dir1,0);
    UtAssert_True(status == OS_SUCCESS, "status after mkdir 1 = %d",(int)status);
               
    /* now create a file in the directory */

    status = OS_creat(filename1,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 1 = %d",(int)status);
    
    fd1 = status;

    /* write the propper buffes into  the file */
    
    size = strlen(buffer1);
    status = OS_write(fd1, buffer1, size);
    UtAssert_True(status == size, "status after write 1 = %d size = %d",(int)status, (int)size);

    /* close the file */

    status = OS_close(fd1);
    UtAssert_True(status == OS_SUCCESS, "status after close 1 = %d",(int)status);

/* now there is 1 subdirectory with 1 file in it */

    /* Rename the directory */

    status = OS_rename(dir1,newdir1);
    UtAssert_True(status == OS_SUCCESS, "status after rename 1 = %d",(int)status);

    status = OS_rename(midname1,newfilename1);
    UtAssert_True(status == OS_SUCCESS, "status after rename 2 = %d",(int)status);

    memset(buffer1, 0, sizeof(buffer1));

    /* try to read the new file out */

    fd1 = OS_open(newfilename1,OS_READ_ONLY,0644);
    UtAssert_True(status >= OS_SUCCESS, "status after open 1 = %d",(int)status);
    
    size  = strlen(copybuffer1);
    status = OS_read(fd1,buffer1,size);
    UtAssert_True(status == size, "status after read 1 = %d size = %d",(int)status, (int)size);
    if (status >= OS_FS_SUCCESS)
    {
        UtAssert_True(strncmp(buffer1,copybuffer1, size) == 0, "Read and Written Results are equal");
    }
    
    status = OS_close(fd1);
    UtAssert_True(status == OS_SUCCESS, "status after close 1 = %d",(int)status);

    /* try removing the file from the drive */
    /* note - if this is not done, subsequent runs of this test will fail */
    status = OS_remove(newfilename1);
    UtAssert_True(status == OS_SUCCESS, "status after remove 1 = %d",(int)status);

    status = OS_rmdir(newdir1);
    UtAssert_True(status == OS_SUCCESS, "status after rmdir 1 = %d",(int)status);
}
/*---------------------------------------------------------------------------------------
 *  Name TestStat()
---------------------------------------------------------------------------------------*/
void TestStat(void)
{
    int32  status; 

    char        filename1[OS_MAX_PATH_LEN];
    char        dir1 [OS_MAX_PATH_LEN];
    char        buffer1 [OS_MAX_PATH_LEN];
    os_fstat_t  StatBuff;
    int32       fd1;
    int         size;

    strcpy(dir1,"/drive0/ThisIsALongDirectoryName");
    strcpy(filename1,"/drive0/ThisIsALongDirectoryName/MyFile1");
    strcpy(buffer1,"111111111111");
 
    /* make the directory */
    status = OS_mkdir(dir1,0);
    UtAssert_True(status == OS_SUCCESS, "status after mkdir 1 = %d",(int)status);
    
    /* now create a file  */
    status = OS_creat(filename1,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 1 = %d",(int)status);
    
    fd1 = status;

    /* Write some data into the file */
    
    size = strlen(buffer1);
    status = OS_write(fd1, buffer1, size);
    UtAssert_True(status == size, "status after write 1 = %d size = %d",(int)status, (int)size);
    
    status = OS_close(fd1);
    UtAssert_True(status == OS_SUCCESS, "status after close 1 = %d",(int)status);

    /* 
    ** Make the stat calls 
    */
    status = OS_stat( "/drive0/ThisIsALongDirectoryName/",&StatBuff);
    UtAssert_True(status == OS_SUCCESS, "status after stat 1 = %d",(int)status);

    status = OS_stat( "/drive0/ThisIsALongDirectoryName",&StatBuff);
    UtAssert_True(status == OS_SUCCESS, "status after stat 2 = %d",(int)status);

    status = OS_stat( "/drive0/ThisIsALongDirectoryName/MyFile1",&StatBuff);
    UtAssert_True(status == OS_SUCCESS, "status after stat 3 = %d",(int)status);

    /* Clean up */
    status = OS_remove(filename1);
    UtAssert_True(status == OS_SUCCESS, "status after remove 1 = %d",(int)status);
 
    status = OS_rmdir(dir1);
    UtAssert_True(status == OS_SUCCESS, "status after rmdir 1 = %d",(int)status);
}

/*---------------------------------------------------------------------------------------
 *  Name: TestOpenFileAPI
 *  This function tests the the misc open File API:
 *    OS_FileOpenCheck(char *Filename);
 *    OS_CloseAllFiles(void);
 *    OS_CloseFileByName(char *Filename);
 *
---------------------------------------------------------------------------------------*/
void TestOpenFileAPI(void)
{
    char filename1 [OS_MAX_PATH_LEN];
    char filename2 [OS_MAX_PATH_LEN];
    char filename3 [OS_MAX_PATH_LEN];
    int status;
    
    strcpy(filename1,"/drive0/Filename1");
    strcpy(filename2,"/drive0/Filename2");
    strcpy(filename3,"/drive0/Filename3");

    /* Create/open a file */
    status = OS_creat(filename1,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 1 = %d",(int)status);

    /* Create/open a file */
    status = OS_creat(filename2,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 2 = %d",(int)status);
   
    /* Create/open a file */
    status = OS_creat(filename3,OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 3 = %d",(int)status);

    /* 
    ** Try OS_FileOpenCheck
    */
    status = OS_FileOpenCheck(filename1);
    UtAssert_True(status >= OS_SUCCESS, "status after OS_FileOpenCheck 1 = %d",(int)status);

    /*
    ** Try OS_CloseFileByName
    */
    status = OS_CloseFileByName(filename1);
    UtAssert_True(status == OS_SUCCESS, "status after OS_CloseFileByName 1 = %d",(int)status);

    /*
    ** Try OS_CloseAllFiles
    */
    status = OS_CloseAllFiles();
    UtAssert_True(status == OS_SUCCESS, "status after OS_CloseAllFiles = %d",(int)status);

    /*
    ** Try OS_CloseFileByName with a file that is already closed
    */
    status = OS_CloseFileByName(filename2);
    UtAssert_True(status < OS_SUCCESS, "status after OS_CloseFileByName 2 = %d",(int)status);
}

