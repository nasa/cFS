# This indicates where to install target binaries created during the build
set(INSTALL_SUBDIR "cf")

# Additional preprocessor macro definitions to identify this platform
add_definitions("-D_VXWORKS_OS_")
add_definitions("-D__PPC__")
add_definitions("-DMCP750")

include_directories($ENV{WIND_BASE}/target/h)
include_directories($ENV{WIND_BASE}/target/h/wrn/coreip)
include_directories($ENV{WIND_BASE}/target/config/mcp750)
