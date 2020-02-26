######################################################################
#
# CMAKE build recipe for VxWorks OSAL implementation
#
######################################################################

# This CMake script generates targets specific to the VxWorks implementation
# It defines an OBJECT target named "osal_vxworks_impl"
add_library(osal_vxworks_impl OBJECT 
	osapi.c
	osfileapi.c
	osfilesys.c
	osloader.c
	osnetwork.c
	osselect.c
	ostimer.c
)