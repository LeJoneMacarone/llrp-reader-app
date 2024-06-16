#include "crossings-buffer.h"
#include "readings-buffer.h"

#include <stdio.h>
#include <string.h>

int readerClientDone = 0;

void setReaderClientDone() {
	readerClientDone = 1;
}

void * dataProcessingRun(void * args) {
	printf("[INFO] data processing started\n");
	
	while (!readerClientDone || readings_count() > 0) {
		Reading * reading = readings_take();
		if (reading && !crossings_contains(reading->rfid)) 
			crossings_add(reading);
	}
	
	printf("[INFO] data processing finished\n");

	readings_print();
	crossings_print();

	return NULL;
}
