#include <stdlib.h>
#include <stdio.h>

#include "system_types.h"
#include "res/level_file.h"

int main(int argc, char** argv)
{
	level_table_t* level_table = level_table_create("maps.crc");

	for(int i=0; i< level_table->num_maps; i++)
	{
		for(int j=0; j < ENTITIES_COUNT; j++)
			entity_print(&level_table->files[i]->entities[j]);
		for(int j=0; j < BLOCKS_COUNT; j++)
			block_print(&level_table->files[i]->blocks[j]);
		for(int j=0; j < COLUMNS_COUNT; j++)
			column_print(&level_table->files[i]->columns[j]);
		for(int j=0; j < MAP_WIDTH; j++)
			for(int k=0; k < MAP_HEIGHT; k++)
				map_entry_print(&level_table->files[i]->map_entries[j*k]);
	}

	level_table_destroy(level_table);
	exit(EXIT_SUCCESS);
}
