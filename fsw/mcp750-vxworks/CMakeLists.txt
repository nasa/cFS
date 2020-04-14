
# Build the mcp750-vxworks implementation as a library
add_library(psp-${CFE_SYSTEM_PSPNAME}-impl OBJECT
    src/cfe_psp_exception.c
    src/cfe_psp_memory.c
    src/cfe_psp_memtab.c
    src/cfe_psp_ssr.c
    src/cfe_psp_start.c
    src/cfe_psp_support.c
    src/cfe_psp_timer.c
    src/cfe_psp_voltab.c
    src/cfe_psp_watchdog.c)

