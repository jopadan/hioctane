#include "model.h"

model_t* model_destroy(model_t* model)
{
	if(model)
	{
		if(model->filename)
		{
			free(model->filename);
			model->filename;
		}
		if(model->name)
		{
			free(model->name);
			model->name = NULL;
		}
		if(model->obj)
		{
			obj_delete(model->obj);
			model->obj = NULL;
		}

		free(model);
		model = NULL;
	}
	return model;
}


bool model_skip(FILE* file)
{
	char* line = NULL;
	char* name_line = NULL;
	size_t length = 0;
	size_t name_length = 0;
	uint32_t checksum = 0;
	char* filename = calloc(PATH_MAX, sizeof(char));
	char* name = calloc(PATH_MAX, sizeof(char));
	struct stat sb;
	bool retval = false;

	/* parse model line entry */
	if(getline(&line, &length, file) != -1 &&
	   getline(&name_line, &name_length, file) != -1 &&
	   sscanf(line, "%08x  %s", &checksum, &filename[0]) == 2 && 
	   sscanf(name_line, "; %s", &name[0]) == 1 && 
			stat(filename, &sb) == 0)
	{
		retval = true;	
	}

	if(line)
		free(line);
	if(name_line)
		free(name_line);
	if(filename)
		free(filename);
	if(name)
		free(name);
	return retval;
}

model_t* model_create(FILE* file)
{
	char* line = NULL;
	char* name_line = NULL;
	size_t line_size = 0;
	size_t name_line_size = 0;
	uint32_t checksum = 0;
	size_t filename_size = PATH_MAX;
	size_t name_size = PATH_MAX;
	char* filename = calloc(PATH_MAX, sizeof(char));
	char* name = calloc(PATH_MAX, sizeof(char));

	model_t* model = calloc(1, sizeof(model_t));

	struct stat sb;
	char* msg = NULL;

	/* parse model line entry */
	if((filename_size = getline(&line, &line_size, file)) != -1 &&
	   (name_size = getline(&name_line, &name_line_size, file)) != -1 &&
	   sscanf(&line[0],"%08x  %s\n", &checksum, filename) &&
	   (name_line[0] == ';') && (name_line[1] == ' '))
	{
		memcpy(name, &name_line[2], name_size - 2);
		model->size = sb.st_size;
		model->checksum = checksum;
		model->filename = strdup(filename);
		model->name = strdup(name);

		/* verify checksum */
		model->data = calloc(model->size, sizeof(uint8_t));
		FILE* entry = fopen(model->filename, "r");
		if(entry != NULL && 
		  (model->size = fread(model->data, sizeof(uint8_t), model->size, entry)) == model->size && 
		  fclose(entry) == 0 && ((model->checksum = crc_32(model->data, model->size)) == checksum))

		{
			asprintf(&msg, "MODEL LOADED %s %08X/%08X %s", model->filename, model->checksum, checksum, model->name);
			/*
			 * model->obj = obj_create(model->filename);
			else
			{
				asprintf(&msg, "MODEL LOAD FAILED %s %08X/%08X %s", model->filename, model->checksum, checksum, name);
				model = model_destroy(model);
			}
			*/
		}
		else
		{
			asprintf(&msg, "MODEL LOAD FAILED %s %08X/%08X %s", model->filename, model->checksum, checksum, name);
			model = model_destroy(model);
		}
	}
	else
	{
		model = model_destroy(model);
		asprintf(&msg, "MODEL CONFIG MODEL LOAD FAILED");
	}

	log_queue(logger, LOG_FILES, msg);
	log_flush(logger);
	free(line);
	free(name_line);
	free(filename);
	free(name);
	free(msg);
	return model;
}


model_table_t* model_table_destroy(model_table_t* mt)
{
	if(mt)
	{
		if(mt->models != NULL && mt->num_models > 0)
			for(int i = 0; i < mt->num_models; i++)
				mt->models[i] = model_destroy(mt->models[i]);

		free(mt);
		mt = NULL;
	}

	return mt;
}

model_table_t* model_table_create(char* model_info_file)
{
	if(model_info_file == NULL)
		return NULL;

	char* msg = NULL;
	struct stat sb;
	model_table_t* mt = NULL;
	FILE* file = NULL;
	char* line = NULL;
	char* name_line = NULL;
	size_t length = 0;
	size_t name_length = 0;
	uint32_t checksum = 0;
	char* filename = calloc(PATH_MAX, sizeof(char));
	char* name = calloc(PATH_MAX, sizeof(char));
	char* cwd = get_current_dir_name();

	chdir(DATA_DIR);

	/* open file and skip first entry */
	if(stat(model_info_file,&sb) == 0 && 
	   (file = fopen(model_info_file, "r")) != NULL && 
	   (mt = calloc(1, sizeof(model_table_t))) != NULL &&
	   /* skip first entry which is a texture */
	   model_skip(file))
	{
		/* count models */
		while(model_skip(file))
			mt->num_models++;

		/* allocate models */
		mt->models = calloc(mt->num_models, sizeof(model_t*));

		/* reset file pos and skip first */
		rewind(file);
		if(model_skip(file))
		{
			/* read models */
			for(int i = 0; i < mt->num_models; i++)
			{
				if((mt->models[i] = model_create(file)) == NULL)
				{
					mt = model_table_destroy(mt);
					asprintf(&msg, "MODEL CONFIG LOAD FAILED %s", model_info_file);
					break;
				}
			}
		}
		else
		{
			mt = model_table_destroy(mt);
			asprintf(&msg, "MODEL CONFIG LOAD FAILED %s", model_info_file);
		}
		if(mt)
			asprintf(&msg, "MODEL CONFIG LOADED %s", model_info_file);
	}
	else
	{
		mt = model_table_destroy(mt);
		asprintf(&msg, "MODEL CONFIG LOAD FAILED %s", model_info_file);
	}

	log_queue(logger, LOG_FILES, msg);
	log_flush(logger);
	if(line)
		free(line);
	if(name_line)
		free(name_line);
	if(msg)
		free(msg);
	if(filename)
		free(filename);
	if(name)
		free(name);
	if(file)
		fclose(file);
	chdir(cwd);
	return mt;
}
