# CMake snippet for building the shared OSAL layer coverage tests

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
    heap
    idmap
    module
    mutex
    network
    printf
    queue
    select
    sockets
    task
    timebase
    time
)

set(SHARED_COVERAGE_LINK_LIST
        ut-adaptor-shared
        ut_osapi_impl_stubs
        ut_osapi_shared_stubs
        ut_osapi_stubs
        ut_libc_stubs
)

add_subdirectory(adaptors)


# Add all coverage tests in the src dir
foreach(MODNAME ${MODULE_LIST})
    add_coverage_testrunner(
        "coverage-${SETNAME}-${MODNAME}"
        "${OSAL_SOURCE_DIR}/src/os/${SETNAME}/src/osapi-${MODNAME}.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/coveragetest-${MODNAME}.c"
        ${SHARED_COVERAGE_LINK_LIST}
    )
endforeach(MODNAME ${MODULE_LIST})

# Add extra defintion to force the OS_STATIC_SYMTABLE_SOURCE to the
# local value within the module coverage test
target_compile_definitions(utobj_coverage-shared-module PRIVATE
    "OS_STATIC_SYMTABLE_SOURCE=OS_UT_STATIC_SYMBOL_TABLE"
)


