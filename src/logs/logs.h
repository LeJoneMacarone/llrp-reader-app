#include <stdio.h>

typedef struct {
	FILE * stream;
	const char * (* callback) (const char * message);
} recorder_t;

typedef struct {
	size_t size;
	recorder_t * recorders;
} logger_t;

/*
 * Initializes a recorder struct. A recorder takes a stream to which messages 
 * will be sentand and a callback that will update the message before writing
 * it to the stream.
 */
recorder_t * recorderInit(FILE * stream, const char * callback(const char *));

/*
 * Allocates memory to a new logger, without initializing any parameters.  
 */
logger_t * loggerInit();

/*
 * Adds a recorder to the logger struct. 
 */
void loggerAdd(logger_t * logger, recorder_t recorder);

/*
 * Writes a message to each stream provided in the recorders of the logger.
 * The callback provided in each recorder is called before commiting the
 * message.
 */
void loggerLog(logger_t * logger, const char * message);

/*
 * Provides a timestamp before the message
 */
const char * addTimestamp(const char * message);
