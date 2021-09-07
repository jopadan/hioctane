#ifndef _MODEL_FILE_H
#define _MODEL_FILE_H

#include "system_types.h"
#include "log.h"
#include <checksum.h>
#include <sys/stat.h>

typedef enum image_type_e
{
	IMAGE_PNG,
	IMAGE_TGA,
	IMAGE_BMP,
} image_type_t;

typedef enum pixel_format_e
{
	PIXEL_FORMAT_RGBA8888,
	PIXEL_FORMAT_RGB565,
	PIXEL_FORMAT_RGBA4444,
} pixel_format_t;

typedef enum compression_type_e
{
	COMPRESSION_ZIP,
	COMPRESSION_GZIP,
	COMPRESSION_LZMA,
	COMPRESSION_XZ,
	COMPRESSION_BZIP2,
} compression_type_t;

typedef struct image_format_s
{
	image_type_t type;
	pixel_format_t pixel;
	compression_type_t comp;
} image_format_t;

typedef struct image_s
{
	image_format_t format;
	GLsizei width;
	GLsizei height;
	GLsizei bpp;
	size_t size;
	size_t compressed_size;
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

GLsizei face_verts[10] = { 1, 2, 2, 2, 3, 3, 3, 4, 4, 0 };

typedef GLsizei index_t;
typedef GLfloat scalar_t;
typedef vec4    vector_t;

typedef struct mesh_s
{
	GLsizei num_vertices;
	vec4* vertices;
	GLsizei num_normals;
	vec3* normals;
	GLsizei num_texcoords;
	vec4* texcoords;
	GLsizei num_faces;
	int face_type;
	scalar_t scalar_type;
	index_t index_type;
	index_t* indices;
	vec3* face_normals;
} mesh_t;

typedef struct model_file_s
{
	uint32_t checksum;
	char* filename;
	char* name;
	GLsizei num_textures;
	texture_t** textures;
	GLsizei num_meshes;
	mesh_t** meshes;
} model_file_t;

typedef struct model_table_s
{
	size_t num_textures;
	size_t num_models;
	texture_t** textures;
	model_file_t** models;
} model_table_t;


image_t* image_destroy(image_t* image);
image_t* image_create(uint8_t* file_data, size_t size);
/* destroy texture and return NULL */
texture_t* texture_destroy(texture_t* texture);
mesh_t* mesh_create();
mesh_t* mesh_destroy(mesh_t* mesh);
model_file_t* model_file_create(uint32_t checksum, char* filename, char* name);
model_file_t* model_file_destroy(model_file_t* model_file);
/* destroy model_table and return NULL */
model_table_t* model_table_destroy(model_table_t* model_table);
/* create texture and return it */
texture_t* texture_create(const uint32_t checksum, const char* filename, const char* name);
/* create model_tabel and return it */
model_table_t* model_table_create(char* model_info_file);

#endif
