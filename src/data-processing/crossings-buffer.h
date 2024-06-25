#include "crossings.h"
#include "readings.h"
#include <stddef.h>

void crossings_init(size_t size);

void crossings_add(Crossing * crossing);

int crossings_addFromReading(Reading * reading);

int crossings_contains(const char * rfid);

cJSON * crossings_toJSON();

char * crossings_toString();
