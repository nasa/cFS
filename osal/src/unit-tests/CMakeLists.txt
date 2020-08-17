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

enable_testing()

include_directories(${UTASSERT_SOURCE_DIR}/inc)
include_directories(inc)

add_subdirectory(oscore-test)
add_subdirectory(osloader-test)
add_subdirectory(osfilesys-test)
add_subdirectory(osfile-test)
add_subdirectory(osnetwork-test)
add_subdirectory(ostimer-test)

