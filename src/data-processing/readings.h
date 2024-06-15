#ifndef READINGS
#define READINGS

#include "../cJSON/cJSON.h"
#include <stdint.h>

typedef struct {
    uint64_t reader_time;
	uint64_t local_time;
	unsigned int antenna;	
    char * rfid; 
    int rssi; 
} Reading;

Reading * reading_create(
	uint64_t reader_time,
	uint64_t local_time,
	unsigned int antenna, 
	const char * rfid,
	int rssi
);

cJSON * reading_toJSON(Reading * reading);

char * reading_toString(Reading * reading);

#endif // READINGS 
