#include "readings.h"
#include <stddef.h>

void readings_add(Reading * reading);

Reading * readings_take();

void readings_init(size_t size); 

int readings_count();

cJSON * readings_toJSON();

char * readings_toString();

