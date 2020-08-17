######################################################################
#
# CMAKE recipe for the OSAL stub library
#
######################################################################

#
# This works in conjunction with the UT Assert library to
# provide "stub" versions of all calls in the OSAL public API.
#

# NOTE: There is no separate public include directory for the stubs.
# By definition, the stubs must implement the same public API that the
# normal OSAL library implements.  Therefore, only the standard OSAL
# header files are used.
add_library(ut_osapi_stubs STATIC
    utstub-helpers.c
    osapi-utstub-binsem.c
    osapi-utstub-clock.c
    osapi-utstub-common.c
    osapi-utstub-countsem.c
    osapi-utstub-dir.c
    osapi-utstub-errors.c
    osapi-utstub-file.c
    osapi-utstub-filesys.c
    osapi-utstub-fpu.c
    osapi-utstub-heap.c
    osapi-utstub-idmap.c
    osapi-utstub-interrupts.c
    osapi-utstub-module.c
    osapi-utstub-mutex.c
    osapi-utstub-network.c
    osapi-utstub-printf.c
    osapi-utstub-queue.c
    osapi-utstub-select.c
    osapi-utstub-sockets.c
    osapi-utstub-task.c
    osapi-utstub-time.c
    osapi-utstub-timebase.c
)

# Some of the internal API definitions in stubs are based on
# types/definitions in the shared layer internal header file.
target_include_directories(ut_osapi_stubs PRIVATE
    ${OSAL_SOURCE_DIR}/src/os/shared/inc
)

# These stubs must always link to UT Assert.
# This also implicitly adds the path to the UT Assert header files.
target_link_libraries(ut_osapi_stubs ut_assert)



