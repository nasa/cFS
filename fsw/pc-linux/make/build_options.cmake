# This indicates where to install target binaries created during the build
set(INSTALL_SUBDIR "cf")

# Some upper-level code may be gated on _LINUX_OS_ being defined
add_definitions("-D_LINUX_OS_")

