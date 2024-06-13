#include "readings-buffer.h"

#include <stdio.h>
#include <stdlib.h>

#define READINGS_BUFFER_SIZE 256
static Reading * readings[READINGS_BUFFER_SIZE];

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

int readings_count() {
	return readingsCount;
}

void readings_print(){
	printf("[INFO] readings count: %i\n", readingsCount);

	for (int i = 0; i < READINGS_BUFFER_SIZE; i++) {
		if (readings[i] == NULL) continue;
		printf("[INFO] Reading[%i] = %s\n", i, reading_toJsonString(readings[i]));
	}
}
