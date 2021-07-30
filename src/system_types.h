#ifndef _SYSTEM_TYPES_H
#define _SYSTEM_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define DATA_DIR "data/"

typedef int16_t fixed16_t;

inline float fixed2float(fixed16_t p)
{
	return (float) (p & 0x00ff) / 255.0f + (float)(p >> 8);
}

inline fixed16_t float2fixed(float v)
{
	fixed16_t x = (fixed16_t)fminf(255.0f, floorf(v));
	return (x << 8) + fminf(255.0f, 255.0f * (v - fminf(255.0f, floorf(v))));
}

#endif

