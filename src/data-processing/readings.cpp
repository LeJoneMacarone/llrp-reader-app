#include "readings.h"
#include "../cJSON/cJSON.h"

#include <stdlib.h>

Reading * reading_create(
	const char * time,
	unsigned int antenna,
	const char * rfid,
	int rssi
) {
	Reading * reading = (Reading *) malloc(sizeof(Reading));

	reading->time = (char *) time;
	reading->rfid = (char *) rfid;
	reading->rssi = rssi;
	reading->antenna = antenna;

	return reading;
}

char * reading_toJsonString(Reading * reading) {
	cJSON * json = cJSON_CreateObject();

	cJSON_AddStringToObject(json, "time", reading->time);
	cJSON_AddStringToObject(json, "rfid", reading->rfid);
	cJSON_AddNumberToObject(json, "rssi", reading->rssi);
	
	char * string = cJSON_Print(json);
	cJSON_free(json);

	return string;
}
