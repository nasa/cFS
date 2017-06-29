## 
## osal-config.mak
##
## This file specifies the Hardware Architecture ( CPU ), 
## Platform ( Board ), OS, and BSP to build.
##
## NOTE: Make sure each selection does not have any spaces
##       after it!
##

##---------------------------------------------------------
## Operating System
## OS = The operating system selected for the Abstraction implementation
## 
## Current selections include:
## 
## OS = posix
## OS = rtems
## OS = vxworks6 
##---------------------------------------------------------
OS = posix

##---------------------------------------------------------
## BSP -- BSP/Operating system for the board
##
## Current selections include:
## 
## BSP = pc-linux-ut         -- Unit test target
## BSP = pc-linux            -- Target to run on PC-linux
## BSP = genppc-vxworks6.4   -- vxWorks Power PC target
## BSP = mcf5235-rtems       -- RTEMS Coldfire evaluation board
## BSP = sis-rtems           -- RTEMS Sparc Instruction Simulator (GDB)
## BSP = sparc-vxworks6.7    -- ut699 SPARC Leon3 development board
##---------------------------------------------------------
BSP = pc-linux

##---------------------------------------------------------
## OSAL_M32 -- optional, build 32-bit images on 64-bit build machines
##
##     ** Need 32-bit images ??  **
##     ** Have 64-bit images ??  **
##     ** Uncomment this option. **
##
## If you are not using GCC, you may need to change -m32 to the
## correct flag for your compiler. This flag may also be specified on
## the "make" command line or in the shell environment.
##---------------------------------------------------------
# OSAL_M32 = -m32
