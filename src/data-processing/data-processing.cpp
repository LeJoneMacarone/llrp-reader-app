#include "crossings-buffer.h"
#include "readings-buffer.h"
#include "event.h"
#include "../data-delivery/internet.h"
#include "../config/data-processing-config.h"
#include "../logs/logs.h"
#include "../files/files.h"
#include "readings.h"

#include <ctime>
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

		if (!crossings_contains(reading->rfid)) {
			int added = crossings_addFromReading(reading);
			
			if (added) {
				log(out, INFO, "reading accepted, creating crossing");
				
				char * url;
				string content;

				time_t seconds = reading->local_time / 1000000;
				struct tm * date = localtime(&seconds);

				asprintf(
					&url, "%s%s,%05i,%2i,%2i:%2i:%2i;%s",
					config->server_url, config->station_name,
					readings_count(), date->tm_mday, 
					date->tm_hour, date->tm_min, date->tm_sec,
					reading->rfid
				);

				log(out, INFO, "sending crossing...");
				int status = performRequest(url, &content);
				
				status < 0 
					? log(out, WARN, "an error occured while sending the crossing")
					: log(out, INFO, "message sent successfully, response = %i", status);

				free(url);
			}
		}
		
		// set status to 1 to say it was already processed
		reading->status = 1;
	}
	
	log(out, INFO, "data processing finished");

	char * crossings = crossings_toString();
	writeFile(config->export_destination, crossings);
	free(crossings);

	return NULL;
}
