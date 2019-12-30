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

# For VxWorks and RTEMS targets there are still a few slight
# variances that need to be accounted for, mainly in the file
# names and/or directory structures that the test case uses.
set(UT_COMPILEDEFS_vxworks  "_VXWORKS_OS_")
set(UT_COMPILEDEFS_rtems    "_RTEMS_OS_")
set(UT_COMPILEDEFS_posix    "_POSIX_OS_")
if (DEFINED UT_COMPILEDEFS_${OSAL_SYSTEM_OSTYPE})
    add_definitions(-D${UT_COMPILEDEFS_${OSAL_SYSTEM_OSTYPE}})
endif()

enable_testing()

include_directories(${OSAL_SOURCE_DIR}/ut_assert/inc)
include_directories(inc)

add_subdirectory(oscore-test)
add_subdirectory(osloader-test)
add_subdirectory(osfilesys-test)
add_subdirectory(osfile-test)
add_subdirectory(osnetwork-test)
add_subdirectory(ostimer-test)
         
