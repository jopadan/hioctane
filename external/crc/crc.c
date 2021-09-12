#include "crc.h"

int main(int argc, char** argv)
{
	int exit_status = EXIT_FAILURE;
	bool print_table = false;
	struct stat sb;
	char* usage = NULL;
	uint32_t poly = CRC_POLY_32;
	uint32_t start = CRC_START_32;

	asprintf(&usage, "Usage: %s [OPTION]... [FILE]...\nOutput CRC checksum file of listed files in option defined style.\n\n      --crc32       \tuse CRC32 algorithm\n      --crc32c      \tuse CRC32C algorithm\n      --ccitt32     \tuse CCITT32 algorithm\n      --zip         \tuse ZIP style of algorithm\n  -t, --table      \tprint CRC table\n      --poly=POLYGON\tuse POLYGON to generate table\n      --start=START\tuse START as starting value for CRC\n      --%s    \tprint message digests, using simple format (default).\n      --%s     \tprint message digests, using SFV format.\n      --%s     \tprint message digests, using BSD-like format\n\n", argv[0],
		chksum_style_str[CHKSUM_STYLE_SIMPLE],
		chksum_style_str[CHKSUM_STYLE_SFV],
		chksum_style_str[CHKSUM_STYLE_BSD]);

	if(argc < 2)
	{
		fputs(usage, stderr);
		free(usage);
		exit(exit_status);
	}



	int i=1;
	while( i < argc && argv[i][0] == '-' )
	{
		if(argv[i][1]=='-')
		{
			if(strcmp(&argv[i][2], chksum_style_str[CHKSUM_STYLE_SIMPLE]) == 0)
				chksum_style = CHKSUM_STYLE_SIMPLE;
			else if(strcmp(&argv[i][2], chksum_style_str[CHKSUM_STYLE_SFV]) == 0)
				chksum_style = CHKSUM_STYLE_SFV;
			else if(strcmp(&argv[i][2], chksum_style_str[CHKSUM_STYLE_BSD]) == 0)
				chksum_style = CHKSUM_STYLE_BSD;
			else if(strcmp(&argv[i][2], chksum_type_str[CRC_CRC32]) == 0)
				crc_type = CRC_CRC32;
			else if(strcmp(&argv[i][2], chksum_type_str[CRC_CRC32C]) == 0)
				crc_type = CRC_CRC32C;
			else if(strcmp(&argv[i][2], chksum_type_str[CRC_CCITT32]) == 0)
				crc_type = CRC_CCITT32;
			else if(strcmp(&argv[i][2], chksum_type_str[CRC_ZIP]) == 0)
				crc_type = CRC_ZIP;
			else if(sscanf(&argv[i][2], "poly=%x", &poly) == 1)
			{
			}
			else if(sscanf(&argv[i][2], "start=%x", &start) == 1);
			{
			}


			if(strcmp(&argv[i][2], "table") == 0)
				print_table = true;
		}

		if(argv[i][1] == 't' && strlen(argv[i]) == 2)
			print_table = true;
		i++;
	}
	//crc32_inittab(CRC_POLY_32);
	//uint32_t* table = crc32_table_create(CRC_CRC32, poly, CRC_TABLE_32);

	if(i >= argc)
	{
		fputs(usage,stderr);
		if(print_table)
			//crc32_table_print(table, CRC_TABLE_32);
		//table = crc32_table_destroy(table);
		free(usage);
		exit(exit_status);
	}

	if(print_table);
	//	crc32_table_print(table, CRC_TABLE_32);


	for(; i < argc; i++)
	{

		if(stat(argv[i], &sb) == 0 && sb.st_size > 0)
		{
			FILE* file = fopen(argv[i], "rb");
			if(file != NULL)
			{
				uint8_t* bytes = calloc(sb.st_size, sizeof(uint8_t));
				if(fread(bytes, sb.st_size, 1, file) != -1)
				{
					uint32_t checksum = crc32(crc_type, start, bytes, sb.st_size);
					switch(chksum_style)
					{
						case CHKSUM_STYLE_SFV:
							fprintf(stdout, "%s %08X\n", argv[i], checksum);
							break;
						case CHKSUM_STYLE_BSD:
							fprintf(stdout, "CRC32 (%s) = %08x\n", argv[i], checksum);
							break;
						case CHKSUM_STYLE_SIMPLE:
						default:
							fprintf(stdout, "%08x  %s\n", checksum, argv[i]);
							break;
					}
					fclose(file);
					continue;
				}
				else
				{
					fprintf(stderr, "Error: reading %s\n", argv[i]);
					exit_status = EXIT_FAILURE;
					fclose(file);
					break;
				}
			}
		}
		fprintf(stderr, "Error: opening %s\n", argv[i]);
		exit_status = EXIT_FAILURE;
	}

	//table = crc32_table_destroy(table);

	free(usage);
	exit(exit_status);
}
