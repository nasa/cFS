/* OSAL coverage stub replacement for dosFsLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/dosFsLib.h>


/* ----------------------------------------- */
/* prototypes normally declared in dosFsLib.h */
/* ----------------------------------------- */
OCS_STATUS OCS_dosFsVolFormat(char *path, int opt, OCS_FUNCPTR pPromptFunc)
{
    return (UT_DEFAULT_IMPL(OCS_dosFsVolFormat));
}

