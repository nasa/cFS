######################################################################
#
# CMAKE build recipe for MCP750 Board Support Package (BSP)
#
######################################################################

add_library(osal_mcp750-vxworks_impl OBJECT 
	src/bsp_start.c
	src/bsp_voltab.c
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

add_library(ut_bsp STATIC EXCLUDE_FROM_ALL
	ut-src/bsp_ut.c
	ut-src/bsp_ut_voltab.c
)
target_include_directories(ut_bsp PRIVATE ${UT_ASSERT_SOURCE_DIR}/inc)


