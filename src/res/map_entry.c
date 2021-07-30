#include "map_entry.h"

bool map_entry_destroy(map_entry_t* map_entry)
{
	if(!map_entry)
		return false;
	free(map_entry);
	return true;
}

map_name_t* map_name_create(const char* name, uint32_t checksum)
{
	map_name_t* map_name = calloc(1, sizeof(map_name_t));

	if(!map_name)
	{
		perror("Error allocating memory for map_name");
		return NULL;
	}
	map_name->name = strdup(name);
	map_name->checksum = checksum;

	return map_name;
}

bool map_name_destroy(map_name_t* map_name)
{
	if(!map_name)
	{
		fprintf(stderr, "ERROR: map_name_destroy(map_name) map_name == NULL!\n");
		return false;
	}
	free(map_name);
	return true;
}
