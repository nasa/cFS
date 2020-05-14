##########################################################################
#
# Build options for "mcp750-vxworks" PSP
# This file specifies any global-scope compiler options when using this PSP
#
##########################################################################

# This indicates where to install target binaries created during the build
# Note - this should be phased out in favor of the staging dir from OSAL BSP
set(INSTALL_SUBDIR "cf")

# Some upper-level code may be gated on _VXWORKS_OS_ being defined
# This is for compatibility with older build scripts which defined this symbol,
# but no CFE/OSAL framework code depends on this symbol.
add_definitions("-D_VXWORKS_OS_")


# Use the mcp750-specific VxWorks BSP include directory
# This needs to be globally used, not just private to the PSP, because
# some VxWorks headers reference files contained here.
include_directories(
    ${WIND_BASE}/target/config/mcp750
)

# NOTE: the __PPC__ and MCP750 macros are referenced in some system headers.
# therefore all code compiled for this platform should always define these symbols.
add_definitions("-D__PPC__")
add_definitions("-DMCP750")

set(CFE_PSP_EXPECTED_OSAL_BSPTYPE "generic-vxworks")

