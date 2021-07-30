#ifndef _BLOCK_H
#define _BLOCK_H

#include <stdint.h>
#include <stdbool.h>

#define BLOCKS_COUNT 1024


typedef struct block_s
{
	union {
		uint8_t data[16];
		struct {
			int8_t n;
			int8_t e;
			int8_t s;
			int8_t w;

			int8_t t;
			int8_t b;
			int8_t nmod;
			int8_t emod;

			int8_t smod;
			int8_t wmod;
			int8_t tmod;
			int8_t bmod;

			int8_t unknown[4];
		};
	};
} block_t;

#endif
