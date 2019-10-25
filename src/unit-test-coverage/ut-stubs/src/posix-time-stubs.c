/* OSAL coverage stub replacement for time.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include <overrides/time.h>

int OCS_clock_getres (OCS_clockid_t clock_id, struct OCS_timespec * res)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_clock_getres);

    return Status;
}

int OCS_clock_gettime (OCS_clockid_t clock_id, struct OCS_timespec * tp)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_clock_gettime);

    if (Status == 0 &&
            UT_Stub_CopyToLocal(UT_KEY(OCS_clock_gettime), tp, sizeof(*tp)) < sizeof(*tp))
    {
        memset(tp, 0, sizeof(*tp));
    }


    return Status;
}

int OCS_clock_nanosleep (OCS_clockid_t clock_id, int flags, const struct OCS_timespec * req, struct OCS_timespec * rem)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_clock_nanosleep);

    return Status;
}

int OCS_clock_settime (OCS_clockid_t clock_id, const struct OCS_timespec * tp)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_clock_settime);

    return Status;
}

int OCS_timer_create (OCS_clockid_t clock_id, struct OCS_sigevent * evp, OCS_timer_t * timerid)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_timer_create);

    return Status;
}

int OCS_timer_delete (OCS_timer_t timerid)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_timer_delete);

    return Status;
}

int OCS_timer_settime (OCS_timer_t timerid, int flags, const struct OCS_itimerspec * value, struct OCS_itimerspec * ovalue)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_timer_settime);

    return Status;
}

int OCS_timer_gettime (OCS_timer_t timerid, struct OCS_itimerspec * value)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_timer_gettime);

    if (Status == 0 && UT_Stub_CopyToLocal(UT_KEY(OCS_timer_gettime), value, sizeof(*value)) < sizeof(*value))
    {
        memset(value, 0, sizeof(*value));
    }

    return Status;
}

