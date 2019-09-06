Files in this directory contain an implmentation that adheres to a defined API
that is applicable to more than one of the supported OS's, but not all.  For
example, the BSD-style sockets API is implemented in VxWorks, RTEMS, and Linux.
Therefore it is beneficial to put the code in here and let each implementation
selectively include this rather than reinventing the wheel.

