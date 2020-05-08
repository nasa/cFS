project(CFEPSP C)

if (NOT CFE_SYSTEM_PSPNAME)
  message(FATAL_ERROR "CFE_SYSTEM_PSPNAME is not defined - do not know which to build")
endif()

add_definitions(-D_CFE_PSP_)

include_directories(fsw/shared)

# Build the PSP implementation which lies in a system-specific subdirectory
include_directories(fsw/shared)
include_directories(fsw/${CFE_SYSTEM_PSPNAME}/inc)
add_subdirectory(fsw/${CFE_SYSTEM_PSPNAME} ${CFE_SYSTEM_PSPNAME})

# Build the "common" parts as a library
add_library(psp-${CFE_SYSTEM_PSPNAME} STATIC
    fsw/shared/cfe_psp_configdata.c
    fsw/shared/cfe_psp_eeprom.c
    fsw/shared/cfe_psp_exceptionstorage.c
    fsw/shared/cfe_psp_memrange.c
    fsw/shared/cfe_psp_memutils.c
    fsw/shared/cfe_psp_module.c
    fsw/shared/cfe_psp_port.c
    fsw/shared/cfe_psp_ram.c
    $<TARGET_OBJECTS:psp-${CFE_SYSTEM_PSPNAME}-impl>)

if (ENABLE_UNIT_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fsw/ut-stubs)
endif (ENABLE_UNIT_TESTS)

