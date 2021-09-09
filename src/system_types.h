#ifndef _SYSTEM_TYPES_H
#define _SYSTEM_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <cglm/cglm.h>
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <checksum.h>
#include "log.h"
#include "obj.h"

#define DATA_DIR "data/"

typedef int16_t fixed16_t;

float fixed2float(fixed16_t p);

fixed16_t float2fixed(float v);

#endif

