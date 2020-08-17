######################################################################
#
# CMAKE build recipe for RTEMS OSAL implementation
#
######################################################################

# This CMake script generates targets specific to the RTEMS implementation
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

# The basic set of files which are always built
set(RTEMS_BASE_SRCLIST
    src/os-impl-binsem.c
    src/os-impl-common.c
    src/os-impl-console.c
    src/os-impl-countsem.c
    src/os-impl-dirs.c
    src/os-impl-errors.c
    src/os-impl-files.c
    src/os-impl-filesys.c
    src/os-impl-idmap.c
    src/os-impl-heap.c
    src/os-impl-mutex.c
    src/os-impl-queues.c
    src/os-impl-tasks.c
    src/os-impl-timebase.c
)

# The FPU and interrupt modules are deprecated.
# If the "OMIT_DEPRECATED" switch is set, then these are not built.
if (NOT OMIT_DEPRECATED)
    list(APPEND RTEMS_BASE_SRCLIST
        src/os-impl-fpu.c
        src/os-impl-interrupts.c
    )
endif (NOT OMIT_DEPRECATED)




# Use portable blocks for basic I/O
set(RTEMS_IMPL_SRCLIST
    ../portable/os-impl-posix-gettime.c
    ../portable/os-impl-console-bsp.c
    ../portable/os-impl-bsd-select.c
    ../portable/os-impl-posix-io.c
    ../portable/os-impl-posix-files.c
    ../portable/os-impl-posix-dirs.c
)

if (OSAL_CONFIG_INCLUDE_SHELL)
    list(APPEND RTEMS_IMPL_SRCLIST
       src/os-impl-shell.c
    )
else ()
    list(APPEND RTEMS_IMPL_SRCLIST
       ../portable/os-impl-no-shell.c
    )
endif ()

# If some form of module loading is configured,
# then build the module loader
if (OSAL_CONFIG_INCLUDE_DYNAMIC_LOADER)
    list(APPEND RTEMS_IMPL_SRCLIST
        src/os-impl-loader.c                    # Use RTEMS-specific versions of the load/unload routines
        ../portable/os-impl-posix-dl-symtab.c   # Use Standard POSIX implementation for symbol lookups
    )
else ()
    list(APPEND RTEMS_IMPL_SRCLIST
        src/os-impl-no-module.c                 # No-op init routine
        ../portable/os-impl-no-loader.c         # Non-implemented stubs for all Impl routines
        ../portable/os-impl-no-symtab.c
    )
endif ()

# Leverage the "standard" BSD socket code if network is enabled
if (OSAL_CONFIG_INCLUDE_NETWORK)
    list(APPEND RTEMS_IMPL_SRCLIST
        src/os-impl-network.c
        ../portable/os-impl-bsd-sockets.c
    )
else()
    list(APPEND RTEMS_IMPL_SRCLIST
        ../portable/os-impl-no-network.c
        ../portable/os-impl-no-sockets.c
    )
endif ()

# Defines an OBJECT target named "osal_rtems_impl" with selected source files
add_library(osal_rtems_impl OBJECT
    ${RTEMS_BASE_SRCLIST}
    ${RTEMS_IMPL_SRCLIST}
)
