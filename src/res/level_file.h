#ifndef _LEVEL_FILE_H
#define _LEVEL_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <checksum.h>

#include "map_entry.h"
#include "entity.h"
#include "column.h"
#include "block.h"

/*
247264   85 bytes, ? entries, Unknown Table 1

292108   something very short

357668 - 358221  unknown 

358222 - 362209, 16 bytes? , 256 entries?

362210 - 366361 Unknown gradient/counter table

402230 - 404619, 12 bytes? , 199 entries?

*/

#define ENTITIES_SIZE 0 + 24 * ENTITIES_COUNT
#define COLUMNS_SIZE 98012 + 26 * COLUMNS_COUNT
#define BLOCKS_SIZE 124636 + 16 * BLOCKS_COUNT
#define MAP_FILE_SIZE 404620 + 12 * MAP_WIDTH * MAP_HEIGHT
#define MAPS_MAX 64


/* level file */
typedef struct map_header_s {
	uint32_t checksum;
	char* filename;
	char* name;
} map_header_t;

typedef union map_file_u {
		uint8_t data[MAP_FILE_SIZE];
		struct {
			entity_t    entities[ENTITIES_COUNT];
			column_t    columns[COLUMNS_COUNT];
			block_t     blocks[BLOCKS_COUNT];

			/* unknown section */
			uint8_t      unknown247264[100 * 85];
			uint8_t      unknown292108[4132 * 16];
			uint8_t      unknown358222[256 * 16];
			uint8_t      gradient[4151];
			map_entry_t  unknown402230[199];
			/* map entries */
			map_entry_t  map_entries[MAP_WIDTH * MAP_HEIGHT];
		};
} map_file_t;


/* level table */  
typedef struct level_table_s {
	size_t num_maps;
	map_header_t** headers;
	map_file_t** files;
} level_table_t;


map_header_t* map_header_create(char* line, char* name_line);
bool map_header_destroy(map_header_t* header);
map_file_t* map_file_create(map_header_t* header);
bool map_file_destroy(map_file_t* file);
level_table_t* level_table_create(char* level_info_file);
bool level_table_destroy(level_table_t* table);

#endif
