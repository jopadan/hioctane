#include "level_file.h"

bool map_header_destroy(map_header_t* header)
{
	if(header == NULL)
		return false;

	if(header->filename)
		free(header->filename);
	if(header->name)
		free(header->name);

	return true;
}

bool level_table_destroy(level_table_t* table)
{
	if(table == NULL)
		return false;
	for(size_t i = 0; i < table->num_maps; i++)
	{
		if(!map_header_destroy(table->headers[i]) || !map_file_destroy(table->files[i]))
			return false;
	}
	free(table);
	return true;
}

bool map_file_destroy(map_file_t* file)
{
	if(file == NULL)
		return false;

	free(file);
	return true;
}

map_header_t* map_header_create(char* line, char* name_line)
{
	map_header_t* map_header = calloc(1, sizeof(map_header_t));

	if(map_header == NULL)
	{
		perror("map_header_create()");
		return NULL;
	}

	if(sscanf(line, "%x %ms\n", &map_header->checksum, &map_header->filename) != 2)
	{
		fprintf(stderr, "map_header->checksum: %x map_header->filename: %s\n", map_header->checksum, map_header->filename);
		free(map_header);
		return NULL;
	}

	if(name_line[0] != ';' && name_line[1] != ' ')
	{
		fprintf(stderr, "map_header_create: invalid name line format please use `; <name>`\n");
		free(map_header);
		return NULL;
	}

	line[strlen(line) -1 ] = '\0';
	name_line[strlen(name_line) -1 ] = '\0';

	if((map_header->name = strdup(&name_line[2])) == NULL)
	{
		fprintf(stderr, "map_header->name %s\n", map_header->name);
		free(map_header);
		return NULL;
	}

	return map_header;
}

map_file_t* map_file_create(map_header_t* header)
{
	map_file_t* map_file = calloc(1, sizeof(map_file_t));

	struct stat sb;
	if(stat(header->filename, &sb) != 0)
	{
		free(map_file);
		perror(header->filename);
		return NULL;
	}

	char* cwd = get_current_dir_name();
	chdir(DATA_DIR);

	if(sb.st_size != MAP_FILE_SIZE)
	{
		free(map_file);
		fprintf(stderr, "ERROR: %s size doesn't equal %d\n", header->filename, MAP_FILE_SIZE);
		return NULL;
	}
	
	/* open header->filename */
	FILE* file = fopen(header->filename, "rb");

	if(file == NULL)
	{
		chdir(cwd);
		free(map_file);
		perror(header->filename);
		return NULL;
	}

	/* read map */
	if(fread(map_file->data, MAP_FILE_SIZE, 1, file) != 1)
	{
		fclose(file);
		chdir(cwd);
		free(map_file);
		perror(header->filename);
		return NULL;
	}

	fclose(file);
	chdir(cwd);

	/* verify checksum of map */
	uint32_t checksum = 0;
	char* msg;
	if((checksum = crc_32(map_file->data, MAP_FILE_SIZE)) != header->checksum)
	{
		free(map_file);
		if(asprintf(&msg,"MAP LOAD FAILED with checksum verification of %s %.8X/%.8X %s", header->filename,header->checksum, checksum, header->name) == -1)
		{
			log_queue(logger, LOG_FILES, "MAP LOAD FAILED calling asprintf!");
			log_flush(logger);
			free(msg);
			return NULL;
		}
	}
	if(asprintf(&msg,"MAP LOADED %s %.8X/%.8X %s", header->filename,header->checksum, checksum, header->name) == -1)
	{
		log_queue(logger, LOG_FILES, "MAP LOAD FAILED calling asprintf!");
		log_flush(logger);
		free(msg);
		return NULL;
	}
	log_queue(logger, LOG_FILES, msg);
	log_flush(logger);
	free(msg);
	return map_file;
}

level_table_t* level_table_create(char* level_info_file)
{
	fprintf(stdout, "\n");
	level_table_t* levels = calloc(1, sizeof(level_table_t));
	if(levels == NULL)
	{
		perror("level_table_craete()");
		return NULL;
	}

	char* cwd = get_current_dir_name();
	chdir(DATA_DIR);

	/* check if level info file exists */
	struct stat sb;
	if(stat(level_info_file, &sb) != 0)
	{
		free(levels);
		perror(level_info_file);
		return NULL;
	}

	if(sb.st_size == 0)
	{
		free(levels);
		fprintf(stderr, "ERROR: %s too small\n", level_info_file);
		return NULL;
	}

	/* open level_info_file */
	FILE* file = fopen(level_info_file, "r");

	if(!file)
	{
		free(levels);
		fprintf(stderr, "ERROR: reading level info file %s\n", level_info_file);
		return NULL;
	}

	/* get number of maps in level_info_file */
	char* line = NULL;
	char* name_line = NULL;
	size_t length = 0;
	size_t name_length = 0;
	while(getline(&line, &length, file) != -1)
	{
		if(getline(&name_line, &name_length, file) == -1)
		{
			fprintf(stderr, "ERROR: missing map name!\n");
			break;
		}
		
		levels->headers = reallocarray(levels->headers, ++levels->num_maps, sizeof(map_header_t*));
		if(levels->headers == NULL)
		{
			perror("unable to create map header");
			fclose(file);
			chdir(cwd);
			level_table_destroy(levels);
			return NULL;
		}
		levels->headers[levels->num_maps - 1] = map_header_create(line, name_line);
		if(levels->headers[levels->num_maps - 1] == NULL)
		{
			perror("unable to create map header");
			fclose(file);
			chdir(cwd);
			level_table_destroy(levels);
			return NULL;
		}
	}
	free(line);
	rewind(file);

	if(levels->num_maps > MAPS_MAX)
	{
		fclose(file);
		chdir(cwd);
		free(levels);
		fprintf(stderr, "ERROR: too many map files in %s\n", level_info_file);

	}

	levels->files = calloc(levels->num_maps, sizeof(map_file_t*));

	/* read map data */
	for(int i = 0; i < levels->num_maps; i++)
	{
		levels->files[i] = map_file_create(levels->headers[i]);
		if(levels->files[i] == NULL)
		{
			perror("unable to create map file");
			fclose(file);
			chdir(cwd);
			level_table_destroy(levels);
			return NULL;
		}
	}

	fclose(file);
	chdir(cwd);

	return levels;
}
