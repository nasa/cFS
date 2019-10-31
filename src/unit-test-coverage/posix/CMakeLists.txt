# CMake snippet for building the shared OSAL layer coverage tests

set(MODULE_LIST osapi)

# This unit test is allowed to directly include any internal file in 
# the respective set under test.
include_directories(${OSAL_SOURCE_DIR}/src/os/${SETNAME})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/inc)

# The "ut-stubs" contains additional stubs specific to this sub-module
add_subdirectory(ut-stubs)

# the "modules" contains wrappers around the actual module under test
# (this is not a stub, this is the real code)
add_subdirectory(modules)

set(MODULE_LINK_MAP_posixio         osfileapi) 
set(MODULE_LINK_MAP_posixfile       osfileapi) 
set(MODULE_LINK_MAP_posixdirs       osfileapi) 
set(MODULE_LINK_MAP_posixgettime    ostimer) 

# Add all coverage tests in the src dir
add_coverage_tests(${SETNAME} ${MODULE_LIST})

