## First run - with default mqueue implementation:

The OSAL black box unit tests tests were run in the following pc-linux environment:

System info:
Linux jsc-er-isl03.jsc.nasa.gov 2.6.32-504.30.3.el6.x86_64 #1 SMP Wed Jul 15 10:13:09 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux

GCC info:
Using built-in specs.
Target: x86_64-redhat-linux
Configured with: ../configure --prefix=/usr --mandir=/usr/share/man --infodir=/usr/share/info --with-bugurl=http://bugzilla.redhat.com/bugzilla --enable-bootstrap --enable-shared --enable-threads=posix --enable-checking=release --with-system-zlib --enable-__cxa_atexit --disable-libunwind-exceptions --enable-gnu-unique-object --enable-languages=c,c++,objc,obj-c++,java,fortran,ada --enable-java-awt=gtk --disable-dssi --with-java-home=/usr/lib/jvm/java-1.5.0-gcj-1.5.0.0/jre --enable-libgcj-multifile --enable-java-maintainer-mode --with-ecj-jar=/usr/share/java/eclipse-ecj.jar --disable-libjava-multilib --with-ppl --with-cloog --with-tune=generic --with-arch_32=i686 --build=x86_64-redhat-linux
Thread model: posix
gcc version 4.4.7 20120313 (Red Hat 4.4.7-16) (GCC) 

libc info:
GNU C Library stable release version 2.12, by Roland McGrath et al.
Copyright (C) 2010 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.
Compiled by GNU CC version 4.4.7 20120313 (Red Hat 4.4.7-16).
Compiled on a Linux 2.6.32 system on 2016-02-16.
Available extensions:
        The C stubs add-on version 2.1.2.
        crypt add-on version 2.1 by Michael Glad and others
        GNU Libidn by Simon Josefsson
        Native POSIX Threads Library by Ulrich Drepper et al
        BIND-8.2.3-T5B
        RT using linux kernel aio
libc ABIs: UNIQUE IFUNC
For bug reporting instructions, please see:
<http://www.gnu.org/software/libc/bugs.html>.

Manually applied -m32 change to build/osal-config.mak.
Manually applied -export-dynamic to src/bsp/pc-linux/make/link-rules.mak.

Note: on the x86_64 platform had to rebuild the eeprom support files for the 
unit-tests/osloader-test.  Instructions: after "make unit-tests":

cd osal/build/unit-tests/osloader-test
make -f Modules.mak clean
make -f Modules.mak
# and continue unit testing

The expected results of the black box unit tests in a pc-linux environment are 
provided in the _log.txt file for each test.

Differences in unit tests from OSAL 4.2.0 Results files:
        ut_oscore: all tests now passing (was 3 failures), 
                   only one is now not-applicable (was 7),
                   overall several formerly-failing and N/A tests were not run.
        ut_osfile: all tests now pass (was 1 test setup failure)
        ut_osfilesys: all tests pass
        ut_osloader & ut_osnetwork: all tests pass, identical results as 4.2.0 files
        ut_ostimer: all tests now pass (was 1 failure)

====================================================================================