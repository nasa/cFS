##########################################################################
#
# Build options for "pc-rtems" BSP
#
##########################################################################

# Link the RTEMS BSP with the "rtemscpu" system library
target_link_libraries(osal_bsp
    rtemscpu
)

# This indicates where to stage target binaries created during the build
# It should reflect the _real_ location of the persistent storage path used by
# the BSP which is intended to be used for runtime modules or files.
set(OSAL_BSP_STAGING_INSTALL_DIR "eeprom")
