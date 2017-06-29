/* pull in the OSAL configuration */
#include "osconfig.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "stub-map-to-real.h"

/* The unmodified/untweaked source can be used for this module */
#include "os/vxworks6/osfilesys.c"
