# CMake snippet for OSAL filesys test

set(TEST_MODULE_FILES 
  ut_osfilesys_diskio_test.c
  ut_osfilesys_test.c)
  
add_stubs(TEST_STUBS os)
add_osal_ut_exe(osal_filesys_UT ${TEST_MODULE_FILES} ${TEST_STUBS})

