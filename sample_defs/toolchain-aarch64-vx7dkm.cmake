#
# Cross compiler toolchain file for aarch64 Raspberry Pi 4 VxWorks 7 DKM mode 
#

#
# This toolchain should work for both the Public SDKs and the Licensed VIP/VSB projects
# It does not currently work with a generated SDK from the licensed installation.
#
# The toolchain depends on one environment variable for the public SDKs and
# a few environment variables for the licensed VSB/VIP projects.
#
# See the file toolchain-common-vx7dkm.cmake for information about the environment variables
#  needed.
#

set(VXWORKS_DKM TRUE)
set(CMAKE_SYSTEM_NAME VxWorks-CFE)
set(CMAKE_SYSTEM_VERSION 7)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

add_definitions("-D__aarch64__")
add_definitions("-DVXSDK_RPI4")

# Common VxWorks DKM system configuration
include("${CMAKE_CURRENT_LIST_DIR}/toolchain-common-vx7dkm.cmake" REQUIRED)

MESSAGE("Toolchain Selected: aarch64-vx7dkm")
MESSAGE("CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
