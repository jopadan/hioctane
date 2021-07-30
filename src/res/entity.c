#include "entity.h"

entity_type_t entity_identify(entity_t* entity)
{
	if(entity->type == 1 && entity->subtype == 5) return ENTITY_CHECKPOINT;

	if(entity->type == 2 && entity->subtype == 1) return ENTITY_EXPLOSION_PARTICLES;
	if(entity->type == 2 && entity->subtype == 2) return ENTITY_DAMAGE_CRAFT;
	if(entity->type == 2 && entity->subtype == 3) return ENTITY_EXPLOSION;
	if(entity->type == 2 && entity->subtype == 5) return ENTITY_STEAM_STRONG;
	if(entity->type == 2 && entity->subtype == 7) return ENTITY_MORPH_SOURCE2;
	if(entity->type == 2 && entity->subtype == 8) return ENTITY_STEAM_LIGHT;
	if(entity->type == 2 && entity->subtype == 9) return ENTITY_MORPH_SOURCE1;
	if(entity->type == 2 && entity->subtype == 16) return ENTITY_MORPH_ONCE;
	if(entity->type == 2 && entity->subtype == 23) return ENTITY_MORPH_PERMANENT;

	if(entity->type == 3 && entity->subtype == 6) return ENTITY_CONE;

	if(entity->type == 5 && entity->subtype == 0) return ENTITY_UNKNOWN_SHIELD_ITEM;
	if(entity->type == 5 && entity->subtype == 1) return ENTITY_UNKNOWN_ITEM;

	if(entity->type == 5 && entity->subtype == 2) return ENTITY_EXTRA_SHIELD;
	if(entity->type == 5 && entity->subtype == 3) return ENTITY_SHIELD_FULL;
	if(entity->type == 5 && entity->subtype == 4) return ENTITY_DOUBLE_SHIELD;
	if(entity->type == 5 && entity->subtype == 5) return ENTITY_EXTRA_AMMO;
	if(entity->type == 5 && entity->subtype == 6) return ENTITY_AMMO_FULL;
	if(entity->type == 5 && entity->subtype == 7) return ENTITY_DOUBLE_AMMO;
	if(entity->type == 5 && entity->subtype == 8) return ENTITY_EXTRA_FUEL;
	if(entity->type == 5 && entity->subtype == 9) return ENTITY_FUEL_FULL;
	if(entity->type == 5 && entity->subtype == 10) return ENTITY_DOUBLE_FUEL;
	if(entity->type == 5 && entity->subtype == 11) return ENTITY_MINIGUN_UPGRADE;
	if(entity->type == 5 && entity->subtype == 12) return ENTITY_MISSILE_UPGRADE;
	if(entity->type == 5 && entity->subtype == 13) return ENTITY_BOOSTER_UPGRADE;

	if(entity->type == 8 && entity->subtype == 0) return ENTITY_TRIGGER_CRAFT;
	if(entity->type == 8 && entity->subtype == 1) return ENTITY_TRIGGER_TIMED;
	if(entity->type == 8 && entity->subtype == 3) return ENTITY_TRIGGER_ROCKET;

	if(entity->type == 9 && entity->subtype == 0) return ENTITY_WALL_SEGMENT;

	if(entity->type == 9 && entity->subtype == 1) return ENTITY_WAYPOINT_SLOW;
	if(entity->type == 9 && entity->subtype == 2) return ENTITY_WAYPOINT_FUEL;
	if(entity->type == 9 && entity->subtype == 3) return ENTITY_WAYPOINT_AMMO;
	if(entity->type == 9 && entity->subtype == 4) return ENTITY_WAYPOINT_SHIELD;
	if(entity->type == 9 && entity->subtype == 6) return ENTITY_WAYPOINT_SHORTCUT;
	if(entity->type == 9 && entity->subtype == 7) return ENTITY_WAYPOINT_SPECIAL1;
	if(entity->type == 9 && entity->subtype == 8) return ENTITY_WAYPOINT_SPECIAL2;
	if(entity->type == 9 && entity->subtype == 9) return ENTITY_WAYPOINT_FAST;
	if(entity->type == 9 && entity->subtype == 10) return ENTITY_WAYPOINT_SPECIAL3;

	if(entity->type == 10 && entity->subtype == 9) return ENTITY_RECOVERY_TRUCK;

	return ENTITY_UNKNOWN;
}
