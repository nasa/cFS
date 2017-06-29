
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TESTCASE_FILES)
add_executable(ostimer-test ${TESTCASE_FILES})
set_target_properties(ostimer-test PROPERTIES LINK_FLAGS "${UT_C_FLAGS}")
target_link_libraries(ostimer-test ut_osal_ostimer ${OSALCOVERAGE_LINK_LIBRARIES})
