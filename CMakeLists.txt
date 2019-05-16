project(CFEPSP C)

if (NOT CFE_SYSTEM_PSPNAME)
  message(FATAL_ERROR "CFE_SYSTEM_PSPNAME is not defined - do not know which to build")
endif()

if (NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/fsw/${CFE_SYSTEM_PSPNAME})
  message(FATAL_ERROR "PSP ${CFE_SYSTEM_PSPNAME} is not implemented")
endif()

add_definitions(-D_CFE_PSP_)
include_directories(fsw/${CFE_SYSTEM_PSPNAME}/inc)
include_directories(fsw/shared)
  
# Use all source files under the shared code directory (always)
# as well as all the files for the selected PSP
set(PSPFILES)
aux_source_directory(fsw/shared PSPFILES)
aux_source_directory(fsw/${CFE_SYSTEM_PSPNAME}/src PSPFILES)

add_library(psp-${CFE_SYSTEM_PSPNAME} STATIC ${PSPFILES})
