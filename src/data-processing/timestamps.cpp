#include "timestamps.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#define MICROSECONDS_IN_SECONDS 1000000

char * timestampToString(uint64_t timestamp) {
    time_t seconds = timestamp / MICROSECONDS_IN_SECONDS;
    suseconds_t microseconds = timestamp % MICROSECONDS_IN_SECONDS;

    struct tm * date = localtime(&seconds);

	int size = 20;
	char dateString[size];
    strftime(dateString, 20, "%Y-%m-%d %H:%M:%S", date);
	
	char * result;
	asprintf(&result, "%s.%06ld", dateString, microseconds);
	return result;
}

uint64_t stringToTimestamp(const char * string) {
    struct tm tm;
    int microseconds;

	// Scan the string for date-time values
    sscanf(
		string, "%d-%d-%d %d:%d:%d.%d",
    	&tm.tm_year, &tm.tm_mon, &tm.tm_mday,
    	&tm.tm_hour, &tm.tm_min, &tm.tm_sec,
    	&microseconds
	);

    // Adjust tm structure to match the expected values
    tm.tm_year -= 1900; 
    tm.tm_mon -= 1;

    // Convert to time_t (seconds since epoch)
    time_t seconds = mktime(&tm);

    // Calculate the timestamp in microseconds
    uint64_t timestamp = (uint64_t) seconds * MICROSECONDS_IN_SECONDS + microseconds;

    return timestamp;
}
