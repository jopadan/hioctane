#ifndef _ENTITY_H
#define _ENTITY_H

#define ENTITIES_COUNT 4000

typedef struct entity_s
{
	union {
		uint8_t data[24];
		struct{
			uint8_t type;
			uint8_t subtype;
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
	ENTITY_UNKNOWN = 0, ENTITY_UNKNOWN_SHIELD_ITEM, ENTITY_UNKNOWN_ITEM,
	ENTITY_EXTRA_SHIELD, ENTITY_SHIELD_FULL, ENTITY_DOUBLE_SHIELD,
	ENTITY_EXTRA_AMMO, ENTITY_AMMO_FULL, ENTITY_DOUBLE_AMMO,
	ENTITY_EXTRA_FUEL, ENTITY_FUEL_FULL, ENTITY_DOUBLE_FUEL,
	ENTITY_MINIGUN_UPGRADE, ENTITY_MISSILE_UPGRADE, ENTITY_BOOSTER_UPGRADE,
	ENTITY_WALL_SEGMENT,
	ENTITY_WAYPOINT_FUEL, ENTITY_WAYPOINT_AMMO, ENTITY_WAYPOINT_SHIELD, ENTITY_WAYPOINT_SPECIAL1, ENTITY_WAYPOINT_SPECIAL2, ENTITY_WAYPOINT_SPECIAL3, ENTITY_WAYPOINT_FAST, ENTITY_WAYPOINT_SLOW, ENTITY_WAYPOINT_SHORTCUT,
	ENTITY_RECOVERY_TRUCK,
	ENTITY_STEAM_STRONG, ENTITY_STEAM_LIGHT, ENTITY_CONE, ENTITY_CHECKPOINT,
	ENTITY_MORPH_SOURCE1, ENTITY_MORPH_SOURCE2, ENTITY_MORPH_ONCE, ENTITY_MORPH_PERMANENT,
	ENTITY_TRIGGER_CRAFT, ENTITY_TRIGGER_TIMED, ENTITY_TRIGGER_ROCKET,
	ENTITY_DAMAGE_CRAFT,
	ENTITY_EXPLOSION, ENTITY_EXPLOSION_PARTICLES,
} entity_type_t;

entity_type_t entity_identify(entity_t* entity);

#endif
