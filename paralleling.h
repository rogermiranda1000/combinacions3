#ifndef _PARALLELING_H_
#define _PARALLELING_H_

#include "combination.h"
#include <stdbool.h>

#define THREADS 32
#define OPERATIONS_PER_THREAD 250000 // cada thread obtendrá 'OPERATIONS_PER_THREAD' valores

#if (THREADS > 1)
    #include <pthread.h>
#endif

void *paralelFunction(void *input);
bool paralel(Lenght *current);

#endif
