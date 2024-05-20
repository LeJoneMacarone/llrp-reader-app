#include "readings.h"
#include "../cJSON/cJSON.h"

#include <stdio.h>
#include <stdlib.h>

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
	
	reading->time = (char*) time;
	sprintf(reading->source, "%s @ %s #%i", reader_name, host, antenna);
	reading->rfid = (char *) rfid;
	reading->rssi = rssi;
	reading->athlete = (char *) athlete;
	reading->crossing = crossing;

	return reading;
}
