######################################################################
#
# CMAKE build recipe for PC-RTEMS Board Support Package (BSP)
#
######################################################################

add_library(osal_pc-rtems_impl OBJECT
	src/bsp_start.c
	src/bsp_voltab.c
	src/bsp_console.c
)

# This BSP only works with "rtems" OS layer.
# Confirming this reduces risk of accidental misconfiguration
set(OSAL_EXPECTED_OSTYPE    "rtems" PARENT_SCOPE)
