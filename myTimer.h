#ifndef MYTIMER_H
#define MYTIMER_H
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

void myWait(clock_t target);
void *myTWait ( void *myvar);

#endif

