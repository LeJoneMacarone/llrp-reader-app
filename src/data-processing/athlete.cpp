#include "athlete.h"

#include "stdio.h"
#include <stdlib.h>
#include <string.h>

Athlete * athlete_create(
	unsigned long id,
	const char * rfid,
	const char * name, 
	const char * country, 
	const char * club, 
	const char * category, 
	const char * gender
) {
    Athlete * athlete = (Athlete *) malloc(sizeof(Athlete));

    athlete->id = id;
    athlete->rfid = strdup(rfid);
    athlete->name = strdup(name);
    athlete->country = strdup(country);
    athlete->club = strdup(club);
    athlete->category = strdup(category);
    athlete->gender = strdup(gender);

    return athlete;
}

Athlete * athlete_fromJSON(cJSON * json) {
	unsigned long id = cJSON_GetObjectItem(json, "id")->valueint;
	char * rfid = cJSON_GetObjectItem(json, "rfid")->valuestring;
	char * name = cJSON_GetObjectItem(json, "name")->valuestring;
	char * country = cJSON_GetObjectItem(json, "country")->valuestring;	
	char * club = cJSON_GetObjectItem(json, "club")->valuestring;
	char * category = cJSON_GetObjectItem(json, "category")->valuestring;
	char * gender = cJSON_GetObjectItem(json, "gender")->valuestring;
	
	return athlete_create(id, rfid, name, country, club, category, gender);
}

char * athlete_toString(Athlete * athlete){
	char * string; 
	asprintf(
		&string, "%lu %s %s %s %s %s %s\n", 
		athlete->id, athlete->name,
		athlete->gender, athlete->country,
		athlete->club, athlete->rfid,
		athlete->category
	);
	return string;
}

