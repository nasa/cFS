# --------------------------------------
# OSAL SHARED LAYER FOR COVERAGE TESTING
# --------------------------------------

# This CMake recipe snippet builds the _actual_ OSAL shared API (not a stub)
# However it differs from the regular non-UT build in the following ways:
# - Each sub-module is compiled to a separate "OBJECT" library
#       this simplifies the testrunner linking by allow only one specific
#       object file to be linked into each test
# - Each sub-module is wrapped to divert C library calls to the 
#       stub (OCS) counterpart 
# - UT_C_FLAGS are enabled to include any code coverage instrumentation  

# the "overrides" dir contains empty versions of the C-library include files.
include_directories(BEFORE ${OSALCOVERAGE_SOURCE_DIR}/ut-stubs/inc/overrides)

foreach(MODULE ${MODULE_LIST})
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/ut-osapi-${MODULE}.c)
    add_library(ut_${SETNAME}_${MODULE} OBJECT src/ut-osapi-${MODULE}.c)
    set_target_properties(ut_${SETNAME}_${MODULE} PROPERTIES COMPILE_FLAGS "${UT_C_FLAGS}")
  endif ()
endforeach()

