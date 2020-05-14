##########################################################################
#
# Build options for "mcp750-vxworks" BSP
#
##########################################################################

# The "-u" switch is required to ensure that "ldppc" pulls in the OS_BSPMain entry point
target_link_libraries(osal_bsp -uOS_BSPMain)
