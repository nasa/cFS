add_library(ut_vxworks_impl_stubs 
    src/osapi-impl-vxworks-stubs.c)

list(APPEND OSALCOVERAGE_STUB_LIB_LIST ut_vxworks_impl_stubs)
set(OSALCOVERAGE_STUB_LIB_LIST ${OSALCOVERAGE_STUB_LIB_LIST} PARENT_SCOPE)
