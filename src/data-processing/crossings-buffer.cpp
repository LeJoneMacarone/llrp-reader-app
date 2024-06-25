#include "crossings-buffer.h"
#include "event.h"
#include "readings.h"

#include <stdio.h>
#include <string.h>

#define USE_READER_TIMESTAMPS

#define CROSSINGS_BUFFER_SIZE 256
Crossing * crossings[CROSSINGS_BUFFER_SIZE];

int crossingsInsertIndex = 0,
	crossingsCount = 0;

void crossings_add(Crossing * crossing) {
	crossings[crossingsInsertIndex] = crossing;
	crossingsInsertIndex = (crossingsInsertIndex + 1) % CROSSINGS_BUFFER_SIZE;
	crossingsCount++;
}

int crossings_contains(const char * rfid) {
	for (int i = 0; i < CROSSINGS_BUFFER_SIZE; i++) {
		if (crossings[i] && strcmp(crossings[i]->reading->rfid, rfid) == 0)
			return 1;
	}
	return 0;
}

int crossings_addFromReading(Reading * reading) {
	list <Competition *> * competitions = getCompetitions();
	for(Competition * competition : *competitions) {
		for(Athlete * athlete : *(competition->athletes)) {
			if (strcmp(athlete->rfid, reading->rfid) == 0) { 
				Crossing * crossing 
					= crossing_create(athlete, competition, reading);
				crossings_add(crossing);
				return 1;
			}
		}
	}
	return 0;
}

cJSON * crossings_toJSON() {
	cJSON * json = cJSON_CreateArray();
	for (int i = 0; i < CROSSINGS_BUFFER_SIZE; i++) {
		if (crossings[i] == NULL) continue;
		cJSON * reading = crossing_toJSON(crossings[i]);
		cJSON_AddItemToArray(json, reading);
	}
	return json;
}

char * crossings_toString() {
	cJSON * json = crossings_toJSON();
	char * string = cJSON_Print(json);
	cJSON_Delete(json);
	return string;
}
