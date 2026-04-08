#
# Example global_build_options.cmake
# ----------------------------------
#
# This may set global definitions that apply to ALL targets in ALL scopes,
# including FSW code that is cross-compiled for a target as well as code
# built for the development host itself (native).
#
# As such, it should only invoke basic commands that have wide applicability,
# such as "add_definitions()" for macro definitions that should be set
# globally.  It should not include any compiler-specific options that might
# change between compiler vendors or target processor families.
#

# If the OMIT_DEPRECATED flag is specified, then define the respective macros
# that omit the deprecated features from the build.  This is conditional in this
# example for CI purposes, so it can be tested both ways.  Most projects would
# likely set this only one way.
set(OMIT_DEPRECATED $ENV{OMIT_DEPRECATED} CACHE STRING "Omit deprecated elements")
if (OMIT_DEPRECATED)
  message (STATUS "OMIT_DEPRECATED=true: Not including deprecated elements in build")
  add_definitions(-DCFE_OMIT_DEPRECATED_6_8 -DCFE_OMIT_DEPRECATED_6_7 -DCFE_OMIT_DEPRECATED_6_6 -DOSAL_OMIT_DEPRECATED)
  set(MISSION_RESOURCEID_MODE "STRICT") # more type safe, but less backward compatible
else()
  message (STATUS "OMIT_DEPRECATED=false: Deprecated elements included in build")
  set(MISSION_RESOURCEID_MODE "SIMPLE") # less type safe, but more backward compatible
endif (OMIT_DEPRECATED)

# Address Sanitizer option.  This enables the ASAN library that is available
# in recent versions of GCC - although it may depend on additional packages being
# installed, depending on development host OS/version.
set(ENABLE_ASAN $ENV{ENABLE_ASAN} CACHE BOOL "Enable address sanitizer")
if (ENABLE_ASAN)
  add_compile_options(-fsanitize=address -fsanitize-recover=address)
  add_link_options(-fsanitize=address)
endif(ENABLE_ASAN)

# The stringop/format truncation and overflow warnings tend to produce lots of false positives
# CFE code is designed to handle/tolerate string truncation so it is generally not a real issue
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 8.0.0)
  add_compile_options(
    -Wno-stringop-overflow
    -Wno-stringop-truncation
    -Wno-format-overflow
    -Wno-format-truncation
  )
endif()

# Include Global Headers
include_directories(${MISSION_DEFS}/inc)
