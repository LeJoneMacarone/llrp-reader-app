#include "timestamps.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

char * stringFromMicroseconds(uint64_t timestamp) {
	time_t seconds = timestamp / 1000000;
	suseconds_t microseconds = timestamp % 1000000;
	unsigned int milliseconds = microseconds / 1000;

    struct tm * date = localtime(&seconds);

	int size = 20;
	char dateString[size];
    strftime(dateString, size, "%Y-%m-%d %H:%M:%S", date);
	
	char * result;
	asprintf(&result, "%s.%03u", dateString, milliseconds);
	return result;
}

uint64_t microsecondsFromString(const char * string) {
    struct tm tm;
    unsigned int milliseconds;

    sscanf(
		string, "%d-%d-%d %d:%d:%d.%d",
    	&tm.tm_year, &tm.tm_mon, &tm.tm_mday,
    	&tm.tm_hour, &tm.tm_min, &tm.tm_sec,
    	&milliseconds
	);
	
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;

    time_t seconds = mktime(&tm);

    uint64_t timestamp = (uint64_t) seconds * 1000000 + milliseconds * 1000;

    return timestamp;
} 
