#include "event.h"
#include "../files/files.h"
#include "../cJSON/cJSON.h"
#include "competition.h"

#include <cstdio>
#include <list>
using namespace std;

list<Competition *> competitions;

void importEventData(const char * file) {
	printf("[INFO] importing event data\n");

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

	printf("[INFO] event data imported successfully\n");
}

list<Competition *> * getCompetitions() {
	return &competitions;
}
