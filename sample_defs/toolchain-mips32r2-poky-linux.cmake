# This example toolchain file describes the cross compiler to use for
# the target architecture indicated in the configuration file.

# Basic cross system configuration
SET(CMAKE_SYSTEM_NAME			Linux)
SET(CMAKE_SYSTEM_VERSION		5.0.12)
SET(CMAKE_SYSTEM_PROCESSOR		mips)
SET(POKY_SYSROOT_ID             mips32r2-poky-linux)

SET(TOOLCHAIN_TOP "/opt/poky/${CMAKE_SYSTEM_VERSION}"
    CACHE PATH "Installation directory for toolchain, as produced from SDK")

# specify the cross compiler - adjust accord to compiler installation
# This uses the compiler-wrapper toolchain that buildroot produces
set(SDKTARGETSYSROOT            "${TOOLCHAIN_TOP}/sysroots/${POKY_SYSROOT_ID}")
set(SDKHOSTBINDIR               "${TOOLCHAIN_TOP}/sysroots/x86_64-pokysdk-linux/usr/bin/${CMAKE_SYSTEM_PROCESSOR}-poky-linux")
set(TARGETPREFIX                "${CMAKE_SYSTEM_PROCESSOR}-poky-linux-")
set(CPUTUNEFLAGS                "-meb -mabi=32 -mhard-float -march=mips32r2")

SET(CMAKE_C_COMPILER            "${SDKHOSTBINDIR}/${TARGETPREFIX}gcc")
SET(CMAKE_CXX_COMPILER          "${SDKHOSTBINDIR}/${TARGETPREFIX}g++")
SET(CMAKE_LINKER                "${SDKHOSTBINDIR}/${TARGETPREFIX}ld")
SET(CMAKE_ASM_COMPILER          "${SDKHOSTBINDIR}/${TARGETPREFIX}as")
SET(CMAKE_STRIP                 "${SDKHOSTBINDIR}/${TARGETPREFIX}strip")
SET(CMAKE_NM                    "${SDKHOSTBINDIR}/${TARGETPREFIX}nm")
SET(CMAKE_AR                    "${SDKHOSTBINDIR}/${TARGETPREFIX}ar")
SET(CMAKE_OBJDUMP               "${SDKHOSTBINDIR}/${TARGETPREFIX}objdump")
SET(CMAKE_OBJCOPY               "${SDKHOSTBINDIR}/${TARGETPREFIX}objcopy")

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH                "${SDKTARGETSYSROOT}")
#SET(CMAKE_SYSROOT                       "${SDKTARGETSYSROOT}")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM   NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY   ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE   ONLY)

# these settings are specific to cFE/OSAL and determines
# which abstraction layers are built when this toolchain is used
SET(CFE_SYSTEM_PSPNAME                  pc-linux)

SET(CMAKE_C_FLAGS_INIT "${CPUTUNEFLAGS} --sysroot=${SDKTARGETSYSROOT}"
    CACHE STRING "C Flags required by platform")

SET(CMAKE_CXX_FLAGS_INIT "${CPUTUNEFLAGS} --sysroot=${SDKTARGETSYSROOT}"
    CACHE STRING "C Flags required by platform")
