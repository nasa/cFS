######################################################################
#
# CMAKE build recipe for MCP750 Board Support Package (BSP)
#
######################################################################

add_library(osal_mcp750-vxworks_impl OBJECT
	src/bsp_start.c
	src/bsp_voltab.c
	src/bsp_console.c
)

target_include_directories(osal_mcp750-vxworks_impl PUBLIC
    $ENV{WIND_BASE}/target/h
    $ENV{WIND_BASE}/target/h/wrn/coreip
    $ENV{WIND_BASE}/target/config/mcp750
)

# NOTE: the __PPC__ and MCP750 macros are referenced in some system headers.
# therefore all code compiled for this platform should always define these symbols.
target_compile_definitions(osal_mcp750-vxworks_impl PUBLIC
    "__PPC__"
    "MCP750"
)

# This BSP only works with "vxworks" OS layer.
# Confirming this reduces risk of accidental misconfiguration
set(OSAL_EXPECTED_OSTYPE    "vxworks" PARENT_SCOPE)
