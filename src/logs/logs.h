#include <cstdlib>
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
recorder_t * recorder_init(FILE * stream, const char * callback(const char *));

/*
 * Allocates memory to a new logger, without initializing any parameters.  
 */
logger_t * logger_init(recorder_t * recorders);

/*
 * Adds a recorder to the logger structure. 
 */
void logger_add(logger_t * logger, recorder_t recorder);

/*
 * Writes a message to each stream provided in the recorders of the logger.
 * The callback provided in each recorder is called before commiting the
 * message.
 */
void logger_log(logger_t * logger, const char * message);

/*
 * Provides a timestamp before the message.
 */
const char * add_timestamp(const char * message);

/*
 * Based on the singleton pattern, provides a unique shared logger instance
 * if needed.
 */
logger_t * logger_instance();
