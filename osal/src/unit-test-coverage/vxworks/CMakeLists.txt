# CMake snippet for building the shared OSAL layer coverage tests

set(VXWORKS_MODULE_LIST
    binsem
    common
    console
    countsem
    dirs
    files
    filesys
    idmap
    heap
    loader
    mutex
    network
    queues
    shell
    symtab
    tasks
    timebase
)

set(VXWORKS_PORTABLE_BLOCK_LIST
    posix-gettime
    posix-io
    posix-files

    console-bsp
    bsd-select
    #bsd-sockets

    no-loader
    no-shell
    #no-symtab
    #no-network
    #no-sockets
)



# This unit test is allowed to directly include any internal file in
# the respective set under test.
include_directories(${OSAL_SOURCE_DIR}/src/os/${SETNAME}/inc)

# The "ut-stubs" contains additional stubs specific to this sub-module
add_subdirectory(ut-stubs)
add_subdirectory(adaptors)

set(VXWORKS_COVERAGE_LINK_LIST
        ut-adaptor-vxworks
        ut_vxworks_impl_stubs
        ut_bsp_impl_stubs
        ut_osapi_impl_stubs
        ut_osapi_shared_stubs
        ut_osapi_stubs
        ut_libc_stubs
)

# Add all coverage tests in the src dir
foreach(MODNAME ${VXWORKS_MODULE_LIST})
    add_coverage_testrunner(
        "coverage-${SETNAME}-${MODNAME}"
        "${OSAL_SOURCE_DIR}/src/os/${SETNAME}/src/os-impl-${MODNAME}.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/coveragetest-${MODNAME}.c"
        ${VXWORKS_COVERAGE_LINK_LIST}
    )
endforeach(MODNAME ${VXWORKS_MODULE_LIST})

foreach(MODNAME ${VXWORKS_PORTABLE_BLOCK_LIST})
    add_coverage_testrunner(
        "coverage-${SETNAME}-${MODNAME}"
        "${OSAL_SOURCE_DIR}/src/os/portable/os-impl-${MODNAME}.c"
        "${OSALCOVERAGE_SOURCE_DIR}/portable/src/coveragetest-${MODNAME}.c"
        ${VXWORKS_COVERAGE_LINK_LIST}
    )
endforeach(MODNAME ${VXWORKS_PORTABLE_BLOCK_LIST})

