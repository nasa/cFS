UT-OSAL README FILE
===================

The objective here is to build the OSAL source code but with all calls/references to the underlying
OS remapped to "stub" implementations and/or local definitions of those identifiers.

The "inc" subdirectory includes empty files for _ALL_ of the header files included by the OSAL source
code.  The "inc" subdirectory is in the compiler search path so these empty files will be used in
place of the real system header file.  THIS IS THE ONLY PLACE WHERE THESE EMPTY STUBS SHOULD BE
USED IN PLACE OF THE REAL FILE.

Two exceptions are made:
 - <stdint.h> maps to the real one, so we have accurate fixed-width types.  
 
 
The paradigm used here:
- Each source file in OSAL has a wrapper here with the same name.

- First the wrapper includes a header with any "extra" logic that the test case needs to add
  This is optional and used in cases where the test needs access to static/private data structures.
  
- Then the wrapper #include's "stub-map-to-real.h"
  This is an important file that has #define macros that map all identifiers typically supplied
  by the C library to one with a "VCS_" prefix.  For example, "strcmp" will become "VCS_strcmp".  

- Then it #include's the unmodified OSAL source code, adding that exact code into the same compilation
  unit, with the stub-remap macros in place.
  
  In the same example as above, all calls to "strcmp()" (a library function) will become calls to 
  "VCS_strcmp" with the same arguments.  The implementation of VCS_strcmp() is provided by the test case.
  
- Finally the implementation for any "extra" logic is directly implmemented in the wrapper file.  
  Because this is in the same compilation unit as the OSAL source code, it can access variables that
  are defined "static".   IMPORTANT: The implementation of these wrappers will also have all stub-mapping
  active, so these must avoid using C library calls as they will NOT go to the C library.
  
  
   