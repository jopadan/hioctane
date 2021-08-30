#ifndef _MODEL_FILE_H
#define _MODEL_FILE_H

#include "system_types.h"
#include "log.h"
#include <checksum.h>
#include <sys/stat.h>

typedef struct image_s
{
	image_format_t format;
	GLsizei width;
	GLsizei height;
	GLsizei bpp;
	uint8_t* data;
	uint8_t* comp;
} image_t;
typedef struct texture_s
{
	char* filename;
	char* name;
	size_t size;
	uint32_t checksum;
	FILE* file;
	uint8_t* file_data;
	GLsizei id;
	size_t num_images;
	image_t** image;
	image_t* current_image;
	image_t* primary_image;
} texture_t;

typedef struct mode_file_s
{
	uint32_t checksum;
	char* filename;
	char* name;
	GLsizei num_textures;
	texture_t** textures;
	GLsizei num_meshes;
	mesh_t** mesh;
} model_file_t;

typedef struct model_table_s
{
	size_t num_textures;
	size_t num_models;
	texture_file_t** textures;
	model_file_t** models;
} model_table_t;

/* destroy texture and return NULL */
texture_t* texture_destroy(texture_t* texture)
/* destroy model_table and return NULL */
model_table_t* model_table_destroy(model_table_t* model_table)
/* create texture and return it */
texture_t* texture_create(const uint32_t checksum, const char* filename, const char* name)
/* create model_tabel and return it */
model_table_t* model_table_create(char* model_info_file)

#endif
