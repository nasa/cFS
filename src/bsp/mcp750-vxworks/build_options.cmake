##########################################################################
#
# Build options for "mcp750-vxworks" BSP
#
##########################################################################

# This indicates where to stage target binaries created during the build
set(OSAL_BSP_STAGING_INSTALL_DIR "CF:0")

# The "-u" switch is required to ensure that "ldppc" pulls in the OS_BSPMain entry point
target_link_libraries(osal_bsp -uOS_BSPMain)
