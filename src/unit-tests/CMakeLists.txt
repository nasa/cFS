# CMake snippet for OSAL unit tests
# 
# NOTE:
# This set of unit tests only includes platform support where
# one of  OSP_ARINC653, _LINUX_OS_ or _VXWORKS_OS_ are defined.
#
# It does not include support for other options like _RTEMS_OS_
# These test do not currently use the same UT assert framework
# that the other tests use.  If/when these tests migrate to using the
# same framework then this style of platform support via macros is
# not necessary and other OS's like RTEMS should work.
#


# This first routine checks for overlap between
# the set of supported OS type macro definitions
# and the set of macros defined in CMAKE_C_FLAGS.
# 
# If no match is found then it means these tests do not
# contain support for that platform and attempting compilation 
# will fail.
set(TEST_SUPPORTED_COMPILEDEF_LIST
    -D_LINUX_OS_
    -D_VXWORKS_OS_ 
    -DOSP_ARINC653)
    
string(REGEX MATCHALL "-D([A-Z_]+)" ALL_COMPILEDEFS "${CMAKE_C_FLAGS}")
set(OSTYPE_INDEX -1)
foreach(DEF ${ALL_COMPILEDEFS})
    list(FIND TEST_SUPPORTED_COMPILEDEF_LIST "${DEF}" OSTYPE_INDEX)
    if (NOT OSTYPE_INDEX LESS 0)
        # Matching index found - OK
        break()
    endif()
endforeach()

if (OSTYPE_INDEX LESS 0)
    message(STATUS "Extended tests do not support ${OSAL_SYSTEM_OSTYPE}, skipping build")
    return()
endif()

enable_testing()

add_definitions(-DUT_VERBOSE)
add_definitions(-D_OSAL_UNIT_TEST_)

function(add_stubs OUTVAR)
  set(RESULT)
  foreach(STUB ${ARGN})
    list(APPEND RESULT ${OSAL_SOURCE_DIR}/src/unit-tests/shared/ut_${STUB}_stubs.c)      
  endforeach()
  set(${OUTVAR} ${RESULT} PARENT_SCOPE)
endfunction(add_stubs)

macro(add_stubs_except OUTVAR)
  foreach(STUB ${STUBFILES})
    list(FIND ${ARGN} ${STUB} ISEXCL)
    if (ISEXCL GREATER -1)
      add_stubs(${OUTVAR} ${STUB})
    endif(ISEXCL GREATER -1)
  endforeach()
endmacro(add_stubs_except)

set(OSAL_TEST_MODULES core)

# filesys file loader network printf timer) 
file(GLOB STUB_SRCS shared/ut_*_stubs.c)
set(STUBFILES)
foreach(STUB ${STUB_SRCS})
  string(REGEX REPLACE ".*/shared/ut_(.*)_stubs\\.c$" "\\1" STUBFILE "${STUB}")
  list(APPEND STUBFILES ${STUBFILE})
endforeach()

include_directories(shared)
add_subdirectory(oscore-test)
add_subdirectory(osloader-test)
add_subdirectory(osfilesys-test)
add_subdirectory(osfile-test)
add_subdirectory(osnetwork-test)
add_subdirectory(ostimer-test)
         
