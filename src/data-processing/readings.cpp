#include "readings.h"
#include "../cJSON/cJSON.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

char * timestampToString(uint64_t timestamp) {
    const uint64_t MICROSECONDS_IN_SECOND = 1000000;

    time_t seconds = timestamp / MICROSECONDS_IN_SECOND;
    suseconds_t microseconds = timestamp % MICROSECONDS_IN_SECOND;

    struct tm * date = localtime(&seconds);

	int size = 20;
	char dateString[size];
    strftime(dateString, 20, "%Y-%m-%d %H:%M:%S", date);
	
	char * result;
	asprintf(&result, "%s.%06ld", dateString, microseconds);
	return result;
}

Reading * reading_create(
	uint64_t reader_time,
	uint64_t local_time,
	unsigned int antenna,
	const char * rfid,
	int rssi
) {
	Reading * reading = (Reading *) malloc(sizeof(Reading));

	reading->reader_time = reader_time;
	reading->local_time = local_time;

	reading->rfid = (char *) rfid;
	reading->rssi = rssi;
	reading->antenna = antenna;

	return reading;
}

char * reading_toJSON(Reading * reading) {
	cJSON * json = cJSON_CreateObject();

	char * readerTimeString = timestampToString(reading->reader_time);
	char * localTimeString = timestampToString(reading->local_time);
	cJSON_AddStringToObject(json, "reader-time", readerTimeString);
	cJSON_AddStringToObject(json, "local-time", localTimeString);
	
	cJSON_AddStringToObject(json, "rfid", reading->rfid);
	cJSON_AddNumberToObject(json, "rssi", reading->rssi);
	cJSON_AddNumberToObject(json, "antenna", reading->antenna);
	
	char * string = cJSON_Print(json);
	
	cJSON_free(json);
	free(readerTimeString);
	free(localTimeString);

	return string;
}
