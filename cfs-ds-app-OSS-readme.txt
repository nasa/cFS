core Flight System (cFS) Data Storage Application (DS) 
Open Source Release Readme

DS Release 2.5.1 

Date: 
February 15, 2017

Introduction:
  The Data Storage application (DS) is a core Flight System (cFS) application 
  that is a plug in to the Core Flight Executive (cFE) component of the cFS.  
  
  The cFS is a platform and project independent reusable software framework and
  set of reusable applications developed by NASA Goddard Space Flight Center.  
  This framework is used as the basis for the flight software for satellite data 
  systems and instruments, but can be used on other embedded systems.  More 
  information on the cFS can be found at http://cfs.gsfc.nasa.gov and 
  http://coreflightsystem.org
  
  The DS application is used for storing software bus messages in files. These 
  files are generally stored on a storage device such as a solid state recorder 
  but they could be stored on any file system. Another cFS application such as 
  CFDP (CF) must be used in order to transfer the files created by DS from 
  their onboard storage location to where they will be viewed and processed.

  The DS application is written in C and depends on the cFS Operating System 
  Abstraction Layer (OSAL) and cFE components.  To build and run the DS
  application, follow the cFS Deployment Guide instructions contained in 
  cFE-6.4.1-OSS-release/docs.  There is additional DS application specific 
  configuration information contained in the application user's guide
  available in cfs-ds-2.4.0-OSS-release/docs/users_guide
  
  There are also "Quick start" instructions provided in 
  cFE-6.5.0-OSS-release/cfe-OSS-readme.txt   
  
  The OSAL is available at http://sourceforge.net/projects/osal/ and 
  github.com/nasa/
  
  The cFE is available at http://sourceforge.net/projects/coreflightexec

  This software is licensed under the NASA Open Source Agreement. 
  http://ti.arc.nasa.gov/opensource/nosa
 
 
Software Included:
  Data Storage application (DS) 2.5.1
  
 
Software Required:

 Operating System Abstraction Layer 4.2.0 or higher can be 
 obtained at http://sourceforge.net/projects/osal or 
 github.com/nasa/osal
 
 core Flight Executive 6.5.0 or higher can be obtained at
 http://sorceforge.net/projects/coreflightexec

  
Runtime Targets Supported:
   The "out of the box" targets in the cFE 6.5.0 distribution include:
     1. 32 bit x86 Linux ( CentOS 6.x )
     2. Motorola MCP750 PowerPC vxWorks 6.x

Other targets: 
    Other targets are included, but may take additional work to
    run. They are included as examples of other target 
    environments.
    1. mcf5235-rtems - This is for the Axiom MCF5235 Coldfire board running
                       RTEMS 4.10. It requires a static loader component for the
                       OS abstraction layer. The static loader is currently
                       not available as open source, so this target is not
                       considered complete. RTEMS 4.11 will have a dynamic
                       loader which will be supported by a future release
                       of the OS Abstraction Layer, completing the RTEMS support
                       for the cFE.
          
                       Once RTEMS 4.11 is released, the goal is to support
                       an RTEMS simulator platform such as SPARC/sis or 
                       quemu.

    2. mac-osx and pc-cygwin - These targets are included for examples. They
                       will most likely be removed in the future. 
                       The pc-linux target is highly recommended. If you 
                       have a windows or mac computer it is relatively easy to 
                       set up a free virtual machine environment with 32 bit 
                       Cent OS 5.x.
                       
EOF                       