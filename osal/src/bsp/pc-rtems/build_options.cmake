##########################################################################
#
# Build options for "pc-rtems" BSP
#
##########################################################################

# Link the RTEMS BSP with the "rtemscpu" system library
target_link_libraries(osal_bsp
    rtemscpu
)
