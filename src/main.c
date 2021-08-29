#include <stdlib.h>
#include <stdio.h>

#include "system_types.h"
#include "res/level_file.h"

int main(int argc, char** argv)
{
	int exit_status = EXIT_SUCCESS;

	/* load entity config at DATA_DIR/entities.cfg */
	entity_type_table = entity_type_table_create("entities.cfg");
	if(entity_type_table == NULL)
	{
		exit_status = EXIT_FAILURE;
		exit(exit_status);
	}
	/* print loaded entities to stdout */
	entity_type_table_print();

	/* load level table config at DATA_DIR/maps.cfg */
	level_table_t* level_table = level_table_create("maps.cfg");
	if(level_table == NULL)
	{
		exit_status = EXIT_FAILURE;
		exit(exit_status);
	}

	for(int i=0; i< level_table->num_maps; i++)
	{
	/*
		for(int j=0; j < ENTITIES_COUNT; j++)
			entity_print(&level_table->files[i]->entities[j]);
		for(int j=0; j < BLOCKS_COUNT; j++)
			block_print(&level_table->files[i]->blocks[j]);
		for(int j=0; j < COLUMNS_COUNT; j++)
			column_print(&level_table->files[i]->columns[j]);
		for(int j=0; j < MAP_WIDTH; j++)
			for(int k=0; k < MAP_HEIGHT; k++)
				map_entry_print(&level_table->files[i]->map_entries[j*k]);
	*/
	}
	
	if(!level_table_destroy(level_table))
	{
		perror("error destroying level_table!");
		exit_status = EXIT_FAILURE;
	}
	if(!entity_type_table_destroy(entity_type_table))
	{
		perror("error destroying entity_type_table!");
		exit_status = EXIT_FAILURE;
	}
	exit(exit_status);
}
