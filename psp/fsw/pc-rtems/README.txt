ABOUT THE PC-RTEMS CFE PSP
==========================

The "pc-rtems" PSP layer is intended to be an easy way to prove out the basic functionality
of CFE running on RTEMS without actually requiring a real hardware target with RTEMS support.  

It is based on:
    - RTEMS built using the "pc686" BSP (i.e. an ordinary off the shelf i686 PC)
    - Running the resulting binary in a QEMU virtual i686 PC
    
The example instructions here are to set up RTEMS under your local home dir in ${HOME}/rtems-4.11.
However any location can be used e.g. /opt/x-tools/rtems-4.11 might be good choice on a shared
build system.  The RTEMS_BSP_TOP directory must be set to the same location either by editing
the example toolchain file or via -D options to the CMake build.
 



I. Setting up and compiling RTEMS BSP

Steps 1 & 5 may require root access.  Everything else should be done as regular user.


1) install RTEMS toolchain for i386-rtems4.11 (or relevant target arch) into /opt/rtems-4.11

The RTEMS official docs have instructions for this, which should supercede anything here.

Basic use of the the "rtems source builder" tool which works at the time of this writing:
OFFICIAL RTEMS SOURCE BUILDER CLONE URL: git://git.rtems.org/rtems-source-builder.git


sudo mkdir -p /opt/rtems-4.11
git clone git://git.rtems.org/rtems-source-builder.git
cd rtems-source-builder/rtems
git checkout 4.11.2
../source-builder/sb-set-builder --prefix=/opt/rtems-4.11 4.11/rtems-i386


2) Clone/Bootstrap RTEMS source tree:

Note - at the time of this writing 4.11 is the current "stable" branch and
4.11.2 represents the latest point release tag on that branch.  The bleeding
edge "development" branch tends to change on a frequent basis.

OFFICIAL RTEMS CLONE URL: git://git.rtems.org/rtems.git

$ git clone -b 4.11.2 git://git.rtems.org/rtems.git
$ export PATH=/opt/rtems-4.11/bin:$PATH
$ (cd rtems && ./bootstrap)


3) Build/Install RTEMS pc686 BSP

Note - use same terminal as previous step, or set PATH environment again.
The "prefix" can be whatever location is preferable for installation.

$ mkdir b-pc686
$ cd b-pc686
$ ../rtems/configure --target=i386-rtems4.11 \
    --enable-rtemsbsp=pc686 \
    --prefix=${HOME}/rtems-4.11 \
    --enable-networking \
    --enable-cxx \
    --disable-posix \
    --disable-deprecated \
    BSP_ENABLE_VGA=0 \
    CLOCK_DRIVER_USE_TSC=1 \
    USE_COM1_AS_CONSOLE=1 \
    BSP_PRESS_KEY_FOR_RESET=0 \
    BSP_RESET_BOARD_AT_EXIT=1    
$ make
$ make install
$ cd ..


4) Install cexp-2.2.x (dynamic module loader library)

This is necessary to get the module loader to compile and work.

Ref page: http://www.slac.stanford.edu/~strauman/rtems/cexp/index.html
Tarball: http://www.slac.stanford.edu/~strauman/rtems/cexp/cexp-2.2.3.tgz

NOTE: As of 2017-10-26 the code is also now on github, but the git version appears to be
missing files and I was not able to get it to compile. 
git clone -b CEXP_Release_2_2_3 https://github.com/till-s/cexpsh.git

$ mkdir b-cexp
$ cd b-cexp
$ ../cexp-CEXP_Release_2_2_3/configure --with-rtems-top=${HOME}/rtems-4.11 --host=i386-rtems4.11 --enable-std-rtems-installdirs
$ make
$ make install
$ cd ..

NOTE: using 2.2.3 against the latest rtems-4.11 produce a compile error regarding dereferencing incomplete types.
This appears to be due to #define XOPEN_SOURCE 500 on cexplock.c.  Remove this and it compiled.


5) RTEMS build module for CMake

This tells CMake how to build basic binaries for RTEMS - it does not know how to do this
"out of the box" on a stock install.

An "RTEMS.cmake" module is now directly included here with the PSP and can be directly 
used by the build system so long as it is included in the CMAKE_MODULE_PATH.  Add a line
like this if not already present (prior to the "project()" function):

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../psp/cmake/Modules" ${CMAKE_MODULE_PATH})


>>> IMPORTANT: Any code compiled with "-fPIC" will seriously confuse Cexp!! <<<

RTEMS does not (yet?) do native shared modules.  This build uses Cexp (installed earlier) 
to get around this but this is _NOT_ a shared module in the traditional sense - it actually
is just a simple relocatable object that it loads.

The "CMAKE_C_COMPILE_OPTIONS_PIC" is actually set to _not_ use -fPIC for this reason. 





II. Mission setup modifications to use PC-RTEMS PSP

Use a CMake toolchain file to build CFE for RTEMS.  This goes under the mission's "_defs" top-level directory.
This example below may be edited and tuned to the mission-specific needs.

Specifically this file should be called "toolchain-<ARCHNAME>.cmake" where <ARCHNAME> can be anything such as 
"cpu3" or "i686-pc-rtems".

Then in the "targets.cmake" file set the "TGT<N>_PLATFORM" variable to be the same ARCHNAME and the toolchain
file will be picked up and used by the build system.

====== START OF EXAMPLE TOOLCHAIN FILE: CUT HERE ===========

set(CMAKE_SYSTEM_NAME       RTEMS)
set(CMAKE_SYSTEM_PROCESSOR  i386)
set(CMAKE_SYSTEM_VERSION    4.11)
set(RTEMS_TOOLS_PREFIX      "/opt/rtems-${CMAKE_SYSTEM_VERSION}")
set(RTEMS_BSP_PREFIX        "$ENV{HOME}/rtems-${CMAKE_SYSTEM_VERSION}")
set(RTEMS_BSP               pc686)

# specify the cross compiler - adjust accord to compiler installation
# This uses the compiler-wrapper toolchain that buildroot produces
set(TARGET_PREFIX           "${CMAKE_SYSTEM_PROCESSOR}-rtems${CMAKE_SYSTEM_VERSION}-")
set(CPUTUNEFLAGS            "-march=i686 -mtune=i686")

SET(CMAKE_C_COMPILER        "${RTEMS_TOOLS_PREFIX}/bin/${TARGET_PREFIX}gcc")
SET(CMAKE_CXX_COMPILER      "${RTEMS_TOOLS_PREFIX}/bin/${TARGET_PREFIX}g++")
SET(CMAKE_LINKER            "${RTEMS_TOOLS_PREFIX}/bin/${TARGET_PREFIX}ld")
SET(CMAKE_ASM_COMPILER      "${RTEMS_TOOLS_PREFIX}/bin/${TARGET_PREFIX}as")
SET(CMAKE_STRIP             "${RTEMS_TOOLS_PREFIX}/bin/${TARGET_PREFIX}strip")
SET(CMAKE_NM                "${RTEMS_TOOLS_PREFIX}/bin/${TARGET_PREFIX}nm")
SET(CMAKE_AR                "${RTEMS_TOOLS_PREFIX}/bin/${TARGET_PREFIX}ar")
SET(CMAKE_OBJDUMP           "${RTEMS_TOOLS_PREFIX}/bin/${TARGET_PREFIX}objdump")
SET(CMAKE_OBJCOPY           "${RTEMS_TOOLS_PREFIX}/bin/${TARGET_PREFIX}objcopy")

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM   NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY   ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE   ONLY)

SET(CMAKE_PREFIX_PATH                   /)

# these settings are specific to cFE/OSAL and determines which
# abstraction layers are built when using this toolchain
SET(CFE_SYSTEM_PSPNAME                  pc-rtems)
SET(OSAL_SYSTEM_BSPTYPE                 pc-rtems)
SET(OSAL_SYSTEM_OSTYPE                  rtems-ng)



2) Get RTEMS GRUB boot image

NOTE: The GRUB image is not needed when using QEMU.  The "-kernel" option to QEMU makes things
much easier and is highly recommended.

RTEMS binaries cannot be directly booted by a PC BIOS...  Grub is one way to boot RTEMS.

Instructions for building GRUB for use with RTEMS@

https://devel.rtems.org/wiki/Building/Grub

If using grub, create a grub configuration file to work with the boot image

Minimal example below.  Replace <TGTNAME> accordingly, and can be otherwise modified as needed.

====== START OF EXAMPLE rtems-grub.cfg FILE: CUT HERE ===========

set default=0
set timeout=1

# Basic menu entry that boots with the console on the emulated VGA screen
menuentry <TGTNAME>-console { multiboot (hd0,0)/core-<TGTNAME>.exe }

# Alternative menu entry to output the RTEMS console to COM1: 
menuentry <TGTNAME>-com1 { multiboot (hd0,0)/core-<TGTNAME>.exe --console=com1 }

====== END OF EXAMPLE rtems-grub.cfg FILE: CUT HERE ===========

NOTE - This should be called "<TGTNAME>_rtems-grub.cfg" for the target_add_hook macro to find it
 
This can also be auto-generated by adding a "FILE(WRITE)" cmake command to the 
"target_add_hook" macro in the toolchain file above.  However by explicitly writing 
the file per cpu this gives extra control / tunability.




III. Building and Running CFE

1) Build mission as normal and run "make install" to copy binaries to /exe (if using cmake makefile wrapper)

2) A minimal QEMU boot sequence for reference (assuming "cpu1")

qemu-system-i386 -m 128 \ 
    -kernel ${INSTALL_DIR}/cpu1/core-cpu1.exe \
    -drive file=fat:rw:${INSTALL_DIR}/cpu1,format=raw \
    -nographic \
    -no-reboot
    
where ${INSTALL_DIR} represents the filesystem location of the installed binaries from "make install"

Note that the "fat" emulation used above is fairly limited, generally only useful for read-only testing.
To get read-write operation one would have to create real filesystem images and copy files into them, then 
use the images directly.  This is also what would need to be done to deploy on real hardware.

The "-no-reboot" flag causes QEMU to exit if a panic or shutdown event occurs, rather than emulating
an actual processor reset.  This is generally a good thing when debugging, since a bad build can
cause a reset loop.   This flag can be omitted if one wishes to emulate the processor reset.


3) Network-enabled boot sequence.

This is an example QEMU command that includes a simulated network device.  This works with the 
"CI_LAB" / "TO_LAB" apps to allow commands and telemetry from CFE to be exchanged with the host PC.

The "device" and "netdev" options can be tuned to your preferences (see documentation)

qemu-system-i386 -m 128 -nographic -no-reboot \
    -kernel ${INSTALL_DIR}/cpu1/core-cpu1.exe \
    -drive file=fat:rw:${INSTALL_DIR}/cpu1,format=raw \
    -device i82557b,netdev=net0,mac=00:04:9F:00:27:61 \
    -netdev user,id=net0,hostfwd=udp:127.0.0.1:1235-:1235 \
    
One can then use the "cmdutil" and "tlm_decode" applications to interact with the CFE.

    
4) Automated/scripted Unit-testing configuration using QEMU
 
The Unit Test BSP (utbsp) by default will start an RTEMS shell and allow
an interactive user to query/interact with the system as part of the unit testing.

The BSP also supports a "--batch-mode" option which is intended for 
automated or script-based execution.  It can be invoked through QEMU's 
"-append" option or through the GRUB bootloader if using real hardware.
When using QEMU this allows for  fully scripted testing.  

The basic QEMU command to execute a single unit test is:

qemu-system-i386 -m 128 \
   -kernel ${test-executable}.exe \
   -append '--batch-mode' \
   -nographic -no-reboot \
       > log-${test-executable}.txt


The entire suite of unit tests can be executed using a for loop:

for i in *test.exe *UT.exe; do \
   qemu-system-i386 -m 128 -kernel $i -append '--batch-mode' -nographic -no-reboot \
       | tee log-${i%%.exe}.txt;  \
done

