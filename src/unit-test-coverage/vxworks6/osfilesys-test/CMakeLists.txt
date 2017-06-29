
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TESTCASE_FILES)
add_executable(osfilesys-test ${TESTCASE_FILES})
set_target_properties(osfilesys-test PROPERTIES LINK_FLAGS "${UT_C_FLAGS}")
target_link_libraries(osfilesys-test ut_osal_osfilesys ${OSALCOVERAGE_LINK_LIBRARIES})
