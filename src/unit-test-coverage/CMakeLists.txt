# CMake build script for OSAL white-box coverage tests
#
# IMPORTANT: This is currently implemented as a completely separate project
# from OSAL itself and it is not connected to the normal OSAL build in any way.
# To run coverage tests this must be built separately.  There are several reasons
# for having it this way:
#    - Not all targets have coverage testing implemented (yet).  Only VxWorks right now.
#    - It may use an entirely different toolchain than the actual target build. 
#       (coverage tests run with the "real" calls stubbed out, so they can be executed
#        on any platform/os - case in point, the VxWorks coveraged paths can be analyzed
#        by running the code on Linux using the specially-crafted inputs) 
#    - By definition this MUST completely rebuild OSAL to add the requisite "coverage" 
#      options so that the binaries include the extra instrumentation.
#
#  NO ARTEFACTS FROM THIS BUILD SHOULD EVER INTERMINGLE WITH THE REAL TARGET BUILD 
#  

cmake_minimum_required(VERSION 2.6.4)
project(OSALCOVERAGE C)

# Ask to generate a "make test" target
enable_testing()

add_definitions(-Wall -Werror)
add_definitions(-D_UNIT_TEST_)

# Assume that this script lives in a subdirectory called <osal>/src/unit-test-coverage
# Get a reference to the top-level OSAL source tree and store it in OSAL_SOURCE_DIR
get_filename_component(OSAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../.." ABSOLUTE)


# Cache any user-specified C flags so they will be retained in future builds
# These can be specified either through cmake command line (e.g. -DUSER_C_FLAGS=-Werror) or
# through an environment variable (e.g. OSALCOVERAGE_USER_C_FLAGS=-Werror cmake ...)
set(OSALCOVERAGE_USER_C_FLAGS "$ENV{OSALCOVERAGE_USER_C_FLAGS}" CACHE STRING "User-specified C flags for OSAL coverage test build")

# The following logic is borrowed from the real OSAL build
# One difference here is that the UT BSP/framework and target OSTYPE need not "match"
# The following cache variables are recognized:
#   OSALCOVERAGE_TARGET_OSTYPE  -> the intended OSAL that runs on the actual target
#   OSALCOVERAGE_HOST_BSPTYPE   -> the platform/bsp that will execute the coverage tests
# 
# The currently supported setup is to use the "pc-linux" BSP to execute the "vxworks6" 
# code coverage analysis.  Because the actual underlying OS calls are stubbed out, there
# is no dependency on the actual underlying OS.
set(OSALCOVERAGE_TARGET_OSTYPE "vxworks;shared" CACHE STRING "OSAL target(s) to build coverage tests for")
set(OSALCOVERAGE_HOST_BSPTYPE "pc-linux" CACHE STRING "OSAL unit test BSP to execute coverage tests")

# OSALCOVERAGE_SYSTEM_OSTYPE indicates which of the BSP packages to include
# This is required and must be defined
if (NOT OSALCOVERAGE_TARGET_OSTYPE)
  message(FATAL_ERROR "OSALCOVERAGE_TARGET_OSTYPE must be set to the appropriate OS")
endif (NOT OSALCOVERAGE_TARGET_OSTYPE)

# Check that coverage has been implemented for this OSTYPE
foreach(OSTYPE ${OSALCOVERAGE_TARGET_OSTYPE})
    if (NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${OSTYPE})
      message(FATAL_ERROR "No coverage tests implemented for ${OSTYPE}")
    endif (NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${OSTYPE})
endforeach(OSTYPE ${OSALCOVERAGE_TARGET_OSTYPE})

# Do NOT Include the OS-specific compiler options (those are for normal builds)
#include(src/os/${OSALCOVERAGE_TARGET_OSTYPE}/build_options.cmake OPTIONAL)
message(STATUS "Coverage Test Target OS: ${OSALCOVERAGE_TARGET_OSTYPE}")

# OSALCOVERAGE_SYSTEM_BSPTYPE indicates which of the BSP packages to use to execute the tests
if (NOT OSALCOVERAGE_HOST_BSPTYPE)
  message(FATAL_ERROR "OSALCOVERAGE_HOST_BSPTYPE must be set to the appropriate BSP")
endif (NOT OSALCOVERAGE_HOST_BSPTYPE)

if (NOT IS_DIRECTORY ${OSAL_SOURCE_DIR}/src/bsp/${OSALCOVERAGE_HOST_BSPTYPE})
  message(FATAL_ERROR "${OSALCOVERAGE_HOST_BSPTYPE} is not a valid host BSP")
endif (NOT IS_DIRECTORY ${OSAL_SOURCE_DIR}/src/bsp/${OSALCOVERAGE_HOST_BSPTYPE})

# Include the BSP-specific (host) compiler options
include(${OSAL_SOURCE_DIR}/src/bsp/${OSALCOVERAGE_HOST_BSPTYPE}/make/build_options.cmake OPTIONAL)
message(STATUS "Coverage Test Host BSP: ${OSALCOVERAGE_HOST_BSPTYPE}")

set(CMAKE_C_FLAGS "${OSALCOVERAGE_C_FLAGS} ${OSAL_C_FLAGS}")
message(STATUS "Coverage Test CFLAGS: ${CMAKE_C_FLAGS}")

# Utilize the shared UT assert library, along with the standard OSAL includes
include_directories(${OSAL_SOURCE_DIR}/ut_assert/inc)
include_directories(${OSAL_SOURCE_DIR}/src/os/inc)

# The "osconfig.h" file for coverage test should ideally be the same one that
# is used for the normal build.  The path to this file can be supplied as OSAL_INCLUDEDIR
# If not supplied, the file from the UT BSP can be used, but coverage results might be 
# less accurate.  
if (DEFINED OSAL_INCLUDEDIR)
    include_directories(${OSAL_INCLUDEDIR})
else (DEFINED OSAL_INCLUDEDIR)
    message(STATUS "No OSAL_INCLUDEDIR specified, using default config from UT BSP")
    include_directories(${OSAL_SOURCE_DIR}/src/bsp/${OSALCOVERAGE_HOST_BSPTYPE}/config)
endif (DEFINED OSAL_INCLUDEDIR)

# These include paths are not public APIs but are needed
# to get to some internal headers for private APIs
include_directories(${OSAL_SOURCE_DIR}/src/os/shared)
include_directories(${OSAL_SOURCE_DIR}/src/os/portable)
include_directories(${CMAKE_SOURCE_DIR}/ut-stubs/inc)

# The "ut_bsp" library is a simple startup BSP that can be used for unit testing
# This removes the need to use the "real" CFE PSP and also provides the necessary 
# UT output functions that UT assert may rely upon to report test messages 
# This is the OSAL BSP but with the _UNIT_TEST_ macro defined so it may have UT-specific features
aux_source_directory(${OSAL_SOURCE_DIR}/src/bsp/${OSALCOVERAGE_HOST_BSPTYPE}/ut-src BSPFILES)
add_library(ut_bsp STATIC ${BSPFILES})    

# The "utassert" library is the core GSFC-provided unit test library
# It is only the generic framework and does _not_ include any of the specific stub/hook functions  
# It is built as static library so it may be linked with either a "real" implementation or a stub 
# library (see next targets) or some combination of those as the test cases dictate.
aux_source_directory(${OSAL_SOURCE_DIR}/ut_assert/src UT_ASSERT_FILES)
add_library(ut_assert STATIC ${UT_ASSERT_FILES})

# The OSALCOVERAGE_STUB_LIB_LIST is a list of stub libraries to link the
# test runner executables with.  It will be appended at various points
# depending on what is under test.
set(OSALCOVERAGE_STUB_LIB_LIST)
add_subdirectory(ut-stubs)

# Build the stub libraries for OSAL itself.
# These are potentially needed by many tests, to handle calls 
# from the module under test into OTHER parts of OSAL 
aux_source_directory(${OSAL_SOURCE_DIR}/src/ut-stubs UT_OSAPI_STUB_FILES)
add_library(ut_osapi_stubs STATIC ${UT_OSAPI_STUB_FILES})

list(APPEND OSALCOVERAGE_STUB_LIB_LIST ut_osapi_stubs)

# A generic function to add a coverage test case source file
# This combines the following into an executable
#  - The test case setup (a C source file starting with "coveragetest" prefix)
#  - The object code of the unit under test (special build with instrumentation)
#  - Links to the stub libraries of everything else, plus UT assert
function (add_coverage_tests SETNAME)
  foreach(MODNAME ${ARGN})
    set (TESTCASE_SRCFILE)
    foreach (SRCFILE 
        "${PROJECT_SOURCE_DIR}/portable/coveragetest-${MODNAME}.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/coveragetest-${MODNAME}.c"
    )
        if (EXISTS "${SRCFILE}")
            set (TESTCASE_SRCFILE "${SRCFILE}")
        endif (EXISTS "${SRCFILE}")
    endforeach()
    
    if (TESTCASE_SRCFILE)
      set(TESTNAME "${SETNAME}-${MODNAME}")
      message (STATUS "Found test case for ${TESTNAME} in ${TESTCASE_SRCFILE}")
      
      if (DEFINED MODULE_LINK_MAP_${MODNAME})
        set(LINKMOD ${MODULE_LINK_MAP_${MODNAME}})
      else()
        set(LINKMOD ${MODNAME})
      endif()
          
      add_executable(${TESTNAME}-testrunner 
        ${TESTCASE_SRCFILE}
        $<TARGET_OBJECTS:ut_${SETNAME}_${LINKMOD}>)
        
      set_target_properties(${TESTNAME}-testrunner PROPERTIES LINK_FLAGS "${UT_C_FLAGS}")
      target_link_libraries(${TESTNAME}-testrunner ${OSALCOVERAGE_STUB_LIB_LIST} ut_bsp ut_assert)
      add_test(${TESTNAME} ${TESTNAME}-testrunner)
    endif()
  endforeach(MODNAME ${ARGN})
  
endfunction(add_coverage_tests SETNAME)


foreach(SETNAME ${OSALCOVERAGE_TARGET_OSTYPE})
    add_subdirectory(${SETNAME})
endforeach(SETNAME ${OSALCOVERAGE_TARGET_OSTYPE})

