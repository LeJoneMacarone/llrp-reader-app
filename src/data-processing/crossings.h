#include "athlete.h"
#include "competition.h"
#include "readings.h"
#include "timestamps.h"

typedef struct {
	Athlete * athlete;
	Competition * competition;
	Reading * reading;
	char * status;
	int status_sent;
	unsigned int row;
} Crossing;


Crossing * crossing_create(
	Athlete * athlete,
	Competition * competition,
	Reading * reading
);

void crossing_free(Crossing * crossing);

cJSON * crossing_toJSON(Crossing * crossing);

char * crossing_toString(Crossing * crossing);
