#
#      Copyright (c) 2019, United States government as represented by the
#      administrator of the National Aeronautics Space Administration.
#      All rights reserved. This software was created at NASA Goddard
#      Space Flight Center pursuant to government contracts.
#
#      This is governed by the NASA Open Source Agreement and may be used,
#      distributed and modified only according to the terms of that agreement.
#

# --------------------------------------
# OSAL SHARED LAYER FOR COVERAGE TESTING
# --------------------------------------

# This CMake recipe snippet builds the _actual_ OSAL implementation (not a stub)
# However it differs from the regular non-UT build in the following ways:
# - Each sub-module is compiled to a separate "OBJECT" library
#       this simplifies the testrunner linking by allow only one specific
#       object file to be linked into each test
# - Each sub-module is wrapped to divert C library calls to the 
#       stub (OCS) counterpart 
# - UT_C_FLAGS are enabled to include any code coverage instrumentation  

# the "overrides" dir contains empty versions of the C-library include files.
include_directories(${OSALCOVERAGE_SOURCE_DIR}/ut-stubs/inc)
include_directories(${OSALCOVERAGE_SOURCE_DIR}/ut-stubs/override_inc)

foreach(MODULE ${MODULE_LIST})
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/ut-${MODULE}.c)
    add_library(ut_${SETNAME}_${MODULE} OBJECT src/ut-${MODULE}.c)
    set_target_properties(ut_${SETNAME}_${MODULE} PROPERTIES COMPILE_FLAGS "${UT_C_FLAGS}")
  endif ()
endforeach()
  
  
