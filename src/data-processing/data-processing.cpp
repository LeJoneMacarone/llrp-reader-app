#include "readings-buffer.h"

#include <stdio.h>

#define CROSSINGS_BUFFER_SIZE 256
const char * crossings[CROSSINGS_BUFFER_SIZE];

int readerClientDone = 0;

void setReaderClientDone() {
	readerClientDone = 1;
}

void * dataProcessingRun(void * args) {
	int i = 0;
	while (!readerClientDone || readings_count() > 0) {
		Reading * reading = readings_take();
	}
	printf("[INFO] data processing finished\n");
	readings_print();
	return NULL;
}
