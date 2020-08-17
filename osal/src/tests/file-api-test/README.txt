Explanation:

test2.c is designed to test the functionality of the functions provided in osfilesys.c in the ../os/<OS> directory. The test is broken up into several modules that test related functions.

The modules are separate and independent. Each does not interfere with the others.

These modules are placed in a specific order, however, which assumes that the previous module worked. This is due to the uncircumventable nature of a filesystem (i.e. you cannot open a file for reading/writing without being able to create the file, and you can testing reading/writing without being able to open a file)


NOTE: When all is working properly, the output from each test will start with "OK", and not "ERROR"

TestCreatRemove:

This module tests the basic functionality of creating and removing files. This module also tests the maximum for the length of the names of files as in the osconfig.h file.

It also tests removing files that are not on the drive.

TestOpenClose:

This module tests the ability to open and close files, opening nonexistant files, and closing files multiple times.

TestReadWriteLseek:

This module tests reading to a file, writing to a file, and seeking to a specific byte in a file. Once something is written, the read/write pointer is position at the beginning, and the contents are read, and compared to what was written.

TestMkRmDirFreeBytes:

This module making and removing different directories, with files in them. It also tests reading and writing to those files.

It also includes three calls to OS_FreeBytes, to test its functionality 

TestOpenCloseReadDir:

This module tests opening and closing directories, as well as reading though the directories in search of specific files or sub-level directories.

TestRename:

This module create 2 directories, with a file in each, writes data to the file, then tries to rename the files and the directories, and then reads the data written to the files back out after the renaming.
