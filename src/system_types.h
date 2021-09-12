#ifndef _SYSTEM_TYPES_H
#define _SYSTEM_TYPES_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>
#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "log.h"
#include "obj.h"
#include "crc.h"

#define DATA_DIR "data/"

typedef int16_t fixed16_t;

float fixed2float(fixed16_t p);

fixed16_t float2fixed(float v);

#endif

