#include "crossings-buffer.h"
#include "readings.h"

#include <stdio.h>
#include <string.h>

#define USE_READER_TIMESTAMPS

#define CROSSINGS_BUFFER_SIZE 256
Reading * crossings[CROSSINGS_BUFFER_SIZE];

int crossingsInsertIndex = 0,
	crossingsCount = 0;

void crossings_add(Reading * reading) {
	crossings[crossingsInsertIndex] = reading;
	crossingsInsertIndex = (crossingsInsertIndex + 1) % CROSSINGS_BUFFER_SIZE;
	crossingsCount++;
}

int crossings_contains(const char * rfid) {
	for (int i = 0; i < CROSSINGS_BUFFER_SIZE; i++) {
		if (crossings[i] == NULL) continue;
		int cmp = strcmp(crossings[i]->rfid, rfid);
		if (cmp == 0) return 1;
	}
	return 0;
}

void crossings_print(){
	printf("[INFO] crossings count: %i\n", crossingsCount);

	for (int i = 0; i < CROSSINGS_BUFFER_SIZE; i++) {
		if (crossings[i] == NULL) continue;
		printf("[INFO] Crossing[%i] = %s\n", i, reading_toString(crossings[i]));
	}
}


