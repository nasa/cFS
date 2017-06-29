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
  set_target_properties(${TGTNAME} PROPERTIES LINK_FLAGS "${UT_C_FLAGS} -u OS_VolumeTable")  
  target_link_libraries(${TGTNAME} ut_bsp osal)
  add_test(${TGTNAME} ${TGTNAME})
  foreach(TGT ${INSTALL_TARGET_LIST})
    install(TARGETS ${TGTNAME} DESTINATION ${TGT}/${UT_INSTALL_SUBDIR})
  endforeach()

endfunction(add_osal_ut_exe)

# HAVE_STDINT enables use of the ISO C99 stdint.h file
# This should always be used whenever available, as it is the most accurate way
# to obtain the proper fixed-width type definitions.
find_file(STDINT_H_FILE stdint.h ONLY_CMAKE_FIND_ROOT_PATH)
if (STDINT_H_FILE)
  set(OSAL_C_FLAGS "${OSAL_C_FLAGS} -D_HAVE_STDINT_")
endif (STDINT_H_FILE)

# OSAL_SYSTEM_OSTYPE indicates which of the BSP packages to include
# This is required and must be defined
if (NOT OSAL_SYSTEM_OSTYPE)
  message(FATAL_ERROR "OSAL_SYSTEM_OSTYPE must be set to the appropriate OS")
endif (NOT OSAL_SYSTEM_OSTYPE)

# Include the OS-specific compiler options
include(src/os/${OSAL_SYSTEM_OSTYPE}/build_options.cmake OPTIONAL)
message(STATUS "OSAL Selection: ${OSAL_SYSTEM_OSTYPE}")

# OSAL_SYSTEM_BSPTYPE indicates which of the BSP packages to include
# It is optional to build this library (e.g. cFE includes its own PSP)
# If used, this should also define the installation location for binaries, 
# which depends on the bsp volume table
# This may also set more "-D" options to the compiler command in order to 
# build code properly for this OS
# This should not be included if CFE_SYSTEM_PSPNAME is given, since the PSP
# replaces the OSAL BSP
if (OSAL_SYSTEM_BSPTYPE AND NOT CFE_SYSTEM_PSPNAME)
  if (IS_DIRECTORY ${OSAL_SOURCE_DIR}/src/bsp/${OSAL_SYSTEM_BSPTYPE})
    include(src/bsp/${OSAL_SYSTEM_BSPTYPE}/make/build_options.cmake OPTIONAL)
    message(STATUS "OSAL BSP Selection: ${OSAL_SYSTEM_BSPTYPE}")
  else()
    message(FATAL_ERROR "Error: ${OSAL_SYSTEM_BSPTYPE} is not a valid BSP")
  endif()
endif (OSAL_SYSTEM_BSPTYPE AND NOT CFE_SYSTEM_PSPNAME)

# Cache any user-specified C flags so they will be retained in future builds
# These can be specified either through cmake command line (e.g. -DUSER_C_FLAGS=-Werror) or
# through an environment variable (e.g. OSAL_USER_C_FLAGS=-Werror cmake ...)
set(OSAL_USER_C_FLAGS "$ENV{OSAL_USER_C_FLAGS}" CACHE STRING "User-specified C flags for OSAL build")

# Set up the final set of C flags for the build.  This is the sum of what the toolchain needs,
# what the BSP/PSP has added and anything else that the user has asked for.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OSAL_USER_C_FLAGS} ${OSAL_C_FLAGS}")

# At a mimimum, also compile with -Wall to show extra warnings.  Only do this if nothing
# added it already (prevents adding this twice in case the User/BSP/PSP already specified it)
if (NOT CMAKE_C_FLAGS MATCHES "-Wall")
  set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
endif(NOT CMAKE_C_FLAGS MATCHES "-Wall")

message(STATUS "OSAL Compile Definitions: ${CMAKE_C_FLAGS}") 
message(STATUS "OSAL Link Libraries: ${OSAL_LINK_LIBS}") 

# Use the OSAL shared include directory
include_directories(src/os/inc)

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
aux_source_directory(src/os/${OSAL_SYSTEM_OSTYPE} OSALFILES)
if(JPH_ENABLE_OSAL_NG)
  aux_source_directory(src/os/shared OSALFILES)
endif()
if (OSAL_SYSTEM_BSPTYPE AND NOT CFE_SYSTEM_PSPNAME)
  aux_source_directory(src/bsp/${OSAL_SYSTEM_BSPTYPE}/src BSPFILES)
endif(OSAL_SYSTEM_BSPTYPE AND NOT CFE_SYSTEM_PSPNAME)

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
  set(INSTALL_SUBDIR ${INSTALL_SUBDIR} PARENT_SCOPE)
endif (BUILD_PARENT)


if (ENABLE_UNIT_TESTS)

  # in order to run the unit test, we need a BSP.
  # when building as part of CFE the OSAL BSP is typically NOT used,
  # but in case of a unit test if one exists then we include it for this
  # we need to compile OSAL again anyway to include the coverage options
  if (DEFINED OSAL_SYSTEM_BSPTYPE)
    set(UT_OSAL_BSPTYPE ${OSAL_SYSTEM_BSPTYPE})
  elseif(DEFINED CFE_SYSTEM_PSPNAME)
    # assume an OSAL bsp with the same name exists
    set(UT_OSAL_BSPTYPE ${CFE_SYSTEM_PSPNAME})
  else()
    set(UT_OSAL_BSPTYPE)
  endif()

  set(UT_OSALFILES)
  
  if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/${UT_OSAL_BSPTYPE}/ut-src)  
    aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/${UT_OSAL_BSPTYPE}/ut-src UT_BSPFILES)
    include(src/bsp/${UT_OSAL_BSPTYPE}/make/build_options.cmake OPTIONAL)
  else()
    # This was originally a warning, but it produces some really weird behavior of
    # subsequent builds if "ENABLE_UNIT_TESTS" is true but the associated OSAL libraries
    # and targets are not available.  More prudent to make this a fatal error, and the
    # user can re-run with "ENABLE_UNIT_TESTS" set false if that is what they want.
    message(FATAL_ERROR "No BSP available for unit tests.  Tests cannot be built.")
  endif()
  
  # Generate the library targets to support unit testing
  
  # NOTE: The "ut_assert" and "ut_bsp" libraries are usable by ANY and ALL subsystem(s) that need 
  # to do unit testing of any kind.  These provide the basic startup procedure, test message output
  # abstractions, and all the "assert" calls to perform unit testing.  They specifically do NOT
  # include any stub functions, as the configuration of stubs vs. real implementations are specific
  # to the unit being tested.
   
  # The "ut_bsp" library is a simple startup BSP that can be used for unit testing
  # This removes the need to use the "real" CFE PSP and also provides the necessary 
  # UT output functions that UT assert may rely upon to report test messages 
  # This is the OSAL BSP but with the _UNIT_TEST_ macro defined so it may have UT-specific features
  add_library(ut_bsp STATIC ${UT_BSPFILES})    
  set_target_properties(ut_bsp PROPERTIES COMPILE_DEFINITIONS "_UNIT_TEST_")
  target_link_libraries(ut_bsp ${OSAL_LINK_LIBS})
    
  # The "utassert" library is the core GSFC-provided unit test library
  # It is only the generic framework and does _not_ include any of the specific stub/hook functions  
  # It is built as static library so it may be linked with either a "real" implementation or a stub 
  # library (see next targets) or some combination of those as the test cases dictate.
  aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ut_assert/src UT_ASSERT_FILES)
  add_library(ut_assert STATIC ${UT_ASSERT_FILES})
  target_link_libraries(ut_bsp ut_assert)

  # Append _UNIT_TEST_ definition to the project-wide unit test CFLAGS
  # This allows the UT implementations to add extra UT hooks/debug where needed
  set(UT_C_FLAGS "-D_UNIT_TEST_ ${UT_C_FLAGS}")
  

  # The "ut_osapi_stubs" library contains "stub" functions of the OSAL API calls, used for
  # testing application code built on top of OSAL.
  add_library(ut_osapi_stubs STATIC src/ut-stubs/osapi_stubs.c src/ut-stubs/osfileapi_stubs.c)
  target_link_libraries(ut_osapi_stubs ut_assert)

  # Compile the OSAL test code
  add_subdirectory(src/tests tests)
  add_subdirectory(src/unit-tests unit-tests)
    
  # If this is part of a larger project, also export the unit test CFLAGS
  if (BUILD_PARENT)
    set(UT_C_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR}/ut_assert/inc ${UT_C_FLAGS}" PARENT_SCOPE)
  endif (BUILD_PARENT)

endif (ENABLE_UNIT_TESTS)

