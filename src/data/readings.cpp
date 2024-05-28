#include "readings.h"
#include "../cJSON/cJSON.h"

#include <stdio.h>
#include <stdlib.h>

#include <list>

using namespace std;

list<Reading> readings;

Reading * reading_create(
	const char * time,
	const char * reader_name,
	const char * host,
	unsigned int antenna,
	const char * rfid,
	int rssi,
	const char * athlete,
	int crossing
) {
	Reading * reading = (Reading *) malloc(sizeof(Reading));

	reading->time = (char *) time;
	reading->rfid = (char *) rfid;
	reading->athlete = (char *) athlete;

	asprintf(&(reading->source), "%s @ %s #%u", reader_name, host, antenna);

	reading->rssi = rssi;
	reading->crossing = crossing;

	return reading;
}

char * reading_toJsonString(Reading * reading) {
	cJSON * json = cJSON_CreateObject();

	cJSON_AddStringToObject(json, "time", reading->time);
	cJSON_AddStringToObject(json, "source", reading->source);
	cJSON_AddStringToObject(json, "rfid", reading->rfid);
	cJSON_AddNumberToObject(json, "rssi", reading->rssi);
	cJSON_AddStringToObject(json, "athlete", reading->athlete);
	
	reading->crossing 
		? cJSON_AddTrueToObject(json, "crossing") 
		: cJSON_AddFalseToObject(json, "crossing");

	char * string = cJSON_Print(json);
	cJSON_free(json);

	return string;
}

void readings_add(Reading reading) {
	readings.push_front(reading);
}

void readings_print(){
	list<Reading>::iterator it;

	for (
		it = readings.begin();
		it != readings.end();
		++it
	) {
		printf("[INFO] %s\n", reading_toJsonString(&(*it)));
	}
}

