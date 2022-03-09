#include "entity.h"
#include <ctype.h>
#include <sys/stat.h>

entity_type_table_t* entity_type_table;


entity_type_t entity_identify(entity_t* entity)
{
	return entity_type_table->entries[entity->type][entity->subtype].type;
}

const char* entity_name(entity_type_t id)
{
	for(int i = 0; i < entity_type_table->TYPE_SIZE; i++)
	{
		for(int j = 0; j < entity_type_table->SUBTYPE_SIZE; j++)
		{
			if(entity_type_table->entries[i][j].type == id)
				return entity_type_table->entries[i][j].name;
		}
	}
	return NULL;
}

char* entity_type_table_print()
{
	/* print neutral element */
	printf("\n");

	/* print all the entity entries */
	for(int i = 0; i < entity_type_table->TYPE_SIZE; i++)
	{
		for(int j = 0; j < entity_type_table->SUBTYPE_SIZE; j++)
		{
			if(entity_type_table->entries[i][j].type != entity_type_table->entries[0][0].type)
			{
				char* msg;

				if(asprintf(&msg,"ENTITY LOADED %.02x/%.02x %.03d - %s", 
				i, j, 
				entity_type_table->entries[i][j].type,
				entity_type_table->entries[i][j].name) == -1)
				{
					log_queue(logger, LOG_FILES, "ENTITY LOAD FAILED calling asprintf!");
					log_flush(logger);
					free(msg);
					return NULL;
				}

				log_queue(logger, LOG_FILES, msg);
				log_flush(logger);
				free(msg);
			}

			else if(i == 0 && j == 0)
			{
				char *msg;
				if(asprintf(&msg,"ENTITY LOADED %.02x/%.02x %.03d - %s neutral element",
						0, 0,
						entity_type_table->entries[i][j].type,
						entity_type_table->entries[i][j].name) == -1)
				{
					log_queue(logger, LOG_FILES, "ENTITY LOAD FAILED calling asprintf!");
					log_flush(logger);
					free(msg);
					return NULL;
				}
				log_queue(logger, LOG_FILES, msg);
				log_flush(logger);
				free(msg);
			}
		}
	}
	return NULL;
}

bool entity_print(entity_t* entity)
{
	printf("%.04x/%.04x-%.02x/%.02x/%.02x/%.02x/%.02x/%.02x-%.04x-%.04x-%f/%f/%f/%f-%.02x/%.02x - %s\n", 
	entity->group, entity->target_group, entity->unknown[0], entity->unknown[1], entity->unknown[2], entity->unknown[3], entity->unknown[4], entity->unknown[5], entity->next_id, entity->value, fixed2float(entity->x), fixed2float(entity->z), fixed2float(entity->offset_x), fixed2float(entity->offset_y),	
	entity->type, entity->subtype, 
	entity_type_table->entries[entity->type][entity->subtype].name
	);


	return true;
}

bool entity_type_table_insert(entity_type_table_t* entities, char* line, char* name_line)
{
	unsigned int type;
	unsigned int subtype;
	int id;

	if(entities == NULL)
	{
		perror("entity_entry_create()");
		return false;
	}

	
	if(sscanf(line, "%02x %02x %03d\n", &type, &subtype, &id) != 3)
	{
		fprintf(stderr, "ENTITY: LOAD FAILED at sscanf %.02x/%.02x %.03d\n", type, subtype, id);
		return false;
	}

	if(name_line[0] != ';' && name_line[1] != ' ')
	{
		fprintf(stderr, "ENTITY: LOAD FAILED with invalid name line format please use `; <name>`\n");
		return false;
	}

	line[strlen(line) -1 ] = '\0';
	name_line[strlen(name_line) -1 ] = '\0';
	if(type >= entities->TYPE_SIZE || subtype >= entities->SUBTYPE_SIZE || id > UINT8_MAX)
	{
		fprintf(stderr, "ENTITY: LOAD FAILED with type or subtype out of range!\n");
		return false;
	}
	entities->entries[type][subtype].type = id;
	if((entities->entries[type][subtype].name = strdup(&name_line[2])) == NULL)
	{
		fprintf(stderr, "entity_entry->name %s\n", &name_line[2]);
		return false;
	}

	char *msg;
	/* if empty fill with neutral element */
	if(entities->num_entries == 0)
	{
		for(int i = 0; i < entities->TYPE_SIZE; i++)
		{
			for(int j = 0; j < entities->SUBTYPE_SIZE; j++)
			{
				entities->entries[i][j] = entities->entries[type][subtype];
			}
		}
		if(asprintf(&msg,"ENTITY LOADED %.02x/%.02x %.03d - %s neutral element",
					0, 0,
					entities->entries[0][0].type,
					entities->entries[0][0].name) == -1)
		{
			log_queue(logger, LOG_FILES, "ENTITY LOAD FAILED calling asprintf!");
			log_flush(logger);
			free(msg);
			return NULL;
		}

		log_queue(logger, LOG_FILES, msg);
		log_flush(logger);
		free(msg);
	}
	else
	{
		if(entities->entries[type][subtype].type != entities->entries[0][0].type)
		{
			if(asprintf(&msg,"ENTITY LOADED %.02x/%.02x %.03d - %s", 
						type, subtype, 
						entities->entries[type][subtype].type,
						entities->entries[type][subtype].name) == -1)
			{
				log_queue(logger, LOG_FILES, "ENTITY LOAD FAILED calling asprintf!");
				log_flush(logger);
				free(msg);
				return NULL;
			}

			log_queue(logger, LOG_FILES, msg);
			log_flush(logger);
			free(msg);
		}
		else if(type == 0 && subtype == 0)
		{
			if(asprintf(&msg,"ENTITY LOADED %.02x/%.02x %.03d - %s neutral element",
						0, 0,
						entities->entries[type][subtype].type,
						entities->entries[type][subtype].name) == -1)
			{
				log_queue(logger, LOG_FILES, "ENTITY LOAD FAILED calling asprintf!");
				log_flush(logger);
				free(msg);
				return NULL;
			}

			log_queue(logger, LOG_FILES, msg);
			log_flush(logger);
			free(msg);
		}
	}
	entities->num_entries++;
	return true;
}

bool entity_type_table_destroy(entity_type_table_t* entities)
{
	if(entities == NULL)
		return false;

	if(entities->entries != NULL)
	{
		for(int i = 0; i < entities->TYPE_SIZE; i++)
		{
			free(entities->entries[i]);
		}
		free(entities->entries);
	}
	free(entities);
	return true;
}

entity_type_table_t* entity_type_table_create(char* entity_info_file)
{
	char* cwd = get_current_dir_name();
	chdir(DATA_DIR);

	/* check if level info file exists */
	struct stat sb;
	if(stat(entity_info_file, &sb) != 0)
	{
		perror(entity_info_file);
		return NULL;
	}

	if(sb.st_size == 0)
	{
		fprintf(stderr, "ERROR: %s too small\n", entity_info_file);
		return NULL;
	}

	/* open level_info_file */
	FILE* file = fopen(entity_info_file, "r");

	if(!file)
	{
		fprintf(stderr, "ERROR: reading level info file %s\n", entity_info_file);
		return NULL;
	}

	char* line = NULL;
	char* name_line = NULL;
	size_t length = 0;
	size_t name_length = 0;
	unsigned int type_size;
	unsigned int subtype_size;

	/* get size of type/subtype in entity_info_file */
	if(getline(&line, &length, file) == -1)
	{
		fprintf(stderr, "ERROR: reading entity info file %s\n", entity_info_file);
		return NULL;
	}
	entity_type_table_t* entities = calloc(1, sizeof(entity_type_table_t));
	if(sscanf(line,"%02x %02x", &type_size, &subtype_size) != 2)
	{
		free(entities);
		fprintf(stderr, "ERROR: reading entity info file %s\n", entity_info_file);
		return NULL;
	}
	if(getline(&line, &length, file) == -1)
	{
		fprintf(stderr, "ERROR: reading entity info file size comment %s\n", entity_info_file);
		return NULL;
	}
	entities->TYPE_SIZE = type_size;
	entities->SUBTYPE_SIZE = subtype_size;
	entities->entries = calloc(entities->TYPE_SIZE, sizeof(entity_entry_t*));

	for(int i = 0; i < entities->TYPE_SIZE; i++)
	{
		entities->entries[i] = calloc(entities->SUBTYPE_SIZE, sizeof(entity_entry_t));
	}

	size_t count = 0;
	/* get entries in entity_info_file */
	while(getline(&line, &length, file) != -1)
	{
		if(getline(&name_line, &name_length, file) == -1)
		{
			log_queue(logger, LOG_FILES, "ENTITY: LOAD FAILED at getline!");
			log_flush(logger);
			entity_type_table_destroy(entities);
			entities = NULL;
			break;
		}

		if(!entity_type_table_insert(entities, line, name_line))
		{
			log_queue(logger, LOG_FILES, "ENTITY: LOAD FAILED at entity_type_table_insert failed!");
			log_flush(logger);
			entity_type_table_destroy(entities);
			entities = NULL;
			break;
		}
		count++;
	}

	free(name_line);
	free(line);
	rewind(file);

	fclose(file);
	chdir(cwd);

	char* msg;
	asprintf(&msg, "ENTITY CONFIG LOADED %s with %zu entries", entity_info_file, count);
	log_queue(logger, LOG_FILES, msg);
	log_flush(logger);
	free(msg);

	return entities;
}
