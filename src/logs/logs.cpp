#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "logs.h"

const char * add_timestamp (const char * message) {
	time_t now = time(NULL);
	
	char * timestamp = ctime(&now);
	size_t timestampSize = strlen(timestamp);
	timestamp[timestampSize - 1] = ']';

	size_t messageSize = strlen(message);
	char * newMessage = (char *) malloc((timestampSize + messageSize) * sizeof(char));

	sprintf(newMessage, "[%s %s", timestamp, message);
	
	return newMessage;
}

recorder_t * recorder_init(FILE * stream, const char * (* callback) (const char *)) {
	recorder_t * recorder = (recorder_t *) malloc(sizeof(recorder_t));
	
	if (NULL == recorder) return NULL;

	recorder->stream = stream;
	recorder->callback = callback;
	return recorder;
}

logger_t * logger_init(recorder_t * recorders, size_t size) {
	logger_t * logger = (logger_t *) malloc(sizeof(logger_t));
	
	if (NULL == logger) return NULL;

	logger->size = size;
	logger->recorders = recorders;
	return logger;
}

void logger_add(logger_t * logger, recorder_t recorder) {
	logger->size ++;
	
	logger->recorders = (recorder_t *) realloc(
		logger->recorders, 
		logger->size * sizeof(recorder_t)
	);

	logger->recorders[logger->size - 1] = recorder;
}

const char * flag_str[] = { "INFO", "WARN", "ERROR", "DEBUG" };

void logger_log(logger_t * logger, logflag_t flag, const char * message) {
	for (size_t i = 0; i < logger->size; i ++) {
		const char * newMessage = logger->recorders[i].callback == NULL ?
			message : (const char *) logger->recorders[i].callback(message);
		
		fprintf(logger->recorders[i].stream, "[%s] %s\n", flag_str[flag], newMessage);
	}
}

logger_t * logger;

logger_t * logger_instance() {
	if (logger == NULL) logger = logger_init(NULL, 0);
	return logger;
}



