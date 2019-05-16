# This indicates where to install target binaries created during the build
set(INSTALL_SUBDIR "cf")

# Some upper-level code may be gated on _LINUX_OS_ being defined
set(OSAL_C_FLAGS "${OSAL_C_FLAGS} -D_VXWORKS_OS_ -D__PPC__ -DMCP750")

include_directories($ENV{WIND_BASE}/target/h)
include_directories($ENV{WIND_BASE}/target/h/wrn/coreip)
include_directories($ENV{WIND_BASE}/target/config/mcp750)
