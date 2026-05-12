# Common Toolchain file to build cFS for VxWorks 7 Downloadable Kernel Module (DKM) model.
#
# This toolchain file is included by an architecture specific file, such as:
#   toolchain-aarch64-vx7dkm.cmake
#   toolchain-x86_64-vx7dkm.cmake
#
# The toolchain files can be used with the VxWorks Public SDKs or the licensed installations.
#
# Building with a licensed VxWorks installation (using a VSB and VIP project)
# ---------------------------------------------------------------------------------
# Normally, VxWorks installations require running an environment setup script to use the tools.
# These toolchain files are compatible with those setup scripts, but you can also build with
# a reduced set of environment variables. It does not seem possible to eliminate the use of
# all environment variables when building with the licensed installation. The compiler stub
# depends on specific environment variables, and it seems to change over releases.
#
# Environment variables needed for licensed VxWorks VSB/VIP builds:
#
#   WIND_HOME - Points to the base directory of the VxWorks installation
#   WIND_BASE - Points to the VxWorks operating system directory within the installation
#   WIND_HOST_TYPE - currently x86-linux2
#   WIND_VX7_HOST_TYPE - currently x86-linux2
#   WIND_CC_SYSROOT - Needs to point to the base directory of the VxWorks Source Build (VSB) project
#
#   Example bash script (note that the '#' chars are not comments in the bash script):
#
#   export WIND_HOME=/tools/WindRiver/23.09
#   export WIND_BASE=$WIND_HOME/vxworks/23.09
#   export WIND_HOST_TYPE=x86-linux2
#   export WIND_VX7_HOST_TYPE=x86_64-linux
#   export WIND_CC_SYSROOT=$HOME/workspace/aarch64-vsb
#
# In the above example, the first 4 environment variables should be consistent, and the
# WIND_CC_SYSROOT will point to the VxWorks Source Build (VSB) project root.
#
# If you wish to run the VxWorks provided env setup, you can use a script like the following:
#
#   Example bash script (note that the '#' chars are not comments in the bash script):
# 
#   WR_ROOT=/tools/WindRiver/23.09
#   eval `$WR_ROOT/wrenv.linux -p vxworks -o print_env -f sh`
#   export WIND_WORKSPACE=$HOME/workspace
#   export VSB_ROOT=$WIND_WORKSPACE/aarch64-vsb
#   export WIND_CC_SYSROOT=$VSB_ROOT
#
# Building with a VxWorks Public SDK 
# ---------------------------------------------------------------------------------
# These toolchain files can also be used with the public SDKs provided by VxWorks.
# In order to build with the public SDK, you can either run an edited setup script
# or export a single environment variable. It is possible that this will
# change with future versions of the Public SDK.
#
#   WIND_SDK_HOME - Points to the base directory of the Public SDK directory
#
#   Example bash script (note that the '#' chars are not comments in the bash script):
#
#   export WIND_SDK_HOME=$HOME/Tools/wrsdk-vxworks7-raspberrypi4b
#
# For the public SDK, once the build system has been generated, the environment variable is not needed.
#
# If you want to use the sdkenv.sh script provided in the Public SDK instead of
# setting up the above environment variable, you must first comment out the line:
# "# export CC=wr-cc".
# When CC is redefined in the sdkenv.sh script, it causes the cFE utility elf2cfetbl
# build to fail.
# ---------------------------------------------------------------------------------
#
if(DEFINED ENV{WIND_HOME})
   # WIND_HOME refers to the top-level product installation directory of the licensed VxWorks product
   set(WIND_HOME "$ENV{WIND_HOME}" CACHE PATH "VxWorks Install Directory")
   set(WIND_BASE "$ENV{WIND_BASE}" CACHE PATH "VxWorks Operating System Directory")
   set(WIND_CC_SYSROOT "$ENV{WIND_CC_SYSROOT}" CACHE PATH "VxWorks SYSROOT")
   set(VXWORKS_TOOLS_PREFIX "${WIND_BASE}/host/x86_64-linux" CACHE PATH "VxWorks Tools Prefix")
elseif(DEFINED ENV{WIND_SDK_HOME})
   # WIND_SDK_HOME refers to the top-level installation directory of the public VxWorks SDK
   set(WIND_HOME "$ENV{WIND_SDK_HOME}" CACHE PATH "VxWorks SDK Install Directory")
   set(WIND_CC_SYSROOT "${WIND_HOME}/vxsdk/sysroot" CACHE PATH "VxWorks SYSROOT")
   set(VXWORKS_TOOLS_PREFIX "${WIND_HOME}/vxsdk/host/x86_64-linux" CACHE PATH "VxWorks Tools Prefix")
else()
   MESSAGE(FATAL_ERROR "WIND_HOME or WIND_SDK_HOME environment variable is not set.")
endif()

SET(CFE_SYSTEM_PSPNAME   "generic-vxworks-dkm")

# Add architecture defs
ADD_DEFINITIONS(-D_WRS_KERNEL)

set(VX_TARGET_TYPE DKM)

# specify the cross compiler - adjust accord to compiler installation
# This uses the compiler-wrapper toolchain that buildroot produces
SET(SDKHOSTBINDIR          "${VXWORKS_TOOLS_PREFIX}/bin")
set(TARGETSUFFIX           "${CMAKE_SYSTEM_PROCESSOR}")
set(VXWORKS_BSP_C_FLAGS     "-dkm")
set(VXWORKS_BSP_CXX_FLAGS   "-dkm")

# set C/C++ compiler
set(CMAKE_C_COMPILER ${SDKHOSTBINDIR}/wr-cc${CMAKE_HOST_EXECUTABLE_SUFFIX})
set(CMAKE_CXX_COMPILER ${SDKHOSTBINDIR}/wr-c++${CMAKE_HOST_EXECUTABLE_SUFFIX})
set(CMAKE_AR ${SDKHOSTBINDIR}/wr-ar${CMAKE_HOST_EXECUTABLE_SUFFIX})
set(CMAKE_NM ${SDKHOSTBINDIR}/wr-nm${CMAKE_HOST_EXECUTABLE_SUFFIX})
set(CMAKE_RANLIB ${SDKHOSTBINDIR}/wr-ranlib${CMAKE_HOST_EXECUTABLE_SUFFIX})
set(CMAKE_OBJCOPY ${SDKHOSTBINDIR}/wr-objcopy${CMAKE_HOST_EXECUTABLE_SUFFIX})
set(CMAKE_LINKER ${SDKHOSTBINDIR}/wr-ld${CMAKE_HOST_EXECUTABLE_SUFFIX})
set(CMAKE_OBJDUMP ${SDKHOSTBINDIR}/wr-objdump${CMAKE_HOST_EXECUTABLE_SUFFIX})
set(CMAKE_READELF ${SDKHOSTBINDIR}/wr-readelf${CMAKE_HOST_EXECUTABLE_SUFFIX})
set(CMAKE_STRIP ${SDKHOSTBINDIR}/wr-strip${CMAKE_HOST_EXECUTABLE_SUFFIX})
set(CMAKE_SIZE ${SDKHOSTBINDIR}/wr-size${CMAKE_HOST_EXECUTABLE_SUFFIX})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM   NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY   ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE   ONLY)

SET(CMAKE_PREFIX_PATH                   /)

# set RTP build flags to compiler
set(CMAKE_C_FLAGS_INIT "-dkm")

include_directories(${WIND_HOME}/vxsdk/sysroot/krnl/h/public)
include_directories(${WIND_HOME}/vxsdk/sysroot/krnl/h/published/UTILS_UNIX)
include_directories(${WIND_HOME}/vxsdk/sysroot/share/h)
