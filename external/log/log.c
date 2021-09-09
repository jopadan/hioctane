#include "log.h"

char* log_section_name[LOG_SECTION_END] = { "TIMER", "FILE", "INPUT", "VIDEO", "SOUND", "MUSIC", "SYSTEM", "MENU", "LEVEL"};

log_t* log_create()
{
	log_t* log = calloc(1,sizeof(log_t));
	log->stream = stderr;
	return log;
}

bool log_update(log_t* log)
{
	return true;
}

bool log_flush_section(log_t* log, int section)
{
	if(log->buffer[section][0] != '\0')
	{
		fprintf(log->stream, "%s", log->buffer[section]);
		log->buffer[section][0] = '\0';
	}
	return true;
}

bool log_flush(log_t* log)
{
	for(int i = 0; i < LOG_SECTION_END; i++)
	{
		if(!log_flush_section(log, i))
			return false;
	}
	return true;
}

bool log_queue(log_t* log, int section, const char* msg)
{
	time_t t = time(NULL);
	char* t_str = ctime(&t);
	t_str[strlen(t_str) - 1] = '\0';

	char* time_msg;
	asprintf(&time_msg, "%s %s: %s", t_str, log_section_name[section], msg);
	size_t msg_len = strlen(time_msg);
	size_t buf_len = strlen(log->buffer[section]);

	if(buf_len + msg_len + 1 >= LOG_BUFFER_LENGTH)
	{
		if(!log_flush_section(log, section))
			return false;
	}

	buf_len = strlen(log->buffer[section]);
	strncat(&log->buffer[section][buf_len], time_msg, MIN(LOG_BUFFER_LENGTH, msg_len + 1));
	buf_len = strlen(log->buffer[section]);
	strncat(&log->buffer[section][buf_len], "\n", 2);
	free(time_msg);
	return true;
}

bool log_destroy(log_t* log)
{
	if(log == NULL)
		return false;
	if(log->stream)
		fclose(log->stream);
	
	free(log);
	return true;
}

bool log_halt(log_t* log)
{
	if(log == NULL)
		return false;

	return log_destroy(log);
}
