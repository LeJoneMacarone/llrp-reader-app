#include <stdio.h>

typedef struct {
	FILE * stream;
	const char * (* callback) (const char * message);
} recorder_t;

typedef struct {
	size_t size;
	recorder_t * recorders;
} logger_t;

recorder_t * recorderInit(FILE * stream, const char * callback(const char *));

logger_t * loggerInit();

void loggerAdd(logger_t * logger, recorder_t recorder);

void loggerLog(logger_t * logger, const char * message);

const char * addTimestamp(const char * message);
