#include <stdio.h>

typedef enum {
	INFO,
	WARN,
	ERROR,
	DEBUG
} logflag;

/*
 * Writes a log message on the stream, in the provided format and starting 
 * with the given flag surrounded by brackets.
 */
void log(FILE * stream, logflag flag, const char * format, ...);
