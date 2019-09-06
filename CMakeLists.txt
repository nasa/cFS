cmake_minimum_required(VERSION 2.6.4)
project(OSAL C)

enable_testing()

# Generic function for consistent definition of a unit testing target
# This is defined here in the top-level OSAL CMakeLists so it can be used
# in both the "tests" and "unit-tests" subdirectories.
# These binaries are linked with the "ut_osal" and "ut_assert" libraries
function(add_osal_ut_exe TGTNAME)

  add_executable(${TGTNAME} ${ARGN})
  # Need to force the OS_VolumeTable as undefined to be sure the linker pulls it in
  # This is workaround specific to UT builds that run the *REAL* OSAL (in other words,
  # the unit test for OSAL itself).
  # It is not an issue for UT builds that use OSAL stubs or no OSAL at all.
  set_target_properties(${TGTNAME} PROPERTIES COMPILE_DEFINITIONS "_UNIT_TEST_")
  set_target_properties(${TGTNAME} PROPERTIES COMPILE_FLAGS "${UT_C_FLAGS}")  
  set_target_properties(${TGTNAME} PROPERTIES LINK_FLAGS "${UT_C_FLAGS} -u OS_VolumeTable -u OS_STATIC_SYMBOL_TABLE -u UT_BSP_DoText")  
  target_link_libraries(${TGTNAME} ut_bsp osal ut_assert ${OSAL_LINK_LIBS})
  add_test(${TGTNAME} ${TGTNAME})
  foreach(TGT ${INSTALL_TARGET_LIST})
    install(TARGETS ${TGTNAME} DESTINATION ${TGT}/${UT_INSTALL_SUBDIR})
  endforeach()

endfunction(add_osal_ut_exe)

#
# Assembly of the FLAGS used for compiler invocation:
# 
# This currently consists of four sets of flags, depending on scope:
# 
# OSAL_COMMON_COMPILE_DEFS  ==> Generic Compile Flags, applies to all BSPs, both C and C++
# OSAL_C_FLAGS              ==> Compile Flags specific to C code
# OSAL_CXX_FLAGS            ==> Compile Flags specific to C++ code
# BSP_COMPILE_FLAGS         ==> Flags specific to the BSP in use for both C and C++
#
# The osal_assemble_compiler_flags() function will set the relevant variables
# for the OSAL build based on the selected OSTYPE and BSPTYPE
#
function(osal_assemble_compiler_flags OSTYPE BSPTYPE)

    # Reset the local variables to initial state
    set(OSAL_COMMON_COMPILE_DEFS)
    set(OSAL_C_FLAGS)
    set(OSAL_CXX_FLAGS)
    set(BSP_COMPILE_FLAGS)
    set(UT_C_FLAGS)
    set(OSAL_LINK_LIBS)
    
    # Include the OS-specific compiler options
    # NOTE: OSTYPE may be passed as FALSE to skip OS-specific flags
    if (OSTYPE)
        include(${OSAL_SOURCE_DIR}/src/os/${OSTYPE}/build_options.cmake OPTIONAL)
    endif()

    # Include the BSP/PSP-specific compiler options
    # NOTE: BSPTYPE may be passed as FALSE to skip BSP-specific flags
    if (BSPTYPE)
        include(${OSAL_SOURCE_DIR}/src/bsp/${BSPTYPE}/make/build_options.cmake OPTIONAL)
    endif()

    # Set up the final set of C flags for the build.  This is the sum of what the toolchain needs,
    # what the BSP/PSP has added and anything else that the user has asked for.
    set(CMAKE_C_FLAGS 
        "${CMAKE_C_FLAGS_INIT} ${OSAL_COMMON_COMPILE_DEFS} ${BSP_COMPILE_FLAGS} ${OSAL_USER_C_FLAGS} ${OSAL_C_FLAGS}"
        PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS 
        "${CMAKE_CXX_FLAGS_INIT} ${OSAL_COMMON_COMPILE_DEFS} ${BSP_COMPILE_FLAGS} ${OSAL_USER_CXX_FLAGS} ${OSAL_CXX_FLAGS}"
        PARENT_SCOPE)
        
    # Additionally export other relevant information to the caller
    set(BSP_UT_C_FLAGS ${UT_C_FLAGS} PARENT_SCOPE)
    set(OSAL_LINK_LIBS ${OSAL_LINK_LIBS} PARENT_SCOPE)
    if (INSTALL_SUBDIR)
        set(INSTALL_SUBDIR ${INSTALL_SUBDIR} PARENT_SCOPE)
    endif (INSTALL_SUBDIR)

endfunction(osal_assemble_compiler_flags OSTYPE BSPTYPE)

# Cache any user-specified C flags so they will be retained in future builds
# These can be specified either through cmake command line (e.g. -DUSER_C_FLAGS=-Werror) or
# through an environment variable (e.g. OSAL_USER_C_FLAGS=-Werror cmake ...)
set(OSAL_USER_C_FLAGS "$ENV{OSAL_USER_C_FLAGS}" CACHE STRING "User-specified C flags for OSAL build")
set(OSAL_USER_CXX_FLAGS "$ENV{OSAL_USER_CXX_FLAGS}" CACHE STRING "User-specified C++ flags for OSAL build")

# OSAL_SYSTEM_OSTYPE indicates which of the OS packages to include
# This is required and must be defined
if (OSAL_SYSTEM_OSTYPE AND 
    IS_DIRECTORY "${OSAL_SOURCE_DIR}/src/os/${OSTYPE}")
  set(OSAL_SELECTED_OSTYPE ${OSAL_SYSTEM_OSTYPE})
else()
  # It is an error if the indicated OSTYPE does not correspond to a subdirectory
  # If this is not caught here then a more obfuscated error will occur later.
  message(FATAL_ERROR "OSAL_SYSTEM_OSTYPE must be set to the appropriate OS")
endif ()

message(STATUS "OSAL Selection: ${OSAL_SELECTED_OSTYPE}")


# OSAL_SYSTEM_BSPTYPE indicates which of the BSP packages to include
# It is optional to build this library (e.g. cFE includes its own PSP)
# If used, this should also define the installation location for binaries, 
# which depends on the bsp volume table
# This may also set more "-D" options to the compiler command in order to 
# build code properly for this OS
# This should not be included if CFE_SYSTEM_PSPNAME is given, since the PSP
# replaces the OSAL BSP
set(OSAL_SELECTED_BSPTYPE FALSE)
if (OSAL_SYSTEM_BSPTYPE AND NOT CFE_SYSTEM_PSPNAME)
  if (IS_DIRECTORY ${OSAL_SOURCE_DIR}/src/bsp/${OSAL_SYSTEM_BSPTYPE})
    set(OSAL_SELECTED_BSPTYPE ${OSAL_SYSTEM_BSPTYPE})
    message(STATUS "OSAL BSP Selection: ${OSAL_SELECTED_BSPTYPE}")
  else()
    message(FATAL_ERROR "Error: ${OSAL_SYSTEM_BSPTYPE} is not a valid BSP")
  endif()
endif (OSAL_SYSTEM_BSPTYPE AND NOT CFE_SYSTEM_PSPNAME)

# Set up the final set of C flags for the build.  This is the sum of what the toolchain needs,
# what the BSP/PSP has added and anything else that the user has asked for.
osal_assemble_compiler_flags(${OSAL_SELECTED_OSTYPE} ${OSAL_SELECTED_BSPTYPE})

# At a mimimum, also compile with -Wall to show extra warnings.  Only do this if nothing
# added it already (prevents adding this twice in case the User/BSP/PSP already specified it)
if (NOT CMAKE_C_FLAGS MATCHES "-Wall")
  set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
endif(NOT CMAKE_C_FLAGS MATCHES "-Wall")

message(STATUS "OSAL Compile Definitions: ${CMAKE_C_FLAGS}") 
message(STATUS "OSAL Link Libraries: ${OSAL_LINK_LIBS}") 

# Use the OSAL shared include directory
include_directories(src/os/inc)
include_directories(src/os/shared)

# Use the UT assert include directory
# Although this is only used for unit tests, putting this out here
# rather than in the "if(ENABLE_UNIT_TESTS)" section keeps things consistent
# between the UT and non-UT builds
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ut_assert/inc)

# Include any user-specified header file directory
if (OSAL_INCLUDEDIR)
  include_directories(${OSAL_INCLUDEDIR})
endif (OSAL_INCLUDEDIR)

# Use all source files under the specific OS and BSP directories
# Use the shared code directory if the OS layer requires it
aux_source_directory(src/os/shared OSALFILES)
aux_source_directory(src/os/${OSAL_SELECTED_OSTYPE} OSALFILES)
if (OSAL_SELECTED_BSPTYPE)
  aux_source_directory(src/bsp/${OSAL_SELECTED_BSPTYPE}/src BSPFILES)
endif (OSAL_SELECTED_BSPTYPE)

add_library(osal STATIC ${OSALFILES} ${BSPFILES})
target_link_libraries(osal ${OSAL_LINK_LIBS})

# Determine if this build is standalone or part of a larger build
# If this is part of a larger build, certain key values will be exported to the parent
# Do this now, before adding the unit test logic into the mix
get_directory_property(BUILD_PARENT PARENT_DIRECTORY)
if (BUILD_PARENT)
  # Important - all code in the entire build should be built using 
  # at least the same C_FLAGS as OSAL used, so push this to parent scope
  set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} PARENT_SCOPE)
  set(INSTALL_SUBDIR ${INSTALL_SUBDIR} PARENT_SCOPE)
endif (BUILD_PARENT)

#
# UNIT TEST SUPPORT LIBRARIES
# the basic library targets are now always created regardless of "ENABLE_UNIT_TESTS",
# but flagged using "EXCLUDE_FROM_ALL" so they won't be built unless actually used.
#

# NOTE: The "ut_assert" and "ut_bsp" libraries are usable by ANY and ALL subsystem(s) that need 
# to do unit testing of any kind.  These provide the basic startup procedure, test message output
# abstractions, and all the "assert" calls to perform unit testing.  They specifically do NOT
# include any stub functions, as the configuration of stubs vs. real implementations are specific
# to the unit being tested.

# The "utassert" library is the core GSFC-provided unit test library
# It is only the generic framework and does _not_ include any of the specific stub/hook functions  
# It is built as static library so it may be linked with either a "real" implementation or a stub 
# library (see next targets) or some combination of those as the test cases dictate.
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ut_assert/src UT_ASSERT_FILES)
add_library(ut_assert STATIC EXCLUDE_FROM_ALL ${UT_ASSERT_FILES})
   
# in order to run the unit test, we need a BSP.
# The "ut_bsp" library is a simple startup BSP that can be used for unit testing
# This removes the need to use the "real" CFE PSP and also provides the necessary 
# UT output functions that UT assert may rely upon to report test messages 
if (NOT DEFINED UT_OSAL_BSPTYPE)
    if (DEFINED OSAL_SYSTEM_BSPTYPE)
      set(UT_OSAL_BSPTYPE ${OSAL_SYSTEM_BSPTYPE})
    elseif(DEFINED CFE_SYSTEM_PSPNAME)
      # assume an OSAL bsp with the same name exists
      set(UT_OSAL_BSPTYPE ${CFE_SYSTEM_PSPNAME})
    endif (DEFINED OSAL_SYSTEM_BSPTYPE)
endif (NOT DEFINED UT_OSAL_BSPTYPE)

# Recompute the compiler flags for UT builds, as this may be using a different BSP
osal_assemble_compiler_flags(${OSAL_SELECTED_OSTYPE} ${UT_OSAL_BSPTYPE})

if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/${UT_OSAL_BSPTYPE}/ut-src)
    aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/${UT_OSAL_BSPTYPE}/ut-src UT_BSPFILES)
    add_library(ut_bsp STATIC EXCLUDE_FROM_ALL ${UT_BSPFILES})    
    set_target_properties(ut_bsp PROPERTIES COMPILE_DEFINITIONS "_UNIT_TEST_")
endif()

if (ENABLE_UNIT_TESTS)

  if (NOT TARGET ut_bsp)
    # This was originally a warning, but it produces some really weird behavior of
    # subsequent builds if "ENABLE_UNIT_TESTS" is true but the associated OSAL libraries
    # and targets are not available.  More prudent to make this a fatal error, and the
    # user can re-run with "ENABLE_UNIT_TESTS" set false if that is what they want.
    message(FATAL_ERROR "No BSP available for unit tests.  Tests cannot be built.")
  endif()
  
  # The "ut_osapi_stubs" library contains "stub" functions of the OSAL API calls, used for
  # testing application code built on top of OSAL.
  set(UT_OSAPI_STUB_SRCFILES 
    src/ut-stubs/utstub-helpers.c
    src/ut-stubs/osapi-utstub-binsem.c 
    src/ut-stubs/osapi-utstub-clock.c 
    src/ut-stubs/osapi-utstub-common.c 
    src/ut-stubs/osapi-utstub-countsem.c 
    src/ut-stubs/osapi-utstub-dir.c 
    src/ut-stubs/osapi-utstub-errors.c
    src/ut-stubs/osapi-utstub-file.c 
    src/ut-stubs/osapi-utstub-filesys.c 
    src/ut-stubs/osapi-utstub-fpu.c 
    src/ut-stubs/osapi-utstub-heap.c 
    src/ut-stubs/osapi-utstub-idmap.c
    src/ut-stubs/osapi-utstub-interrupts.c 
    src/ut-stubs/osapi-utstub-module.c 
    src/ut-stubs/osapi-utstub-mutex.c 
    src/ut-stubs/osapi-utstub-network.c 
    src/ut-stubs/osapi-utstub-printf.c 
    src/ut-stubs/osapi-utstub-queue.c 
    src/ut-stubs/osapi-utstub-select.c 
    src/ut-stubs/osapi-utstub-sockets.c 
    src/ut-stubs/osapi-utstub-task.c 
    src/ut-stubs/osapi-utstub-time.c
    src/ut-stubs/osapi-utstub-timebase.c)
         
  add_library(ut_osapi_stubs STATIC ${UT_OSAPI_STUB_SRCFILES})
  target_link_libraries(ut_osapi_stubs ut_assert)

  add_subdirectory(src/tests tests)
  add_subdirectory(src/unit-tests unit-tests)

  if (BUILD_PARENT)
    # Append _UNIT_TEST_ definition to the project-wide unit test CFLAGS
    # This allows the UT implementations to add extra UT hooks/debug where needed
    # For compatibility this also needs to expand the include path to include UT assert headers
    # (this assumes a gcc style compiler)
    set(UT_C_FLAGS "-D_UNIT_TEST_ -I${CMAKE_CURRENT_SOURCE_DIR}/ut_assert/inc ${BSP_UT_C_FLAGS}" PARENT_SCOPE)
  endif (BUILD_PARENT)

endif (ENABLE_UNIT_TESTS)

