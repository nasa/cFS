# This toolchain file describes the QNX SDP 7.10/8.0 environment 

# QNX_CONFIGURATION indicates that the QNX environment is setup
set(QNX_CONFIG_ENV "$ENV{QNX_CONFIGURATION}")
if(NOT QNX_CONFIG_ENV)
  MESSAGE(FATAL_ERROR "The QNX environment variables are not present, the env setup script is required, i.e.: source qnxsdp-env.sh")
endif(NOT QNX_CONFIG_ENV)
set(QNX_SDP_HOST   "$ENV{QNX_HOST}")
set(QNX_SDP_TARGET "$ENV{QNX_TARGET}")
set(QNX_SDP_BSP    "$ENV{BSP_ROOT_DIR}")

MESSAGE("QNX_CONFIG_ENV = ${QNX_CONFIG_ENV}")
MESSAGE("QNX_SDP_HOST   = ${QNX_SDP_HOST}")
MESSAGE("QNX_SDP_TARGET = ${QNX_SDP_TARGET}")
MESSAGE("QNX_SDP_BSP    = ${QNX_SDP_BSP}")
MESSAGE("QNX_SDP_ARCH   = ${QNX_SDP_ARCH}")

# adjust these settings to where the cross compiler actually resides
SET(CMAKE_C_COMPILER      "qcc")
SET(CMAKE_CXX_COMPILER    "q++")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM   NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY   NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE   NEVER)

# Dump QCC Predefined Macros
execute_process(COMMAND ${CMAKE_C_COMPILER} -E -dM -xc /dev/null OUTPUT_VARIABLE QCC_PREDEFINED_MACROS)

# Extract QNX Version
string(REGEX MATCH "__QNX__ ([0-9]+)"  _match "${QCC_PREDEFINED_MACROS}")
set(QCC_QNX_VERSION ${CMAKE_MATCH_1})
MESSAGE("QCC_QNX_VERSION = ${QCC_QNX_VERSION}")

# Set SDP Version (setting a target property based on the extracted value)
if (QCC_QNX_VERSION VERSION_GREATER_EQUAL 800)
    set(QNX_SDP_VERSION 800)
else()
    set(QNX_SDP_VERSION 710)
endif()

MESSAGE("QNX_SDP_VERSION = ${QNX_SDP_VERSION}")

# these settings are specific to cFE/OSAL and determines which 
# abstraction layers are built when using this toolchain
SET(CFE_SYSTEM_PSPNAME      "generic-qnx")
SET(OSAL_SYSTEM_BSPTYPE     "generic-qnx")
SET(OSAL_SYSTEM_OSTYPE      "qnx")

# This is for version specific QNX ifdefs needed by the OSAL and PSP
ADD_DEFINITIONS(-DQNX_SDP)

# Set QNX flags for unit test coverage options
set(UT_COVERAGE_COMPILE_FLAGS -pg -Wc,-fprofile-arcs,-ftest-coverage)
set(UT_COVERAGE_LINK_FLAGS    -pg -Wl,-lgcov)

# Update CFLAGS and CXXFLAGS with QNX specific definitions
set(CMAKE_C_FLAGS   "-Vgcc_nto${QNX_SDP_ARCH}     ${CMAKE_C_FLAGS}"   CACHE STRING "Set C Compiler Flags (Select QNX GCC Arch)"   FORCE)
set(CMAKE_CXX_FLAGS "-Vgcc_nto${QNX_SDP_ARCH}_cxx ${CMAKE_CXX_FLAGS}" CACHE STRING "Set C++ Compiler Flags (Select QNX GCC Arch)" FORCE)

include_directories(${QNX_SDP_TARGET}/usr/include)
