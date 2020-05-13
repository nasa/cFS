Travis-CI: [![Build Status](https://travis-ci.com/nasa/osal.svg)](https://travis-ci.com/nasa/osal)

# Operating System Abstraction Layer Project

![OSAL Logo by Michael Cudmore](./doc/OSAL-Logo.png)

The OS Abstraction Layer was originally devloped by the Flight Software Systems Branch at NASA Goddard Space Flight Center.

# OS Abstraction Layer information:

This distribution contains:

1. The OS Abstraction Layer Library
2. Tests and example applications
3. A directory structure and makefiles to manage it all.

## Version History

### Development Build: 5.0.16

- Resized buffers and added explicit termination to string copies. No warnings on GCC9 with strict settings and optimization enabled.

- New API to reverse lookup an OS-provided thread/task identifier back to an OSAL ID. Any use of existing OStask_id field within the task property structure is now deprecated.

- See <https://github.com/nasa/osal/pull/458>

### Development Build: 5.0.15

- Changes the build system.
- No more user-maintained osconfig.h file, this is now replaced by a cmake configuration file.
- Breaks up low-level implementation into small, separate subsystem units, with a separate header file for each one.
- See <https://github.com/nasa/osal/pull/444>

### Development Build: 5.0.14

- Adds library build, functional, and coverage test to CI
- Deprecates `OS_FS_SUCCESS, OS_FS_ERROR , OS_FS_ERR_INVALID_POINTER, OS_FS_ERR_NO_FREE_FDS , OS_FS_ERR_INVALID_FD, and OS_FS_UNIMPLEMENTED` from from `osapi-os-filesys.h`
- Individual directory names now limited to OS_MAX_FILE_NAME
- Fix tautology, local_idx1 is now compared with local_idx2
- Module files are generated when the `osal_loader_UT` test is built and run
- Consistent osal-core-test execution status
- See <https://github.com/nasa/osal/pull/440> for more details

### Development Build: 5.0.13

- Added coverage test to `OS_TimerCreate` for `OS_ERR_NAME_TOO_LONG`.
- Externalize enum for `SelectSingle`, ensures that pointers passed to `SelectFd...()` APIs are not null, ensures that pointer to `SelectSingle` is not null.
- Command to run in shell and output to fill will fail with default (not implemented) setting.
- Builds successfully using the inferred OS when only `OSAL_SYSTEM_BSPTYPE` is set. Generates a warning when `OSAL_SYSTEM_BSPTYPE` and `OSAL_SYSTEM_OSTYPE` are both set but are mismatched.
- See <https://github.com/nasa/osal/pull/433> for more details

### Development Build: 5.0.12

- Use the target_include_directories and target_compile_definitions functions from CMake to manage the build flags per target.
- Build implementation components using a separate CMakeLists.txt file rather than aux_source_directory.
- Provide sufficient framework for combining the OSAL BSP, UT BSP, and the CFE PSP and eliminating the duplication/overlap between these items.
- Minor updates (see <https://github.com/nasa/osal/pull/417>)

### Development Build: 5.0.11

- The more descriptive return value OS_ERR_NAME_NOT_FOUND (instead of OS_FS_ERROR) will now be returned from the following functions (): OS_rmfs, OS_mount, OS_unmount, OS_FS_GetPhysDriveName
- Wraps OS_ShMem* prototype and unit test wrapper additions in OSAL_OMIT_DEPRECATED
- Minor updates (see <https://github.com/nasa/osal/pull/408>)

### Development Build: 5.0.10

- Minor updates (see <https://github.com/nasa/osal/pull/401>)

  - 5.0.9: DEVELOPMENT

- Documentation updates (see <https://github.com/nasa/osal/pull/375>)

### Development Build: 5.0.8

- Minor updates (see <https://github.com/nasa/osal/pull/369>)

### Development Build: 5.0.7

- Fixes memset bug
- Minor updates (see <https://github.com/nasa/osal/pull/361>)

### Development Build: 5.0.6

- Minor updates (see <https://github.com/nasa/osal/pull/355>)

### Development Build: 5.0.5

- Fixed osal_timer_UT test failure case
- Minor updates (see <https://github.com/nasa/osal/pull/350>)

### Development Build: 5.0.4

- Minor updates (see <https://github.com/nasa/osal/pull/334>)

### Development Build: 5.0.3

- Minor updates (see <https://github.com/nasa/osal/pull/292>)

### Development Build: 5.0.2

- Bug fixes and minor updates (see <https://github.com/nasa/osal/pull/281>)

### Development Build: 5.0.1

- Minor updates (see <https://github.com/nasa/osal/pull/264>)

### **_Release Candidate: 5.0.0µ_**

- In build verification testing to be considered for official release
- Release documentation in work
- This is a point release from an internal repository

### **_OFFICIAL RELEASE: 4.2.1a_**

- Released under the NOSA license, see 

  <license>
  </license>

- See [version description document](OSAL%204.2.1.0%20Version%20Description%20Document.pdf)
- This is a point release from an internal repository

# Getting Started:

See the document _doc/OSAL-Configuration-Guide.doc_ for complete details.

See User's Guide: <https://github.com/nasa/cFS/blob/gh-pages/OSAL_Users_Guide.pdf>

An easy way to get started is to use the Linux port and classic build:

1. Set the _OSAL_SRC_ environment variable to point to the OSAL source code.

  - Running setvars.sh will set the variable for you ($source ./setvars.sh)

2. Edit the _build/osal-config.mak_ file and set the following options:

  - BSP - Set this to the board you are running on. For a PC running linux, use _pc-linux_
  - OS - Set this to the OS you are running. For a PC running linux, use _posix_.
  - OSAL_M32 - Uncomment/Add this build variable for building 32-bit images using "native" GCC on a 64-bit X86 Linux platform

Buiding on a PC running linux:

```
export OSAL_SRC = /home/acudmore/osal/src
```

In build/osal-config.mak:

```
OS=posix
BSP=pc-linux

Optional: OSAL_M32 = -m32 (Note: Usage of this flag may require an optional 'multilib' (or similar)
package to be installed. Refer to your operating system and toolchain documentation for details, if
adding the appropriate flag causes your builds to fail due to (for example) missing 32-bit or
multilib related headers or libraries.)
```

Optional: Some Linux systems may require an additional linker option in src/bsp/pc-linux/make/link-rules.mak:

```
LDFLAGS ?= $(OSAL_M32) -export-dynamic

If the symbol-api-test fails, then you need this option.
```

Now just type _make config; make_ from the build directory and it should build the OSAL core files, tests, and sample applications for you. The binary for each application is in its own directory (i.e. build/examples/tasking-example/tasking-example.bin) You can switch to that directory and run it. You can also debug it using GDB.

NOTE: Running on linux may require root or adjusting the posix message queue maximum sizes.

The Embedded targets take a little more work to run, because they must be cross compiled and booted on the board. By copying a target, you should be able to come up with a new target.

If you would like just the OSAL itself, just look in src/os/inc for the include files and src/os/

<your os="" here="">
for the OSAL implementation.</your>

The API documentation is in the _doc_ directory.

There are two sets of tests: build/tests and build/unit-tests. To build and the unit tests, perform the build steps above then _make unit-tests_ in the build directory.

Instructions on how to use the newely supported cmake build system are provided in the OSAL Configuration Guide located in the _doc_ directory.

# Contact Information:

```
Alan Cudmore
NASA Goddard Space Flight Center
Code 582.0
Greenbelt, MD 20771
Alan.P.Cudmore@nasa.gov
```

# Copyright notice:

Copyright United States Government as represented by the Administrator of the National Aeronautics and Space Administration

# License information:

This software is licensed under NASAs Open Source Agreement. The release of the software is conditional upon the recipients acceptance of the Open Source Agreement. Please see the file: NASA_Open_Source_Agreement_1_3-OS_AbstractionLayer.txt
