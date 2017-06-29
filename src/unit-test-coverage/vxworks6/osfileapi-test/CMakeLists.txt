
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TESTCASE_FILES)
add_executable(osfileapi-test ${TESTCASE_FILES})
set_target_properties(osfileapi-test PROPERTIES LINK_FLAGS "${UT_C_FLAGS}")
target_link_libraries(osfileapi-test ut_osal_osfileapi ${OSALCOVERAGE_LINK_LIBRARIES})
add_test(osapi-test osapi-test)
