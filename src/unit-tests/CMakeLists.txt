# CMake snippet for OSAL unit tests

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
         
