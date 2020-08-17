# "Adaptors" help enable the unit test code to reach functions/objects that
# are otherwise not exposed.  This is generally required for any OSAL subsystem
# which tracks an internal resource state (i.e. anything with a table).

# NOTE: These source files are compile with OVERRIDES on the headers just like
# the FSW code is compiled.  This is how it is able to include internal headers
# which otherwise would fail.  But that also means that adaptor code cannot call
# any library functions, as this would also reach a stub, not the real function.

add_library(ut-adaptor-${SETNAME} STATIC
    src/ut-adaptor-module.c
)

# the "override_inc" dir contains replacement versions of the C-library include files.
target_include_directories(ut-adaptor-${SETNAME} BEFORE PRIVATE
    ${OSALCOVERAGE_SOURCE_DIR}/ut-stubs/override_inc
)

target_include_directories(ut-adaptor-${SETNAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
