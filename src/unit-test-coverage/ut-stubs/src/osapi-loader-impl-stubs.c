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


/*
 * Module Loader API
 */
UT_DEFAULT_STUB(OS_ModuleLoad_Impl,( uint32 module_id, char *translated_path ))
UT_DEFAULT_STUB(OS_ModuleUnload_Impl,( uint32 module_id ))
UT_DEFAULT_STUB(OS_ModuleGetInfo_Impl,( uint32 module_id, OS_module_prop_t *module_prop ))
UT_DEFAULT_STUB(OS_SymbolLookup_Impl,( cpuaddr *SymbolAddress, const char *SymbolName ))
UT_DEFAULT_STUB(OS_SymbolTableDump_Impl,( const char *filename, uint32 size_limit ))

