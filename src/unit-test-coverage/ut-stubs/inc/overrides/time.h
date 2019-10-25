/* OSAL coverage stub replacement for time.h */
#ifndef _OSAL_STUB_TIME_H_
#define _OSAL_STUB_TIME_H_

/* ----------------------------------------- */
/* constants normally defined in time.h */
/* ----------------------------------------- */

#define OCS_CLOCK_REALTIME          0x1301
#define OCS_CLOCK_MONOTONIC         0x1302
#define OCS_TIMER_ABSTIME           0x1701

/* ----------------------------------------- */
/* types normally defined in time.h */
/* ----------------------------------------- */

typedef int             OCS_clockid_t;
typedef int             OCS_timer_t;
typedef long            OCS_time_t;

struct OCS_timespec
{
    long tv_sec;
    long tv_nsec;
};

struct OCS_itimerspec
{
    struct OCS_timespec it_interval;
    struct OCS_timespec it_value;
};

struct OCS_sigevent;

typedef void (*OCS_VOIDFUNCPTR)(void);

/* ----------------------------------------- */
/* prototypes normally declared in time.h */
/* ----------------------------------------- */

extern int OCS_clock_getres (OCS_clockid_t clock_id, struct OCS_timespec * res);
extern int OCS_clock_gettime (OCS_clockid_t clock_id, struct OCS_timespec * tp);
extern int OCS_clock_nanosleep (OCS_clockid_t clock_id, int flags, const struct OCS_timespec * req, struct OCS_timespec * rem);
extern int OCS_clock_settime (OCS_clockid_t clock_id, const struct OCS_timespec * tp);
extern int OCS_timer_create (OCS_clockid_t clock_id, struct OCS_sigevent * evp, OCS_timer_t * timerid);
extern int OCS_timer_delete (OCS_timer_t timerid) ;
extern int OCS_timer_gettime (OCS_timer_t timerid, struct OCS_itimerspec * value);
extern int OCS_timer_settime (OCS_timer_t timerid, int flags, const struct OCS_itimerspec * value, struct OCS_itimerspec * ovalue);

extern int OCS_timer_connect(OCS_timer_t, OCS_VOIDFUNCPTR, int);


#endif /* _OSAL_STUB_TIME_H_ */

