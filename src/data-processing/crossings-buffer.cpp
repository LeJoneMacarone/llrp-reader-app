#include "crossings-buffer.h"
#include "event.h"
#include "readings.h"

#include <cstdlib>
#include <stdio.h>
#include <string.h>

#define USE_READER_TIMESTAMPS

size_t crossingsBufferSize;
Crossing ** crossings;

int crossingsInsertIndex = 0,
	crossingsCount = 0;

void crossings_init(size_t size) {
	crossingsBufferSize = size;
	crossings = (Crossing **) malloc(size * sizeof(Crossing *));
	memset(crossings, 0, size * sizeof(Crossing *));
}

void crossings_add(Crossing * crossing) {
	crossings[crossingsInsertIndex] = crossing;
	crossingsInsertIndex = (crossingsInsertIndex + 1) % crossingsBufferSize;
	crossingsCount++;
}

int crossings_contains(const char * rfid) {
	for (size_t i = 0; i < crossingsBufferSize; i++) {
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
	for (size_t i = 0; i < crossingsBufferSize; i++) {
		if (!crossings[i]) continue;
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
