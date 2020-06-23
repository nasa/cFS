# "Adaptors" help enable the unit test code to reach functions/objects that
# are otherwise not exposed.  This is generally required for any OSAL subsystem
# which tracks an internal resource state (i.e. anything with a table).

# NOTE: These source files are compile with OVERRIDES on the headers just like
# the FSW code is compiled.  This is how it is able to include internal headers
# which otherwise would fail.  But that also means that adaptor code cannot call
# any library functions, as this would also reach a stub, not the real function.

add_library(ut-adaptor-${SETNAME} STATIC
    src/ut-adaptor-binsem.c
    src/ut-adaptor-common.c
    src/ut-adaptor-console.c
    src/ut-adaptor-countsem.c
    src/ut-adaptor-dirs.c
    src/ut-adaptor-files.c
    src/ut-adaptor-filesys.c
    src/ut-adaptor-idmap.c
    src/ut-adaptor-loader.c
    src/ut-adaptor-mutex.c
    src/ut-adaptor-queues.c
    src/ut-adaptor-filetable-stub.c
    src/ut-adaptor-symtab.c
    src/ut-adaptor-tasks.c
    src/ut-adaptor-timebase.c

    ${OSALCOVERAGE_SOURCE_DIR}/portable/adaptors/src/ut-adaptor-portable-posix-io.c
    ${OSALCOVERAGE_SOURCE_DIR}/portable/adaptors/src/ut-adaptor-portable-posix-files.c
)

# the "override_inc" dir contains replacement versions of the C-library include files.
target_include_directories(ut-adaptor-${SETNAME} BEFORE PRIVATE
    ${OSALCOVERAGE_SOURCE_DIR}/ut-stubs/override_inc
)

target_include_directories(ut-adaptor-${SETNAME} PUBLIC
    ${OSALCOVERAGE_SOURCE_DIR}/portable/adaptors/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
