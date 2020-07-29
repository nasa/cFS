# Scheduler

NASA core Flight System Scheduler Application

## Description

The Scheduler application (SCH) is a core Flight System (cFS) application that is a plug in to the Core Flight Executive (cFE) component of the cFS.

The cFS is a platform and project independent reusable software framework and set of reusable applications developed by NASA Goddard Space Flight Center. This framework is used as the basis for the flight software for satellite data systems and instruments, but can be used on other embedded systems. More information on the cFS can be found at [http://cfs.gsfc.nasa.gov](http://cfs.gsfc.nasa.gov)

The SCH application provides a method of generating software bus messages at pre-determined timing intervals. This allows the system to operate in a Time Division Multiplexed (TDM) fashion with deterministic behavior. The TDM major frame is defined by the Major Time Synchronization Signal used by the
cFE TIME Services (typically 1 Hz). The Minor Frame timing (number of slots executed within each Major Frame) is also configurable.

## License

This software is licensed under the NASA Open Source Agreement. http://ti.arc.nasa.gov/opensource/nosa
