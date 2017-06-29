# CMake snippet for building the vxworks6 OSAL coverage tests

include_directories(ut-stubs/inc)

add_subdirectory(ut-osal)

# Build contents of all the *-test directories into an executable for each one 
file(GLOB COVERAGE_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/*-test)
foreach(TEST ${COVERAGE_TESTS})
  get_filename_component(DIRNAME ${TEST} NAME_WE)
  string(REPLACE -test "" TESTNAME ${DIRNAME})
  set(TESTCASE_FILES) 
  aux_source_directory(${TEST} TESTCASE_FILES)
  add_executable(${TESTNAME}-testrunner ${TESTCASE_FILES})
  set_target_properties(${TESTNAME}-testrunner PROPERTIES LINK_FLAGS "${UT_C_FLAGS}")
  target_link_libraries(${TESTNAME}-testrunner ut_osal_${TESTNAME} ${OSALCOVERAGE_LINK_LIBRARIES})
  add_test(${TESTNAME} ${TESTNAME}-testrunner)
endforeach()
  
