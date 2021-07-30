#include <stdlib.h>
#include <stdio.h>

#include "system_types.h"
#include "res/level_file.h"

int main(int argc, char** argv)
{
	level_table_t* level_table = level_table_create("maps.crc");
	level_table_destroy(level_table);
	exit(EXIT_SUCCESS);
}
