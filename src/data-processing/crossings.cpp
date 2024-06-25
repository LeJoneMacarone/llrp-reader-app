#include "crossings.h"
#include "../cJSON/cJSON.h"
#include "../files/files.h"

#include <string.h>

int rowCount;

Crossing * crossing_create(
	Athlete * athlete,
	Competition * competition,
	Reading * reading
) {
	Crossing * crossing = (Crossing *) malloc(sizeof(Crossing));
	
	crossing->athlete = athlete,
	crossing->competition = competition,
	crossing->reading = reading;

	crossing->status = strdup("EP");
	crossing->status_sent = 0;
	crossing->row = rowCount++;

	return crossing;
}

void crossing_free(Crossing * crossing) {
	free(crossing->status);
	free(crossing);
}

cJSON * crossing_toJSON(Crossing * crossing) {
	cJSON * json = cJSON_CreateObject();

	cJSON_AddNumberToObject(json, "id", crossing->athlete->id);
	cJSON_AddStringToObject(json, "name", crossing->athlete->name);
	cJSON_AddStringToObject(json, "country", crossing->athlete->country); 
	cJSON_AddStringToObject(json, "club", crossing->athlete->club);
	cJSON_AddStringToObject(json, "category", crossing->athlete->category);
	cJSON_AddStringToObject(json, "gender", crossing->athlete->gender);
	
	cJSON_AddStringToObject(json, "competition_name", crossing->competition->name);
	cJSON_AddStringToObject(json, "competition_color", crossing->competition->color);
	
	char * competitionStart = stringFromMicroseconds(crossing->competition->start);
	cJSON_AddStringToObject(json, "competition_start", competitionStart);
	
	uint64_t timeDiff = crossing->reading->reader_time - crossing->competition->start;
	char * competitionTime = stringFromMicroseconds(timeDiff);
	cJSON_AddStringToObject(json, "competition_time", competitionTime);
	
	cJSON_AddStringToObject(json, "status", "EP");
	crossing->status_sent 
		? cJSON_AddTrueToObject(json, "status_sent")
		: cJSON_AddFalseToObject(json, "status_sent");
	cJSON_AddNumberToObject(json, "row", crossing->row);

	return json;
}

char * crossing_toString(Crossing * crossing) {
	cJSON * json = crossing_toJSON(crossing);
	char * string = cJSON_Print(json);
	cJSON_Delete(json);
	return string;
}

