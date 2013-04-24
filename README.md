Operating System Abstraction Layer Project
==========================================

![OSAL Logo by Michael Cudmore](./doc/OSAL-Logo.png)


The OS Abstraction Layer was originally devloped by the Flight Software Systems Branch at NASA Goddard Space Flight Center.

OS Abstraction Layer information:
=================================

This distribution contains:

1. The OS Abstraction Layer Library
2. Tests and example applications
3. Impelemtations for the following targets:
    - Generic PPC running vxWorks 6.x ( Tested on vxWorks 6.4 )
    - Axiom m5235BCC Coldfire Evaluation board running the RTEMS 4.7 Operating System
    - A standard Intel Pentium PC running Linux 
    - The sparc/sis simulator running RTEMS 
4. A directory structre and makefiles to manage it all.

Getting Started:
================

See the document doc/OSAL-Configuration-Guide-1.0.doc for complete details.

An easy way to get started is to use the Linux port:
----------------------------------------------------

1. Set the OSAL_SRC environment variable to point to the OSAL source code. 
    Running setvars.sh will set the variable for you ($ . ./setvars.sh)
2. Edit the build/osal-config.mak file and set the following options:
    BSP - Set this to the board you are running on. For a PC running linux, just usepc-linux 
    OS - Set this to the OS you are running. For a PC running linux, use posix.

Example of a PC running linux:

    export OSAL_SRC = /home/acudmore/osal/src

In build/osal-config.mak:

    OS=posix
    BSP=pc-linux

Now just type *make* from the build directory and it should build the OSAL core files, tests, and sample applications for you.The binary for each application is its own directory ( i.e. build/examples/tasking-example/tasking-example.bin ) You can switch to that directory and run it. You can also debug it using GDB.

The Embedded targets take a little more work to run, because they must be cross compiled and booted on the board. By copying a target, you should be able to come up with a new target.

If you would like just the OSAL itself, just look in src/os/inc for the include files and src/os/<your os here> for the OSAL implementation. 

The API document is in the *doc* directory.

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

