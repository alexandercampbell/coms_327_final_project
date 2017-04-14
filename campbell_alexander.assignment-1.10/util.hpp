
#pragma once

#include <stdlib.h>

#define RAND_BETWEEN(a, b) (a + rand() % (b-a+1))
#define CLAMPED_TO(a, min, max) ((a) < (min) ? (min) : ((a) > (max)) ? (max) : (a))
#define FRAND() (float)rand()/(float)(RAND_MAX)

