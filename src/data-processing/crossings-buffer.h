#include "crossings.h"
#include "readings.h"

void crossings_add(Crossing * crossing);

void crossings_addFromReading(Reading * reading);

int crossings_contains(const char * rfid);

void crossings_print();
