# This example toolchain file describes the cross compiler to use for
# the target architecture indicated in the configuration file.

# In this sample application, the cross toolchain is configured to
# use a cross compiler for the TI AM3359 processor which is used on
# the "beaglebone black" development board (this is an ARM Cortex-A8
# processor).  The GCC toolchain is named accordingly and assumed
# to be installed in "/opt/x-tools".

# Basic cross system configuration
SET(CMAKE_SYSTEM_NAME			Linux)
SET(CMAKE_SYSTEM_VERSION		1)
SET(CMAKE_SYSTEM_PROCESSOR		arm)

# adjust these settings to where the cross compiler actually resides
SET(CMAKE_C_COMPILER			"/opt/x-tools/arm-cortexa8_neon-linux-gnueabi/bin/arm-cortexa8_neon-linux-gnueabi-gcc")
SET(CMAKE_CXX_COMPILER			"/opt/x-tools/arm-cortexa8_neon-linux-gnueabi/bin/arm-cortexa8_neon-linux-gnueabi-g++")

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH		"/opt/x-tools/arm-cortexa8_neon-linux-gnueabi/arm-cortexa8_neon-linux-gnueabi/sysroot")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM	NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY	ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE	ONLY)

# these settings are specific to cFE/OSAL and determines which 
# abstraction layers are built when using this toolchain
# Note that "pc-linux" works fine even though this is not technically a "pc"
SET(CFE_SYSTEM_PSPNAME      "pc-linux")
SET(OSAL_SYSTEM_OSTYPE      "posix")
