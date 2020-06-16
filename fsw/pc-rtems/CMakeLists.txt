######################################################################
#
# CMAKE build recipe for pc-rtems PSP component
#
######################################################################

# This contains the fully platform-specific code to 
# run CFE on this target.

include_directories(inc) 

# Build the pc-rtems implementation as a library
add_library(psp-${CFE_PSP_TARGETNAME}-impl OBJECT
    src/cfe_psp_exception.c
    src/cfe_psp_memory.c
    src/cfe_psp_memtab.c
    src/cfe_psp_ssr.c
    src/cfe_psp_start.c
    src/cfe_psp_support.c
    src/cfe_psp_timer.c
    src/cfe_psp_watchdog.c)

