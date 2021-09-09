# hioctane
Hi-Octane port in C

# Dependencies
- [libzip] (http://github.com/nih-at/libzip/)
- [libcrc] (http://github.com/jopadan/libcrc/)
- [obj] (http://github.com/rlk/obj/)

# Building

```c
cmake .
make
make install
```

# Status

new map list format using simple checksum 
with additional comment line as name

create maps.cfg:

```c
cd data && rhash -C -simple maps/*
```

add map name as a comment beginning with:

```c
; <name>
```

after the line following the checksum and file


required header information to create entities.cfg:

```c
<hexadecimal type size> <hexadecimal subtype size>
; types count / subtypes count
<hexadecimal type> <hexadecimal subtype> <decimal enum int id>
; Unknown Zero
```

for every entity

```c
<hexadecimal type> <hexadecimal subtype> <decimal enum int id>
; <entity name string>
```

must be added past the header of entities.cfg


layout of included standard entities


```c
typedef struct entity_entry_s
{
	entity_type_t type;
	const char*   name;
} entity_entry_t;

entity_entry_t entities->entries[ENTITY_TYPE_COUNT][ENTITY_SUBTYPE_COUNT] =
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
	[ 8][ 1] = { ENTITY_TRIGGER_TIMED,       "Trigger Timed" },
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
```
