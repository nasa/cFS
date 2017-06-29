# CMake snippet for OSAL network test

set(TEST_MODULE_FILES 
  ut_osnetwork_misc_test.c
  ut_osnetwork_test.c)
  
add_stubs(TEST_STUBS os)
add_osal_ut_exe(osal_network_UT ${TEST_MODULE_FILES} ${TEST_STUBS})

