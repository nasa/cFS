######################################################################
#
# CMAKE build recipe for RTEMS OSAL implementation
#
######################################################################

# This CMake script generates targets specific to the RTEMS implementation
# It defines an OBJECT target named "osal_rtems_impl"
add_library(osal_rtems_impl OBJECT 
	osapi.c
	osfileapi.c
	osfilesys.c
	osloader.c
	osnetwork.c
	osselect.c
	ostimer.c
)
