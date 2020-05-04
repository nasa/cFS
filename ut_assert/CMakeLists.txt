######################################################################
#
# CMAKE recipe for the UT assert library
#
######################################################################

#
# The "ut_assert" library is the core GSFC-provided unit test library
#
project(UT_ASSERT C)

# The "ut_assert" library is usable by ANY and ALL subsystem(s) that need
# to do unit testing of any kind.  This library implements an OSAL application
# that contains APIs to aid in unit testing.  It provides the OS_Application_Startup
# and OS_Application_Run functions that a normal standalone OSAL application would.

# It uses the same OSAL BSP as a normal application would use to provide the basic
# startup procedure and text message output abstractions.

# NOTE: This library does NOT include any stub functions here, as the configuration
# of stubs vs. real implementations are specific to the unit being tested.  All
# stub functions are compiled as separate libraries.

add_library(ut_assert STATIC EXCLUDE_FROM_ALL
    src/utassert.c
    src/utbsp.c
    src/utlist.c
    src/utstubs.c
    src/uttest.c
    src/uttools.c
)

target_include_directories(ut_assert PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
)
target_include_directories(ut_assert PRIVATE
    "${OSAL_SOURCE_DIR}/src/bsp/shared/inc"
)
target_compile_definitions(ut_assert PUBLIC
    "_UNIT_TEST_"
)
target_link_libraries(ut_assert osal_bsp)



