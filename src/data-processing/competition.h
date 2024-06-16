#ifndef COMPETITION_H 
#define COMPETITION_H

#include "athlete.h"
#include "../cJSON/cJSON.h"

#include <stdint.h>
#include <list>
using namespace std;

typedef struct {
    char * name;
    uint64_t start;
    uint64_t end;
    char * color;
	list<Athlete *> * athletes;
} Competition;

Competition * competition_fromJSON(cJSON * json);

void competition_addAthlete(Competition * competition, Athlete * athlete);

#endif  // COMPETITION_H
