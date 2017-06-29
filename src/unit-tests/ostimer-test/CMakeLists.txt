# CMake snippet for OSAL timer test

set(TEST_MODULE_FILES 
  ut_ostimer_timerio_test.c
  ut_ostimer_test.c)
  
add_stubs(TEST_STUBS os)
add_osal_ut_exe(osal_timer_UT ${TEST_MODULE_FILES} ${TEST_STUBS})

