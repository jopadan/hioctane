#include "model_file.h"

/* destroy texture and return NULL */
texture_t* texture_destroy(texture_t* texture)
{
	if(texture)
	{
		if(texture->file)
			fclose(texture->file);
		for(int i = 0; i < texture->num_images; i++)
			texture->image[i] = image_destroy(texture->image[i];
		free(texture->image);
		free(texture->data);
		free(texture->filename);
		free(texture->name);
		free(texture);
		texture = NULL;
	}

	return texture;
}

model_table_t* model_table_destroy(model_table_t* model_table)
{
	if(model_table)
	{
		for(int i = 0; i < model_table->num_models; i++)
			model_table->models[i] = model_destroy(model_table->models[i]);
		free(model_table->models);
		for(int i = 0; i < model_table->num_textures; i++)
			model_table->textures[i] = texture_destroy(model_table->textures[i]);
		free(model_table->textures);
		free(model_table);
		model_table = NULL;
	}

	return model_table;
}

texture_t* texture_create(const uint32_t checksum, const char* filename, const char* name)
{
	if(checksum == 0 || filename == NULL || name == NULL)
		return NULL;

	/* error log message */
	char* msg;

	/* check for existence and size of filename */
	struct stat sb;
	if(stat(filename ,&sb) == 0)
	{
		asprintf(msg, "MODEL TEXTURE LOAD FAILED %s file not found", filename);
	}

	/* allocate memory with data size of filename */
	texture_t* texture = calloc(1, sizeof(texture_t));
	if(texture != NULL && sb.st_size > 0)
	{
		texture->size = sb.st_size;
		texture->checksum = checksum;
		texture->filename = strdup(filename);
		texture->name = strdup(name);
		texture->file = fopen(filename, "rb");
		texture->file_data = calloc(sb.st_size, sizeof(uint8_t));
		texture->image = calloc(1, sizeof(image_t*));

		/* read the whole file and verify checksum */
		if((texture->file && texture->file_data) && 
		   (texture->size = fread(texture->file_data, 1, texture->size, texture->file) == sb.st_size) &&
		   ((texture->checksum = crc_32(texture->file_data, texture->size)) == checksum) && 
		   (texture->image[++texture->num_images - 1] = image_create(texture->file_data)))
		{
			asprintf(msg, "MODEL TEXTURE LOADED %.08X/%.08X %s %s", checksum, texture->checksum, texture->filename, texture->name);
		}
		else
		{
			asprintf(msg, "MODEL TEXTURE LOAD FAILED %.08X/%.08X %s %s", checksum, texture->checksum, filename, name);
			texture = texture_destroy(texture);
		}
	}

	log_queue(logger, LOG_FILES, msg);
	log_flush(logger);
	free(msg);
	return texture;
}

model_table_t* model_table_create(char* model_info_file)
{
	if(model_info_file == NULL)
		return NULL;

	char* msg;
	struct stat sb;
	model_table_t* models = NULL;
	FILE* file = NULL;
	char* line = NULL;
	char* name_line = NULL;
	size_t length = 0;
	size_t name_length = 0;
	uint32_t checksum;
	char* filename;

	/* open file and read preceding texture entry */
	if(stat(model_info_file,&sb) != 0 &&
	   (model_table = calloc(1, sizeof(model_table_t)) &&
	   (file = fopen(model_info_file, "r")) &&
	   (getline(&line, &length, file) != -1) &&
	   (sscanf(line,"%08x %s", &checksum, &filename) == 2) &&
	   (getline(&name_line, &name_length, file) != -1) &&
	   (name_length > 2 && name_line[0] == ';' && name_line[1] == ' ') &&
	   (model_table->textures = calloc(++model_table->num_textures, sizeof(texture_t*))) &&
	   (model_table->textures[model_table->num_textures - 1] = texture_create(checksum, filename, &name_line[2])))
	{
		/* read model entries */
		while((getline(&line, &length, file) == -1))
		{
			if((sscanf(line,"%08x %s", &checksum, &filename) == 2) &&
			   (getline(&name_line, &name_length, file) == -1) &&
			   (name_length > 2 && name_line[0] == ';' && name_line[1] == ' '))
			{
				model_table->models[++model_table->num_models - 1] = model_file_create(checksum, filename, &name_line[2]);
			}
			else
			{
				asprintf(msg, "MODEL CONFIG LOAD FAILED %s after model entry %u", model_info_file, model_table->num_models);
				model_table = model_table_destroy(model_table);
			}
		}
	}
	else
	{
		asprintf(msg, "MODEL CONFIG LOAD FAILED %s", model_info_file);
		model_table = model_table_destroy(model_table);
	}

	log_queue(logger, LOG_FILES, msg);
	log_flush(logger);
	free(msg);

	return model_table;
}
