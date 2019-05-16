# This indicates where to install target binaries created during the build
set(INSTALL_SUBDIR "cf")

# Some upper-level code may be gated on _LINUX_OS_ being defined
set(OSAL_C_FLAGS "${OSAL_C_FLAGS} -D_LINUX_OS_")

