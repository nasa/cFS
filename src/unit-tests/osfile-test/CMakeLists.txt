# CMake snippet for OSAL file test

set(TEST_MODULE_FILES 
  ut_osfile_fileio_test.c
  ut_osfile_dirio_test.c
  ut_osfile_test.c)
  
add_osal_ut_exe(osal_file_UT ${TEST_MODULE_FILES})

