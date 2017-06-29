
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TESTCASE_FILES)
add_executable(osloader-test ${TESTCASE_FILES})
set_target_properties(osloader-test PROPERTIES LINK_FLAGS "${UT_C_FLAGS}")
target_link_libraries(osloader-test ut_osal_osloader ${OSALCOVERAGE_LINK_LIBRARIES})
