# This example toolchain file describes the cross compiler to use for
# the target architecture indicated in the configuration file.

# Basic cross system configuration
set(CMAKE_SYSTEM_NAME VxWorks-CFE)
set(CMAKE_SYSTEM_PROCESSOR ppc)
set(CMAKE_SYSTEM_VERSION 6.9)
set(VXWORKS_GCC_VERSION 4.3.3)
set(VXWORKS_HOST_VERSION x86-linux2)
set(WIND_HOME_DFL "$ENV{WIND_HOME}")
if(NOT WIND_HOME_DFL)
  set(WIND_HOME_DFL "/opt/WindRiver")
endif(NOT WIND_HOME_DFL)
set(WIND_HOME "${WIND_HOME_DFL}" CACHE PATH "Wind River installation directory")
set(VXWORKS_TOOLS_PREFIX "${WIND_HOME}/gnu/${VXWORKS_GCC_VERSION}-vxworks-${CMAKE_SYSTEM_VERSION}/${VXWORKS_HOST_VERSION}")


# specify the cross compiler - adjust accord to compiler installation
# This uses the compiler-wrapper toolchain that buildroot produces
SET(SDKHOSTBINDIR               "${VXWORKS_TOOLS_PREFIX}/bin")
set(TARGETSUFFIX                "${CMAKE_SYSTEM_PROCESSOR}")
#set(VXWORKS_BSP_C_FLAGS           "-march=i686 -mtune=i686 -fno-common")
#set(VXWORKS_BSP_CXX_FLAGS         ${VXWORKS_BSP_C_FLAGS})

SET(CMAKE_C_COMPILER            "${SDKHOSTBINDIR}/cc${TARGETSUFFIX}")
SET(CMAKE_CXX_COMPILER          "${SDKHOSTBINDIR}/c++${TARGETSUFFIX}")
SET(CMAKE_LINKER                "${SDKHOSTBINDIR}/ld${TARGETSUFFIX}")
SET(CMAKE_ASM_COMPILER          "${SDKHOSTBINDIR}/as${TARGETSUFFIX}")
SET(CMAKE_AR                    "${SDKHOSTBINDIR}/ar${TARGETSUFFIX}")
SET(CMAKE_OBJDUMP               "${SDKHOSTBINDIR}/objdump${TARGETSUFFIX}")
SET(CMAKE_RANLIB                "${SDKHOSTBINDIR}/ranlib${TARGETSUFFIX}")

# Note that CEXP is not a shared library loader - it will not support code compiled with -fPIC
# Also exception handling is very iffy.  These two options disable eh_frame creation.
#set(CMAKE_C_COMPILE_OPTIONS_PIC -fno-exceptions -fno-asynchronous-unwind-tables)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM   NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY   ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE   ONLY)

SET(CMAKE_PREFIX_PATH                   /)

# these settings are specific to cFE/OSAL and determines which
# abstraction layers are built when using this toolchain
SET(CFE_SYSTEM_PSPNAME                  mcp750-vxworks)
SET(OSAL_SYSTEM_BSPTYPE                 mcp750-vxworks)
SET(OSAL_SYSTEM_OSTYPE                  vxworks)

