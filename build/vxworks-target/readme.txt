The scripts in this directory are for loading test code to a VxWorks embedded
processor board, running tests on the embedded board from the console window,
and getting test log files off a RAM disk on the embedded board.

The load and get log scripts were originally written to generically work with 
all cfe, osal, and psp unit tests and white box tests in JSC's CFS_TST build
project, modeled after the de-facto standard build/run approach in gsfc_build.
But since they are to work separately with each layer (cfe, osal, psp) now,
they have been modified to only work in each respective layer (although the 
versions in each layer will be 90%+ the same script).

Also, currently, both the unit test and white box test run scripts are located
in this directory since the load and get log scripts currently still function
with both sets of tests.

Usage:
NOTE: Unit tests, scripts, expect a ram disk called "/ram0" with a cf/apps dir
to have already been created. This can be done either in the kernel config
of by the user on the command line before running these tests.

Steps for sparc-vxworks6.7
--------------------------------
1) $ cd osal
2) $ source setvars.sh       --- sets $OSAL_SRC
3) Set VxWorks environment if building for VxWorks
   At JSC, set env for SPARC-VxWorks 6.7
        $ source ~/.vx67
        $ wre
4) $ cd build
5) Edit osal-config.mak to select the desired OSAL and BSP to use with the tests
   For example:
   FROM default: OS = posix TO OS = vxworks6
   FROM default: BSP = pc-linux TO BSP = sparc-vxworks6.7

6) $ make config --- copies the target's osconfig.h to the right place to build
7) $ make clean-unit-tests
8) $ make unit-tests
9) Run unit tests on target board
    $ cd vxworks-target
    $ ./load_ut osal ostest, where ostest = oscore | osfilesys | osfile | osloader | osnetwork | ostimer

10) After loading a test, go to console window connected to target board
    -> cd "/ram0/cf/apps"
    -> runvx_osal_test, where test = oscore | osfilesys | osfile | osloader | osnetwork | ostimer

11) After running each test, get the log file
    $ ./get_log osal test, where test = oscore | osfilesys | osfile | osloader | osnetwork | ostimer





