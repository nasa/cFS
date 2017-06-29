#
# For POSIX systems based on glibc (i.e. Linux) certain features must be enabled
# Glibc headers support multiple standards and multiple versions of the standards,
# so the definitions of these macros determine which standard we will use
#
# OSAL needs conformance to at least POSIX.1c (aka POSIX 1995) - this includes all the
# real-time support and threading extensions.
#
# When compiling against glibc, using "_XOPEN_SOURCE=600" enables the X/Open 6 standard.
# XPG6 includes all necessary XPG5, POSIX.1c features as well as SUSv2/UNIX98 extensions.
# Although current POSIX implementation does not specifically use anything from XPG6 
# (only 5) it may be beneficial to enable XPG6 anyway, so applications can use the real
# time features that it adds.
#
# Note that this definition assumes glibc -- in case of compiling OSAL for some platform
# that supports POSIX but does not use glibc (e.g. uclibc) this definition shouldn't
# harm anything, but it may need to be tweaked.  
#
# See http://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
# for a more detailed description of the feature test macros and available values
#
set(OSAL_C_FLAGS "${OSAL_C_FLAGS} -D_XOPEN_SOURCE=600")

# OSAL_LINK_LIBS determines which system-level libraries must be included in the
# link command in order to produce the final binary.  These libs will be used for
# ALL targets that utilize the POSIX OS layer.  Additional target-specific libraries 
# may also be specified in the BSP or the cross-compile toolchain. 
set (OSAL_LINK_LIBS ${OSAL_LINK_LIBS} pthread dl rt)
