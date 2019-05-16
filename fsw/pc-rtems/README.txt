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

The RTEMS official docs have instructions for this.


2) Clone/Bootstrap RTEMS source tree:

Note - This is assuming running the newest "development" RTEMS version which, as of this writing,
seems to be something of a moving target.  Depending on when the clone is done it may work perfectly
or may not even compile.  FWIW, Git revision "52f8e90" seems to work OK as a point of reference.
It may be worthwhile to try using the 4.10.2 tag -- this may be more stable, but I have not used
this with the pc-rtems BSP yet.

OFFICIAL RTEMS CLONE URL: git://git.rtems.org/rtems

$ git clone <rtems-source-url>
$ export PATH=/opt/rtems-4.11/bin:$PATH
$ (cd rtems && ./bootstrap)


3) Build/Install RTEMS pc686 BSP

Note - use same terminal as previous step, or set PATH environment again.
The "prefix" can be whatever location is preferable for installation.

$ mkdir b-pc686
$ cd b-pc686
$ ../rtems/configure --target=i386-rtems4.11 --enable-rtemsbsp=pc686 --prefix=${HOME}/rtems-4.11
$ make
$ make install
$ cd ..


4) Install cexp-2.2 (dynamic module loader library)

This is necessary to get the module loader to compile and work.

Ref page: http://www.slac.stanford.edu/~strauman/rtems/cexp/index.html
Tarball: http://www.slac.stanford.edu/~strauman/rtems/cexp/cexp-2.2.tgz

$ tar zxvf cexp-2.2.tgz
$ mkdir b-cexp
$ cd b-cexp
$ ../cexp-CEXP_Release_2_2/configure --with-rtems-top=${HOME}/rtems-4.11 --host=i386-rtems4.11 --enable-std-rtems-installdirs
$ make
$ make install
$ cd ..


5) install RTEMS build module for CMake

This tells CMake how to build basic binaries for RTEMS - it does not know how to do this
"out of the box" on a stock install.  

>>> IMPORTANT: Any code compiled with "-fPIC" will seriously confuse Cexp!! <<<

RTEMS does not (yet?) do native shared modules.  This build uses Cexp (installed earlier) 
to get around this but this is _NOT_ a shared module in the traditional sense - it actually
is just a simple relocatable object that it loads.

The "CMAKE_C_COMPILE_OPTIONS_PIC" is actually set to _not_ use -fPIC for this reason. 

====== START OF EXAMPLE RTEMS CMAKE PLATFORM MODULE: CUT HERE ===========

# CMAKE variables for compiling for RTEMS

set(CMAKE_DL_LIBS "")

# Note - RTEMS does not support shared libraries natively.
# This creates a simple relocatable object file that can be loaded via cexp
set(CMAKE_SHARED_LIBRARY_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "")
set(CMAKE_SHARED_MODULE_C_FLAGS "")
set(CMAKE_SHARED_MODULE_CXX_FLAGS "")
set(CMAKE_SHARED_MODULE_SUFFIX ".o")
set(CMAKE_C_CREATE_SHARED_MODULE "<CMAKE_LINKER> <LINK_FLAGS> -o <TARGET> -r <OBJECTS> <LINK_LIBRARIES>")
set(CMAKE_CXX_CREATE_SHARED_MODULE ${CMAKE_C_CREATE_SHARED_MODULE})
set(CMAKE_C_CREATE_SHARED_LIBRARY ${CMAKE_C_CREATE_SHARED_MODULE})
set(CMAKE_CXX_CREATE_SHARED_LIBRARY ${CMAKE_C_CREATE_SHARED_MODULE})

# Note that CEXP is not a shared library loader - it will not support code compiled with -fPIC
# Also exception handling is very iffy.  These two options disable eh_frame creation.
set(CMAKE_C_COMPILE_OPTIONS_PIC -fno-exceptions -fno-asynchronous-unwind-tables)

# The RTEMS_C_FLAGS flags need to be added onto all RTEMS gcc commands
set(RTEMS_C_FLAGS "--pipe -specs bsp_specs -qrtems")
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
set (CMAKE_EXE_LINKER_FLAGS_INIT  "-Wl,-Ttext,0x00100000 -u Init")
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> ${RTEMS_C_FLAGS} -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> ${RTEMS_C_FLAGS} -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
set(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER> <DEFINES> <FLAGS> ${RTEMS_C_FLAGS} -o <OBJECT> -c <SOURCE>")
set(CMAKE_CXX_COMPILE_OBJECT "<CMAKE_CXX_COMPILER> <DEFINES> <FLAGS> ${RTEMS_C_FLAGS} -o <OBJECT> -c <SOURCE>")

include(Platform/UnixPaths)

====== START OF EXAMPLE RTEMS CMAKE PLATFORM MODULE: CUT HERE ===========

If using a CMake package from the host Linux build system's official repo (CentOS/Ubuntu) then 
it can be patched by copying the file in.  Example installation of this file to system-wide CMake:

$ sudo cp RTEMS.cmake /usr/share/cmake-*/Modules/Platform/RTEMS.cmake




II. Mission setup modifications to use PC-RTEMS PSP

1) Toolchain file

Use a CMake toolchain file to build CFE for RTEMS.  This goes under the mission's "_defs" top-level directory.
This example below may be edited and tuned to the mission-specific needs.

Specifically this file should be called "toolchain-<ARCHNAME>.cmake" where <ARCHNAME> can be anything such as 
"cpu3" or "i686-pc-rtems".

Then in the "targets.cmake" file set the "TGT<N>_PLATFORM" variable to be the same ARCHNAME and the toolchain
file will be picked up and used by the build system.

====== START OF EXAMPLE TOOLCHAIN FILE: CUT HERE ===========

SET(RTEMS_TOOLS_TOP     "/opt/rtems-4.11"       CACHE PATH "Rtems tools directory")
SET(RTEMS_BSP_TOP       "$ENV{HOME}/rtems-4.11" CACHE PATH "Rtems BSP install directory")

SET(CMAKE_SYSTEM_NAME         RTEMS)
SET(CMAKE_SYSTEM_VERSION      1)
SET(CMAKE_SYSTEM_PROCESSOR    i686)
SET(CMAKE_C_COMPILER          ${RTEMS_TOOLS_TOP}/bin/i386-rtems4.11-gcc)
SET(CMAKE_CXX_COMPILER        ${RTEMS_TOOLS_TOP}/bin/i386-rtems4.11-g++)
SET(CMAKE_LINKER              ${RTEMS_TOOLS_TOP}/bin/i386-rtems4.11-ld)

SET(CMAKE_FIND_ROOT_PATH                ${RTEMS_TOOLS_TOP}/i386-rtems4.11 ${RTEMS_TOOLS_TOP}/lib/gcc/i386-rtems4.11/4.8.2 ${RTEMS_BSP_TOP}/i386-rtems4.11/pc686)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM   NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY   ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE   ONLY)

SET(CMAKE_PREFIX_PATH                   /)
SET(CFE_SYSTEM_PSPNAME                  pc-rtems)
SET(OSAL_SYSTEM_OSTYPE                  rtems)
SET(CFE_ENTRY_SYM                       Init)

SET(CMAKE_C_FLAGS_INIT "-march=i686 -B${RTEMS_BSP_TOP}/i386-rtems4.11/pc686/lib" CACHE STRING "C Flags required by platform")

# Macro to copy the "rtems-grub.cfg" file into place in the exe directory
macro(target_add_hook TGTNAME)
  install(FILES ${MISSION_DEFS}/${TGTNAME}_rtems-grub.cfg DESTINATION ${TGTNAME} RENAME rtems-grub.cfg)
endmacro()

====== END OF EXAMPLE TOOLCHAIN FILE: CUT HERE ===========


2) Get RTEMS GRUB boot image

RTEMS binaries cannot be directly booted by a PC BIOS...  Grub is one way to boot RTEMS.
The RTEMS FTP site has an example pre-built GRUB image for this at the following URL:

ftp://ftp.rtems.org/pub/rtems/archive/current_contrib/pc386_grub_image

Once downloaded this file can be saved OUTSIDE the mission git tree and used forever.  It
only serves as an intermediate to boot the binary image since the RTEMS binary cannot be
booted directly by a PC BIOS.  Alternatively one could build a custom loader if needed.

NOTE: the current grub image being distributed from the FTP site contains a grub config file.
This configuration file would need to be replaced with the customized version containing 
the correct boot command (see next section).


3) Create a grub configuration file to work with the boot image

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

2) A minimal QEMU boot sequence for reference

First copy the "core-<TGTNAME>.exe" file and the grub configuration file into a separate directory 
that can be mounted as /boot on the target:

$ mkdir boot
$ cp core-<TGTNAME>.exe rtems-grub.cfg boot

Then boot QEMU from the boot floppy, use "boot" as the first virtual disk and "eeprom" as the second virtual disk:

$ qemu-system-i386 -m 128 -boot a -fda ./rtems-boot.img -hda fat:$PWD/boot -hdb fat:$PWD/eeprom -serial stdio

Note - depending on needs, that command might be different - QEMU has _lots_ of options. 

Also note that the "fat" emulation used above is fairly limited, generally only useful for read-only testing.
To get read-write operation one would have to create real filesystem images and copy files into them, then 
use the images directly.  This is also what would need to be done to deploy on real hardware.

If using the "COM1" (serial port console) variant of the boot options above, the "-serial stdio" option
to QEMU is helpful - this connects the virtualized com1 to the standard I/O of the QEMU process.  You must
give _something_ as a "-serial" option when using this or the console will not be visible.


