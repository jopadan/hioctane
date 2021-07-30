#ifndef _COLUMN_H
#define _COLUMN_H

#include <stdint.h>
#include <stdbool.h>

#define COLUMNS_COUNT 1024
#define COLUMNS_START 98012

typedef struct column_s
{
	union {
		uint8_t data[26];
		struct {
			uint8_t shape;
			uint8_t unnused;
			int16_t floor_texid;
			int16_t unknown1;
			union {
				int16_t blocks[8];
				struct {
					int16_t a;
					int16_t b;
					int16_t c;
					int16_t d;
					int16_t e;
					int16_t f;
					int16_t g;
					int16_t h;
				};
				int16_t unknown2;
				int16_t unknown3;
			};
		};
	};
} column_t;

//column_t* column_create(uint8_t* data);
//bool column_destroy(column_t* column);
#endif
