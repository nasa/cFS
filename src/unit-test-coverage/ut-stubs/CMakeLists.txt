# ----------------------------------------
#    Stub libraries for coverage testing
# ----------------------------------------

# This provides suitable "stub" implementations of every 
# function call used internally by the various OSAL modules
# for which there is not a stub already defined.
#
# (Note this is not the public OSAL API - that is stubbed separately)
#
# This includes:
# - Stub versions of C library calls defined per ANSI C89/C99
#     i.e. memset, strcmp, etc - these should be relevant for all
#     supported operating systems as they are standard C
# - Stub versions of internal "shared" OSAL implementation functions
#     i.e. everything declared in the internal API.  These are needed by 
#     any coverage test referencing on the shared/ng OSAL layer.
#

# The "ut_libc_stubs" target provides stub versions of C library calls.
# They are prefixed with "OCS_" and target code must be recompiled to
# call the OCS_ version of the syscall instead of the regular syscall.
# This is because in some circumstances for these calls the stub actually 
# needs to invoke the real version or else weird things happen.  
# This library includes stubs from all supported operating systems.  This
# is generally OK as we do not use any actual OS system headers
#
# These files are generally organized to match whatever header file  
# defines the function. For instance, POSIX defines the "mqueue.h"
# header file which in turn provides mq_open, mq_close, etc.  So
# the OCS_mq_open, OCS_mq_close declarations are in overrides/mqueue.h, and 
# the respective implementation is in posix-mqueue-stubs.c.
#
# This keeps things relatively organized, and by keeping the source files 
# relatively small and targeted like this the linker should only pull in
# the OCS functions that are actually used. 
#
add_library(ut_libc_stubs STATIC EXCLUDE_FROM_ALL
    src/bsd-select-stubs.c
    src/libc-ctype-stubs.c
    src/libc-stdio-stubs.c
    src/libc-stdlib-stubs.c
    src/libc-string-stubs.c
    src/posix-dirent-stubs.c
    src/posix-dlfcn-stubs.c
    src/posix-errno-stubs.c
    src/posix-fcntl-stubs.c
    src/posix-ioctl-stubs.c
    src/posix-mqueue-stubs.c
    src/posix-pthread-stubs.c
    src/posix-sched-stubs.c
    src/posix-semaphore-stubs.c
    src/posix-signal-stubs.c
    src/posix-stat-stubs.c
    src/posix-time-stubs.c
    src/posix-unistd-stubs.c
    src/vxworks-ataDrv-stubs.c
    src/vxworks-dosFsLib-stubs.c
    src/vxworks-errnoLib-stubs.c
    src/vxworks-hostLib-stubs.c
    src/vxworks-intLib-stubs.c
    src/vxworks-loadLib-stubs.c
    src/vxworks-memPartLib-stubs.c
    src/vxworks-moduleLib-stubs.c
    src/vxworks-msgQLib-stubs.c
    src/vxworks-ramDrv-stubs.c
    src/vxworks-semLib-stubs.c
    src/vxworks-shellLib-stubs.c
    src/vxworks-symLib-stubs.c
    src/vxworks-sysLib-stubs.c
    src/vxworks-taskLib-stubs.c
    src/vxworks-taskVarLib-stubs.c
    src/vxworks-xbdBlkDev-stubs.c)
    
target_include_directories(ut_libc_stubs PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
    
    
# The "ut_osapi_impl_stubs" provides stub functions for internal
# OSAL calls used by or implemented by the shared layer.  These
# are not public API calls.   This is only compiled if used. 
add_library(ut_osapi_impl_stubs STATIC EXCLUDE_FROM_ALL
    src/osapi-binsem-impl-stubs.c
    src/osapi-common-impl-stubs.c
    src/osapi-console-impl-stubs.c
    src/osapi-countsem-impl-stubs.c
    src/osapi-error-impl-stubs.c
    src/osapi-file-impl-stubs.c
    src/osapi-filesys-impl-stubs.c
    src/osapi-fpu-impl-stubs.c
    src/osapi-heap-impl-stubs.c
    src/osapi-idmap-impl-stubs.c
    src/osapi-loader-impl-stubs.c
    src/osapi-mutex-impl-stubs.c
    src/osapi-network-impl-stubs.c
    src/osapi-queue-impl-stubs.c
    src/osapi-select-impl-stubs.c
    src/osapi-shared-common-stubs.c
    src/osapi-shared-debug-stubs.c
    src/osapi-shared-idmap-stubs.c
    src/osapi-task-impl-stubs.c
    src/osapi-timer-impl-stubs.c
    src/portable-console-bsp-impl-stubs.c
)

# The "ut_osapi_shared_stubs" provides stub objects for shared
# objects used by the implementation layer.  These
# are not public.   This is only compiled if used. 
add_library(ut_osapi_shared_stubs STATIC EXCLUDE_FROM_ALL
    src/osapi-shared-common-stubs.c
    src/osapi-shared-idmap-stubs.c
    src/osapi-shared-binsem-table-stubs.c
    src/osapi-shared-console-table-stubs.c
    src/osapi-shared-countsem-table-stubs.c
    src/osapi-shared-dir-table-stubs.c
    src/osapi-shared-filesys-table-stubs.c
    src/osapi-shared-module-table-stubs.c
    src/osapi-shared-mutex-table-stubs.c
    src/osapi-shared-queue-table-stubs.c
    src/osapi-shared-stream-table-stubs.c
    src/osapi-shared-task-table-stubs.c
    src/osapi-shared-timebase-table-stubs.c
    src/osapi-shared-timecb-table-stubs.c
    src/osapi-shared-debug-stubs.c 
)

add_library(ut_bsp_impl_stubs STATIC EXCLUDE_FROM_ALL
    src/bsp-console-impl-stubs.c
)
