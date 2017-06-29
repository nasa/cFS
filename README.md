Operating System Abstraction Layer Project
==========================================

![OSAL Logo by Michael Cudmore](./doc/OSAL-Logo.png)


The OS Abstraction Layer was originally devloped by the Flight Software Systems Branch at NASA Goddard Space Flight Center.

OS Abstraction Layer information:
=================================

This distribution contains:

1. The OS Abstraction Layer Library
2. Tests and example applications
3.  for the following targets:
    - Generic PPC running vxWorks 6.x ( Tested on vxWorks 6.4 )
    - Axiom m5235BCC Coldfire Evaluation board running the RTEMS 4.10 Operating System
    - A PC running Linux ( Tested on CentOS 5.x and 6.x 32 bit )
    - The sparc/sis simulator running RTEMS 4.10.2
4. A directory structure and makefiles to manage it all.

Getting Started:
================

See the document *doc/OSAL-Configuration-Guide.doc* for complete details.

An easy way to get started is to use the Linux port and classic build: 

1. Set the *OSAL_SRC* environment variable to point to the OSAL source code. 
     - Running setvars.sh will set the variable for you ($source ./setvars.sh)
2. Edit the *build/osal-config.mak* file and set the following options:
     - BSP - Set this to the board you are running on. For a PC running linux, use *pc-linux* 
     - OS - Set this to the OS you are running. For a PC running linux, use *posix*.
     - OSAL_M32 - Uncomment/Add this build variable for building 32-bit images using "native" 
       GCC on a 64-bit X86 Linux platform

Buiding on a PC running linux:

    export OSAL_SRC = /home/acudmore/osal/src

In build/osal-config.mak:

    OS=posix
    BSP=pc-linux
    
    Optional: OSAL_M32 = -m32 (Note: Usage of this flag may require an optional 'multilib' (or similar) 
    package to be installed. Refer to your operating system and toolchain documentation for details, if 
    adding the appropriate flag causes your builds to fail due to (for example) missing 32-bit or 
    multilib related headers or libraries.) 

Optional:  Some Linux systems may require an additional linker option in
    src/bsp/pc-linux/make/link-rules.mak:

    LDFLAGS ?= $(OSAL_M32) -export-dynamic

    If the symbol-api-test fails, then you need this option.

Now just type *make config; make* from the build directory and it should build the OSAL core files, tests, and sample
applications for you. The binary for each application is in its own directory 
(i.e. build/examples/tasking-example/tasking-example.bin) You can switch to that directory and run it. You 
can also debug it using GDB.

NOTE: Running on linux may require root or adjusting the posix message queue maximum sizes. 

The Embedded targets take a little more work to run, because they must be cross compiled and booted on the board. 
By copying a target, you should be able to come up with a new target.

If you would like just the OSAL itself, just look in src/os/inc for the include files and src/os/<your os here> 
for the OSAL implementation. 

The API documentation is in the *doc* directory.

There are two sets of tests: build/tests and build/unit-tests.  To build and the unit tests,
perform the build steps above then *make unit-tests* in the build directory.

Instructions on how to use the newely supported cmake build system are provided in the OSAL Configuration Guide
located in the *doc* directory.

Contact Information: 
====================

    Alan Cudmore
    NASA Goddard Space Flight Center
    Code 582.0
    Greenbelt, MD 20771
    Alan.P.Cudmore@nasa.gov

Copyright notice:
=================

Copyright United States Government as represented by the Administrator of the National Aeronautics and Space Administration

License information:
==================== 

This software is licensed under NASAs Open Source Agreement. The release of the software is conditional upon the recipients acceptance of the Open Source Agreement. Please see the file: NASA_Open_Source_Agreement_1_3-OS_AbstractionLayer.txt

