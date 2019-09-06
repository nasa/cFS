add_library(ut_posix_impl_stubs 
    src/osapi-impl-posix-stubs.c)

list(APPEND OSALCOVERAGE_STUB_LIB_LIST ut_posix_impl_stubs)
set(OSALCOVERAGE_STUB_LIB_LIST ${OSALCOVERAGE_STUB_LIB_LIST} PARENT_SCOPE)
