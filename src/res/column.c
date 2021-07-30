#include "column.h"
/*
column_t* column_create(int id, int offset, uint8_t* data)
{
	column_t* column = calloc(1, sizeof(column_t));
	if(!column)
	{
		fprintf(stderr, "ERROR: column_create(...) allocating memory failed!\n");
		return NULL;
	}

	column->id = id;
	column->offset = offset;
	memcpy(column->data, data, 26);

	return column;
}

bool column_destroy(column_t* column)
{
	if(!column)
	{
		fprintf(stderr, "ERROR: column_destroy(...) column == NULL!\n");
		return false;
	}

	free(column->data);
	free(column);

	return true;
}
*/
