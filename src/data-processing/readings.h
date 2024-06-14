#ifndef READINGS
#define READINGS

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

char * reading_toJSON(Reading * reading);

#endif // READINGS 
