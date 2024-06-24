#ifndef READINGS_H
#define READINGS_H

#include "../cJSON/cJSON.h"
#include <stdint.h>

typedef struct Reading {
    uint64_t reader_time;
	uint64_t local_time;
	unsigned int antenna;	
    char * rfid; 
    int rssi;
	int status; 
} Reading;

Reading * reading_create(
	uint64_t reader_time,
	uint64_t local_time,
	unsigned int antenna, 
	const char * rfid,
	int rssi
);

void reading_free(Reading * reading);

cJSON * reading_toJSON(Reading * reading);

char * reading_toString(Reading * reading);

#endif // READINGS 
