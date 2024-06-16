#include "crossings-buffer.h"
#include "readings-buffer.h"
#include "event.h"

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
		if (reading == NULL) continue;
		Reading copy = *reading;

		if (!crossings_contains(copy.rfid)) 
			crossings_addFromReading(&copy);
	}
	
	printf("[INFO] data processing finished\n");

	readings_print();
	crossings_print();

	return NULL;
}
