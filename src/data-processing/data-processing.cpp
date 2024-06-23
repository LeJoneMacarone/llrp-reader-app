#include "crossings-buffer.h"
#include "readings-buffer.h"
#include "event.h"
#include "../logs/logs.h"
#include "../files/files.h"
#include "readings.h"

#include <stdio.h>
#include <string.h>

int readerClientDone = 0;

void setReaderClientDone() {
	readerClientDone = 1;
}

void * dataProcessingRun(void * args) {
	log(stdout, INFO, "data processing started");

	while (!readerClientDone || readings_count() > 0) {
		Reading * reading = readings_take();
		if (reading == NULL) continue;
		Reading copy = *reading;

		if (!crossings_contains(copy.rfid)) 
			crossings_addFromReading(&copy);
	}
	
	log(stdout, INFO, "data processing finished");

	// TODO: make the path a parameter 
	char * readings = readings_toString();
	writeFile("logs/READINGS.json", readings);
	
	// TODO: make the path a parameter 
	char * crossings = crossings_toString();
	writeFile("logs/CROSSINGS.json", readings);
	
	free(readings);
	free(crossings);

	return NULL;
}
