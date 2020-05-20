//Author: Gordon Griesel
#ifdef PROFILE
#include <ctime>
#include <string.h>

void timeGet(struct timespec *t);
double timeDiff(struct timespec *start, struct timespec *end);
void timeCopy(struct timespec *dest, struct timespec *source);

#endif
