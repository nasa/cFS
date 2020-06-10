######################################################################
#
# CMAKE build recipe for Generic VxWorks Board Support Package (BSP)
#
######################################################################

add_library(osal_generic-vxworks_impl OBJECT
	src/bsp_start.c
	src/bsp_console.c
)

# This BSP only works with "vxworks" OS layer.
# Confirming this reduces risk of accidental misconfiguration
set(OSAL_EXPECTED_OSTYPE    "vxworks" PARENT_SCOPE)
