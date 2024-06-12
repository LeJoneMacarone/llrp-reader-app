#include "readings.h"
#include "../cJSON/cJSON.h"

#include <stdio.h>
#include <stdlib.h>

#define READINGS_BUFFER_SIZE 256
Reading * readings[READINGS_BUFFER_SIZE];

#define CROSSINGS_BUFFER_SIZE 256
const char * crossings[CROSSINGS_BUFFER_SIZE];

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

int insertIndex = 0,
	removeIndex = 0;

void readings_add(Reading * reading) {
	readings[insertIndex] = reading;
	insertIndex = (insertIndex + 1) % READINGS_BUFFER_SIZE;
}

void readings_take() {
	readings[removeIndex] = NULL;
	removeIndex = (removeIndex + 1) % READINGS_BUFFER_SIZE;
}

void readings_print(){
	for (int i = 0; i < READINGS_BUFFER_SIZE; i++) {
		if (readings[i] == NULL) continue;
		printf("[INFO] Reading[%i] = %s\n", i, reading_toJsonString(readings[i]));
	}
}
