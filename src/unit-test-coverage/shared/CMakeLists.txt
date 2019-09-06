# CMake snippet for building the shared OSAL layer coverage tests

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/inc)

# A list of modules in the shared OSAL implementation.
# Each entry corresponds to a source file name osapi-${MODULE}.c
set(MODULE_LIST  
    binsem
    clock
    common
    countsem
    dir
    errors
    file
    filesys
    fpu
    heap
    idmap
    interrupts
    module
    mutex
    network
    printf
    queue
    select
    sockets
    task
    timebase
    time)

# The "modules" subdirectory builds the _ACTUAL_ OSAL code (not a stub)
# However it is compiled specially to include the coverage instrumentation,
# and all internal C-library calls should be mapped to the OCS_ version.
add_subdirectory(modules)

# Add all coverage tests in the src dir
add_coverage_tests(${SETNAME} ${MODULE_LIST})

