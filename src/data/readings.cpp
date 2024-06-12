#include "readings.h"
#include "../cJSON/cJSON.h"

#include <stdio.h>
#include <stdlib.h>

#define READINGS_BUFFER_SIZE 256
Reading * readings[READINGS_BUFFER_SIZE];

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

// TODO: implement locks for producer-consumer 
void readings_add(Reading * reading) {
	int i = 0;
	while (readings[i] != NULL) i = (i + 1) % READINGS_BUFFER_SIZE;
	readings[i] = reading;
}

void readings_print(){
	for (int i = 0; i < READINGS_BUFFER_SIZE; i++) {
		printf("[INFO] Reading[%i] = %s", i, reading_toJsonString(readings[i]));
	}
}

