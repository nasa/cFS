/**
 * @file impl-stubs.c
 *
 * UT assert stubs for low-level implementation functions.
 * These are prototyped in the "os-impl.h" file within the shared API
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"
#include "os-impl.h"

UT_DEFAULT_STUB(OS_SelectSingle_Impl,(uint32 stream_id, uint32 *SelectFlags, int32 msecs))
UT_DEFAULT_STUB(OS_SelectMultiple_Impl,(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs))
