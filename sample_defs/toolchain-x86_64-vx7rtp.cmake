#
# Cross compiler toolchain file for x86_64 QEMU VxWorks 7 RTP
#

#
# This toolchain should work for both the Public SDKs and the Licensed VIP/VSB projects
# It does not currently work with a generated SDK from the licensed installation.
#
# The toolchain depends on one environment variable for the public SDKs and
# a few environment variables for the licensed VSB/VIP projects.
#
# See the file toolchain-common-vx7rtp.cmake for information about the environment variables
#  needed.
#

set(VXWORKS_RTP TRUE)
set(CMAKE_SYSTEM_NAME VxWorks-CFE)
set(CMAKE_SYSTEM_VERSION 7)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

add_definitions("-D__x86_64__")
add_definitions("-DX86QEMU")

# Common VxWorks RTP system configuration
include("${CMAKE_CURRENT_LIST_DIR}/toolchain-common-vx7rtp.cmake" REQUIRED)

MESSAGE("Toolchain Selected: x86_64-vx7rtp")
MESSAGE("CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
