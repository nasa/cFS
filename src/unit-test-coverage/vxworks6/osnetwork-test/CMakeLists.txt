
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TESTCASE_FILES)
add_executable(osnetwork-test ${TESTCASE_FILES})
set_target_properties(osnetwork-test PROPERTIES LINK_FLAGS "${UT_C_FLAGS}")
target_link_libraries(osnetwork-test ut_osal_osnetwork ${OSALCOVERAGE_LINK_LIBRARIES})
