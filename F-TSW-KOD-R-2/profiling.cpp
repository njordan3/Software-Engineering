//Author: Gordon Griesel
#ifdef PROFILE
#include "profiling.h"

void timeGet(struct timespec *t)
{
    clock_gettime(CLOCK_REALTIME, t);
}

double timeDiff(struct timespec *start, struct timespec *end)
{
    //return the difference of two times
    double ret =
        (double)(end->tv_sec - start->tv_sec) +
        (double)(end->tv_nsec - start->tv_nsec);
    return ret;
}

void timeCopy(struct timespec *dest, struct timespec *source)
{
    //copy one time structure to another
    memcpy(dest, source, sizeof(struct timespec));
}
#endif
