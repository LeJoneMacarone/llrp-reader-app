#include "readings.h"
#include "timestamps.h"
#include "../cJSON/cJSON.h"

#include <string.h>
#include <stdlib.h>

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
	reading->rfid =  strdup(rfid);
	reading->rssi = rssi;
	reading->antenna = antenna;

	return reading;
}

char * reading_toString(Reading * reading) {
	cJSON * json = reading_toJSON(reading);
	char * string = cJSON_Print(json);
	cJSON_free(json);
	return string;
}

cJSON * reading_toJSON(Reading * reading) {
	cJSON * json = cJSON_CreateObject();

	char * readerTimeString = timestampToString(reading->reader_time);
	char * localTimeString = timestampToString(reading->local_time);
	cJSON_AddStringToObject(json, "reader-time", readerTimeString);
	cJSON_AddStringToObject(json, "local-time", localTimeString);

	cJSON_AddStringToObject(json, "rfid", reading->rfid);
	cJSON_AddNumberToObject(json, "rssi", reading->rssi);
	cJSON_AddNumberToObject(json, "antenna", reading->antenna);

	free(readerTimeString);
	free(localTimeString);

	return json;
}
