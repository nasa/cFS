######################################################################
#
# CMAKE build recipe for POSIX OSAL implementation
#
######################################################################

# This CMake script generates targets specific to the POSIX implementation
# It defines an OBJECT target named "osal_posix_impl"

add_library(osal_posix_impl OBJECT 
	osapi.c
	osfileapi.c
	osfilesys.c
	osloader.c
	osnetwork.c
	osselect.c
	ostimer.c
        ../portable/os-impl-no-shell.c
)
