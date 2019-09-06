# CMake snippet for building the shared OSAL layer coverage tests

set(MODULE_LIST osapi osfileapi osfilesys osloader osnetwork osselect ostimer)

# This unit test is allowed to directly include any internal file in 
# the respective set under test.
include_directories(${OSAL_SOURCE_DIR}/src/os/${SETNAME})

# The "ut-stubs" contains additional stubs specific to this sub-module
add_subdirectory(ut-stubs)

# the "modules" contains wrappers around the actual module under test
# (this is not a stub, this is the real code)
add_subdirectory(modules)

# Add all coverage tests in the src dir
add_coverage_tests(${SETNAME} ${MODULE_LIST})

