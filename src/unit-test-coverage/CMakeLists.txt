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

# Assume that this script lives in a subdirectory called <osal>/src/unit-test-coverage
# Get a reference to the top-level OSAL source tree and store it in OSAL_SOURCE_DIR
get_filename_component(OSAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../.." ABSOLUTE)


# Cache any user-specified C flags so they will be retained in future builds
# These can be specified either through cmake command line (e.g. -DUSER_C_FLAGS=-Werror) or
# through an environment variable (e.g. OSALCOVERAGE_USER_C_FLAGS=-Werror cmake ...)
set(OSALCOVERAGE_USER_C_FLAGS "$ENV{OSALCOVERAGE_USER_C_FLAGS}" CACHE STRING "User-specified C flags for OSAL coverage test build")
set(OSALCOVERAGE_C_FLAGS "${OSALCOVERAGE_USER_C_FLAGS} -D_UNIT_TEST_")

# The following logic is borrowed from the real OSAL build
# One difference here is that the UT BSP/framework and target OSTYPE need not "match"
# The following cache variables are recognized:
#   OSALCOVERAGE_TARGET_OSTYPE  -> the intended OSAL that runs on the actual target
#   OSALCOVERAGE_HOST_BSPTYPE   -> the platform/bsp that will execute the coverage tests
# 
# The currently supported setup is to use the "pc-linux" BSP to execute the "vxworks6" 
# code coverage analysis.  Because the actual underlying OS calls are stubbed out, there
# is no dependency on the actual underlying OS.
set(OSALCOVERAGE_TARGET_OSTYPE "vxworks6" CACHE STRING "OSAL target to build coverage tests for")
set(OSALCOVERAGE_HOST_BSPTYPE "pc-linux" CACHE STRING "OSAL unit test BSP to execute coverage tests")


# HAVE_STDINT enables use of the ISO C99 stdint.h file
# This should always be used whenever available, as it is the most accurate way
# to obtain the proper fixed-width type definitions.
find_file(STDINT_H_FILE stdint.h ONLY_CMAKE_FIND_ROOT_PATH)
if (STDINT_H_FILE)
  set(OSALCOVERAGE_C_FLAGS "${OSALCOVERAGE_C_FLAGS} -D_HAVE_STDINT_")
endif (STDINT_H_FILE)

# OSALCOVERAGE_SYSTEM_OSTYPE indicates which of the BSP packages to include
# This is required and must be defined
if (NOT OSALCOVERAGE_TARGET_OSTYPE)
  message(FATAL_ERROR "OSALCOVERAGE_TARGET_OSTYPE must be set to the appropriate OS")
endif (NOT OSALCOVERAGE_TARGET_OSTYPE)

# Check that coverage has been implemented for this OSTYPE
if (NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${OSALCOVERAGE_TARGET_OSTYPE})
  message(FATAL_ERROR "No coverage tests implemented for ${OSALCOVERAGE_TARGET_OSTYPE}")
endif (NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${OSALCOVERAGE_TARGET_OSTYPE})

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
include_directories(${OSAL_SOURCE_DIR}/src/bsp/${OSALCOVERAGE_HOST_BSPTYPE}/config)

# The "ut_bsp" library is a simple startup BSP that can be used for unit testing
# This removes the need to use the "real" CFE PSP and also provides the necessary 
# UT output functions that UT assert may rely upon to report test messages 
# This is the OSAL BSP but with the _UNIT_TEST_ macro defined so it may have UT-specific features
aux_source_directory(${OSAL_SOURCE_DIR}/src/bsp/${OSALCOVERAGE_HOST_BSPTYPE}/ut-src BSPFILES)
add_library(ut_bsp STATIC ${BSPFILES})    
set_target_properties(ut_bsp PROPERTIES COMPILE_DEFINITIONS "_UNIT_TEST_")

# The "utassert" library is the core GSFC-provided unit test library
# It is only the generic framework and does _not_ include any of the specific stub/hook functions  
# It is built as static library so it may be linked with either a "real" implementation or a stub 
# library (see next targets) or some combination of those as the test cases dictate.
aux_source_directory(${OSAL_SOURCE_DIR}/ut_assert/src UT_ASSERT_FILES)
add_library(ut_assert STATIC ${UT_ASSERT_FILES})

SET(OSALCOVERAGE_LINK_LIBRARIES ut_assert ut_bsp)

add_subdirectory(${OSALCOVERAGE_TARGET_OSTYPE})

