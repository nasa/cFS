# This example toolchain file describes the cross compiler to use for
# the target architecture indicated in the configuration file.

# In this sample application, the cross toolchain is configured to
# use the QNX cross compiler for the 64-bit ARM processor (such as the
# ARM Cortex-A72 processor on the "raspberry pi 4" development board).

# Basic cross system configuration
SET(CMAKE_SYSTEM_NAME        QNX)
SET(CMAKE_SYSTEM_VERSION     1)
SET(CMAKE_SYSTEM_PROCESSOR   aarch64le)
SET(QNX_SDP_ARCH             ${CMAKE_SYSTEM_PROCESSOR})

# Basic QNX system configuration
include("${CMAKE_CURRENT_LIST_DIR}/toolchain-qnx-sdp-init.cmake" REQUIRED)

# The QNX toolchain relies on several environment variables,
# which should be set already by an environment setup script.
MESSAGE("Toolchain Selected: aarch64le-qnx")
MESSAGE("CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
MESSAGE("QNX_CONFIGURATION = $ENV{QNX_CONFIGURATION}")
