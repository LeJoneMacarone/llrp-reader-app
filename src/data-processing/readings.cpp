#include "readings.h"
#include "../cJSON/cJSON.h"

#include <stdio.h>
#include <stdlib.h>

#define READINGS_BUFFER_SIZE 256
static Reading * readings[READINGS_BUFFER_SIZE];

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
	removeIndex = 0,
	readingsCount = 0;

void readings_add(Reading * reading) {
	readings[insertIndex] = reading;
	insertIndex = (insertIndex + 1) % READINGS_BUFFER_SIZE;
	readingsCount++;
}

Reading * readings_take() {
	Reading * reading = readings[removeIndex];
	readings[removeIndex] = NULL;
	removeIndex = (removeIndex + 1) % READINGS_BUFFER_SIZE;
	readingsCount--;
	return reading;
}

void readings_print(){
	if (readingsCount == 0) {
		printf("[INFO] readings buffer empty\n");
		return;
	}

	for (int i = 0; i < READINGS_BUFFER_SIZE; i++) {
		if (readings[i] == NULL) continue;
		printf("[INFO] Reading[%i] = %s\n", i, reading_toJsonString(readings[i]));
	}
}

int readerClientDone = 0;

void setReaderClientDone() {
	readerClientDone = 1;
}

void * dataProcessingRun(void * args) {
	int i = 0;
	while (!readerClientDone || readingsCount > 0) {
		Reading * reading = readings_take();
	}
	printf("[INFO] data processing finished\n");
	return NULL;
}
