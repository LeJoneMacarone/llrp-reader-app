#ifndef ATHLETE_H
#define ATHLETE_H

#include "../cJSON/cJSON.h"

typedef struct {
	unsigned long id;
	char * rfid;
	char * name;
	char * country;
	char * club;
	char * category;
	char * gender;
} Athlete;

Athlete * athlete_create(
	unsigned long id,
	const char * rfid,
	const char * name, 
	const char * country, 
	const char * club, 
	const char * category, 
	const char * gender
);

void athlete_free(Athlete * athlete);

Athlete * athlete_fromJSON(cJSON * json);

char * athlete_toString(Athlete * athlete);

#endif // ATHLETE_H
