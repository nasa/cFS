/* OSAL coverage stub replacement for memPartLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/memPartLib.h>

OCS_STATUS OCS_memPartShow(OCS_PART_ID partId, int type)
{
    return (UT_DEFAULT_IMPL(OCS_memPartShow));
}

OCS_STATUS OCS_memPartInfoGet(OCS_PART_ID partId, OCS_MEM_PART_STATS * ppartStats)
{
    return (UT_DEFAULT_IMPL(OCS_memPartInfoGet));
}



OCS_PART_ID OCS_memSysPartId;

