#ifndef _COLUMN_H
#define _COLUMN_H

#include "system_types.h"

#define COLUMNS_COUNT 1024
#define COLUMNS_START 98012

typedef struct column_s
{
	union {
		uint8_t data[26];
		struct {
			uint8_t shape;
			uint8_t unused;
			int16_t floor_texid;
			int16_t unknown1;
			union {
				int16_t blocks[8];
				struct {
					int16_t a;
					int16_t b;
					int16_t c;
					int16_t d;
					int16_t e;
					int16_t f;
					int16_t g;
					int16_t h;
				};
				int16_t unknown2;
				int16_t unknown3;
			};
		};
	};
} column_t;

typedef struct vbo_s
{
	int32_t indices;
	vec3* pos;
	vec3* normal;
	vec2* uv;
} vbo_t;

typedef struct column_model_s
{
/*
	vao_handle
	positive_vbo_handle;
	normal_vbo_handle;
	uv_vbo_handle;
	ebo_handle;
*/
	vbo_t vbo;
	vec3* vertices;
/*
	atlas_material_t* material;
	directional_light_t light;
*/
	vec4 tint_color;
	float opacity;
	float grayscale;
	bool destroy_on_morph;
	bool hidden;
} column_model_t;
bool column_print(column_t* column);
//column_t* column_create(uint8_t* data);
//bool column_destroy(column_t* column);
#endif
