######################################################################
#
# CMAKE build recipe for Operating System Abstraction Layer (OSAL)
#
######################################################################
#
# This defines the following static library target(s):
#
#         osal   : The main library containing the OSAL binary code
#                  This is based off the OSAL_SYSTEM_OSTYPE selection
#
#     osal_bsp   : The board support library containing the system-
#                  specific entry point function (e.g. main) and the
#                  file system volume table for the target board.
#                  This is based off the OSAL_SYSTEM_BSPTYPE selection
#
#    ut_assert   : The unit test support library.  This implements
#                  an application entry point (OS_Application_Startup)
#                  that contains a unit test subsystem.  This uses
#                  the OSAL BSP to provide system-specific entry point.
#                  Linking with this library also links with osal_bsp,
#                  but not necessarily the osal library itself.
#
# Additionally the following target is defined if ENABLE_UNIT_TESTS
# is set TRUE:
#
# ut_osapi_stubs : Stub library correlating to the OSAL public API
#                  This is for unit testing OSAL-based applications
#                  It operates in conjunction with the ut_assert library.
#
# This also exports the following variables:
#
#  UT_COVERAGE_COMPILE_FLAGS : Compiler flags that must be used to
#                  instrument code for coverage testing
#  UT_COVERAGE_LINK_FLAGS : Linker flags that must be used to
#                  instrument code for coverage testing
#
# The ENABLE_UNIT_TESTS option also builds a set of test applications from
# that demonstrate the usage and validate the runtime behavior of various
# OSAL resources.
#
######################################################################
cmake_minimum_required(VERSION 2.8.12)
project(OSAL C)

# The "OSAL_EXT_SOURCE_DIR" cache variable may be set to a path
# on the host containing extra OS/BSP implementations which are not
# part of the open source release.
# CAUTION: The API between the OSAL and the low level implementation and/or BSP
# is not stabilized, and may change with every OSAL release.  No attempt is made
# to provide backward compatibility with to external sources. 
set(OSAL_EXT_SOURCE_DIR "$ENV{OSAL_EXT_SOURCE_DIR}" 
    CACHE PATH "External source directory to check for additional OS/BSP implementations")
    
# Read the default compile-time configuration, and update with
# any mission/project specific options in the OSAL_CONFIGURATION_FILE
include("${OSAL_SOURCE_DIR}/default_config.cmake")

# The user-specified file is optional, but in case the value is defined but the
# file does not exist, this should be treated as an error.
foreach(CONFIG ${OSAL_CONFIGURATION_FILE})
    include(${CONFIG})
endforeach(CONFIG OSAL_CONFIGURATION_FILE)

# Use the supplied configuration to generate the osconfig.h file
# which can be referenced by the code.  This will be stored in the top level
# "inc" directory of the binary output directory
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/inc")
configure_file(
    "${OSAL_SOURCE_DIR}/osconfig.h.in"
    "${OSAL_BINARY_DIR}/osconfig.gen"
    @ONLY
)

# Only copy the osconfig.h into place if different from the existing file
# This avoids unnecessarily rebuilding all code in case cmake was re-run
# and but generated the same file.
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${OSAL_BINARY_DIR}/osconfig.gen"
    "${CMAKE_BINARY_DIR}/inc/osconfig.h"
)

# The initial set of directories that define the OSAL API
# This is used to initialize the interface include directory property of external targets
set(OSAL_API_INCLUDE_DIRECTORIES
    "${OSAL_SOURCE_DIR}/src/os/inc"
    "${CMAKE_BINARY_DIR}/inc"
)
include_directories(${OSAL_API_INCLUDE_DIRECTORIES})

# In case the OSAL_USER_C_FLAGS was specified, use them
add_definitions(${OSAL_USER_C_FLAGS})

#
# Build UT assert -
# the basic ut_assert library target is always defined regardless of "ENABLE_UNIT_TESTS",
# but flagged using "EXCLUDE_FROM_ALL" so they won't be built unless actually used.  This
# is because the library is usable with functional tests, not just unit (coverage) tests.
# This is done early, so that other targets may reference UT_ASSERT_SOURCE_DIR if needed
add_subdirectory(ut_assert)

#
# Step 1:
# Build the BSP layer
#


# OSAL_SYSTEM_BSPTYPE indicate which of the BSP packages
# to build.  These is required and must be defined. Confirm that this exists
# and error out now if it does not.
if (NOT DEFINED OSAL_SYSTEM_BSPTYPE)
  message(FATAL_ERROR "OSAL_SYSTEM_BSPTYPE must be set to the appropriate BSP")
endif ()
if (OSAL_EXT_SOURCE_DIR AND IS_DIRECTORY "${OSAL_EXT_SOURCE_DIR}/${OSAL_SYSTEM_BSPTYPE}")
    set(OSAL_BSP_SOURCE_DIR "${OSAL_EXT_SOURCE_DIR}/${OSAL_SYSTEM_BSPTYPE}")
elseif(IS_DIRECTORY "${OSAL_SOURCE_DIR}/src/bsp/${OSAL_SYSTEM_BSPTYPE}")
    set(OSAL_BSP_SOURCE_DIR "${OSAL_SOURCE_DIR}/src/bsp/${OSAL_SYSTEM_BSPTYPE}")
else()
  # It is an error if the indicated BSPTYPE does not correspond to a subdirectory
  # If this is not caught here then a more obfuscated error will occur later.
  message(FATAL_ERROR "Error: No source directory found for \"${OSAL_SYSTEM_BSPTYPE}\" BSP type")
endif()

message(STATUS "BSP Selection: ${OSAL_SYSTEM_BSPTYPE} at ${OSAL_BSP_SOURCE_DIR}")


# The BSP library is a separate target from OSAL and can be used
# independently of the OSAL library and/or in combination with
# UT assert and the OSAL stub library for unit testing.
#
# The Implementation-Specific BSP subdirectory should define
# an OBJECT target named "osal_${OSAL_SYSTEM_BSPTYPE}_impl"
add_subdirectory(${OSAL_BSP_SOURCE_DIR} ${OSAL_SYSTEM_BSPTYPE}_impl)
target_include_directories(osal_${OSAL_SYSTEM_BSPTYPE}_impl PRIVATE
    ${OSAL_SOURCE_DIR}/src/bsp/shared/inc
)

# Confirm that the selected OS is compatible with the selected BSP.
if (DEFINED OSAL_EXPECTED_OSTYPE)
    if (NOT DEFINED OSAL_SYSTEM_OSTYPE)
        # In the event that OSAL_SYSTEM_OSTYPE was not specified at all,
        # implicitly assume the expected OSTYPE.
        set(OSAL_SYSTEM_OSTYPE ${OSAL_EXPECTED_OSTYPE})
    elseif(NOT OSAL_SYSTEM_OSTYPE STREQUAL OSAL_EXPECTED_OSTYPE)
        # Generate a warning about the OSTYPE not being expected.
        # Not calling this a fatal error because it could possibly be intended during development
        message(WARNING "Mismatched BSP/OS: ${OSAL_SYSTEM_BSPTYPE} implies ${OSAL_EXPECTED_OSTYPE}, but ${OSAL_SYSTEM_OSTYPE} is configured")
    endif(NOT DEFINED OSAL_SYSTEM_OSTYPE)
endif (DEFINED OSAL_EXPECTED_OSTYPE)

# Propagate the BSP-specific compile definitions and include directories
# Apply these to the directory-scope COMPILE_DEFINITIONS  and INCLUDE_DIRECTORIES
# Note this needs to append to the directory property, not overwrite it.
get_directory_property(OSAL_BASE_COMPILE_DEFINITIONS COMPILE_DEFINITIONS)
get_target_property(OSAL_BSP_COMPILE_DEFINITIONS osal_${OSAL_SYSTEM_BSPTYPE}_impl INTERFACE_COMPILE_DEFINITIONS)
set(OSAL_COMPILE_DEFINITIONS)
if (OSAL_BASE_COMPILE_DEFINITIONS)
    list(APPEND OSAL_COMPILE_DEFINITIONS ${OSAL_BASE_COMPILE_DEFINITIONS})
endif (OSAL_BASE_COMPILE_DEFINITIONS)
if (OSAL_BSP_COMPILE_DEFINITIONS)
    list(APPEND OSAL_COMPILE_DEFINITIONS ${OSAL_BSP_COMPILE_DEFINITIONS})
endif (OSAL_BSP_COMPILE_DEFINITIONS)
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS "${OSAL_COMPILE_DEFINITIONS}")
message(STATUS "OSAL Compile Definitions: ${OSAL_COMPILE_DEFINITIONS}")

# The include directories is simpler, as the include_directories() function
# appends to the directory property
get_target_property(OSAL_BSP_INCLUDE_DIRECTORIES osal_${OSAL_SYSTEM_BSPTYPE}_impl INTERFACE_INCLUDE_DIRECTORIES)
if (OSAL_BSP_INCLUDE_DIRECTORIES)
    include_directories(${OSAL_BSP_INCLUDE_DIRECTORIES})
endif (OSAL_BSP_INCLUDE_DIRECTORIES)

set(BSP_SRCLIST 
    src/bsp/shared/src/osapi-bsp.c
    src/bsp/shared/src/bsp_default_app_run.c
    src/bsp/shared/src/bsp_default_app_startup.c
    src/bsp/shared/src/bsp_default_symtab.c
)

if (NOT OMIT_DEPRECATED)
    list(APPEND BSP_SRCLIST
        src/bsp/shared/src/bsp_default_voltab.c
    )
endif (NOT OMIT_DEPRECATED)

# Define the external "osal_bsp" static library target
add_library(osal_bsp STATIC
    ${BSP_SRCLIST}
    $<TARGET_OBJECTS:osal_${OSAL_SYSTEM_BSPTYPE}_impl>
)

target_include_directories(osal_bsp INTERFACE
    ${OSAL_API_INCLUDE_DIRECTORIES}
)

target_include_directories(osal_bsp PRIVATE
    ${OSAL_SOURCE_DIR}/src/bsp/shared/inc
)


#
# Step 2:
# Build the OSAL layer
#

# OSAL_SYSTEM_OSTYPE indicates which of the OS packages
# to build.  If not defined, this may be inferred by the BSP type.
if (NOT DEFINED OSAL_SYSTEM_OSTYPE)
  message(FATAL_ERROR "OSAL_SYSTEM_OSTYPE must be set to the appropriate OS")
endif ()
if (OSAL_EXT_SOURCE_DIR AND IS_DIRECTORY "${OSAL_EXT_SOURCE_DIR}/${OSAL_SYSTEM_OSTYPE}")
    set(OSAL_OS_SOURCE_DIR "${OSAL_EXT_SOURCE_DIR}/${OSAL_SYSTEM_OSTYPE}")
elseif(IS_DIRECTORY "${OSAL_SOURCE_DIR}/src/os/${OSAL_SYSTEM_OSTYPE}")
    set(OSAL_OS_SOURCE_DIR "${OSAL_SOURCE_DIR}/src/os/${OSAL_SYSTEM_OSTYPE}")
else()
  # It is an error if the indicated OSTYPE does not correspond to a subdirectory
  # If this is not caught here then a more obfuscated error will occur later.
  message(FATAL_ERROR  "Error: No source directory found for \"${OSAL_SYSTEM_OSTYPE}\" OS type")
endif()


message(STATUS "OSAL Selection: ${OSAL_SYSTEM_OSTYPE} at ${OSAL_OS_SOURCE_DIR}")

# The implementation-specific OSAL subdirectory should define
# an OBJECT target named "osal_${OSAL_SYSTEM_OSTYPE}_impl"
add_subdirectory(${OSAL_OS_SOURCE_DIR} ${OSAL_SYSTEM_OSTYPE}_impl)

# The "shared" directory contains internal components which
# are referenced in implementation OSAL modules, but should _NOT_
# be referenced outside the OSAL code
target_include_directories(osal_${OSAL_SYSTEM_OSTYPE}_impl PRIVATE
    ${OSAL_SOURCE_DIR}/src/os/shared/inc
    ${OSAL_SOURCE_DIR}/src/bsp/shared/inc
)

set(OSAL_SRCLIST
    src/os/shared/src/osapi-binsem.c
    src/os/shared/src/osapi-clock.c
    src/os/shared/src/osapi-common.c
    src/os/shared/src/osapi-countsem.c
    src/os/shared/src/osapi-dir.c
    src/os/shared/src/osapi-errors.c
    src/os/shared/src/osapi-file.c
    src/os/shared/src/osapi-filesys.c
    src/os/shared/src/osapi-heap.c
    src/os/shared/src/osapi-idmap.c
    src/os/shared/src/osapi-module.c
    src/os/shared/src/osapi-mutex.c
    src/os/shared/src/osapi-network.c
    src/os/shared/src/osapi-printf.c
    src/os/shared/src/osapi-queue.c
    src/os/shared/src/osapi-select.c
    src/os/shared/src/osapi-shell.c
    src/os/shared/src/osapi-sockets.c
    src/os/shared/src/osapi-task.c
    src/os/shared/src/osapi-timebase.c
    src/os/shared/src/osapi-time.c
)

if (OSAL_CONFIG_DEBUG_PRINTF)
    list(APPEND OSAL_SRCLIST
        src/os/shared/src/osapi-debug.c
    )
endif (OSAL_CONFIG_DEBUG_PRINTF)


# The FPU and interrupt modules are deprecated.
# If the "OMIT_DEPRECATED" switch is set, then these are not built.
if (NOT OMIT_DEPRECATED)
    list(APPEND OSAL_SRCLIST
        src/os/shared/src/osapi-fpu.c
        src/os/shared/src/osapi-interrupts.c
    )
endif (NOT OMIT_DEPRECATED)


# Define the external "osal" static library target
# This is a combination of the generic parts with the low level
# system-specific parts
add_library(osal STATIC
    ${OSAL_SRCLIST}
    $<TARGET_OBJECTS:osal_${OSAL_SYSTEM_OSTYPE}_impl>
)

target_include_directories(osal INTERFACE
    ${OSAL_API_INCLUDE_DIRECTORIES}
)

target_include_directories(osal PRIVATE
    ${OSAL_SOURCE_DIR}/src/os/shared/inc
    ${OSAL_SOURCE_DIR}/src/bsp/shared/inc
)

# Link the OSAL with the BSP
target_link_libraries(osal osal_bsp)

# propagate the BSP-specific compile flags to OSAL external library target, if defined
if (OSAL_BSP_COMPILE_DEFINITIONS)
    target_compile_definitions(osal INTERFACE
        ${OSAL_BSP_COMPILE_DEFINITIONS}
    )
endif(OSAL_BSP_COMPILE_DEFINITIONS)

# propagate the BSP-specific include directories OSAL all external library target, if defined
if (OSAL_BSP_INCLUDE_DIRECTORIES)
    target_include_directories(osal INTERFACE
        ${OSAL_BSP_INCLUDE_DIRECTORIES}
    )
endif(OSAL_BSP_INCLUDE_DIRECTORIES)



# The "build_options.cmake" file within each component may
# fine-tune the library for this particular build.  This is included
# AFTER The basic targets are defined, so it may set properties
# on the defined targets and/or use target-specific commands.
include("${OSAL_BSP_SOURCE_DIR}/build_options.cmake" OPTIONAL)
include("${OSAL_OS_SOURCE_DIR}/build_options.cmake" OPTIONAL)

#
# UNIT TEST SUPPORT
#
if (ENABLE_UNIT_TESTS)

    enable_testing()

    # Generic function for consistent definition of a unit testing target
    # This is defined here in the top-level OSAL CMakeLists so it can be used
    # in both the "tests" and "unit-tests" subdirectories.
    function(add_osal_ut_exe TGTNAME)

      add_executable(${TGTNAME} ${ARGN})
      target_link_libraries(${TGTNAME} ut_assert osal)
      add_test(${TGTNAME} ${TGTNAME})
      foreach(TGT ${INSTALL_TARGET_LIST})
        install(TARGETS ${TGTNAME} DESTINATION ${TGT}/${UT_INSTALL_SUBDIR})
      endforeach()

    endfunction(add_osal_ut_exe)

    # The "ut_osapi_stubs" library contains "stub" functions of the OSAL API calls, used for
    # testing other application code built on top of OSAL.
    add_subdirectory(src/ut-stubs ut-stubs)

    # The "unit-test-coverage" is a white-box line coverage test.
    # It re-compiles each source unit for coverage analysis and links
    # with stub dependencies and a test sequence designed to execute
    # every line of source code within OSAL.
    add_subdirectory(src/unit-test-coverage unit-test-coverage)

    # The "tests" and "unit-tests" subdirectories implement examples and verification tests
    # of the OSAL library.  Note that these are both black box tests that link with the full
    # OSAL (not a stub/coverage test).
    add_subdirectory(src/tests tests)
    add_subdirectory(src/unit-tests unit-tests)

endif (ENABLE_UNIT_TESTS)

# If this build is being performed as a subdirectory within a larger project,
# then export the important data regarding compile flags/dirs to that parent
# This is conditional to avoid warnings in a standalone build.
get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if (HAS_PARENT)
    # Export the UT coverage compiler/linker flags to the parent build.
    # These flags are based on the target system type and should be used
    # when building code intended for coverage analysis.
    set(UT_COVERAGE_COMPILE_FLAGS "${UT_COVERAGE_COMPILE_FLAGS}" PARENT_SCOPE)
    set(UT_COVERAGE_LINK_FLAGS "${UT_COVERAGE_LINK_FLAGS}" PARENT_SCOPE)
endif(HAS_PARENT)




