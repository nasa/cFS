#
# CMake build snippet for OSAL tests
#

# Indicates that this should output a ctest script
enable_testing()

# Each test module is stored within its own subdir
file(GLOB OSAL_TESTS *-test)

# The original OSAL tests ran forever until CTRL+C, this does not work for scripted testing
# This SCRIPT_MODE define plus some hooks in the code allow for limited runs
add_definitions(-DSCRIPT_MODE)

foreach(OSTEST ${OSAL_TESTS})
  get_filename_component(TESTNAME ${OSTEST} NAME)
  set(TESTFILES)
  aux_source_directory(${OSTEST} TESTFILES)
  add_osal_ut_exe(${TESTNAME} ${TESTFILES})
endforeach(OSTEST ${OSAL_TESTS})
