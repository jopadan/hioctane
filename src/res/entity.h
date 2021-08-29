#ifndef _ENTITY_H
#define _ENTITY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <system_types.h>

#define ENTITIES_COUNT 4000

typedef struct entity_s
{
	union {
		uint8_t data[24];
		struct{
			union {
				struct {
					uint8_t type;
					uint8_t subtype;
				};
				uint8_t type_field;
			};
			int16_t group;
			int16_t target_group;
			uint8_t unknown[6];
			int16_t next_id;
			int16_t value;
			fixed16_t x;
			fixed16_t z;
			fixed16_t offset_x;
			fixed16_t offset_y;
		};
	};
} entity_t;

typedef enum entity_type_e
{
	/* unknown */
	ENTITY_UNKNOWN = 0,
	ENTITY_UNKNOWN_SHIELD_ITEM,
	ENTITY_UNKNOWN_ITEM,

	/* shield */
	ENTITY_EXTRA_SHIELD,
	ENTITY_SHIELD_FULL,
	ENTITY_DOUBLE_SHIELD,

	/* ammo */
	ENTITY_EXTRA_AMMO,
	ENTITY_AMMO_FULL,
	ENTITY_DOUBLE_AMMO,

	/* fuel */
	ENTITY_EXTRA_FUEL,
	ENTITY_FUEL_FULL,
	ENTITY_DOUBLE_FUEL,

	/* item upgrades */
	ENTITY_MINIGUN_UPGRADE,
	ENTITY_MISSILE_UPGRADE,
	ENTITY_BOOSTER_UPGRADE,

	/* walls */
	ENTITY_WALL_SEGMENT,

	/* waypoints */
	ENTITY_WAYPOINT_FUEL,
	ENTITY_WAYPOINT_AMMO,
	ENTITY_WAYPOINT_SHIELD,
	ENTITY_WAYPOINT_SPECIAL1,
	ENTITY_WAYPOINT_SPECIAL2,
	ENTITY_WAYPOINT_SPECIAL3,
	ENTITY_WAYPOINT_FAST,
	ENTITY_WAYPOINT_SLOW,
	ENTITY_WAYPOINT_SHORTCUT,

	/* recovery */
	ENTITY_RECOVERY_TRUCK,

	/* checkpoint steam */
	ENTITY_STEAM_STRONG,
	ENTITY_STEAM_LIGHT,
	ENTITY_BARREL,
	ENTITY_CONE,
	ENTITY_CHECKPOINT,

	/* morph */
	ENTITY_MORPH_SOURCE1,
	ENTITY_MORPH_SOURCE2,
	ENTITY_MORPH_ONCE,
	ENTITY_MORPH_PERMANENT,

	/* triggers */
	ENTITY_TRIGGER_CRAFT,
	ENTITY_TRIGGER_TIMED,
	ENTITY_TRIGGER_ROCKET,

	/* damage */
	ENTITY_DAMAGE_CRAFT,

	/* explosions */
	ENTITY_EXPLOSION,
	ENTITY_EXPLOSION_PARTICLES,

	/* sizes */
	ENTITY_TYPE_SIZE,
	ENTITY_TYPE_MAX = UINT8_MAX,
} entity_type_t;

typedef struct entity_entry_s
{
	entity_type_t type;
	const char*   name;
} entity_entry_t;

typedef struct entity_type_table_s
{
	size_t TYPE_SIZE;
	size_t SUBTYPE_SIZE;
	size_t num_entries;
	char** names;
	entity_entry_t** entries;
} entity_type_table_t;

extern entity_type_table_t* entity_type_table;

bool entity_type_table_destroy(entity_type_table_t* entities);
entity_type_table_t* entity_type_table_create(char* entity_info_file);
char* entity_type_table_print();
entity_type_t entity_identify(entity_t* entity);
bool entity_print(entity_t* entity);

#endif
