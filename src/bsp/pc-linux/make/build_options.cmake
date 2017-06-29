# This indicates where to install target binaries created during the build
set(INSTALL_SUBDIR "eeprom1")

# Some upper-level code may be gated on _LINUX_OS_ being defined
set(OSAL_C_FLAGS "${OSAL_C_FLAGS} -D_LINUX_OS_")

# C flags that should be used when (re-) compiling code for unit testing.
# Note: --coverage is just a shortcut for "-ftest-coverage" and "-fprofile-arcs"
# This also does not work well when cross compiling since paths to the _compile_ dir
# are baked into the executables, so they will not be there when copied to the target
if (NOT CMAKE_CROSSCOMPILING)
  set(UT_C_FLAGS "-pg --coverage")
endif()

