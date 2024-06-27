#include "crossings-buffer.h"
#include "readings-buffer.h"
#include "event.h"
#include "../config/data-processing-config.h"
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
	DataProcessingConfig * config = (DataProcessingConfig *) args;
	
	FILE * out = strcmp(config->logs_destination, "stdout")
		? fopen(config->logs_destination, "w") : stdout;

	log(out, INFO, "data processing started");

	while (!readerClientDone || readings_count() > 0) {
		Reading * reading = readings_take();

		// skip if reading doesnt exist or it was already processed
		if (reading == NULL || !reading->status) continue;

		if (!crossings_contains(reading->rfid))
			log(out, INFO, crossings_addFromReading(reading) 
				? "reading accepted, crossing added" 
				: "reading rejected"
			);
		
		// set status to 1 to say it was already processed
		reading->status = 1;
	}
	
	log(out, INFO, "data processing finished");

	char * crossings = crossings_toString();
	writeFile(config->export_destination, crossings);
	free(crossings);

	return NULL;
}
