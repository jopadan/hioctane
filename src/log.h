#ifndef _LOG_H
#define _LOG_H
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/param.h>
#include <time.h>

#define LOG_BUFFER_LENGTH 2048

enum log_secion_e { LOG_TIMER, LOG_FILES, LOG_INPUT, LOG_VIDEO, LOG_SOUND, LOG_MUSIC, LOG_SYSTEM, LOG_MENU, LOG_LEVEL, LOG_SECTION_END };
extern char* log_section_name[LOG_SECTION_END];

typedef struct log_s
{
	FILE* stream;
	char buffer[LOG_SECTION_END][LOG_BUFFER_LENGTH];
} log_t;

extern log_t* logger;

bool log_halt(log_t* log);
bool log_destroy(log_t* log);
bool log_queue(log_t* log, int section, const char* msg);
bool log_flush(log_t* log);
bool log_update(log_t* log);
log_t* log_create();


#endif
