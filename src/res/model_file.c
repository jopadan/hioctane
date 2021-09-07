#include "model_file.h"
#include <errno.h>
image_t* image_destroy(image_t* image)
{
	if(image)
	{
		if(image->data)
			free(image->data);
		if(image->comp)
			free(image->comp);
		free(image);
		image = NULL;
	}
	return image;
}
/* destroy texture and return NULL */
texture_t* texture_destroy(texture_t* texture)
{
	if(texture)
	{
		if(texture->file)
			fclose(texture->file);
		for(int i = 0; i < texture->num_images; i++)
			texture->image[i] = image_destroy(texture->image[i]);
		free(texture->file_data);
		free(texture->image);
		free(texture->filename);
		free(texture->name);
		free(texture);
		texture = NULL;
	}

	return texture;
}

mesh_t* mesh_destroy(mesh_t* mesh)
{
	if(mesh)
	{
		if(mesh->vertices)
			free(mesh->vertices);
		if(mesh->normals)
			free(mesh->normals);
		if(mesh->texcoords)
			free(mesh->texcoords);
		if(mesh->indices)
			free(mesh->indices);
		if(mesh->face_normals)
			free(mesh->face_normals);

		free(mesh);
		mesh = NULL;
	}
	return mesh;
}
image_t* image_uncompress(image_t* image)
{
	if(!image || !image->data)
		return NULL;
	if(!image->comp)
		image->comp = calloc(image->size, sizeof(uint8_t));
	memcpy(image->comp, image->data, image->size);
	return image;
}
image_t* image_create(uint8_t* file_data, size_t size)
{
	image_t* image = calloc(1, sizeof(image_t));

	image->data = calloc(size, sizeof(uint8_t));
	image->data = memcpy(image->data,file_data, size);
	if(image_uncompress(image) == NULL)
		image = image_destroy(image);
	return image;
}
texture_t* texture_create(const uint32_t checksum, const char* filename, const char* name)
{
	printf("%08X %s %s\n", checksum, filename, name);
	if(checksum == 0 || filename == NULL || name == NULL)
		return NULL;

	/* error log message */
	char* msg;

	/* check for existence and size of filename */
	struct stat sb;
	if(stat(filename ,&sb) == 0)
	{
		asprintf(&msg, "MODEL TEXTURE LOAD FAILED %s file not found", filename);
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
		if((texture->file && texture->file_data))
		{
			if((texture->size = fread(texture->file_data, 1, texture->size, texture->file)) == sb.st_size)
			{
		   		if((texture->checksum = crc_32(texture->file_data, texture->size)) != checksum)
				{
					asprintf(&msg, "MODEL TEXTURE LOAD FAILED %.08X/%.08X %s %s", checksum, texture->checksum, filename, name);
					texture = texture_destroy(texture);
					goto exit_texture_create;
				}
				if((texture->image[++texture->num_images - 1] = image_create(texture->file_data, texture->size)) == NULL)
				{
					asprintf(&msg, "MODEL TEXTURE LOAD FAILED %.08X/%.08X %s %s", checksum, texture->checksum, filename, name);
					texture = texture_destroy(texture);
					goto exit_texture_create;
				}
				printf("%s: %lu\n", filename, sb.st_size);
			}
			asprintf(&msg, "MODEL TEXTURE LOADED %.08X/%.08X %s %s", checksum, texture->checksum, texture->filename, texture->name);
		}
	}
exit_texture_create:
	fclose(texture->file);
	log_queue(logger, LOG_FILES, msg);
	log_flush(logger);
	free(msg);
	return texture;
}

model_file_t* model_file_create(uint32_t checksum, char* filename, char* name)
{
	struct stat sb;
	char* line;
	size_t line_length;
	model_file_t* model = calloc(1, sizeof(model_file_t));
	model->checksum = checksum;
	model->filename = strdup(filename);
	model->name = strdup(name);

	if(!stat(filename, &sb) && sb.st_size < 0)
	{
		return NULL;
	}

	FILE* file = fopen(filename, "r");
	mesh_t* mesh = calloc(1, sizeof(mesh_t));

	/* count entries and allocate enought space */
	while(getline(&line, &line_length, file) != -1)
	{
		if(line[0] == 'g')
			model->num_textures++;
		if(line[0] == 'v' && line[1] == ' ')
			mesh->num_vertices++;
		if(line[0] == 'v' && line[1] == 't')
			mesh->num_texcoords++;
		if(line[0] == 'v' && line[1] == 'n')
			mesh->num_normals++;
		if(line[0] == 'f' && line[1] == ' ')
			mesh->num_faces++;
	}

	rewind(file);
	mesh->vertices = calloc(mesh->num_vertices, sizeof(vec4));
	mesh->texcoords = calloc(mesh->num_texcoords, sizeof(vec2));
	mesh->normals = calloc(mesh->num_normals, sizeof(vec3));
	mesh->indices = calloc(mesh->num_faces, face_verts[mesh->face_type] * sizeof(mesh->index_type));
	mesh->face_normals = calloc(mesh->num_faces, sizeof(vec3));

	GLsizei g,n,t,v,f = 0;
	while(getline(&line, &line_length, file) != -1)
	{
		switch(line[0])
		{
			/* global texture */
			case 'g':
				model->textures = calloc(++model->num_textures, sizeof(texture_t*));
				model->textures[0] = texture_manager_get_texture(&line[2]);
				break;
			/* vertex */
			case 'v':
				switch(line[1])
				{
					/* normal */
					case 'n':
						sscanf(&line[2], "%f %f %f", &mesh->normals[n][0], &mesh->normals[n][1], &mesh->normals[n][2]);
						n++;
						break;
					/* texcoord */
					case 't':
						sscanf(&line[2], "%f %f", &mesh->texcoords[t][0], &mesh->texcoords[t][1]);
						t++;
						break;
					/* vertex */
					case ' ':
						sscanf(&line[2], "%f %f %f", &mesh->vertices[v][0], &mesh->vertices[v][1], &mesh->vertices[v][2]);
						mesh->vertices[v][3] = 1.0f;
						v++;
						break;
					default:
						break;
				}
				break;
			/* face */
			case 'f':
				sscanf(&line[2], "%d/%d/%d %d/%d/%d %d/%d/%d", 
				&mesh->indices[f * face_verts[mesh->face_type] * sizeof(mesh->index_type) + 0],
				&mesh->indices[f * face_verts[mesh->face_type] * sizeof(mesh->index_type) + 1],
				&mesh->indices[f * face_verts[mesh->face_type] * sizeof(mesh->index_type) + 2],
				&mesh->indices[f * face_verts[mesh->face_type] * sizeof(mesh->index_type) + 3],
				&mesh->indices[f * face_verts[mesh->face_type] * sizeof(mesh->index_type) + 4],
				&mesh->indices[f * face_verts[mesh->face_type] * sizeof(mesh->index_type) + 5],
				&mesh->indices[f * face_verts[mesh->face_type] * sizeof(mesh->index_type) + 6],
				&mesh->indices[f * face_verts[mesh->face_type] * sizeof(mesh->index_type) + 7],
				&mesh->indices[f * face_verts[mesh->face_type] * sizeof(mesh->index_type) + 8]);
				f++;
				break;
			default:
				break;
		}
	}
	return model;
}

model_file_t* model_file_destroy(model_file_t* model_file)
{
	if(model_file)
	{
		if(model_file->filename)
			free(model_file->filename);
		if(model_file->name)
			free(model_file->name);
		for(int i = 0; i <  model_file->num_textures; i++)
			texture_destroy(model_file->textures[i]);
		for(int i = 0; i < model_file->num_meshes; i++)
			mesh_destroy(model_file->meshes[i]);

		if(model_file->textures)
			free(model_file->textures);
		if(model_file->meshes)
			free(model_file->meshes);

		free(model_file);
		model_file = NULL;
	}
	return model_file;
}
model_table_t* model_table_destroy(model_table_t* model_table)
{
	if(model_table)
	{
		for(int i = 0; i < model_table->num_models; i++)
			model_table->models[i] = model_file_destroy(model_table->models[i]);
		free(model_table->models);
		for(int i = 0; i < model_table->num_textures; i++)
			model_table->textures[i] = texture_destroy(model_table->textures[i]);
		free(model_table->textures);
		free(model_table);
		model_table = NULL;
	}

	return model_table;
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
	char* filename = calloc(32, sizeof(char));
	char* name = calloc(32, sizeof(char));

	char* cwd = get_current_dir_name();
	chdir(DATA_DIR);
	/* open file and read preceding texture entry */
	if(stat(model_info_file,&sb) == -1)
	{
		asprintf(&msg, "MODEL CONFIG LOAD FAILED cannot stat %s", model_info_file);
		goto exit_create_model_table;
	}
	if((models = calloc(1, sizeof(model_table_t))) == NULL)
	{
		asprintf(&msg, "MODEL CONFIG LOAD FAILED failed to allocate %lu", sizeof(model_table_t));
		goto exit_create_model_table;
	}
	if((file = fopen(model_info_file, "r")) == NULL)
	{
		asprintf(&msg, "MODEL CONFIG LOAD FAILED failed to open %s", model_info_file);
		goto exit_create_model_table;
	}
	if(getline(&line, &length, file) == -1)
	{
		asprintf(&msg, "MODEL CONFIG LOAD FAILED failed to read line %s", model_info_file);
		goto exit_create_model_table;
	}
	if(getline(&name_line, &name_length, file) == -1 || name_length <= 2 || name_line[0] != ';' || name_line[1] != ' ')
	{
		asprintf(&msg, "MODEL CONFIG LOAD FAILED failed to read name line %s", model_info_file, strerror(errno));
		goto exit_create_model_table;
	}
	if(sscanf(&line[0],"%08x  %s", &checksum, filename) != 2 ||
	sscanf(&name_line[0],"; %s", name) != 1)
	{
	printf("%08x %s %s\n", checksum, filename, name);
		asprintf(&msg, "MODEL CONFIG LOAD FAILED failed to parse entry %s", model_info_file);
		goto exit_create_model_table;
		
	}
	if ((models->textures = calloc(++models->num_textures, sizeof(texture_t*))) == NULL)
	{
		asprintf(&msg, "MODEL CONFIG LOAD FAILED failed to allocate %lu", sizeof(texture_t*));
		goto exit_create_model_table;
	}
	if((models->textures[models->num_textures - 1] = texture_create(checksum, filename, &name_line[2])) != NULL)
	{
		printf("%X %s %s\n", models->textures[models->num_textures -1]->checksum, models->textures[models->num_textures-1]->filename, models->textures[models->num_textures -1]->name);
		/* read model entries */
		while((getline(&line, &length, file) != -1))
		{
			if(getline(&name_line, &name_length, file) == -1)
			{
				asprintf(&msg, "MODEL CONFIG LOAD FAILED %s after model entry %lu", model_info_file, models->num_models);
				break;
			}
			if((sscanf(&line[0],"%08x %s", &checksum, filename) == 2) &&
			   (name_length > 2 && name_line[0] == ';' && name_line[1] == ' '))
			{
				models->models[++models->num_models - 1] = model_file_create(checksum, filename, &name_line[2]);
			}
			else
			{
				asprintf(&msg, "MODEL CONFIG LOAD FAILED %s after model entry %lu", model_info_file, models->num_models);
				break;
			}
		}
	}
	else
	{
		asprintf(&msg, "MODEL CONFIG LOAD FAILED %s", model_info_file);
	}
exit_create_model_table:
	log_queue(logger, LOG_FILES, msg);
	log_flush(logger);
	free(msg);
	if(file)
		fclose(file);
	if(models)
		models = model_table_destroy(models);
	chdir(cwd);
	return models;
}
