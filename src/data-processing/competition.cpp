#include "competition.h"
#include "athlete.h"
#include "timestamps.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

Competition * competition_create(
	const char * name, 
	uint64_t start, 
	uint64_t end, 
	const char * color
) {
    Competition * competition = (Competition *) malloc(sizeof(Competition));

    competition->name = strdup(name);
    competition->start = start;
    competition->end = end;
    competition->color = strdup(color);
	competition->athletes = new list<Athlete *>();

    return competition;
}

Competition * competition_fromJSON(cJSON * json) {
	char * name = cJSON_GetObjectItem(json, "short_name")->valuestring;
	char * color = cJSON_GetObjectItem(json, "color")->valuestring;

	char * startString = cJSON_GetObjectItem(json, "start")->valuestring;
	char * endString = cJSON_GetObjectItem(json, "end")->valuestring;
	uint64_t start = stringToTimestamp(startString);
	uint64_t end = stringToTimestamp(endString);

	Competition * competition = competition_create(name, start, end, color);

	cJSON * element = NULL;
	cJSON * athletes = cJSON_GetObjectItem(json, "athletes");
	cJSON_ArrayForEach(element, athletes) {
		Athlete * athlete = athlete_fromJSON(element);
		competition_addAthlete(competition, athlete);
	}

	return competition;
}

void competition_addAthlete(Competition * competition, Athlete * athlete) {
	competition->athletes->push_front(athlete);
}
