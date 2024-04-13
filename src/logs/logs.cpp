#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "logs.h"

const char * addTimestamp (const char * message) {
	time_t now = time(NULL);
	
	char * timestamp = ctime(&now);
	size_t timestampSize = strlen(timestamp);
	timestamp[timestampSize - 1] = ']';

	size_t messageSize = strlen(message);
	char * newMessage = (char *) malloc((timestampSize + messageSize) * sizeof(char));

	sprintf(newMessage, "[%s %s", timestamp, message);
	
	return newMessage;
}

recorder_t * recorderInit(FILE * stream, const char * (* callback) (const char *)) {
	recorder_t * recorder = (recorder_t *) malloc(sizeof(recorder_t));
	
	if (NULL == recorder) return NULL;

	recorder->stream = stream;
	recorder->callback = callback;
	return recorder;
}

logger_t * loggerInit() {
	logger_t * logger = (logger_t *) malloc(sizeof(logger_t));
	
	if (NULL == logger) return NULL;

	logger->size = 0;
	logger->recorders = NULL;
	return logger;
}

void loggerAdd(logger_t * logger, recorder_t recorder) {
	logger->size ++;	
	
	logger->recorders = (recorder_t *) realloc(
		logger->recorders, 
		logger->size * sizeof(recorder_t)
	);
	
	logger->recorders[logger->size - 1] = recorder;
}

void loggerLog(logger_t * logger, const char * message) {
	for (size_t i = 0; i < logger->size; i ++) {
		const char * newMessage = 
			logger->recorders[i].callback == NULL ?
			message : (const char *) logger->recorders[i].callback(message);
		
		fprintf(logger->recorders[i].stream, "%s\n", newMessage);
	}
}
