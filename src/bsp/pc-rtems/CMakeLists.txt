######################################################################
#
# CMAKE build recipe for PC-RTEMS Board Support Package (BSP)
#
######################################################################

add_library(osal_pc-rtems_impl OBJECT 
	src/bsp_start.c
	src/bsp_voltab.c
)

add_library(ut_bsp STATIC EXCLUDE_FROM_ALL
	ut-src/bsp_ut.c
	ut-src/bsp_ut_voltab.c
)
target_include_directories(ut_bsp PRIVATE ${UT_ASSERT_SOURCE_DIR}/inc)
