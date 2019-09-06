/* OSAL coverage stub replacement for sched.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/sched.h>


int OCS_sched_get_priority_max(int policy)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OCS_sched_get_priority_max,9);

    return Status;
}


int OCS_sched_get_priority_min(int policy)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL_RC(OCS_sched_get_priority_min,1);

    return Status;
}



