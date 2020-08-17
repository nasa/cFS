######################################################################
#
# CMAKE build recipe for CFE Platform Support Package (PSP)
#
######################################################################

project(CFEPSP C)

if (NOT CFE_SYSTEM_PSPNAME)
  message(FATAL_ERROR "CFE_SYSTEM_PSPNAME is not defined - do not know which to build")
endif()

set(CFE_PSP_TARGETNAME "${CFE_SYSTEM_PSPNAME}")
add_definitions(-D_CFE_PSP_)

# The PSP is currently built in two parts, consisting of a fully platform-specific
# module combined with a shared component which is built for multiple targets.
# The "shared" component is compiled using headers from the platform-specific module
# so it is still ultimately a platform-specific binary, and it all gets wrapped into
# a single PSP static library target.
include_directories(fsw/shared/inc)
add_subdirectory(fsw/${CFE_PSP_TARGETNAME} ${CFE_PSP_TARGETNAME}-impl)
add_subdirectory(fsw/shared ${CFE_PSP_TARGETNAME}-shared)

add_library(psp-${CFE_PSP_TARGETNAME} STATIC
    $<TARGET_OBJECTS:psp-${CFE_PSP_TARGETNAME}-shared>
    $<TARGET_OBJECTS:psp-${CFE_PSP_TARGETNAME}-impl>
)
    
if (ENABLE_UNIT_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fsw/ut-stubs)
endif (ENABLE_UNIT_TESTS)

