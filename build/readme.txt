To build and run unit-tests in Linux:
1) $ cd osal
2) $ source setvars.sh       --- sets $OSAL_SRC
4) $ cd build
5) You should not need to edit osal-config.mak, it defaults to Linux.
   (Note 64-bit Linux systems may need the OSAL_M32 option.)
    OS = posix
    BSP = pc-linux

6) $ make config --- copies the target's osconfig.h to the right place to build
7) $ make --- builds the osal
8) $ make clean-unit-tests
9) $ make unit-tests
10) Run unit tests
    $ cd unit-tests/test, where test = oscore-test |osfilesys-test | 
        osfile-test |osloader-test | osnetwork-test | ostimer-test
    $ run the .bin file, i.e. for oscore
    $ ./oscore-test.bin


To build and run unit-tests in VxWorks, see vxworks-target/readme.txt
