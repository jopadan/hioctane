#include "column.h"

bool column_print(column_t* column)
{
	if(column->shape == 0 && column->unused == 0 && column->floor_texid == 0 && column->unknown1 == 0 && column->a == 0 && column->b == 0 && column->c == 0 && column->d == 0 && column->e == 0 && column->f == 0 && column->g == 0 && column->h == 0 &&  column->unknown2 == 0 && column->unknown3 == 0)
	{
		return false;
	}

	printf("shape: %u\nunused: %u\nfloor_texid: %d\nunknown1: %d\na: %d\nb: %d\nc: %d\nd: %d\ne: %d\nf: %d\ng: %d\nh: %d\nunknown2: %d\nunknown3: %d\n\n",
		column->shape, column->unused, column->floor_texid, column->unknown1,
		column->a, column->b, column->c, column->d, column->e, column->f, column->g, column->h,
		column->unknown2, column->unknown3);
	return true;
}
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
