#ifndef _MAP_ENTRY_H
#define _MAP_ENTRY_H

#include "system_types.h"
#include "column.h"
#include "map_entry.h"
#include <cglm/cglm.h>

#define MAP_WIDTH 256
#define MAP_HEIGHT 160

typedef struct map_tile_s
{
	float height;
	int16_t texid;
	int8_t texmod;
} map_tile_t;

typedef struct map_entry_s
{
	union {
		uint8_t data[10];
		struct {
			uint8_t unknown1[2];
			fixed16_t height;
			int16_t texid;
			uint8_t unknown2[3];
			int8_t texmod;
		};
	};
} map_entry_t;

bool map_entry_print(map_entry_t* entry);

inline float map_entry_get_height(map_entry_t* map_entry)
{
	return (float)map_entry->data[2] / 255.0f + (float)map_entry->data[3];
}

inline void map_entry_set_height(map_entry_t* map_entry, float height)
{
	map_entry->height = float2fixed(height);
}

inline int8_t map_entry_get_texmod(map_entry_t* map_entry)
{
	return map_entry->texmod >> 4;
}

inline void map_entry_set_texmod(map_entry_t* map_entry, int8_t texmod )
{
	map_entry->texmod = texmod << 4;
}

inline int16_t map_entry_get_texid(map_entry_t* map_entry, column_t* columns)
{
	return map_entry->texid < 0 ? columns[-map_entry->texid].floor_texid 
					 : map_entry->texid;
}

typedef struct map_name_s
{
	uint32_t checksum;
	char* name; 
} map_name_t;

/* map point of interest */
typedef struct map_poi_s
{
	vec3 pos;
	int val;
} map_poi_t;

inline map_tile_t* map_entry_to_tile(map_entry_t* map_entry, column_t* columns)
{
	map_tile_t* map_tile = calloc(1, sizeof(map_tile_t));
	map_tile->height = map_entry_get_height(map_entry);
	map_tile->texid = map_entry_get_texid(map_entry, columns);
	map_tile->texmod = map_entry_get_texmod(map_entry);
	
}

#endif
