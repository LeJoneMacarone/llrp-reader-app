#ifndef COMPETITION_H 
#define COMPETITION_H

#include "athlete.h"
#include "../cJSON/cJSON.h"
#include <stdint.h>

typedef struct {
    char * name;
    uint64_t start;
    uint64_t end;
    char * color;
} Competition;

Competition * competition_fromJSON(cJSON * json);
	
#endif  // COMPETITION_H
