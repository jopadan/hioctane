#include <stdlib.h>
#include <stdio.h>

#include "block.h"
#include "system_types.h"
#include "entity.h"
#include "column.h"
#include "map_entry.h"

typedef struct level_file_s {
	entity_t     entities[ENTITIES_COUNT];
	column_t     column[COLUMNS_COUNT];
	block_t      block_tex[BLOCKS_COUNT];
	/* unknown section */
	uint8_t      unknown247264[100 * 85];
	uint8_t      unknown292108[4132 * 16];
	uint8_t      unknown358222[256 * 16];
	uint8_t      gradient[4151];
	map_entry_t  unknown402230[199];
	/* map entries */
	map_entry_t  map_entries[MAP_WIDTH * MAP_HEIGHT];
} level_file_t;

int main(int argc, char** argv)
{
	printf("%lu\n", sizeof(level_file_t));
//	level_file_t* level_file = level_file_create("LEVEL0-1.DAT");
//	level_file_destroy(level_file);
	exit(EXIT_SUCCESS);
}
