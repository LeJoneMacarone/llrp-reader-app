#include "readings.h"

void readings_add(Reading * reading);

Reading * readings_take();

void readings_init(); 

int readings_count();

cJSON * readings_toJSON();

char * readings_toString();

