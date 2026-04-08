# This example toolchain file describes the cross compiler to use for
# the target architecture indicated in the configuration file.

# Note this is based on the vendor-supplied ARM cross compiler
# downloadable from here: 
# https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

# Basic cross system configuration
set(CMAKE_SYSTEM_NAME       Linux)
set(CMAKE_SYSTEM_PROCESSOR  arm)
set(CMAKE_SYSTEM_VERSION    1)

set(XTOOL_BASE_DIR "/opt")

# specify the cross compiler - adjust accord to compiler installation
SET(TOOLCHAIN_VARIANT "arm-gnu-toolchain-11.3.rel1-x86_64-arm-none-linux-gnueabihf"
    CACHE STRING "Specific compiler variant to use from the tools repo")

SET(TOOLCHAIN_TOP "${XTOOL_BASE_DIR}/${TOOLCHAIN_VARIANT}"
    CACHE PATH "Location of the ARM cross toolchain to use")

set(SDKHOSTBINDIR               "${TOOLCHAIN_TOP}/bin")
set(SDKTARGETSYSROOT            "${TOOLCHAIN_TOP}/arm-none-linux-gnueabihf/libc")
set(TARGETPREFIX                "arm-none-linux-gnueabihf-")

SET(CMAKE_C_COMPILER            "${SDKHOSTBINDIR}/${TARGETPREFIX}gcc")
SET(CMAKE_CXX_COMPILER          "${SDKHOSTBINDIR}/${TARGETPREFIX}g++")
#SET(CMAKE_LINKER                "${SDKHOSTBINDIR}/${TARGETPREFIX}ld")
SET(CMAKE_ASM_COMPILER          "${SDKHOSTBINDIR}/${TARGETPREFIX}as")
SET(CMAKE_STRIP                 "${SDKHOSTBINDIR}/${TARGETPREFIX}strip")
SET(CMAKE_NM                    "${SDKHOSTBINDIR}/${TARGETPREFIX}nm")
SET(CMAKE_AR                    "${SDKHOSTBINDIR}/${TARGETPREFIX}ar")
SET(CMAKE_OBJDUMP               "${SDKHOSTBINDIR}/${TARGETPREFIX}objdump")
SET(CMAKE_OBJCOPY               "${SDKHOSTBINDIR}/${TARGETPREFIX}objcopy")

SET(CMAKE_FIND_ROOT_PATH ${SDKTARGETSYSROOT})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM   NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY   ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE   ONLY)

SET(CMAKE_PREFIX_PATH                   /)

# these settings are specific to cFE/OSAL and determines which
# abstraction layers are built when using this toolchain
SET(CFE_SYSTEM_PSPNAME                  pc-linux)
