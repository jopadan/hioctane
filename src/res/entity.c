#include "entity.h"
#include <ctype.h>
#include <sys/stat.h>

entity_type_table_t* entity_type_table;
/*
[ENTITY_TYPE_COUNT][ENTITY_SUBTYPE_COUNT] =
{
	{ ENTITY_UNKNOWN,                        "Unknown" },

	[ 1][ 5] = { ENTITY_CHECKPOINT,          "Checkpoint" },

	[ 2][ 1] = { ENTITY_EXPLOSION_PARTICLES, "Explosion Particles" },
	[ 2][ 2] = { ENTITY_DAMAGE_CRAFT,        "Damage Craft" },
	[ 2][ 3] = { ENTITY_EXPLOSION,           "Explosion" },	
	[ 2][ 5] = { ENTITY_STEAM_STRONG,        "Steam Strong" },
	[ 2][ 7] = { ENTITY_MORPH_SOURCE2,       "Morph Source2" },
	[ 2][ 8] = { ENTITY_STEAM_LIGHT,         "Steam Light" },
	[ 2][ 9] = { ENTITY_MORPH_SOURCE1,       "Morph Source1" },
	[ 2][16] = { ENTITY_MORPH_ONCE,          "Morph Once" },
	[ 2][23] = { ENTITY_MORPH_PERMANENT,     "Morph Permanent" },

	[ 3][ 3] = { ENTITY_BARREL,              "Entity Barrel" },
	[ 3][ 6] = { ENTITY_CONE,                "Entity Cone" },

	[ 5][ 0] = { ENTITY_UNKNOWN_SHIELD_ITEM, "Unknown Shield" },
	[ 5][ 1] = { ENTITY_UNKNOWN_ITEM,        "Unknown Item" },

	[ 5][ 2] = { ENTITY_EXTRA_SHIELD,        "Extra Shield" },
	[ 5][ 3] = { ENTITY_SHIELD_FULL,         "Shield Full" },
	[ 5][ 4] = { ENTITY_DOUBLE_SHIELD,       "Double Shield" },
	[ 5][ 5] = { ENTITY_EXTRA_AMMO,          "Extra Ammo" },
	[ 5][ 6] = { ENTITY_AMMO_FULL,           "Ammo Full" },
	[ 5][ 7] = { ENTITY_DOUBLE_AMMO,         "Double Ammo" },
	[ 5][ 8] = { ENTITY_EXTRA_FUEL,          "Extra Fuel" },
	[ 5][ 9] = { ENTITY_FUEL_FULL,           "Fuel Full" },
	[ 5][10] = { ENTITY_DOUBLE_FUEL,         "Double Fuel" },
	[ 5][11] = { ENTITY_MINIGUN_UPGRADE,     "Minigun Upgrade" },
	[ 5][12] = { ENTITY_MISSILE_UPGRADE,     "Missile Upgrade" },
	[ 5][13] = { ENTITY_BOOSTER_UPGRADE,     "Booster Upgrade" },

	[ 8][ 0] = { ENTITY_TRIGGER_CRAFT,       "Trigger Craft" },
	[ 8][ 1] = { ENTITY_TRIGGER_TIMED,       "Trigger Craft" },
	[ 8][ 3] = { ENTITY_TRIGGER_ROCKET,      "Trigger Rocket" },

	[ 9][ 0] = { ENTITY_WALL_SEGMENT,        "Wall Segment" },

	[ 9][ 1] = { ENTITY_WAYPOINT_SLOW,       "Waypoint Slow" },
	[ 9][ 2] = { ENTITY_WAYPOINT_FUEL,       "Waypoint Fuel" },
	[ 9][ 3] = { ENTITY_WAYPOINT_AMMO,       "Waypoint Ammo" },
	[ 9][ 4] = { ENTITY_WAYPOINT_SHIELD,     "Waypoint Shield" },
	[ 9][ 6] = { ENTITY_WAYPOINT_SHORTCUT,   "Waypoint Shortcut" },
	[ 9][ 7] = { ENTITY_WAYPOINT_SPECIAL1,   "Waypoint Special1" },
	[ 9][ 8] = { ENTITY_WAYPOINT_SPECIAL2,   "Waypoint Special2" },
	[ 9][ 9] = { ENTITY_WAYPOINT_FAST,       "Waypoint Fast" },
	[ 9][10] = { ENTITY_WAYPOINT_SPECIAL3,   "Waypoint Special3" },

	[10][ 9] = { ENTITY_RECOVERY_TRUCK,      "Recovery Truck" },
};
*/

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
				printf("ENTITY: LOADED %.02x/%.02x %.03d - %s\n", i, j, entity_type_table->entries[i][j].type, entity_type_table->entries[i][j].name);
			else if(i == 0 && j == 0)
			{
				printf("ENTITY: LOADED %.02x/%.02x %.03d - %s neutral element\n",
						0, 0,
						entity_type_table->entries[i][j].type,
						entity_type_table->entries[i][j].name);
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

	/* get entries in entity_info_file */
	while(getline(&line, &length, file) != -1)
	{
		if(getline(&name_line, &name_length, file) == -1)
		{
			fprintf(stderr, "ENTITY: LOAD FAILED at getline!\n");
			entity_type_table_destroy(entities);
			entities = NULL;
			break;
		}

		if(!entity_type_table_insert(entities, line, name_line))
		{
			fprintf(stderr,"ENTITY: LOAD FAILED at entity_type_table_insert failed!\n");
			entity_type_table_destroy(entities);
			entities = NULL;
			break;
		}
	}
	free(name_line);
	free(line);
	rewind(file);

	fclose(file);
	chdir(cwd);

	return entities;
}
