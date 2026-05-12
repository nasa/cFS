# This example toolchain file describes the cross compiler to use for
# the target architecture indicated in the configuration file.

# In this sample application, the cross toolchain is configured to
# use a cross compiler for the PowerPC 440 processor which is used on
# the IBM "Bamboo" development board.  The GCC toolchain is named
# accordingly and assumed to be installed in "/opt/x-tools".

# Basic cross system configuration
SET(PPC440_BR_TOP "$ENV{PPC440_BR_TOP}"
    CACHE PATH "Top directory for Linux buildroot filesystem project")

SET(CMAKE_SYSTEM_NAME			Linux)
SET(CMAKE_SYSTEM_VERSION		1)
SET(CMAKE_SYSTEM_PROCESSOR		powerpc)

# specify the cross compiler - adjust accord to compiler installation
# This uses the compiler-wrapper toolchain that buildroot produces
SET(CMAKE_C_COMPILER			"${PPC440_BR_TOP}/host/usr/bin/powerpc-440_softfp-linux-gnu-gcc")
SET(CMAKE_CXX_COMPILER			"${PPC440_BR_TOP}/host/usr/bin/powerpc-440_softfp-linux-gnu-g++")

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH		"/opt/x-tools/powerpc-440_softfp-linux-gnu/powerpc-440_softfp-linux-gnu/sysroot"
                                "${PPC440_BR_TOP}/staging")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM	NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY	ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE	ONLY)

# these settings are specific to cFE/OSAL and determines
# which abstraction layers are built when this toolchain is used
SET(CFE_SYSTEM_PSPNAME			pc-linux)
SET(OSAL_SYSTEM_OSTYPE			posix)
