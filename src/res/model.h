#ifndef _MODEL_H
#define _MODEL_H

#include "system_types.h"

typedef GLsizei index_t;
typedef GLfloat scalar_t;
typedef vec4    vector_t;

typedef struct model_s
{
	uint32_t checksum;
	char* filename;
	char* name;
	size_t size;
	uint8_t* data;
	obj* obj;
} model_t;

typedef struct model_table_s
{
	size_t num_models;
	model_t** models;
} model_table_t;

/* create model_file and return it */
model_t* model_create(FILE* file);
model_t* model_destroy(model_t* model);
void model_write(model_t* model, const char* file);

/* create model_tabel and return it */
model_table_t* model_table_create(char* model_info_file);
model_table_t* model_table_destroy(model_table_t* model_table);

#endif
