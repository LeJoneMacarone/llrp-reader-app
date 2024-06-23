#include "event.h"
#include "../files/files.h"
#include "../cJSON/cJSON.h"
#include "competition.h"

#include <stdio.h>
#include <list>
using namespace std;

list<Competition *> competitions;

void importEventData(const char * file) {
	char * sessionData;
	readFile(file, &sessionData);

	cJSON * sessionJSON = cJSON_Parse(sessionData);
	cJSON * eventJSON = cJSON_GetObjectItem(sessionJSON, "event");

	cJSON * competitionJSON = NULL;
	cJSON * competitionsJSON = cJSON_GetObjectItem(eventJSON, "competitions");
	cJSON_ArrayForEach(competitionJSON, competitionsJSON) {
		Competition * competition = competition_fromJSON(competitionJSON);
		competitions.push_front(competition);
	}
}

list<Competition *> * getCompetitions() {
	return &competitions;
}
